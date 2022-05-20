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
#ifndef __I386_DIV64
#define __I386_DIV64

#define do_div(n,base) ({ \
	unsigned long __upper, __low, __high, __mod; \
	asm("":"=a" (__low), "=d" (__high):"A" (n)); \
	__upper = __high; \
	if (__high) { \
		__upper = __high % (base); \
		__high = __high / (base); \
	} \
	asm("divl %2":"=a" (__low), "=d" (__mod):"rm" (base), "0" (__low), "1" (__upper)); \
	asm("":"=A" (n):"a" (__low),"d" (__high)); \
	__mod; \
})

#endif
