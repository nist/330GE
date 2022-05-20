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
/* include/net/dsfield.h - Manipulation of the Differentiated Services field */

/* Written 1998-2000 by Werner Almesberger, EPFL ICA */


#ifndef __NET_DSFIELD_H
#define __NET_DSFIELD_H

#include <linux/types.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <asm/byteorder.h>


static inline __u8 ipv4_get_dsfield(struct iphdr *iph)
{
	return iph->tos;
}


static inline __u8 ipv6_get_dsfield(struct ipv6hdr *ipv6h)
{
	return ntohs(*(__u16 *) ipv6h) >> 4;
}


static inline void ipv4_change_dsfield(struct iphdr *iph,__u8 mask,
    __u8 value)
{
        __u32 check = ntohs(iph->check);
	__u8 dsfield;

	dsfield = (iph->tos & mask) | value;
	check += iph->tos;
	if ((check+1) >> 16) check = (check+1) & 0xffff;
	check -= dsfield;
	check += check >> 16; /* adjust carry */
	iph->check = htons(check);
	iph->tos = dsfield;
}


static inline void ipv6_change_dsfield(struct ipv6hdr *ipv6h,__u8 mask,
    __u8 value)
{
        __u16 tmp;

	tmp = ntohs(*(__u16 *) ipv6h);
	tmp = (tmp & ((mask << 4) | 0xf00f)) | (value << 4);
	*(__u16 *) ipv6h = htons(tmp);
}


#endif
