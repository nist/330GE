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
 *  linux/include/asm-arm/arch-rpc/hardware.h
 *
 *  Copyright (C) 1996-1999 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  This file contains the hardware definitions of the RiscPC series machines.
 */
#ifndef __ASM_ARCH_HARDWARE_H
#define __ASM_ARCH_HARDWARE_H

#include <asm/arch/memory.h>

/*
 * What hardware must be present
 */
#define HAS_IOMD
#define HAS_VIDC20

/* Hardware addresses of major areas.
 *  *_START is the physical address
 *  *_SIZE  is the size of the region
 *  *_BASE  is the virtual address
 */
#define RAM_SIZE		0x10000000
#define RAM_START		0x10000000

#define EASI_SIZE		0x08000000	/* EASI I/O */
#define EASI_START		0x08000000
#define EASI_BASE		0xe5000000

#define IO_START		0x03000000	/* I/O */
#define IO_SIZE			0x01000000
#define IO_BASE			0xe0000000

#define SCREEN_START		0x02000000	/* VRAM */
#define SCREEN_END		0xdfc00000
#define SCREEN_BASE		0xdf800000

#define FLUSH_BASE		0xdf000000
#define UNCACHEABLE_ADDR	0xdf010000

/*
 * IO Addresses
 */
#define VIDC_BASE		0xe0400000
#define EXPMASK_BASE		0xe0360000
#define IOMD_BASE		0xe0200000
#define IOC_BASE		0xe0200000
#define PCIO_BASE		0xe0010000
#define FLOPPYDMA_BASE		0xe002a000

#define FLUSH_BASE_PHYS		0x00000000	/* ROM */

#define vidc_writel(val)	__raw_writel(val, VIDC_BASE)

#define IO_EC_EASI_BASE		0x81400000
#define IO_EC_IOC4_BASE		0x8009c000
#define IO_EC_IOC_BASE		0x80090000
#define IO_EC_MEMC8_BASE	0x8000ac00
#define IO_EC_MEMC_BASE		0x80000000

#define	EXPMASK_STATUS		(EXPMASK_BASE + 0x00)
#define EXPMASK_ENABLE		(EXPMASK_BASE + 0x04)

#endif