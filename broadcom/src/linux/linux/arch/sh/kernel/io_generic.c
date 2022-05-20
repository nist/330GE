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
/* $Id: io_generic.c,v 1.1.1.1 2007/01/25 12:51:51 jiahao_jhou Exp $
 *
 * linux/arch/sh/kernel/io_generic.c
 *
 * Copyright (C) 2000  Niibe Yutaka
 *
 * Generic I/O routine. These can be used where a machine specific version
 * is not required.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 */

#include <asm/io.h>
#include <asm/machvec.h>
#include <linux/module.h>

#if defined(__sh3__)
/* I'm not sure SH7709 has this kind of bug */
#define SH3_PCMCIA_BUG_WORKAROUND 1
#define DUMMY_READ_AREA6	  0xba000000
#endif

#define PORT2ADDR(x) (sh_mv.mv_isa_port2addr(x))

unsigned long generic_io_base;

static inline void delay(void)
{
	ctrl_inw(0xa0000000);
}

unsigned char generic_inb(unsigned long port)
{
	return *(volatile unsigned char*)PORT2ADDR(port);
}

unsigned short generic_inw(unsigned long port)
{
	return *(volatile unsigned short*)PORT2ADDR(port);
}

unsigned int generic_inl(unsigned long port)
{
	return *(volatile unsigned long*)PORT2ADDR(port);
}

unsigned char generic_inb_p(unsigned long port)
{
	unsigned long v = *(volatile unsigned char*)PORT2ADDR(port);

	delay();
	return v;
}

unsigned short generic_inw_p(unsigned long port)
{
	unsigned long v = *(volatile unsigned short*)PORT2ADDR(port);

	delay();
	return v;
}

unsigned int generic_inl_p(unsigned long port)
{
	unsigned long v = *(volatile unsigned long*)PORT2ADDR(port);

	delay();
	return v;
}

void generic_insb(unsigned long port, void *buffer, unsigned long count)
{
	unsigned char *buf=buffer;
	while(count--) *buf++=inb(port);
}

void generic_insw(unsigned long port, void *buffer, unsigned long count)
{
	unsigned short *buf=buffer;
	while(count--) *buf++=inw(port);
#ifdef SH3_PCMCIA_BUG_WORKAROUND
	ctrl_inb (DUMMY_READ_AREA6);
#endif
}

void generic_insl(unsigned long port, void *buffer, unsigned long count)
{
	unsigned long *buf=buffer;
	while(count--) *buf++=inl(port);
#ifdef SH3_PCMCIA_BUG_WORKAROUND
	ctrl_inb (DUMMY_READ_AREA6);
#endif
}

void generic_outb(unsigned char b, unsigned long port)
{
	*(volatile unsigned char*)PORT2ADDR(port) = b;
}

void generic_outw(unsigned short b, unsigned long port)
{
	*(volatile unsigned short*)PORT2ADDR(port) = b;
}

void generic_outl(unsigned int b, unsigned long port)
{
        *(volatile unsigned long*)PORT2ADDR(port) = b;
}

void generic_outb_p(unsigned char b, unsigned long port)
{
	*(volatile unsigned char*)PORT2ADDR(port) = b;
	delay();
}

void generic_outw_p(unsigned short b, unsigned long port)
{
	*(volatile unsigned short*)PORT2ADDR(port) = b;
	delay();
}

void generic_outl_p(unsigned int b, unsigned long port)
{
	*(volatile unsigned long*)PORT2ADDR(port) = b;
	delay();
}

void generic_outsb(unsigned long port, const void *buffer, unsigned long count)
{
	const unsigned char *buf=buffer;
	while(count--) outb(*buf++, port);
}

void generic_outsw(unsigned long port, const void *buffer, unsigned long count)
{
	const unsigned short *buf=buffer;
	while(count--) outw(*buf++, port);
#ifdef SH3_PCMCIA_BUG_WORKAROUND
	ctrl_inb (DUMMY_READ_AREA6);
#endif
}

void generic_outsl(unsigned long port, const void *buffer, unsigned long count)
{
	const unsigned long *buf=buffer;
	while(count--) outl(*buf++, port);
#ifdef SH3_PCMCIA_BUG_WORKAROUND
	ctrl_inb (DUMMY_READ_AREA6);
#endif
}

unsigned char generic_readb(unsigned long addr)
{
	return *(volatile unsigned char*)addr;
}

unsigned short generic_readw(unsigned long addr)
{
	return *(volatile unsigned short*)addr;
}

unsigned int generic_readl(unsigned long addr)
{
	return *(volatile unsigned long*)addr;
}

void generic_writeb(unsigned char b, unsigned long addr)
{
	*(volatile unsigned char*)addr = b;
}

void generic_writew(unsigned short b, unsigned long addr)
{
	*(volatile unsigned short*)addr = b;
}

void generic_writel(unsigned int b, unsigned long addr)
{
        *(volatile unsigned long*)addr = b;
}

void * generic_ioremap(unsigned long offset, unsigned long size)
{
	return (void *) P2SEGADDR(offset);
}
EXPORT_SYMBOL(generic_ioremap);

void generic_iounmap(void *addr)
{
}
EXPORT_SYMBOL(generic_iounmap);

unsigned long generic_isa_port2addr(unsigned long offset)
{
	return offset + generic_io_base;
}
