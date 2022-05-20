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
#ifndef _IP_NAT_HELPER_H
#define _IP_NAT_HELPER_H
/* NAT protocol helper routines. */

#include <linux/netfilter_ipv4/ip_conntrack.h>

struct sk_buff;

/* Flags */
/* NAT helper must be called on every packet (for TCP) */
#define IP_NAT_HELPER_F_ALWAYS		0x01
/* Standalone NAT helper, without a conntrack part */
#define IP_NAT_HELPER_F_STANDALONE	0x02

struct ip_nat_helper
{
	struct list_head list;		/* Internal use */

	const char *name;		/* name of the module */
	unsigned char flags;		/* Flags (see above) */
	struct module *me;		/* pointer to self */
	
	/* Mask of things we will help: vs. tuple from server */
	struct ip_conntrack_tuple tuple;
	struct ip_conntrack_tuple mask;
	
	/* Helper function: returns verdict */
	unsigned int (*help)(struct ip_conntrack *ct,
			     struct ip_conntrack_expect *exp,
			     struct ip_nat_info *info,
			     enum ip_conntrack_info ctinfo,
			     unsigned int hooknum,
			     struct sk_buff **pskb);

	/* Returns verdict and sets up NAT for this connection */
	unsigned int (*expect)(struct sk_buff **pskb,
			       unsigned int hooknum,
			       struct ip_conntrack *ct,
			       struct ip_nat_info *info);
};

extern struct list_head helpers;

extern int ip_nat_helper_register(struct ip_nat_helper *me);
extern void ip_nat_helper_unregister(struct ip_nat_helper *me);
extern int ip_nat_mangle_tcp_packet(struct sk_buff **skb,
				struct ip_conntrack *ct,
				enum ip_conntrack_info ctinfo,
				unsigned int match_offset,
				unsigned int match_len,
				char *rep_buffer,
				unsigned int rep_len);
extern int ip_nat_mangle_udp_packet(struct sk_buff **skb,
				struct ip_conntrack *ct,
				enum ip_conntrack_info ctinfo,
				unsigned int match_offset,
				unsigned int match_len,
				char *rep_buffer,
				unsigned int rep_len);
extern int ip_nat_seq_adjust(struct sk_buff *skb,
				struct ip_conntrack *ct,
				enum ip_conntrack_info ctinfo);
extern void ip_nat_delete_sack(struct sk_buff *skb);
#endif
