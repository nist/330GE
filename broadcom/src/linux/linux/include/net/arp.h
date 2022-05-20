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
/* linux/net/inet/arp.h */
#ifndef _ARP_H
#define _ARP_H

#include <linux/if_arp.h>
#include <net/neighbour.h>

extern struct neigh_table arp_tbl;

extern void	arp_init(void);
extern int	arp_rcv(struct sk_buff *skb, struct net_device *dev,
			struct packet_type *pt);
extern int	arp_find(unsigned char *haddr, struct sk_buff *skb);
extern int	arp_ioctl(unsigned int cmd, void *arg);
extern void     arp_send(int type, int ptype, u32 dest_ip, 
			 struct net_device *dev, u32 src_ip, 
			 unsigned char *dest_hw, unsigned char *src_hw, unsigned char *th);
extern int	arp_bind_neighbour(struct dst_entry *dst);
extern int	arp_mc_map(u32 addr, u8 *haddr, struct net_device *dev, int dir);
extern void	arp_ifdown(struct net_device *dev);

extern struct neigh_ops arp_broken_ops;

#endif	/* _ARP_H */
