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
#ifndef _ASM_IA64_BYTEORDER_H
#define _ASM_IA64_BYTEORDER_H

/*
 * Copyright (C) 1998, 1999 Hewlett-Packard Co
 * Copyright (C) 1998, 1999 David Mosberger-Tang <davidm@hpl.hp.com>
 */

#include <asm/types.h>

static __inline__ __const__ __u64
__ia64_swab64 (__u64 x)
{
	__u64 result;

	__asm__ ("mux1 %0=%1,@rev" : "=r" (result) : "r" (x));
	return result;
}

static __inline__ __const__ __u32
__ia64_swab32 (__u32 x)
{
	return __ia64_swab64(x) >> 32;
}

static __inline__ __const__ __u16
__ia64_swab16(__u16 x)
{
	return __ia64_swab64(x) >> 48;
}

#define __arch__swab64(x) __ia64_swab64(x)
#define __arch__swab32(x) __ia64_swab32(x)
#define __arch__swab16(x) __ia64_swab16(x)

#define __BYTEORDER_HAS_U64__

#include <linux/byteorder/little_endian.h>

#endif /* _ASM_IA64_BYTEORDER_H */
