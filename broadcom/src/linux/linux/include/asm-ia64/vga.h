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
#ifndef _ASM_IA64_VGA_H
#define _ASM_IA64_VGA_H

/*
 *	Access to VGA videoram
 *
 *	(c) 1998 Martin Mares <mj@ucw.cz>
 *	(c) 1999 Asit Mallick <asit.k.mallick@intel.com>
 *	(c) 1999 Don Dugger <don.dugger@intel.com>
 *	Copyright (C) 2002 Hewlett-Packard Co
 */
/*
 * 2002/07/19	davidm@hpl.hp.com	Access frame-buffer memory via readX/writeX.
 */

#include <asm/io.h>

#define VT_BUF_HAVE_RW

#define VGA_MAP_MEM(x)	((unsigned long) ioremap((x), 0))

#define vga_readb	__raw_readb
#define vga_writeb	__raw_writeb

extern inline void
scr_writew (u16 val, volatile u16 *addr)
{
	/* Note: ADDR may point to normal memory.  That's OK on ia64.  */
	__raw_writew(val, (unsigned long) addr);
}

extern inline u16
scr_readw (volatile const u16 *addr)
{
	/* Note: ADDR may point to normal memory.  That's OK on ia64.  */
	return __raw_readw((unsigned long) addr);
}

#endif /* _ASM_IA64_VGA_H */
