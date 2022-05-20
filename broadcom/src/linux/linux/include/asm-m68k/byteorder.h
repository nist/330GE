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
#ifndef _M68K_BYTEORDER_H
#define _M68K_BYTEORDER_H

#include <asm/types.h>

#ifdef __GNUC__

static __inline__ __const__ __u32 ___arch__swab32(__u32 val)
{
	__asm__("rolw #8,%0; swap %0; rolw #8,%0" : "=d" (val) : "0" (val));
	return val;
}
#define __arch__swab32(x) ___arch__swab32(x)

#endif

#if defined(__GNUC__) && !defined(__STRICT_ANSI__) || defined(__KERNEL__)
#  define __BYTEORDER_HAS_U64__
#  define __SWAB_64_THRU_32__
#endif

#include <linux/byteorder/big_endian.h>

#endif /* _M68K_BYTEORDER_H */
