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
 * linux/include/asm-arm/arch-shark/hardware.h
 *
 * by Alexander Schulz
 *
 * derived from:
 * linux/include/asm-arm/arch-ebsa110/hardware.h
 * Copyright (C) 1996-1999 Russell King.
 */
#ifndef __ASM_ARCH_HARDWARE_H
#define __ASM_ARCH_HARDWARE_H

#ifndef __ASSEMBLY__

/*
 * Mapping areas
 */
#define IO_BASE			0xe0000000

/*
 * RAM definitions
 */
#define FLUSH_BASE_PHYS		0x80000000

#else

#define IO_BASE			0

#endif

#define IO_SIZE			0x08000000
#define IO_START		0x40000000
#define ROMCARD_SIZE		0x08000000
#define ROMCARD_START		0x10000000

#define FLUSH_BASE		0xdf000000
#define PCIO_BASE		0xe0000000


/* defines for the Framebuffer */
#define FB_START                0x06000000

#define UNCACHEABLE_ADDR        0xdf010000

#define SEQUOIA_LED_GREEN       (1<<6)
#define SEQUOIA_LED_AMBER       (1<<5)
#define SEQUOIA_LED_BACK        (1<<7)

#define pcibios_assign_all_busses()     1

#define PCIBIOS_MIN_IO          0x6000
#define PCIBIOS_MIN_MEM         0x50000000

#endif

