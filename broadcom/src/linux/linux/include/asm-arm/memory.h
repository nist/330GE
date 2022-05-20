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
 *  linux/include/asm-arm/memory.h
 *
 *  Copyright (C) 2000 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Note: this file should not be included by non-asm/.h files
 *
 *  Modifications:
 */
#ifndef __ASM_ARM_MEMORY_H
#define __ASM_ARM_MEMORY_H

#include <asm/arch/memory.h>

static inline unsigned long virt_to_phys(volatile void *x)
{
	return __virt_to_phys((unsigned long)(x));
}

static inline void *phys_to_virt(unsigned long x)
{
	return (void *)(__phys_to_virt((unsigned long)(x)));
}

/*
 * Virtual <-> DMA view memory address translations
 */
#define virt_to_bus(x)		(__virt_to_bus((unsigned long)(x)))
#define bus_to_virt(x)		((void *)(__bus_to_virt((unsigned long)(x))))

#endif
