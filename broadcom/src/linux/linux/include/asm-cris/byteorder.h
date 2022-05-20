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
/* $Id: byteorder.h,v 1.1.1.1 2007/01/25 12:52:04 jiahao_jhou Exp $ */

#ifndef _CRIS_BYTEORDER_H
#define _CRIS_BYTEORDER_H

#include <asm/types.h>

#ifdef __GNUC__

/* we just define these two (as we can do the swap in a single
 * asm instruction in CRIS) and the arch-independent files will put
 * them together into ntohl etc.
 */

static __inline__ __const__ __u32 ___arch__swab32(__u32 x)
{
	__asm__ ("swapwb %0" : "=r" (x) : "0" (x));
  
	return(x);
}

static __inline__ __const__ __u16 ___arch__swab16(__u16 x)
{
	__asm__ ("swapb %0" : "=r" (x) : "0" (x));
	
	return(x);
}

/* defines are necessary because the other files detect the presence
 * of a defined __arch_swab32, not an inline
 */

#define __arch__swab32(x) ___arch__swab32(x)
#define __arch__swab16(x) ___arch__swab16(x)

#if !defined(__STRICT_ANSI__) || defined(__KERNEL__)
#  define __BYTEORDER_HAS_U64__
#  define __SWAB_64_THRU_32__
#endif

#endif /* __GNUC__ */

#include <linux/byteorder/little_endian.h>

#endif


