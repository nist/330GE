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
 * Generic Broadcom Home Networking Division (HND) PIO engine HW interface
 * This supports the following chips: BCM42xx, 44xx, 47xx .
 *
 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 * $Id: sbhndpio.h,v 1.1.1.1 2007/01/25 12:52:12 jiahao_jhou Exp $
 */

#ifndef	_sbhndpio_h_
#define	_sbhndpio_h_

/* PIO structure,
 *  support two PIO format: 2 bytes access and 4 bytes access
 *  basic FIFO register set is per channel(transmit or receive)
 *  a pair of channels is defined for convenience
 */

/* 2byte-wide pio register set per channel(xmt or rcv) */
typedef volatile struct {
	uint16	fifocontrol;
	uint16	fifodata;
	uint16	fifofree;	/* only valid in xmt channel, not in rcv channel */
	uint16	PAD;
} pio2regs_t;

/* a pair of pio channels(tx and rx) */
typedef volatile struct {
	pio2regs_t	tx;
	pio2regs_t	rx;
} pio2regp_t;

/* 4byte-wide pio register set per channel(xmt or rcv) */
typedef volatile struct {
	uint32	fifocontrol;
	uint32	fifodata;
} pio4regs_t;

/* a pair of pio channels(tx and rx) */
typedef volatile struct {
	pio4regs_t	tx;
	pio4regs_t	rx;
} pio4regp_t;

#endif /* _sbhndpio_h_ */
