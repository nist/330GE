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
#ifndef __LINUX_BRIDGE_NETFILTER_H
#define __LINUX_BRIDGE_NETFILTER_H

/* bridge-specific defines for netfilter. 
 */

#include <linux/config.h>
#include <linux/netfilter.h>

/* Bridge Hooks */
/* After promisc drops, checksum checks. */
#define NF_BR_PRE_ROUTING	0
/* If the packet is destined for this box. */
#define NF_BR_LOCAL_IN		1
/* If the packet is destined for another interface. */
#define NF_BR_FORWARD		2
/* Packets coming from a local process. */
#define NF_BR_LOCAL_OUT		3
/* Packets about to hit the wire. */
#define NF_BR_POST_ROUTING	4
#define NF_BR_NUMHOOKS		5


#endif
