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
#ifndef __LINUX_IPX_NETFILTER_H
#define __LINUX_IPX_NETFILTER_H

/* IPX-specific defines for netfilter.  Complete me sometime.
 * (C)1998 Rusty Russell -- This code is GPL.
 */

#include <linux/netfilter.h>

/* IPX Hooks */
#define NF_IPX_INPUT	0
#define NF_IPX_FORWARD	1
#define NF_IPX_OUTPUT	2
#endif /*__LINUX_IPX_NETFILTER_H*/
