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
#ifndef _NET_DN_NEIGH_H
#define _NET_DN_NEIGH_H

/*
 * The position of the first two fields of
 * this structure are critical - SJW
 */
struct dn_neigh {
        struct neighbour n;
	dn_address addr;
        unsigned long flags;
#define DN_NDFLAG_R1    0x0001 /* Router L1      */
#define DN_NDFLAG_R2    0x0002 /* Router L2      */
#define DN_NDFLAG_P3    0x0004 /* Phase III Node */
        unsigned long blksize;
	unsigned char priority;
};

extern void dn_neigh_init(void);
extern void dn_neigh_cleanup(void);
extern struct neighbour *dn_neigh_lookup(struct neigh_table *tbl, void *ptr);
extern int dn_neigh_router_hello(struct sk_buff *skb);
extern int dn_neigh_endnode_hello(struct sk_buff *skb);
extern void dn_neigh_pointopoint_hello(struct sk_buff *skb);
extern int dn_neigh_elist(struct net_device *dev, unsigned char *ptr, int n);

extern struct neigh_table dn_neigh_table;

#endif /* _NET_DN_NEIGH_H */
