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
#ifndef _IPT_POOL_H
#define _IPT_POOL_H

#include <linux/netfilter_ipv4/ip_pool.h>

#define IPT_POOL_INV_SRC	0x00000001
#define IPT_POOL_INV_DST	0x00000002
#define IPT_POOL_DEL_SRC	0x00000004
#define IPT_POOL_DEL_DST	0x00000008
#define IPT_POOL_INV_MOD_SRC	0x00000010
#define IPT_POOL_INV_MOD_DST	0x00000020
#define IPT_POOL_MOD_SRC_ACCEPT	0x00000040
#define IPT_POOL_MOD_DST_ACCEPT	0x00000080
#define IPT_POOL_MOD_SRC_DROP	0x00000100
#define IPT_POOL_MOD_DST_DROP	0x00000200

/* match info */
struct ipt_pool_info
{
	ip_pool_t src;
	ip_pool_t dst;
	unsigned flags;
};

#endif /*_IPT_POOL_H*/
