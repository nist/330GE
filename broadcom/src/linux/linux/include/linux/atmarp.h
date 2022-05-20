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
/* atmarp.h - ATM ARP protocol and kernel-demon interface definitions */
 
/* Written 1995-1999 by Werner Almesberger, EPFL LRC/ICA */
 

#ifndef _LINUX_ATMARP_H
#define _LINUX_ATMARP_H

#ifdef __KERNEL__
#include <linux/types.h>
#endif
#include <linux/atmapi.h>
#include <linux/atmioc.h>


#define ATMARP_RETRY_DELAY 30		/* request next resolution or forget
					   NAK after 30 sec - should go into
					   atmclip.h */
#define ATMARP_MAX_UNRES_PACKETS 5	/* queue that many packets while
					   waiting for the resolver */


#define ATMARPD_CTRL	_IO('a',ATMIOC_CLIP+1)	/* become atmarpd ctrl sock */
#define ATMARP_MKIP	_IO('a',ATMIOC_CLIP+2)	/* attach socket to IP */
#define ATMARP_SETENTRY	_IO('a',ATMIOC_CLIP+3)	/* fill or hide ARP entry */
#define ATMARP_ENCAP	_IO('a',ATMIOC_CLIP+5)	/* change encapsulation */


enum atmarp_ctrl_type {
	act_invalid,		/* catch uninitialized structures */
	act_need,		/* need address resolution */
	act_up,			/* interface is coming up */
	act_down,		/* interface is going down */
	act_change		/* interface configuration has changed */
};

struct atmarp_ctrl {
	enum atmarp_ctrl_type	type;	/* message type */
	int			itf_num;/* interface number (if present) */
	uint32_t		ip;	/* IP address (act_need only) */
};

#endif
