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
 *	Access to VGA videoram
 *
 *	(c) 1998 Martin Mares <mj@ucw.cz>
 */

#ifndef _LINUX_ASM_VGA_H_
#define _LINUX_ASM_VGA_H_

#include <asm/io.h>

#define VT_BUF_HAVE_RW
#define VT_BUF_HAVE_MEMSETW
#define VT_BUF_HAVE_MEMCPYW

extern inline void scr_writew(u16 val, volatile u16 *addr)
{
	if (__is_ioaddr((unsigned long) addr))
		__raw_writew(val, (unsigned long) addr);
	else
		*addr = val;
}

extern inline u16 scr_readw(volatile const u16 *addr)
{
	if (__is_ioaddr((unsigned long) addr))
		return __raw_readw((unsigned long) addr);
	else
		return *addr;
}

extern inline void scr_memsetw(u16 *s, u16 c, unsigned int count)
{
	if (__is_ioaddr((unsigned long) s))
		memsetw_io(s, c, count);
	else
		memsetw(s, c, count);
}

/* Do not trust that the usage will be correct; analyze the arguments.  */
extern void scr_memcpyw(u16 *d, const u16 *s, unsigned int count);

/* ??? These are currently only used for downloading character sets.  As
   such, they don't need memory barriers.  Is this all they are intended
   to be used for?  */
#define vga_readb	readb
#define vga_writeb	writeb

#define VGA_MAP_MEM(x)	((unsigned long) ioremap((x), 0))

#endif
