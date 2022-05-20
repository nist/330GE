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
#ifndef _ASM_GENERIC_BITOPS_H_
#define _ASM_GENERIC_BITOPS_H_

/*
 * For the benefit of those who are trying to port Linux to another
 * architecture, here are some C-language equivalents.  You should
 * recode these in the native assembly language, if at all possible.
 * To guarantee atomicity, these routines call cli() and sti() to
 * disable interrupts while they operate.  (You have to provide inline
 * routines to cli() and sti().)
 *
 * Also note, these routines assume that you have 32 bit longs.
 * You will have to change this if you are trying to port Linux to the
 * Alpha architecture or to a Cray.  :-)
 * 
 * C language equivalents written by Theodore Ts'o, 9/26/92
 */

extern __inline__ int set_bit(int nr,long * addr)
{
	int	mask, retval;

	addr += nr >> 5;
	mask = 1 << (nr & 0x1f);
	cli();
	retval = (mask & *addr) != 0;
	*addr |= mask;
	sti();
	return retval;
}

extern __inline__ int clear_bit(int nr, long * addr)
{
	int	mask, retval;

	addr += nr >> 5;
	mask = 1 << (nr & 0x1f);
	cli();
	retval = (mask & *addr) != 0;
	*addr &= ~mask;
	sti();
	return retval;
}

extern __inline__ int test_bit(int nr, long * addr)
{
	int	mask;

	addr += nr >> 5;
	mask = 1 << (nr & 0x1f);
	return ((mask & *addr) != 0);
}

#ifdef __KERNEL__

/*
 * ffs: find first bit set. This is defined the same way as
 * the libc and compiler builtin ffs routines, therefore
 * differs in spirit from the above ffz (man ffs).
 */

#define ffs(x) generic_ffs(x)

/*
 * hweightN: returns the hamming weight (i.e. the number
 * of bits set) of a N-bit word
 */

#define hweight32(x) generic_hweight32(x)
#define hweight16(x) generic_hweight16(x)
#define hweight8(x) generic_hweight8(x)

#endif /* __KERNEL__ */

#endif /* _ASM_GENERIC_BITOPS_H */
