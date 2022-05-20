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
/* $Id: PeeCeeI.c,v 1.1.1.1 2007/01/25 12:51:51 jiahao_jhou Exp $
 * PeeCeeI.c: The emerging standard...
 *
 * Copyright (C) 1997 David S. Miller (davem@caip.rutgers.edu)
 */

#include <linux/config.h>

#ifdef CONFIG_PCI

#include <asm/io.h>
#include <asm/byteorder.h>

void outsb(unsigned long addr, const void *src, unsigned long count)
{
	const u8 *p = src;

	while(count--)
		outb(*p++, addr);
}

void outsw(unsigned long addr, const void *src, unsigned long count)
{
	if(count) {
		u16 *ps = (u16 *)src;
		u32 *pi;

		if(((u64)src) & 0x2) {
			u16 val = le16_to_cpup(ps);
			outw(val, addr);
			ps++;
			count--;
		}
		pi = (u32 *)ps;
		while(count >= 2) {
			u32 w = le32_to_cpup(pi);

			pi++;
			outw(w >> 0, addr);
			outw(w >> 16, addr);
			count -= 2;
		}
		ps = (u16 *)pi;
		if(count) {
			u16 val = le16_to_cpup(ps);
			outw(val, addr);
		}
	}
}

void outsl(unsigned long addr, const void *src, unsigned long count)
{
	if(count) {
		if((((u64)src) & 0x3) == 0) {
			u32 *p = (u32 *)src;
			while(count--) {
				u32 val = cpu_to_le32p(p);
				outl(val, addr);
				p++;
			}
		} else {
			u8 *pb;
			u16 *ps = (u16 *)src;
			u32 l = 0, l2;
			u32 *pi;

			switch(((u64)src) & 0x3) {
			case 0x2:
				count -= 1;
				l = cpu_to_le16p(ps) << 16;
				ps++;
				pi = (u32 *)ps;
				while(count--) {
					l2 = cpu_to_le32p(pi);
					pi++;
					outl(((l >> 16) | (l2 << 16)), addr);
					l = l2;
				}
				ps = (u16 *)pi;
				l2 = cpu_to_le16p(ps);
				outl(((l >> 16) | (l2 << 16)), addr);
				break;

			case 0x1:
				count -= 1;
				pb = (u8 *)src;
				l = (*pb++ << 8);
				ps = (u16 *)pb;
				l2 = cpu_to_le16p(ps);
				ps++;
				l |= (l2 << 16);
				pi = (u32 *)ps;
				while(count--) {
					l2 = cpu_to_le32p(pi);
					pi++;
					outl(((l >> 8) | (l2 << 24)), addr);
					l = l2;
				}
				pb = (u8 *)pi;
				outl(((l >> 8) | (*pb << 24)), addr);
				break;

			case 0x3:
				count -= 1;
				pb = (u8 *)src;
				l = (*pb++ << 24);
				pi = (u32 *)pb;
				while(count--) {
					l2 = cpu_to_le32p(pi);
					pi++;
					outl(((l >> 24) | (l2 << 8)), addr);
					l = l2;
				}
				ps = (u16 *)pi;
				l2 = cpu_to_le16p(ps);
				ps++;
				pb = (u8 *)ps;
				l2 |= (*pb << 16);
				outl(((l >> 24) | (l2 << 8)), addr);
				break;
			}
		}
	}
}

void insb(unsigned long addr, void *dst, unsigned long count)
{
	if(count) {
		u32 *pi;
		u8 *pb = dst;

		while((((unsigned long)pb) & 0x3) && count--)
			*pb++ = inb(addr);
		pi = (u32 *)pb;
		while(count >= 4) {
			u32 w;

			w  = (inb(addr) << 24);
			w |= (inb(addr) << 16);
			w |= (inb(addr) << 8);
			w |= (inb(addr) << 0);
			*pi++ = w;
			count -= 4;
		}
		pb = (u8 *)pi;
		while(count--)
			*pb++ = inb(addr);
	}
}

void insw(unsigned long addr, void *dst, unsigned long count)
{
	if(count) {
		u16 *ps = dst;
		u32 *pi;

		if(((unsigned long)ps) & 0x2) {
			*ps++ = le16_to_cpu(inw(addr));
			count--;
		}
		pi = (u32 *)ps;
		while(count >= 2) {
			u32 w;

			w  = (le16_to_cpu(inw(addr)) << 16);
			w |= (le16_to_cpu(inw(addr)) << 0);
			*pi++ = w;
			count -= 2;
		}
		ps = (u16 *)pi;
		if(count)
			*ps = le16_to_cpu(inw(addr));
	}
}

void insl(unsigned long addr, void *dst, unsigned long count)
{
	if(count) {
		if((((unsigned long)dst) & 0x3) == 0) {
			u32 *pi = dst;
			while(count--)
				*pi++ = le32_to_cpu(inl(addr));
		} else {
			u32 l = 0, l2, *pi;
			u16 *ps;
			u8 *pb;

			switch(((unsigned long)dst) & 3) {
			case 0x2:
				ps = dst;
				count -= 1;
				l = le32_to_cpu(inl(addr));
				*ps++ = l;
				pi = (u32 *)ps;
				while(count--) {
					l2 = le32_to_cpu(inl(addr));
					*pi++ = (l << 16) | (l2 >> 16);
					l = l2;
				}
				ps = (u16 *)pi;
				*ps = l;
				break;

			case 0x1:
				pb = dst;
				count -= 1;
				l = le32_to_cpu(inl(addr));
				*pb++ = l >> 24;
				ps = (u16 *)pb;
				*ps++ = ((l >> 8) & 0xffff);
				pi = (u32 *)ps;
				while(count--) {
					l2 = le32_to_cpu(inl(addr));
					*pi++ = (l << 24) | (l2 >> 8);
					l = l2;
				}
				pb = (u8 *)pi;
				*pb = l;
				break;

			case 0x3:
				pb = (u8 *)dst;
				count -= 1;
				l = le32_to_cpu(inl(addr));
				*pb++ = l >> 24;
				pi = (u32 *)pb;
				while(count--) {
					l2 = le32_to_cpu(inl(addr));
					*pi++ = (l << 8) | (l2 >> 24);
					l = l2;
				}
				ps = (u16 *)pi;
				*ps++ = ((l >> 8) & 0xffff);
				pb = (u8 *)ps;
				*pb = l;
				break;
			}
		}
	}
}

#endif /* CONFIG_PCI */
