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
#ifndef _IF_TUNNEL_H_
#define _IF_TUNNEL_H_

#define SIOCGETTUNNEL   (SIOCDEVPRIVATE + 0)
#define SIOCADDTUNNEL   (SIOCDEVPRIVATE + 1)
#define SIOCDELTUNNEL   (SIOCDEVPRIVATE + 2)
#define SIOCCHGTUNNEL   (SIOCDEVPRIVATE + 3)

#define GRE_CSUM	__constant_htons(0x8000)
#define GRE_ROUTING	__constant_htons(0x4000)
#define GRE_KEY		__constant_htons(0x2000)
#define GRE_SEQ		__constant_htons(0x1000)
#define GRE_STRICT	__constant_htons(0x0800)
#define GRE_REC		__constant_htons(0x0700)
#define GRE_FLAGS	__constant_htons(0x00F8)
#define GRE_VERSION	__constant_htons(0x0007)

struct ip_tunnel_parm
{
	char			name[IFNAMSIZ];
	int			link;
	__u16			i_flags;
	__u16			o_flags;
	__u32			i_key;
	__u32			o_key;
	struct iphdr		iph;
};

#endif /* _IF_TUNNEL_H_ */
