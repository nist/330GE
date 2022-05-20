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
 *  linux/include/asm-arm/arch-anakin/vmalloc.h
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

#ifndef __ASM_ARCH_VMALLOC_H
#define __ASM_ARCH_VMALLOC_H

/*
 * VMALLOC_ARCH_OFFSET must be set to VMALLOC_OFFSET (check
 * linux/arch/arm/kernel/traps.c)
 */
#define VMALLOC_ARCH_OFFSET	(8 * 1024 * 1024)
#define VMALLOC_VMADDR(a)	((unsigned int) (a))
#define VMALLOC_START		((VMALLOC_VMADDR(high_memory) + VMALLOC_ARCH_OFFSET) & ~(VMALLOC_ARCH_OFFSET - 1))
#define VMALLOC_END		(PAGE_OFFSET + 0x10000000)

#endif
