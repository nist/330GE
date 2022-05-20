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
#include <linux/config.h>
#include <linux/types.h>

#include <asm/io.h>

/*
 * Copy data from IO memory space to "real" memory space.
 * This needs to be optimized.
 */
void
__ia64_memcpy_fromio (void * to, unsigned long from, long count)
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
void
__ia64_memcpy_toio (unsigned long to, void * from, long count)
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
void
__ia64_memset_c_io (unsigned long dst, unsigned long c, long count)
{
	unsigned char ch = (char)(c & 0xff);

	while (count) {
		count--;
		writeb(ch, dst);
		dst++;
	}
}

#ifdef CONFIG_IA64_GENERIC

unsigned int
ia64_inb (unsigned long port)
{
	return __ia64_inb(port);
}

unsigned int
ia64_inw (unsigned long port)
{
	return __ia64_inw(port);
}

unsigned int
ia64_inl (unsigned long port)
{
	return __ia64_inl(port);
}

void
ia64_outb (unsigned char val, unsigned long port)
{
	__ia64_outb(val, port);
}

void
ia64_outw (unsigned short val, unsigned long port)
{
	__ia64_outw(val, port);
}

void
ia64_outl (unsigned int val, unsigned long port)
{
	__ia64_outl(val, port);
}

/* define aliases: */

asm (".global __ia64_inb, __ia64_inw, __ia64_inl");
asm ("__ia64_inb = ia64_inb");
asm ("__ia64_inw = ia64_inw");
asm ("__ia64_inl = ia64_inl");

asm (".global __ia64_outb, __ia64_outw, __ia64_outl");
asm ("__ia64_outb = ia64_outb");
asm ("__ia64_outw = ia64_outw");
asm ("__ia64_outl = ia64_outl");

#endif /* CONFIG_IA64_GENERIC */
