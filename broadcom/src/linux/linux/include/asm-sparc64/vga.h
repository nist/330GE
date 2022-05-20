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

#include <asm/types.h>

#define VT_BUF_HAVE_RW

static inline void scr_writew(u16 val, u16 *addr)
{
	if ((long) addr < 0)
		*addr = val;
	else
		writew(val, (unsigned long) addr);
}

static inline u16 scr_readw(const u16 *addr)
{
	if ((long) addr < 0)
		return *addr;
	else
		return readw((unsigned long) addr);
}

#define VGA_MAP_MEM(x) (x)

#endif
