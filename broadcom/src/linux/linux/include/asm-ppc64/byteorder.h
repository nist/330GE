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
#ifndef _PPC64_BYTEORDER_H
#define _PPC64_BYTEORDER_H

/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <asm/types.h>

#ifdef __GNUC__

static __inline__ __u16 ld_le16(const volatile __u16 *addr)
{
	__u16 val;

	__asm__ __volatile__ ("lhbrx %0,0,%1" : "=r" (val) : "r" (addr), "m" (*addr));
	return val;
}

static __inline__ void st_le16(volatile __u16 *addr, const __u16 val)
{
	__asm__ __volatile__ ("sthbrx %1,0,%2" : "=m" (*addr) : "r" (val), "r" (addr));
}

static __inline__ __u32 ld_le32(const volatile __u32 *addr)
{
	__u32 val;

	__asm__ __volatile__ ("lwbrx %0,0,%1" : "=r" (val) : "r" (addr), "m" (*addr));
	return val;
}

static __inline__ void st_le32(volatile __u32 *addr, const __u32 val)
{
	__asm__ __volatile__ ("stwbrx %1,0,%2" : "=m" (*addr) : "r" (val), "r" (addr));
}


/* The same, but returns converted value from the location pointer by addr. */
#define __arch__swab16p(addr) ld_le16(addr)
#define __arch__swab32p(addr) ld_le32(addr)

/* The same, but do the conversion in situ, ie. put the value back to addr. */
#define __arch__swab16s(addr) st_le16(addr,*addr)
#define __arch__swab32s(addr) st_le32(addr,*addr)

#endif /* __GNUC__ */

/* MIKEC: What does __BYTEORDER_HAS_U64__ mean? */
#ifndef __STRICT_ANSI__
#define __BYTEORDER_HAS_U64__
#endif

#include <linux/byteorder/big_endian.h>

#endif /* _PPC64_BYTEORDER_H */
