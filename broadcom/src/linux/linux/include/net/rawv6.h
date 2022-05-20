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
#ifndef _NET_RAWV6_H
#define _NET_RAWV6_H

#ifdef __KERNEL__

#define RAWV6_HTABLE_SIZE	MAX_INET_PROTOS
extern struct sock *raw_v6_htable[RAWV6_HTABLE_SIZE];
extern rwlock_t raw_v6_lock;

extern struct sock * ipv6_raw_deliver(struct sk_buff *skb,
				      int nexthdr);


extern struct sock *__raw_v6_lookup(struct sock *sk, unsigned short num,
				    struct in6_addr *loc_addr, struct in6_addr *rmt_addr);

extern int			rawv6_rcv(struct sock *sk,
					  struct sk_buff *skb);


extern void			rawv6_err(struct sock *sk,
					  struct sk_buff *skb,
					  struct inet6_skb_parm *opt,
					  int type, int code, 
					  int offset, u32 info);

#endif

#endif
