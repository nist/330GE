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
/* Header file for kernel module to match connection tracking information.
 * GPL (C) 2001  Marc Boucher (marc@mbsi.ca).
 */

#ifndef _IPT_CONNTRACK_H
#define _IPT_CONNTRACK_H

#define IPT_CONNTRACK_STATE_BIT(ctinfo) (1 << ((ctinfo)%IP_CT_IS_REPLY+1))
#define IPT_CONNTRACK_STATE_INVALID (1 << 0)

#define IPT_CONNTRACK_STATE_SNAT (1 << (IP_CT_NUMBER + 1))
#define IPT_CONNTRACK_STATE_DNAT (1 << (IP_CT_NUMBER + 2))

/* flags, invflags: */
#define IPT_CONNTRACK_STATE	0x01
#define IPT_CONNTRACK_PROTO	0x02
#define IPT_CONNTRACK_ORIGSRC	0x04
#define IPT_CONNTRACK_ORIGDST	0x08
#define IPT_CONNTRACK_REPLSRC	0x10
#define IPT_CONNTRACK_REPLDST	0x20
#define IPT_CONNTRACK_STATUS	0x40
#define IPT_CONNTRACK_EXPIRES	0x80

struct ipt_conntrack_info
{
	unsigned int statemask, statusmask;

	struct ip_conntrack_tuple tuple[IP_CT_DIR_MAX];
	struct in_addr sipmsk[IP_CT_DIR_MAX], dipmsk[IP_CT_DIR_MAX];

	unsigned long expires_min, expires_max;

	/* Flags word */
	u_int8_t flags;
	/* Inverse flags */
	u_int8_t invflags;
};
#endif /*_IPT_CONNTRACK_H*/
