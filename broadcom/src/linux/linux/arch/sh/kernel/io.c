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
 * linux/arch/sh/kernel/io.c
 *
 * Copyright (C) 2000  Stuart Menefy
 *
 * Provide real functions which expand to whatever the header file defined.
 * Also definitions of machine independant IO functions.
 */

#include <asm/io.h>
#include <linux/module.h>

unsigned char _inb(unsigned long port)
{
	return __inb(port);
}
EXPORT_SYMBOL(_inb);

unsigned short _inw(unsigned long port)
{
	return __inw(port);
}
EXPORT_SYMBOL(_inw);

unsigned int _inl(unsigned long port)
{
	return __inl(port);
}
EXPORT_SYMBOL(_inl);

void _outb(unsigned char b, unsigned long port)
{
	__outb(b, port);
}
EXPORT_SYMBOL(_outb);

void _outw(unsigned short b, unsigned long port)
{
	__outw(b, port);
}
EXPORT_SYMBOL(_outw);


void _outl(unsigned int b, unsigned long port)
{
	__outl(b, port);
}
EXPORT_SYMBOL(_outl);


unsigned char _inb_p(unsigned long port)
{
	return __inb_p(port);
}
EXPORT_SYMBOL(_inb_p);

unsigned short _inw_p(unsigned long port)
{
	return __inw_p(port);
}
EXPORT_SYMBOL(_inw_p);


void _outb_p(unsigned char b, unsigned long port)
{
	__outb_p(b, port);
}
EXPORT_SYMBOL(_outb_p);

void _outw_p(unsigned short b, unsigned long port)
{
	__outw_p(b, port);
}
EXPORT_SYMBOL(_outw_p);

void _insb(unsigned long port, void *buffer, unsigned long count)
{
	return __insb(port, buffer, count);
}
EXPORT_SYMBOL(_insb);

void _insw(unsigned long port, void *buffer, unsigned long count)
{
	__insw(port, buffer, count);
}
EXPORT_SYMBOL(_insw);

void _insl(unsigned long port, void *buffer, unsigned long count)
{
	__insl(port, buffer, count);
}
EXPORT_SYMBOL(_insl);

void _outsb(unsigned long port, const void *buffer, unsigned long count)
{
	__outsb(port, buffer, count);
}
EXPORT_SYMBOL(_outsb);

void _outsw(unsigned long port, const void *buffer, unsigned long count)
{
	__outsw(port, buffer, count);
}
EXPORT_SYMBOL(_outsw);

void _outsl(unsigned long port, const void *buffer, unsigned long count)
{
	__outsl(port, buffer, count);

}
EXPORT_SYMBOL(_outsl);

unsigned char ___raw_readb(unsigned long addr)
{
	return __readb(addr);
}
EXPORT_SYMBOL(___raw_readb);

unsigned short ___raw_readw(unsigned long addr)
{
	return __readw(addr);
}
EXPORT_SYMBOL(___raw_readw);

unsigned int ___raw_readl(unsigned long addr)
{
	return __readl(addr);
}
EXPORT_SYMBOL(___raw_readl);

unsigned char _readb(unsigned long addr)
{
	unsigned long r = __readb(addr);
	mb();
	return r;
}
EXPORT_SYMBOL(_readb);

unsigned short _readw(unsigned long addr)
{
	unsigned long r = __readw(addr);
	mb();
	return r;
}
EXPORT_SYMBOL(_readw);

unsigned int _readl(unsigned long addr)
{
	unsigned long r = __readl(addr);
	mb();
	return r;
}
EXPORT_SYMBOL(_readl);

void ___raw_writeb(unsigned char b, unsigned long addr)
{
	__writeb(b, addr);
}

void ___raw_writew(unsigned short b, unsigned long addr)
{
	__writew(b, addr);
}
EXPORT_SYMBOL(___raw_writew);

void ___raw_writel(unsigned int b, unsigned long addr)
{
	__writel(b, addr);
}
EXPORT_SYMBOL(___raw_writel);

void _writeb(unsigned char b, unsigned long addr)
{
	__writeb(b, addr);
	mb();
}
EXPORT_SYMBOL(_writeb);

void _writew(unsigned short b, unsigned long addr)
{
	__writew(b, addr);
	mb();
}
EXPORT_SYMBOL(_writew);

void _writel(unsigned int b, unsigned long addr)
{
	__writel(b, addr);
	mb();
}
EXPORT_SYMBOL(_writel);

/*
 * Copy data from IO memory space to "real" memory space.
 * This needs to be optimized.
 */
void  memcpy_fromio(void * to, unsigned long from, unsigned long count)
{
        while (count) {
                count--;
                *(char *) to = readb(from);
                ((char *) to)++;
                from++;
        }
}
 
/*
 * Copy data from "real" memory space to IO memory space.
 * This needs to be optimized.
 */
void  memcpy_toio(unsigned long to, const void * from, unsigned long count)
{
        while (count) {
                count--;
                writeb(*(char *) from, to);
                ((char *) from)++;
                to++;
        }
}
 
/*
 * "memset" on IO memory space.
 * This needs to be optimized.
 */
void  memset_io(unsigned long dst, int c, unsigned long count)
{
        while (count) {
                count--;
                writeb(c, dst);
                dst++;
        }
}
