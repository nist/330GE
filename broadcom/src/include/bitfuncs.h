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
 * bit manipulation utility functions
 *
 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 * $Id: bitfuncs.h,v 1.1.1.1 2007/01/25 12:52:12 jiahao_jhou Exp $
 */

#ifndef _BITFUNCS_H
#define _BITFUNCS_H

#include <typedefs.h>

/* local prototypes */
static INLINE uint32 find_msbit(uint32 x);


/*
 * find_msbit: returns index of most significant set bit in x, with index
 *   range defined as 0-31.  NOTE: returns zero if input is zero.
 */

#if defined(USE_PENTIUM_BSR) && defined(__GNUC__)

/*
 * Implementation for Pentium processors and gcc.  Note that this
 * instruction is actually very slow on some processors (e.g., family 5,
 * model 2, stepping 12, "Pentium 75 - 200"), so we use the generic
 * implementation instead.
 */
static INLINE uint32 find_msbit(uint32 x)
{
	uint msbit;
	__asm__("bsrl %1,%0"
	        :"=r" (msbit)
	        :"r" (x));
	return msbit;
}

#else	/* !USE_PENTIUM_BSR || !__GNUC__ */

/*
 * Generic Implementation
 */

#define DB_POW_MASK16	0xffff0000
#define DB_POW_MASK8	0x0000ff00
#define DB_POW_MASK4	0x000000f0
#define DB_POW_MASK2	0x0000000c
#define DB_POW_MASK1	0x00000002

static INLINE uint32 find_msbit(uint32 x)
{
	uint32 temp_x = x;
	uint msbit = 0;
	if (temp_x & DB_POW_MASK16) {
		temp_x >>= 16;
		msbit = 16;
	}
	if (temp_x & DB_POW_MASK8) {
		temp_x >>= 8;
		msbit += 8;
	}
	if (temp_x & DB_POW_MASK4) {
		temp_x >>= 4;
		msbit += 4;
	}
	if (temp_x & DB_POW_MASK2) {
		temp_x >>= 2;
		msbit += 2;
	}
	if (temp_x & DB_POW_MASK1) {
		msbit += 1;
	}
	return (msbit);
}

#endif	/* USE_PENTIUM_BSR && __GNUC__ */

#endif /* _BITFUNCS_H */
