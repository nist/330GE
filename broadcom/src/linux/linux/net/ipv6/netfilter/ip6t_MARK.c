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
/* This is a module which is used for setting the NFMARK field of an skb. */
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <net/checksum.h>

#include <linux/netfilter_ipv6/ip6_tables.h>
#include <linux/netfilter_ipv6/ip6t_MARK.h>

static unsigned int
target(struct sk_buff **pskb,
       unsigned int hooknum,
       const struct net_device *in,
       const struct net_device *out,
       const void *targinfo,
       void *userinfo)
{
	const struct ip6t_mark_target_info *markinfo = targinfo;

	if((*pskb)->nfmark != markinfo->mark) {
		(*pskb)->nfmark = markinfo->mark;
		(*pskb)->nfcache |= NFC_ALTERED;
	}
	return IP6T_CONTINUE;
}

static int
checkentry(const char *tablename,
	   const struct ip6t_entry *e,
           void *targinfo,
           unsigned int targinfosize,
           unsigned int hook_mask)
{
	if (targinfosize != IP6T_ALIGN(sizeof(struct ip6t_mark_target_info))) {
		printk(KERN_WARNING "MARK: targinfosize %u != %Zu\n",
		       targinfosize,
		       IP6T_ALIGN(sizeof(struct ip6t_mark_target_info)));
		return 0;
	}

	if (strcmp(tablename, "mangle") != 0) {
		printk(KERN_WARNING "MARK: can only be called from \"mangle\" table, not \"%s\"\n", tablename);
		return 0;
	}

	return 1;
}

static struct ip6t_target ip6t_mark_reg
= { { NULL, NULL }, "MARK", target, checkentry, NULL, THIS_MODULE };

static int __init init(void)
{
	printk(KERN_DEBUG "registering ipv6 mark target\n");
	if (ip6t_register_target(&ip6t_mark_reg))
		return -EINVAL;

	return 0;
}

static void __exit fini(void)
{
	ip6t_unregister_target(&ip6t_mark_reg);
}

module_init(init);
module_exit(fini);
MODULE_LICENSE("GPL");
