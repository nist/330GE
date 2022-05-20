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
/*
 * INET		An implementation of the TCP/IP protocol suite for the LINUX
 *		operating system.  NET  is implemented using the  BSD Socket
 *		interface as the means of communication with the user level.
 *
 *		Definitions for the Token-ring handlers.
 *
 * Version:	@(#)eth.h	1.0.4	05/13/93
 *
 * Authors:	Ross Biro, <bir7@leland.Stanford.Edu>
 *		Fred N. van Kempen, <waltje@uWalt.NL.Mugnet.ORG>
 *
 *		Relocated to include/linux where it belongs by Alan Cox 
 *							<gw4pts@gw4pts.ampr.org>
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 *	WARNING: This move may well be temporary. This file will get merged with others RSN.
 *
 */
#ifndef _LINUX_TRDEVICE_H
#define _LINUX_TRDEVICE_H


#include <linux/if_tr.h>

#ifdef __KERNEL__
extern int		tr_header(struct sk_buff *skb, struct net_device *dev,
				   unsigned short type, void *daddr,
				   void *saddr, unsigned len);
extern void		tr_source_route(struct sk_buff *skb,
					struct trh_hdr *trh,
					struct net_device *dev);
extern int		tr_rebuild_header(struct sk_buff *skb);
extern unsigned short	tr_type_trans(struct sk_buff *skb, struct net_device *dev);
extern struct net_device *init_trdev(struct net_device *dev, int sizeof_priv);
extern struct net_device *alloc_trdev(int sizeof_priv);
extern int register_trdev(struct net_device *dev);
extern void unregister_trdev(struct net_device *dev);

#endif

#endif	/* _LINUX_TRDEVICE_H */
