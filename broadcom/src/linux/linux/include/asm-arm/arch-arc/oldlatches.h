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
 *  linux/include/asm-arm/arch-arc/oldlatches.h
 *
 *  Copyright (C) 1996 Russell King, Dave Gilbert
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Modifications:
 *   04-04-1998	PJB/RMK	Merged arc and a5k versions
 */
#ifndef _ASM_ARCH_OLDLATCH_H
#define _ASM_ARCH_OLDLATCH_H

#define LATCHA_FDSEL0    (1<<0)
#define LATCHA_FDSEL1    (1<<1)
#define LATCHA_FDSEL2    (1<<2)
#define LATCHA_FDSEL3    (1<<3)
#define LATCHA_FDSELALL  (0xf)
#define LATCHA_SIDESEL   (1<<4)
#define LATCHA_MOTOR     (1<<5)
#define LATCHA_INUSE     (1<<6)
#define LATCHA_CHANGERST (1<<7)

#define LATCHB_FDCDENSITY  (1<<1)
#define LATCHB_FDCRESET    (1<<3)
#define LATCHB_PRINTSTROBE (1<<4)

/* newval=(oldval & mask)|newdata */
void oldlatch_bupdate(unsigned char mask,unsigned char newdata);

/* newval=(oldval & mask)|newdata */
void oldlatch_aupdate(unsigned char mask,unsigned char newdata);

#endif

