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
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>

#include <linux/netfilter_ipv4/ipt_pkttype.h>
#include <linux/netfilter_ipv4/ip_tables.h>

MODULE_LICENSE("GPL");

static int match(const struct sk_buff *skb,
      const struct net_device *in,
      const struct net_device *out,
      const void *matchinfo,
      int offset,
      const void *hdr,
      u_int16_t datalen,
      int *hotdrop)
{
    const struct ipt_pkttype_info *info = matchinfo;

    return (skb->pkt_type == info->pkttype) ^ info->invert;
}

static int checkentry(const char *tablename,
		   const struct ipt_ip *ip,
		   void *matchinfo,
		   unsigned int matchsize,
		   unsigned int hook_mask)
{
/*
	if (hook_mask
	    & ~((1 << NF_IP_PRE_ROUTING) | (1 << NF_IP_LOCAL_IN)
		| (1 << NF_IP_FORWARD))) {
		printk("ipt_pkttype: only valid for PRE_ROUTING, LOCAL_IN or FORWARD.\n");
		return 0;
	}
*/
	if (matchsize != IPT_ALIGN(sizeof(struct ipt_pkttype_info)))
		return 0;

	return 1;
}

static struct ipt_match pkttype_match
= { { NULL, NULL }, "pkttype", &match, &checkentry, NULL, THIS_MODULE };

static int __init init(void)
{
	return ipt_register_match(&pkttype_match);
}

static void __exit fini(void)
{
	ipt_unregister_match(&pkttype_match);
}

module_init(init);
module_exit(fini);
