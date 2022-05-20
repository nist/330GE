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
#ifndef _X86_64_BYTEORDER_H
#define _X86_64_BYTEORDER_H

#include <asm/types.h>

#ifdef __GNUC__

static __inline__ __const__ __u64 ___arch__swab64(__u64 x)
{
	__asm__("bswapq %0" : "=r" (x) : "0" (x));
	return x;
}

static __inline__ __const__ __u32 ___arch__swab32(__u32 x)
{
	__asm__("bswapl %0" : "=r" (x) : "0" (x));
	return x;
}

/* Do not define swab16.  Gcc is smart enought to recognize "C" version and
   convert it into rotation or exhange.  */

#define __arch__swab32(x) ___arch__swab32(x)
#define __arch__swab64(x) ___arch__swab64(x)

#endif /* __GNUC__ */

#define __BYTEORDER_HAS_U64__

#include <linux/byteorder/little_endian.h>

#endif /* _X86_64_BYTEORDER_H */
