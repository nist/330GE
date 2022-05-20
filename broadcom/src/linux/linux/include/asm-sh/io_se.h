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
 * include/asm-sh/io_se.h
 *
 * Copyright 2000 Stuart Menefy (stuart.menefy@st.com)
 *
 * May be copied or modified under the terms of the GNU General Public
 * License.  See linux/COPYING for more information.
 *
 * IO functions for an Hitachi SolutionEngine
 */

#ifndef _ASM_SH_IO_SE_H
#define _ASM_SH_IO_SE_H

#include <asm/io_generic.h>

extern unsigned char se_inb(unsigned long port);
extern unsigned short se_inw(unsigned long port);
extern unsigned int se_inl(unsigned long port);

extern void se_outb(unsigned char value, unsigned long port);
extern void se_outw(unsigned short value, unsigned long port);
extern void se_outl(unsigned int value, unsigned long port);

extern unsigned char se_inb_p(unsigned long port);
extern void se_outb_p(unsigned char value, unsigned long port);

extern void se_insb(unsigned long port, void *addr, unsigned long count);
extern void se_insw(unsigned long port, void *addr, unsigned long count);
extern void se_insl(unsigned long port, void *addr, unsigned long count);
extern void se_outsb(unsigned long port, const void *addr, unsigned long count);
extern void se_outsw(unsigned long port, const void *addr, unsigned long count);
extern void se_outsl(unsigned long port, const void *addr, unsigned long count);

extern unsigned char se_readb(unsigned long addr);
extern unsigned short se_readw(unsigned long addr);
extern unsigned int se_readl(unsigned long addr);
extern void se_writeb(unsigned char b, unsigned long addr);
extern void se_writew(unsigned short b, unsigned long addr);
extern void se_writel(unsigned int b, unsigned long addr);

extern unsigned long se_isa_port2addr(unsigned long offset);

#ifdef __WANT_IO_DEF

# define __inb			se_inb
# define __inw			se_inw
# define __inl			se_inl
# define __outb			se_outb
# define __outw			se_outw
# define __outl			se_outl

# define __inb_p		se_inb_p
# define __inw_p		se_inw
# define __inl_p		se_inl
# define __outb_p		se_outb_p
# define __outw_p		se_outw
# define __outl_p		se_outl

# define __insb			se_insb
# define __insw			se_insw
# define __insl			se_insl
# define __outsb		se_outsb
# define __outsw		se_outsw
# define __outsl		se_outsl

# define __readb		se_readb
# define __readw		se_readw
# define __readl		se_readl
# define __writeb		se_writeb
# define __writew		se_writew
# define __writel		se_writel

# define __isa_port2addr	se_isa_port2addr
# define __ioremap		generic_ioremap
# define __iounmap		generic_iounmap

#endif

#endif /* _ASM_SH_IO_SE_H */
