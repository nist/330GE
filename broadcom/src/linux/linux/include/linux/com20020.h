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
/*
 * Linux ARCnet driver - COM20020 chipset support - function declarations
 * 
 * Written 1997 by David Woodhouse.
 * Written 1994-1999 by Avery Pennarun.
 * Derived from skeleton.c by Donald Becker.
 *
 * Special thanks to Contemporary Controls, Inc. (www.ccontrols.com)
 *  for sponsoring the further development of this driver.
 *
 * **********************
 *
 * The original copyright of skeleton.c was as follows:
 *
 * skeleton.c Written 1993 by Donald Becker.
 * Copyright 1993 United States Government as represented by the
 * Director, National Security Agency.  This software may only be used
 * and distributed according to the terms of the GNU General Public License as
 * modified by SRC, incorporated herein by reference.
 *
 * **********************
 *
 * For more details, see drivers/net/arcnet.c
 *
 * **********************
 */
#ifndef __COM20020_H
#define __COM20020_H

int com20020_check(struct net_device *dev);
int com20020_found(struct net_device *dev, int shared);
void com20020_remove(struct net_device *dev);

/* The number of low I/O ports used by the card. */
#define ARCNET_TOTAL_SIZE 9

/* various register addresses */
#define _INTMASK  (ioaddr+0)	/* writable */
#define _STATUS   (ioaddr+0)	/* readable */
#define _COMMAND  (ioaddr+1)	/* standard arcnet commands */
#define _DIAGSTAT (ioaddr+1)	/* diagnostic status register */
#define _ADDR_HI  (ioaddr+2)	/* control registers for IO-mapped memory */
#define _ADDR_LO  (ioaddr+3)
#define _MEMDATA  (ioaddr+4)	/* data port for IO-mapped memory */
#define _SUBADR   (ioaddr+5)	/* the extended port _XREG refers to */
#define _CONFIG   (ioaddr+6)	/* configuration register */
#define _XREG     (ioaddr+7)	/* extra registers (indexed by _CONFIG 
					or _SUBADR) */

/* in the ADDR_HI register */
#define RDDATAflag	0x80	/* next access is a read (not a write) */

/* in the DIAGSTAT register */
#define NEWNXTIDflag	0x02	/* ID to which token is passed has changed */

/* in the CONFIG register */
#define RESETcfg	0x80	/* put card in reset state */
#define TXENcfg		0x20	/* enable TX */

/* in SETUP register */
#define PROMISCset	0x10	/* enable RCV_ALL */

/* COM2002x */
#define SUB_TENTATIVE	0	/* tentative node ID */
#define SUB_NODE	1	/* node ID */
#define SUB_SETUP1	2	/* various options */
#define SUB_TEST	3	/* test/diag register */

/* COM20022 only */
#define SUB_SETUP2	4	/* sundry options */
#define SUB_BUSCTL	5	/* bus control options */
#define SUB_DMACOUNT	6	/* DMA count options */

#define SET_SUBADR(x) do { \
	if ((x) < 4) \
	{ \
		lp->config = (lp->config & ~0x03) | (x); \
		SETCONF; \
	} \
	else \
	{ \
		outb(x, _SUBADR); \
	} \
} while (0)

#undef ARCRESET
#undef ASTATUS
#undef ACOMMAND
#undef AINTMASK

#define ARCRESET { outb(lp->config | 0x80, _CONFIG); \
		    udelay(5);                        \
		    outb(lp->config , _CONFIG);       \
                  }
#define ARCRESET0 { outb(0x18 | 0x80, _CONFIG);   \
		    udelay(5);                       \
		    outb(0x18 , _CONFIG);            \
                  }

#define ASTATUS()	inb(_STATUS)
#define ACOMMAND(cmd)	outb((cmd),_COMMAND)
#define AINTMASK(msk)	outb((msk),_INTMASK)

#define SETCONF		outb(lp->config, _CONFIG)

#endif /* __COM20020_H */