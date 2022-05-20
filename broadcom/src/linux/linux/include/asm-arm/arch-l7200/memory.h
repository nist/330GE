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
 * linux/include/asm-arm/arch-l7200/memory.h
 *
 * Copyright (c) 2000 Steve Hill (sjhill@cotw.com)
 * Copyright (c) 2000 Rob Scott (rscott@mtrob.fdns.net)
 *
 * Changelog:
 *  03-13-2000	SJH	Created
 *  04-13-2000  RS      Changed bus macros for new addr
 *  05-03-2000  SJH     Removed bus macros and fixed virt_to_phys macro
 */
#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

/*
 * Task size: 3GB
 */
#define TASK_SIZE       (0xc0000000UL)
#define TASK_SIZE_26    (0x04000000UL)

/*
 * This decides where the kernel will search for a free chunk of vm
 * space during mmap's.
 */
#define TASK_UNMAPPED_BASE (TASK_SIZE / 3)

/*
 * Page offset: 3GB
 */
#define PAGE_OFFSET     (0xc0000000UL)

/*
 * Physical DRAM offset on the L7200 SDB.
 */
#define PHYS_OFFSET     (0xf0000000UL)

/*
 * The DRAM is contiguous.
 */
#define __virt_to_phys__is_a_macro
#define __virt_to_phys(vpage) ((vpage) - PAGE_OFFSET + PHYS_OFFSET)
#define __phys_to_virt__is_a_macro
#define __phys_to_virt(ppage) ((ppage) + PAGE_OFFSET - PHYS_OFFSET)

#define __virt_to_bus__is_a_macro
#define __virt_to_bus(x) __virt_to_phys(x)
#define __bus_to_virt__is_a_macro
#define __bus_to_virt(x) __phys_to_virt(x)

#define PHYS_TO_NID(addr)	(0)

#endif
