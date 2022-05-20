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
 *  linux/include/asm-arm/ide.h
 *
 *  Copyright (C) 1994-1996  Linus Torvalds & authors
 */

/* s390 does not have IDE */

#ifndef __ASMS390_IDE_H
#define __ASMS390_IDE_H

#ifdef __KERNEL__

#ifndef MAX_HWIFS
#define MAX_HWIFS	0
#endif

#define ide__sti()	do {} while (0)

typedef union {
	unsigned all			: 8;	/* all of the bits together */
	struct {
		unsigned head		: 4;	/* always zeros here */
		unsigned unit		: 1;	/* drive select number, 0 or 1 */
		unsigned bit5		: 1;	/* always 1 */
		unsigned lba		: 1;	/* using LBA instead of CHS */
		unsigned bit7		: 1;	/* always 1 */
	} b;
} select_t;

typedef union {
	unsigned all			: 8;	/* all of the bits together */
	struct {
		unsigned bit0		: 1;
		unsigned nIEN		: 1;	/* device INTRQ to host */
		unsigned SRST		: 1;	/* host soft reset bit */
		unsigned bit3		: 1;	/* ATA-2 thingy */
		unsigned reserved456	: 3;
		unsigned HOB		: 1;	/* 48-bit address ordering */
	} b;
} control_t;

#define ide_request_irq(irq,hand,flg,dev,id)	do {} while (0)
#define ide_free_irq(irq,dev_id)		do {} while (0)
#define ide_check_region(from,extent)		do {} while (0)
#define ide_request_region(from,extent,name)	do {} while (0)
#define ide_release_region(from,extent)		do {} while (0)

/*
 * The following are not needed for the non-m68k ports
 */
#define ide_ack_intr(hwif)		(1)
#define ide_fix_driveid(id)		do {} while (0)
#define ide_release_lock(lock)		do {} while (0)
#define ide_get_lock(lock, hdlr, data)	do {} while (0)

/*
 * We always use the new IDE port registering,
 * so these are fixed here.
 */
#define ide_default_io_base(i)		((ide_ioreg_t)0)
#define ide_default_irq(b)		(0)

#endif /* __KERNEL__ */

#endif /* __ASMARM_IDE_H */
