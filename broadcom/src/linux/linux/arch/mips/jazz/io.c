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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Low level I/O functions for Jazz family machines.
 *
 * Copyright (C) 1997 by Ralf Baechle.
 */
#include <linux/string.h>
#include <linux/spinlock.h>
#include <asm/addrspace.h>
#include <asm/system.h>
#include <asm/jazz.h>

/*
 * Map an 16mb segment of the EISA address space to 0xe3000000;
 */
static inline void map_eisa_address(unsigned long address)
{
  /* We've got an wired entry in the TLB.  We just need to modify it.
     fast and clean.  But since we want to get rid of wired entries
     things are a little bit more complicated ... */
}

static unsigned char jazz_readb(unsigned long addr)
{
	unsigned char res;

	map_eisa_address(addr);
	addr &= 0xffffff;
	res = *(volatile unsigned char *) (JAZZ_EISA_BASE + addr);

	return res;
}

static unsigned short jazz_readw(unsigned long addr)
{
	unsigned short res;

	map_eisa_address(addr);
	addr &= 0xffffff;
	res = *(volatile unsigned char *) (JAZZ_EISA_BASE + addr);

	return res;
}

static unsigned int jazz_readl(unsigned long addr)
{
	unsigned int res;

	map_eisa_address(addr);
	addr &= 0xffffff;
	res = *(volatile unsigned char *) (JAZZ_EISA_BASE + addr);

	return res;
}

static void jazz_writeb(unsigned char val, unsigned long addr)
{
	map_eisa_address(addr);
	addr &= 0xffffff;
	*(volatile unsigned char *) (JAZZ_EISA_BASE + addr) = val;
}

static void jazz_writew(unsigned short val, unsigned long addr)
{
	map_eisa_address(addr);
	addr &= 0xffffff;
	*(volatile unsigned char *) (JAZZ_EISA_BASE + addr) = val;
}

static void jazz_writel(unsigned int val, unsigned long addr)
{
	map_eisa_address(addr);
	addr &= 0xffffff;
	*(volatile unsigned char *) (JAZZ_EISA_BASE + addr) = val;
}

static void jazz_memset_io(unsigned long addr, int val, unsigned long len)
{
	unsigned long waddr;

	waddr = JAZZ_EISA_BASE | (addr & 0xffffff);
	while(len) {
		unsigned long fraglen;

		fraglen = (~addr + 1) & 0xffffff;
		fraglen = (fraglen < len) ? fraglen : len;
		map_eisa_address(addr);
		memset((char *)waddr, val, fraglen);
		addr += fraglen;
		waddr = waddr + fraglen - 0x1000000;
		len -= fraglen;
	}
}

static void jazz_memcpy_fromio(unsigned long to, unsigned long from, unsigned long len)
{
	unsigned long waddr;

	waddr = JAZZ_EISA_BASE | (from & 0xffffff);
	while(len) {
		unsigned long fraglen;

		fraglen = (~from + 1) & 0xffffff;
		fraglen = (fraglen < len) ? fraglen : len;
		map_eisa_address(from);
		memcpy((void *)to, (void *)waddr, fraglen);
		to += fraglen;
		from += fraglen;
		waddr = waddr + fraglen - 0x1000000;
		len -= fraglen;
	}
}

static void jazz_memcpy_toio(unsigned long to, unsigned long from, unsigned long len)
{
	unsigned long waddr;

	waddr = JAZZ_EISA_BASE | (to & 0xffffff);
	while(len) {
		unsigned long fraglen;

		fraglen = (~to + 1) & 0xffffff;
		fraglen = (fraglen < len) ? fraglen : len;
		map_eisa_address(to);
		memcpy((char *)to + JAZZ_EISA_BASE, (void *)from, fraglen);
		to += fraglen;
		from += fraglen;
		waddr = waddr + fraglen - 0x1000000;
		len -= fraglen;
	}
}
