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
 * some sbus structures and macros to make usage of sbus drivers possible
 */

#ifndef __M68K_SBUS_H
#define __M68K_SBUS_H

struct linux_sbus_device {
	struct {
		unsigned int which_io;
		unsigned int phys_addr;
	} reg_addrs[1];
};

extern void *sparc_alloc_io (u32, void *, int, char *, u32, int);
#define sparc_alloc_io(a,b,c,d,e,f)	(a)

#define ARCH_SUN4  0

/* sbus IO functions stolen from include/asm-sparc/io.h for the serial driver */
/* No SBUS on the Sun3, kludge -- sam */

extern inline void _sbus_writeb(unsigned char val, unsigned long addr)
{
	*(volatile unsigned char *)addr = val;
}

extern inline unsigned char _sbus_readb(unsigned long addr)
{
	return *(volatile unsigned char *)addr;
}

extern inline void _sbus_writel(unsigned long val, unsigned long addr)
{
	*(volatile unsigned long *)addr = val;

}

#define sbus_readb(a) _sbus_readb((unsigned long)a)
#define sbus_writeb(v, a) _sbus_writeb(v, (unsigned long)a)
#define sbus_writel(v, a) _sbus_writel(v, (unsigned long)a)

#endif
