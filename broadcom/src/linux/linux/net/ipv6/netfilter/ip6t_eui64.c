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
/* Kernel module to match EUI64 address parameters. */
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ipv6.h>
#include <linux/if_ether.h>

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

    unsigned char eui64[8];
    int i=0;

     if ( !(skb->mac.raw >= skb->head
                && (skb->mac.raw + ETH_HLEN) <= skb->data)
                && offset != 0) {
                        *hotdrop = 1;
                        return 0;
                }
    
    memset(eui64, 0, sizeof(eui64));

    if (skb->mac.ethernet->h_proto == ntohs(ETH_P_IPV6)) {
      if (skb->nh.ipv6h->version == 0x6) { 
         memcpy(eui64, skb->mac.ethernet->h_source, 3);
         memcpy(eui64 + 5, skb->mac.ethernet->h_source + 3, 3);
	 eui64[3]=0xff;
	 eui64[4]=0xfe;
	 eui64[0] |= 0x02;

	 i=0;
	 while ((skb->nh.ipv6h->saddr.in6_u.u6_addr8[8+i] ==
			 eui64[i]) && (i<8)) i++;

	 if ( i == 8 )
	 	return 1;
      }
    }

    return 0;
}

static int
ip6t_eui64_checkentry(const char *tablename,
		   const struct ip6t_ip6 *ip,
		   void *matchinfo,
		   unsigned int matchsize,
		   unsigned int hook_mask)
{
	if (hook_mask
	    & ~((1 << NF_IP6_PRE_ROUTING) | (1 << NF_IP6_LOCAL_IN) |
		(1 << NF_IP6_PRE_ROUTING) )) {
		printk("ip6t_eui64: only valid for PRE_ROUTING, LOCAL_IN or FORWARD.\n");
		return 0;
	}

	if (matchsize != IP6T_ALIGN(sizeof(int)))
		return 0;

	return 1;
}

static struct ip6t_match eui64_match
= { { NULL, NULL }, "eui64", &match, &ip6t_eui64_checkentry, NULL, THIS_MODULE };

static int __init init(void)
{
	return ip6t_register_match(&eui64_match);
}

static void __exit fini(void)
{
	ip6t_unregister_match(&eui64_match);
}

module_init(init);
module_exit(fini);
MODULE_DESCRIPTION("IPv6 EUI64 address checking match");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Andras Kis-Szabo <kisza@sch.bme.hu>");
