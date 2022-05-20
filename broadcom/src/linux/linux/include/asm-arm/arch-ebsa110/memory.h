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
 *  linux/include/asm-arm/arch-ebsa110/memory.h
 *
 *  Copyright (C) 1996-1999 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Changelog:
 *   20-Oct-1996 RMK	Created
 *   31-Dec-1997 RMK	Fixed definitions to reduce warnings
 *   21-Mar-1999 RMK	Renamed to memory.h
 *		 RMK	Moved TASK_SIZE and PAGE_OFFSET here
 */
#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

/*
 * Task size: 3GB
 */
#define TASK_SIZE	(0xc0000000UL)
#define TASK_SIZE_26	(0x04000000UL)

/*
 * This decides where the kernel will search for a free chunk of vm
 * space during mmap's.
 */
#define TASK_UNMAPPED_BASE (TASK_SIZE / 3)

/*
 * Page offset: 3GB
 */
#define PAGE_OFFSET	(0xc0000000UL)
#define PHYS_OFFSET	(0x00000000UL)

#define __virt_to_phys__is_a_macro
#define __virt_to_phys(vpage)	((vpage) - PAGE_OFFSET)
#define __phys_to_virt__is_a_macro
#define __phys_to_virt(ppage)	((ppage) + PAGE_OFFSET)

/*
 * We keep this 1:1 so that we don't interfere
 * with the PCMCIA memory regions
 */
#define __virt_to_bus__is_a_macro
#define __virt_to_bus(x)	(x)
#define __bus_to_virt__is_a_macro
#define __bus_to_virt(x)	(x)

#define PHYS_TO_NID(addr)	(0)

#endif
