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
#ifndef __S390_DIV64
#define __S390_DIV64

/* for do_div "base" needs to be smaller than 2^31-1 */
 
#define do_div(n, base) ({                                      \
	unsigned long long __n = (n);				\
	unsigned long __r;					\
								\
	asm ("   slr  0,0\n"					\
	     "   l    1,%1\n"					\
	     "   srdl 0,1\n"					\
	     "   dr   0,%2\n"					\
	     "   alr  1,1\n"					\
	     "   alr  0,0\n"					\
	     "   lhi  2,1\n"					\
	     "   n    2,%1\n"					\
	     "   alr  0,2\n"					\
	     "   clr  0,%2\n"					\
	     "   jl   0f\n"					\
	     "   slr  0,%2\n"					\
             "   ahi  1,1\n"					\
	     "0: st   1,%1\n"					\
	     "   l    1,4+%1\n"					\
	     "   srdl 0,1\n"					\
             "   dr   0,%2\n"					\
	     "   alr  1,1\n"					\
	     "   alr  0,0\n"					\
	     "   lhi  2,1\n"					\
	     "   n    2,4+%1\n"					\
	     "   alr  0,2\n"					\
	     "   clr  0,%2\n"					\
             "   jl   1f\n"					\
	     "   slr  0,%2\n"					\
	     "   ahi  1,1\n"					\
	     "1: st   1,4+%1\n"					\
             "   lr   %0,0"					\
	     : "=d" (__r), "+m" (__n)				\
	     : "d" (base) : "0", "1", "2", "cc" );		\
	(n) = (__n);						\
        __r;                                                    \
})

#endif
