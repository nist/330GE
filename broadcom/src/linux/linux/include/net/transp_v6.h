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
#ifndef _TRANSP_V6_H
#define _TRANSP_V6_H

#include <net/checksum.h>

/*
 *	IPv6 transport protocols
 */

#ifdef __KERNEL__

extern struct proto rawv6_prot;
extern struct proto udpv6_prot;
extern struct proto tcpv6_prot;

struct flowi;

extern void				rawv6_init(void);
extern void				udpv6_init(void);
extern void				tcpv6_init(void);

extern int				udpv6_connect(struct sock *sk,
						      struct sockaddr *uaddr,
						      int addr_len);

extern int			datagram_recv_ctl(struct sock *sk,
						  struct msghdr *msg,
						  struct sk_buff *skb);

extern int			datagram_send_ctl(struct msghdr *msg,
						  struct flowi *fl,
						  struct ipv6_txoptions *opt,
						  int *hlimit);

#define		LOOPBACK4_IPV6		__constant_htonl(0x7f000006)

/*
 *	address family specific functions
 */
extern struct tcp_func	ipv4_specific;

extern int inet6_destroy_sock(struct sock *sk);

#endif

#endif
