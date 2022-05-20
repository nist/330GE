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
 * Common [OS-independent] header file for
 * Broadcom BCM47XX 10/100Mbps Ethernet Device Driver
 *
 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.                
 *                                     
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ASUSTek Inc.;   
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior      
 * written permission of ASUSTek Inc..                            
 *
 * $Id$
 */

#ifndef _etc_h_
#define _etc_h_

#include <etioctl.h>

#define	MAXMULTILIST	32

#ifndef ch_t
#define	ch_t void
#endif

struct etc_info;	/* forward declaration */
struct bcmstrbuf;	/* forward declaration */

/* each chip type supports a set of chip-type-specific ops */
struct chops {
	bool (*id)(uint vendor, uint device);		/* return true if match */
	void *(*attach)(struct etc_info *etc, void *dev, void *regs);
	void (*detach)(ch_t *ch);			/* free chip private state */
	void (*reset)(ch_t *ch);			/* chip reset */
	void (*init)(ch_t *ch, uint options);		/* chip init */
	bool (*tx)(ch_t *ch, void *p);			/* transmit frame */
	void *(*rx)(ch_t *ch);				/* receive frame */
	void (*rxfill)(ch_t *ch);			/* post dma rx buffers */
	int (*getintrevents)(ch_t *ch, bool in_isr);	/* return intr events */
	bool (*errors)(ch_t *ch);			/* handle chip errors */
	void (*intrson)(ch_t *ch);			/* enable chip interrupts */
	void (*intrsoff)(ch_t *ch);			/* disable chip interrupts */
	void (*txreclaim)(ch_t *ch, bool all);		/* reclaim transmit resources */
	void (*rxreclaim)(ch_t *ch);			/* reclaim receive resources */
	void (*statsupd)(ch_t *ch);			/* update sw stat counters */
	void (*enablepme)(ch_t *ch);			/* enable PME */
	void (*disablepme)(ch_t *ch);			/* disable PME */
	void (*phyreset)(ch_t *ch, uint phyaddr);	/* reset phy */
	uint16 (*phyrd)(ch_t *ch, uint phyaddr, uint reg);	/* read phy register */
	void (*phywr)(ch_t *ch, uint phyaddr, uint reg, uint16 val);	/* write phy register */
	void (*dump)(ch_t *ch, struct bcmstrbuf *b);		/* debugging output */
	void (*longname)(ch_t *ch, char *buf, uint bufsize);	/* return descriptive name */
	void (*duplexupd)(ch_t *ch);			/* keep mac duplex consistent */
};

/*
 * "Common" os-independent software state structure.
 */
