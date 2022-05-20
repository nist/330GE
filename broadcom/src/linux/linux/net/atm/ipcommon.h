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
/* net/atm/ipcommon.h - Common items for all ways of doing IP over ATM */

/* Written 1996-2000 by Werner Almesberger, EPFL LRC/ICA */


#ifndef NET_ATM_IPCOMMON_H
#define NET_ATM_IPCOMMON_H


#include <linux/string.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/atmdev.h>


extern struct net_device *clip_devs;

/*
 * Appends all skbs from "from" to "to". The operation is atomic with respect
 * to all other skb operations on "from" or "to".
 */

void skb_migrate(struct sk_buff_head *from,struct sk_buff_head *to);

#endif
