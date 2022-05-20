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
 *  linux/include/asm-arm/arch-anakin/memory.h
 *
 *  Copyright (C) 2001 Aleph One Ltd. for Acunia N.V.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Changelog:
 *   09-Apr-2001 TTC	Created
 */

#ifndef __ASM_ARCH_MEMORY_H
#define __ASM_ARCH_MEMORY_H

#define TASK_SIZE		(3u * 1024 * 1024 * 1024)
#define TASK_SIZE_26		(64u * 1024 * 1024)
#define TASK_UNMAPPED_BASE	(1u * 1024 * 1024 * 1024)

#define PAGE_OFFSET		0xc0000000
#define PHYS_OFFSET		0x20000000

#define __virt_to_phys(a)	((a) - PAGE_OFFSET + PHYS_OFFSET)
#define __phys_to_virt(a)	((a) + PAGE_OFFSET - PHYS_OFFSET)
#define __virt_to_bus(a)	__virt_to_phys(a)
#define __bus_to_virt(a)	__phys_to_virt(a)

#define __virt_to_phys__is_a_macro
#define __phys_to_virt__is_a_macro
#define __virt_to_bus__is_a_macro
#define __bus_to_virt__is_a_macro

#define PHYS_TO_NID(addr)	(0)

#endif
