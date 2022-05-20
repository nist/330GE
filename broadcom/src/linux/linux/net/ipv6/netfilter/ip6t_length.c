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
/* Length Match - IPv6 Port */

#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/netfilter_ipv6/ip6t_length.h>
#include <linux/netfilter_ipv6/ip6_tables.h>

static int
match(const struct sk_buff *skb,
      const struct net_device *in,
      const struct net_device *out,
      const void *matchinfo,
      int offset,
      const void *hdr,
      u_int16_t datalen,
      int *hotdrop)
{
	const struct ip6t_length_info *info = matchinfo;
	u_int16_t pktlen = ntohs(skb->nh.ipv6h->payload_len) + sizeof(struct ipv6hdr);
	
	return (pktlen >= info->min && pktlen <= info->max) ^ info->invert;
}

static int
checkentry(const char *tablename,
           const struct ip6t_ip6 *ip,
           void *matchinfo,
           unsigned int matchsize,
           unsigned int hook_mask)
{
	if (matchsize != IP6T_ALIGN(sizeof(struct ip6t_length_info)))
		return 0;

	return 1;
}

static struct ip6t_match length_match
= { { NULL, NULL }, "length", &match, &checkentry, NULL, THIS_MODULE };

static int __init init(void)
{
	return ip6t_register_match(&length_match);
}

static void __exit fini(void)
{
	ip6t_unregister_match(&length_match);
}

module_init(init);
module_exit(fini);
