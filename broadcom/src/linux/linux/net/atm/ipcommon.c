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
/* net/atm/ipcommon.c - Common items for all ways of doing IP over ATM */

/* Written 1996-2000 by Werner Almesberger, EPFL LRC/ICA */


#include <linux/module.h>
#include <linux/string.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/in.h>
#include <linux/atmdev.h>
#include <linux/atmclip.h>

#include "common.h"
#include "ipcommon.h"


#define DPRINTK(format,args...)


const unsigned char llc_oui[] = {
	0xaa,	/* DSAP: non-ISO */
	0xaa,	/* SSAP: non-ISO */
	0x03,	/* Ctrl: Unnumbered Information Command PDU */
	0x00,	/* OUI: EtherType */
	0x00,
	0x00 };


/*
 * skb_migrate appends the list at "from" to "to", emptying "from" in the
 * process. skb_migrate is atomic with respect to all other skb operations on
 * "from" and "to". Note that it locks both lists at the same time, so beware
 * of potential deadlocks.
 *
 * This function should live in skbuff.c or skbuff.h.
 */


void skb_migrate(struct sk_buff_head *from,struct sk_buff_head *to)
{
	struct sk_buff *skb;
	unsigned long flags;
	struct sk_buff *skb_from = (struct sk_buff *) from;
	struct sk_buff *skb_to = (struct sk_buff *) to;
	struct sk_buff *prev;

	spin_lock_irqsave(&from->lock,flags);
	spin_lock(&to->lock);
	prev = from->prev;
	from->next->prev = to->prev;
	prev->next = skb_to;
	to->prev->next = from->next;
	to->prev = from->prev;
	for (skb = from->next; skb != skb_to; skb = skb->next)
		skb->list = to;
	to->qlen += from->qlen;
	spin_unlock(&to->lock);
	from->prev = skb_from;
	from->next = skb_from;
	from->qlen = 0;
	spin_unlock_irqrestore(&from->lock,flags);
}


EXPORT_SYMBOL(skb_migrate);
