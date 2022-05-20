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
 * Hardware info common to DECstation 5000/1xx systems (otherwise
 * known as 3min or kn02ba) and Personal DECstations 5000/xx ones
 * (otherwise known as maxine or kn02ca).
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1995,1996 by Paul M. Antoine, some code and definitions
 * are by courtesy of Chris Fraser.
 * Copyright (C) 2000, 2002  Maciej W. Rozycki
 *
 * These are addresses which have to be known early in the boot process.
 * For other addresses refer to tc.h, ioasic_addrs.h and friends.
 */
#ifndef __ASM_MIPS_DEC_KN02XA_H
#define __ASM_MIPS_DEC_KN02XA_H

#include <asm/addrspace.h>

/*
 * Some port addresses...
 */
#define KN02XA_IOASIC_BASE	KSEG1ADDR(0x1c040000)	/* I/O ASIC */
#define KN02XA_RTC_BASE		KSEG1ADDR(0x1c200000)	/* RTC */

#endif /* __ASM_MIPS_DEC_KN02XA_H */
