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
#ifndef _ALPHA_BYTEORDER_H
#define _ALPHA_BYTEORDER_H

#include <asm/types.h>

#ifdef __GNUC__

static __inline __u32 __attribute__((__const)) __arch__swab32(__u32 x)
{
	/*
	 * Unfortunately, we can't use the 6 instruction sequence
	 * on ev6 since the latency of the UNPKBW is 3, which is
	 * pretty hard to hide.  Just in case a future implementation
	 * has a lower latency, here's the sequence (also by Mike Burrows)
	 *
	 * UNPKBW a0, v0       v0: 00AA00BB00CC00DD
	 * SLL v0, 24, a0      a0: BB00CC00DD000000
	 * BIS v0, a0, a0      a0: BBAACCBBDDCC00DD
	 * EXTWL a0, 6, v0     v0: 000000000000BBAA
	 * ZAP a0, 0xf3, a0    a0: 00000000DDCC0000
	 * ADDL a0, v0, v0     v0: ssssssssDDCCBBAA
	 */

	__u64 t0, t1, t2, t3;

	__asm__("inslh %1, 7, %0"	/* t0 : 0000000000AABBCC */
		: "=r"(t0) : "r"(x));
	__asm__("inswl %1, 3, %0"	/* t1 : 000000CCDD000000 */
		: "=r"(t1) : "r"(x));

	t1 |= t0;			/* t1 : 000000CCDDAABBCC */
	t2 = t1 >> 16;			/* t2 : 0000000000CCDDAA */
	t0 = t1 & 0xFF00FF00;		/* t0 : 00000000DD00BB00 */
	t3 = t2 & 0x00FF00FF;		/* t3 : 0000000000CC00AA */
	t1 = t0 + t3;			/* t1 : ssssssssDDCCBBAA */

	return t1;
}

#define __arch__swab32 __arch__swab32

#endif /* __GNUC__ */

#define __BYTEORDER_HAS_U64__

#include <linux/byteorder/little_endian.h>

#endif /* _ALPHA_BYTEORDER_H */
