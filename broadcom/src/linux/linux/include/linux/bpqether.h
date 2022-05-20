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
#ifndef	__BPQETHER_H
#define	__BPQETHER_H

/*
 * 	Defines for the BPQETHER pseudo device driver
 */

#ifndef __LINUX_IF_ETHER_H
#include <linux/if_ether.h>
#endif

#define SIOCSBPQETHOPT		(SIOCDEVPRIVATE+0)	/* reserved */
#define SIOCSBPQETHADDR		(SIOCDEVPRIVATE+1)
 
struct bpq_ethaddr {
	unsigned char destination[ETH_ALEN];
	unsigned char accept[ETH_ALEN];
};

/* 
 * For SIOCSBPQETHOPT - this is compatible with PI2/PacketTwin card drivers,
 * currently not implemented, though. If someone wants to hook a radio
 * to his Ethernet card he may find this useful. ;-)
 */

#define SIOCGBPQETHPARAM	0x5000  /* get Level 1 parameters */
#define SIOCSBPQETHPARAM	0x5001  /* set */

struct bpq_req  {
    int cmd;
    int speed;			/* unused */
    int clockmode;		/* unused */
    int txdelay;
    unsigned char persist;	/* unused */
    int slotime;		/* unused */
    int squeldelay;
    int dmachan;		/* unused */
    int irq;			/* unused */
};

#endif
