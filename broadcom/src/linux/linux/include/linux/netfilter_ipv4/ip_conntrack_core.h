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
#ifndef _IP_CONNTRACK_CORE_H
#define _IP_CONNTRACK_CORE_H
#include <linux/netfilter_ipv4/lockhelp.h>

/* This header is used to share core functionality between the
   standalone connection tracking module, and the compatibility layer's use
   of connection tracking. */
extern unsigned int ip_conntrack_in(unsigned int hooknum,
				    struct sk_buff **pskb,
				    const struct net_device *in,
				    const struct net_device *out,
				    int (*okfn)(struct sk_buff *));

extern int ip_conntrack_init(void);
extern void ip_conntrack_cleanup(void);

struct ip_conntrack_protocol;
extern struct ip_conntrack_protocol *ip_ct_find_proto(u_int8_t protocol);
/* Like above, but you already have conntrack read lock. */
extern struct ip_conntrack_protocol *__ip_ct_find_proto(u_int8_t protocol);
extern struct list_head protocol_list;

/* Returns conntrack if it dealt with ICMP, and filled in skb->nfct */
extern struct ip_conntrack *icmp_error_track(struct sk_buff *skb,
					     enum ip_conntrack_info *ctinfo,
					     unsigned int hooknum);
extern int get_tuple(const struct iphdr *iph, size_t len,
		     struct ip_conntrack_tuple *tuple,
		     struct ip_conntrack_protocol *protocol);

/* Find a connection corresponding to a tuple. */
struct ip_conntrack_tuple_hash *
ip_conntrack_find_get(const struct ip_conntrack_tuple *tuple,
		      const struct ip_conntrack *ignored_conntrack);

extern int __ip_conntrack_confirm(struct nf_ct_info *nfct);

/* Confirm a connection: returns NF_DROP if packet must be dropped. */
static inline int ip_conntrack_confirm(struct sk_buff *skb)
{
	if (skb->nfct
	    && !is_confirmed((struct ip_conntrack *)skb->nfct->master))
		return __ip_conntrack_confirm(skb->nfct);
	return NF_ACCEPT;
}

extern struct list_head *ip_conntrack_hash;
extern struct list_head ip_conntrack_expect_list;
DECLARE_RWLOCK_EXTERN(ip_conntrack_lock);
#endif /* _IP_CONNTRACK_CORE_H */

