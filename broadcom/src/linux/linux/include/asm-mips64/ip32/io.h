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
#ifndef __ASM_IP32_IO_H__
#define __ASM_IP32_IO_H__

#include <asm/ip32/mace.h>

/*#ifdef CONFIG_MIPS_UNCACHED*/
#define UNCACHEDADDR(x) (0x9000000000000000UL | (u64)(x))
/*#else
#define UNCACHEDADDR(x) (x)
#endif*/
/*#define UNCACHEDADDR(x) (KSEG1ADDR (x)) */
#define IO_SPACE_BASE UNCACHEDADDR (MACEPCI_HI_MEMORY)
#define IO_SPACE_LIMIT 0xffffffffUL

#endif
