/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
/* Masquerade.  Simple mapping which alters range to a local IP address
   (depending on route). */
#include <linux/config.h>
#include <linux/types.h>
#include <linux/ip.h>
#include <linux/timer.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <net/protocol.h>
#include <net/checksum.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv4/ip_nat_rule.h>
#include <linux/netfilter_ipv4/ip_tables.h>

#define DEBUGP(format, args...)

/* Lock protects masq region inside conntrack */
static DECLARE_RWLOCK(masq_lock);

static int
masquerade_check(const char *tablename,
		 const struct ipt_entry *e,
		 void *targinfo,
		 unsigned int targinfosize,
		 unsigned int hook_mask)
{
	const struct ip_nat_multi_range *mr = targinfo;

	if (strcmp(tablename, "nat") != 0) {
		DEBUGP("masquerade_check: bad table `%s'.\n", table);
		return 0;
	}
	if (targinfosize != IPT_ALIGN(sizeof(*mr))) {
		DEBUGP("masquerade_check: size %u != %u.\n",
		       targinfosize, sizeof(*mr));
		return 0;
	}
	if (hook_mask & ~(1 << NF_IP_POST_ROUTING)) {
		DEBUGP("masquerade_check: bad hooks %x.\n", hook_mask);
		return 0;
	}
	if (mr->range[0].flags & IP_NAT_RANGE_MAP_IPS) {
		DEBUGP("masquerade_check: bad MAP_IPS.\n");
		return 0;
	}
	if (mr->rangesize != 1) {
		DEBUGP("masquerade_check: bad rangesize %u.\n", mr->rangesize);
		return 0;
	}
	return 1;
}

static unsigned int
masquerade_target(struct sk_buff **pskb,
		  unsigned int hooknum,
		  const struct net_device *in,
		  const struct net_device *out,
		  const void *targinfo,
		  void *userinfo)
{
	struct ip_conntrack *ct;
	enum ip_conntrack_info ctinfo;
	const struct ip_nat_multi_range *mr;
	struct ip_nat_multi_range newrange;
	u_int32_t newsrc;
	struct rtable *rt;
	struct rt_key key;

	IP_NF_ASSERT(hooknum == NF_IP_POST_ROUTING);

	if ((*pskb)->sk)
		return NF_ACCEPT;

	ct = ip_conntrack_get(*pskb, &ctinfo);
	IP_NF_ASSERT(ct && (ctinfo == IP_CT_NEW
				  || ctinfo == IP_CT_RELATED));

	mr = targinfo;

	key.dst = (*pskb)->nh.iph->daddr;
	key.src = 0; /* Unknown: that's what we're trying to establish */
	key.tos = RT_TOS((*pskb)->nh.iph->tos)|RTO_CONN;
	key.oif = out->ifindex;
#ifdef CONFIG_IP_ROUTE_FWMARK
	key.fwmark = (*pskb)->nfmark;
#endif
	if (ip_route_output_key(&rt, &key) != 0) {
		/* Shouldn't happen */
		printk("MASQUERADE: No route: Rusty's brain broke!\n");
		return NF_DROP;
	}

	newsrc = rt->rt_src;
	DEBUGP("newsrc = %u.%u.%u.%u\n", NIPQUAD(newsrc));
	ip_rt_put(rt);

	WRITE_LOCK(&masq_lock);
	ct->nat.masq_index = out->ifindex;
	WRITE_UNLOCK(&masq_lock);

	/* Transfer from original range. */
	newrange = ((struct ip_nat_multi_range)
		{ 1, { { mr->range[0].flags | IP_NAT_RANGE_MAP_IPS,
			 newsrc, newsrc,
			 mr->range[0].min, mr->range[0].max } } });

	/* Hand modified range to generic setup. */
	return ip_nat_setup_info(ct, &newrange, hooknum);
}

static inline int
device_cmp(const struct ip_conntrack *i, void *ifindex)
{
	int ret;

	READ_LOCK(&masq_lock);
	ret = (i->nat.masq_index == (int)(long)ifindex);
	READ_UNLOCK(&masq_lock);

	return ret;
}

static int masq_device_event(struct notifier_block *this,
			     unsigned long event,
			     void *ptr)
{
	struct net_device *dev = ptr;

	if (event == NETDEV_DOWN) {
		/* Device was downed.  Search entire table for
		   conntracks which were associated with that device,
		   and forget them. */
		IP_NF_ASSERT(dev->ifindex != 0);

		ip_ct_selective_cleanup(device_cmp, (void *)(long)dev->ifindex);
	}

	return NOTIFY_DONE;
}

static int masq_inet_event(struct notifier_block *this,
			   unsigned long event,
			   void *ptr)
{
	struct net_device *dev = ((struct in_ifaddr *)ptr)->ifa_dev->dev;

	if (event == NETDEV_DOWN) {
		/* IP address was deleted.  Search entire table for
		   conntracks which were associated with that device,
		   and forget them. */
		IP_NF_ASSERT(dev->ifindex != 0);

		ip_ct_selective_cleanup(device_cmp, (void *)(long)dev->ifindex);
	}

	return NOTIFY_DONE;
}

static struct notifier_block masq_dev_notifier = {
	masq_device_event,
	NULL,
	0
};

static struct notifier_block masq_inet_notifier = {
	masq_inet_event,
	NULL,
	0
};

static struct ipt_target masquerade
= { { NULL, NULL }, "MASQUERADE", masquerade_target, masquerade_check, NULL,
    THIS_MODULE };

static int __init init(void)
{
	int ret;

	ret = ipt_register_target(&masquerade);

	if (ret == 0) {
		/* Register for device down reports */
		register_netdevice_notifier(&masq_dev_notifier);
		/* Register IP address change reports */
		register_inetaddr_notifier(&masq_inet_notifier);
	}

	return ret;
}

static void __exit fini(void)
{
	ipt_unregister_target(&masquerade);
	unregister_netdevice_notifier(&masq_dev_notifier);
	unregister_inetaddr_notifier(&masq_inet_notifier);	
}

module_init(init);
module_exit(fini);
MODULE_LICENSE("GPL");
