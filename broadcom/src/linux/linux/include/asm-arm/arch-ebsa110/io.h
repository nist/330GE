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
 *  linux/include/asm-arm/arch-ebsa110/io.h
 *
 *  Copyright (C) 1997,1998 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *  06-Dec-1997	RMK	Created.
 */
#ifndef __ASM_ARM_ARCH_IO_H
#define __ASM_ARM_ARCH_IO_H

#define IO_SPACE_LIMIT 0xffff

/*
 * Generic virtual read/write
 */
#define __arch_getw(a)		(*(volatile unsigned short *)(a))
#define __arch_putw(v,a)	(*(volatile unsigned short *)(a) = (v))

u8 __inb8(unsigned int port);
void __outb8(u8  val, unsigned int port);

u8 __inb16(unsigned int port);
void __outb16(u8  val, unsigned int port);

u16 __inw(unsigned int port);
void __outw(u16 val, unsigned int port);

u32 __inl(unsigned int port);
void __outl(u32 val, unsigned int port);

u8  __readb(void *addr);
u16 __readw(void *addr);
u32 __readl(void *addr);

void __writeb(u8  val, void *addr);
void __writew(u16 val, void *addr);
void __writel(u32 val, void *addr);

/*
 * Argh, someone forgot the IOCS16 line.  We therefore have to handle
 * the byte stearing by selecting the correct byte IO functions here.
 */
#ifdef ISA_SIXTEEN_BIT_PERIPHERAL
#define inb(p)			__inb16(p)
#define outb(v,p)		__outb16(v,p)
#else
#define inb(p)			__inb8(p)
#define outb(v,p)		__outb8(v,p)
#endif

#define inw(p)			__inw(p)
#define outw(v,p)		__outw(v,p)

#define inl(p)			__inl(p)
#define outl(v,p)		__outl(v,p)

#define readb(b)		__readb(b)
#define readw(b)		__readw(b)
#define readl(b)		__readl(b)

#define writeb(v,b)		__writeb(v,b)
#define writew(v,b)		__writew(v,b)
#define writel(v,b)		__writel(v,b)

#define __arch_ioremap(off,sz,c)	((void *)(off))
#define __arch_iounmap(virt)		do { } while (0)

extern void insb(unsigned int port, void *buf, int sz);
extern void insw(unsigned int port, void *buf, int sz);
extern void insl(unsigned int port, void *buf, int sz);

extern void outsb(unsigned int port, const void *buf, int sz);
extern void outsw(unsigned int port, const void *buf, int sz);
extern void outsl(unsigned int port, const void *buf, int sz);

#endif
