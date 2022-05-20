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
/* $Id: byteorder.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $ */
#ifndef _SPARC64_BYTEORDER_H
#define _SPARC64_BYTEORDER_H

#include <asm/types.h>
#include <asm/asi.h>

#ifdef __GNUC__

static __inline__ __u16 ___arch__swab16p(__u16 *addr)
{
	__u16 ret;

	__asm__ __volatile__ ("lduha [%1] %2, %0"
			      : "=r" (ret)
			      : "r" (addr), "i" (ASI_PL));
	return ret;
}

static __inline__ __u32 ___arch__swab32p(__u32 *addr)
{
	__u32 ret;

	__asm__ __volatile__ ("lduwa [%1] %2, %0"
			      : "=r" (ret)
			      : "r" (addr), "i" (ASI_PL));
	return ret;
}

static __inline__ __u64 ___arch__swab64p(__u64 *addr)
{
	__u64 ret;

	__asm__ __volatile__ ("ldxa [%1] %2, %0"
			      : "=r" (ret)
			      : "r" (addr), "i" (ASI_PL));
	return ret;
}

#define __arch__swab16p(x) ___arch__swab16p(x)
#define __arch__swab32p(x) ___arch__swab32p(x)
#define __arch__swab64p(x) ___arch__swab64p(x)

#define __BYTEORDER_HAS_U64__

#endif /* __GNUC__ */

#include <linux/byteorder/big_endian.h>

#endif /* _SPARC64_BYTEORDER_H */
