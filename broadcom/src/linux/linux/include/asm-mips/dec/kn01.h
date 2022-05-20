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
 * Hardware info about DECstation DS2100/3100 systems (otherwise known as
 * pmin/pmax or KN01).
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1995,1996 by Paul M. Antoine, some code and definitions
 * are by courtesy of Chris Fraser.
 * Copyright (C) 2002  Maciej W. Rozycki
 */
#ifndef __ASM_MIPS_DEC_KN01_H
#define __ASM_MIPS_DEC_KN01_H

#include <asm/addrspace.h>

/*
 * Some port addresses...
 */
#define KN01_SLOT_SIZE	0x01000000

#define KN01_LANCE_BASE (KSEG1ADDR(0x18000000)) /* 0xB8000000 */
#define KN01_DZ11_BASE	(KSEG1ADDR(0x1c000000)) /* 0xBC000000 */
#define KN01_RTC_BASE	(KSEG1ADDR(0x1d000000)) /* 0xBD000000 */


/*
 * CPU interrupt bits.
 */
#define KN01_CPU_INR_MEMORY	6	/* memory, I/O bus write errors */
#define KN01_CPU_INR_VIDEO	6	/* framebuffer */
#define KN01_CPU_INR_RTC	5	/* DS1287 RTC */
#define KN01_CPU_INR_DZ11	4	/* DZ11 (DC7085) serial */
#define KN01_CPU_INR_LANCE	3	/* LANCE (Am7990) Ethernet */
#define KN01_CPU_INR_SII	2	/* SII (DC7061) SCSI */

#endif /* __ASM_MIPS_DEC_KN01_H */