typedef struct etc_info {
	void		*et;		/* pointer to os-specific private state */
	uint		unit;		/* device instance number */
	void 		*osh; 		/* pointer to os handler */
	bool		up;		/* interface up and running */
	bool		promisc;	/* promiscuous destination address */
	bool		qos;		/* QoS priority determination on rx */
	bool		loopbk;		/* loopback override mode */

	int		forcespeed;	/* disable autonegotiation and force speed/duplex */
	uint		advertise;	/* control speed/duplex advertised capability bits */
	bool		needautoneg;	/* request restart autonegotiation */
	int		speed;		/* current speed: 10, 100 */
	int		duplex;		/* current duplex: 0=half, 1=full */

	bool		piomode;	/* enable programmed io (!dma) */
	void		*pioactive;	/* points to pio packet being transmitted */
	volatile uint	*txavail;	/* dma: # tx descriptors available */

	uint16		vendorid;	/* pci function vendor id */
	uint16		deviceid;	/* pci function device id */
	uint		chip;		/* chip number */
	uint		chiprev;	/* chip revision */

	bool		nicmode;	/* is this core using its own pci i/f */

	struct chops	*chops;		/* pointer to chip-specific opsvec */
	void		*ch;		/* pointer to chip-specific state */
	void		*robo;		/* optional robo private data */

	uint		coreunit;	/* sb chips: chip enet instance # */
	uint		phyaddr;	/* sb chips: mdio 5-bit phy address */
	uint		mdcport;	/* sb chips: which mii to use (enet core #) to access phy */

	struct ether_addr cur_etheraddr; /* our local ethernet address */
	struct ether_addr perm_etheraddr; /* original sprom local ethernet address */

	struct ether_addr multicast[MAXMULTILIST];
	uint		nmulticast;
	bool		allmulti;	/* enable all multicasts */

	bool		linkstate;	/* link integrity state */
	bool		pm_modechange;	/* true if mode change is to due pm */

	uint32		now;		/* elapsed seconds */

	uint32		boardflags;	/* board flags */

	/* chip-maintained plus a few sw-maintained stat counters */
	bcmenetmib_t	mib;		/* mib statistic counters */
	uint32		txframe;	/* transmitted frames */
	uint32		txbyte;		/* transmitted bytes */
	uint32		rxframe;	/* received frames */
	uint32		rxbyte;		/* received bytes */
	uint32		txerror;	/* total tx errors */
	uint32		txnobuf;	/* tx out-of-buffer errors */
	uint32		rxerror;	/* total rx errors */
	uint32		rxnobuf;	/* rx out-of-buffer errors */
	uint32		reset;		/* reset count */
	uint32		dmade;		/* pci descriptor errors */
	uint32		dmada;		/* pci data errors */
	uint32		dmape;		/* descriptor protocol error */
	uint32		rxdmauflo;	/* receive descriptor underflow */
	uint32		rxoflo;		/* receive fifo overflow */
	uint32		txuflo;		/* transmit fifo underflow */
	uint32		rxbadlen;	/* 802.3 len field != read length */
} etc_info_t;

/* interrupt event bitvec */
#define	INTR_TX		0x1
#define	INTR_RX		0x2
#define	INTR_ERROR	0x4
#define	INTR_TO		0x8
#define	INTR_NEW	0x10

/* forcespeed values */
#define	ET_AUTO		-1
#define	ET_10HALF	0
#define	ET_10FULL	1
#define	ET_100HALF	2
#define	ET_100FULL	3

/* init options */
#define ET_INIT_FULL     0x1
#define ET_INIT_INTRON   0x2

/* Specific init options for et_init */
#define ET_INIT_DEF_OPTIONS   (ET_INIT_FULL | ET_INIT_INTRON)
#define ET_INIT_INTROFF       (ET_INIT_FULL)
#define ET_INIT_PARTIAL      (0)

/* macro to safely clear the UP flag */
#define ET_FLAG_DOWN(x)   (*(x)->chops->intrsoff)((x)->ch);  \
			  (x)->up = FALSE;

/*
 * Least-common denominator rxbuf start-of-data offset:
 * Must be >= size of largest rxhdr
 * Must be 2-mod-4 aligned so IP is 0-mod-4
 */
#define	HWRXOFF		30

/* exported prototypes */
extern struct chops *etc_chipmatch(uint vendor, uint device);
extern void *etc_attach(void *et, uint vendor, uint device, uint unit, void *dev, void *regsva);
extern void etc_detach(etc_info_t *etc);
extern void etc_reset(etc_info_t *etc);
extern void etc_init(etc_info_t *etc, uint options);
extern void etc_up(etc_info_t *etc);
extern uint etc_down(etc_info_t *etc, int reset);
extern int etc_ioctl(etc_info_t *etc, int cmd, void *arg);
extern void etc_promisc(etc_info_t *etc, uint on);
extern void etc_qos(etc_info_t *etc, uint on);
extern void etc_dump(etc_info_t *etc, struct bcmstrbuf *b);
extern void etc_watchdog(etc_info_t *etc);
extern uint etc_totlen(etc_info_t *etc, void *p);

#endif	/* _etc_h_ */