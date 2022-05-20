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
/* IP tables module for matching the value of the IPv4 DSCP field
 *
 * ipt_dscp.c,v 1.3 2002/08/05 19:00:21 laforge Exp
 *
 * (C) 2002 by Harald Welte <laforge@gnumonks.org>
 *
 * This software is distributed under the terms  GNU GPL
 */

#include <linux/module.h>
#include <linux/skbuff.h>

#include <linux/netfilter_ipv4/ipt_dscp.h>
#include <linux/netfilter_ipv4/ip_tables.h>

MODULE_AUTHOR("Harald Welte <laforge@gnumonks.org>");
MODULE_DESCRIPTION("IP tables DSCP matching module");
MODULE_LICENSE("GPL");

static int match(const struct sk_buff *skb, const struct net_device *in,
		 const struct net_device *out, const void *matchinfo,
		 int offset, const void *hdr, u_int16_t datalen,
		 int *hotdrop)
{
	const struct ipt_dscp_info *info = matchinfo;
	const struct iphdr *iph = skb->nh.iph;

	u_int8_t sh_dscp = ((info->dscp << IPT_DSCP_SHIFT) & IPT_DSCP_MASK);

	return ((iph->tos&IPT_DSCP_MASK) == sh_dscp) ^ info->invert;
}

static int checkentry(const char *tablename, const struct ipt_ip *ip,
		      void *matchinfo, unsigned int matchsize,
		      unsigned int hook_mask)
{
	if (matchsize != IPT_ALIGN(sizeof(struct ipt_dscp_info)))
		return 0;

	return 1;
}

static struct ipt_match dscp_match = { { NULL, NULL }, "dscp", &match,
		&checkentry, NULL, THIS_MODULE };

static int __init init(void)
{
	return ipt_register_match(&dscp_match);
}

static void __exit fini(void)
{
	ipt_unregister_match(&dscp_match);

}

module_init(init);
module_exit(fini);