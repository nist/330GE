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
 * linux/include/asm-arm/arch-nexuspci/io.h
 *
 * Copyright (C) 1997-1999 Russell King
 * Copyright (C) 2000 FutureTV Labs Ltd.
 */
#ifndef __ASM_ARM_ARCH_IO_H
#define __ASM_ARM_ARCH_IO_H

#define IO_SPACE_LIMIT 0xffff

/*
 * Translation of various region addresses to virtual addresses
 */
#define __io(a)			(PCIO_BASE + (a))
#define __mem_pci(a)		((unsigned long)(a))
#define __mem_isa(a)		(PCIMEM_BASE + (unsigned long)(a))

/*
 * Generic virtual read/write
 */
#define __arch_getw(a)		(*(volatile unsigned short *)(a))
#define __arch_putw(v,a)	(*(volatile unsigned short *)(a) = (v))

/*
 * ioremap support - validate a PCI memory address,
 * and convert a PCI memory address to a physical
 * address for the page tables.
 */
#define iomem_valid_addr(iomem,sz)	\
	((iomem) < 0x80000000 && (iomem) + (sz) <= 0x80000000)
#define iomem_to_phys(iomem)	((iomem) + PLX_MEM_START)

#endif
