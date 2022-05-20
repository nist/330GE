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
#ifndef __ASM_PARISC_DIV64
#define __ASM_PARISC_DIV64

#ifdef __LP64__

/*
 * Copyright (C) 1999 Hewlett-Packard Co
 * Copyright (C) 1999 David Mosberger-Tang <davidm@hpl.hp.com>
 *
 * vsprintf uses this to divide a 64-bit integer N by a small integer BASE.
 * This is incredibly hard on IA-64 and HPPA
 */

#define do_div(n,base)						\
({								\
	int _res;						\
	_res = ((unsigned long) (n)) % (unsigned) (base);	\
	(n) = ((unsigned long) (n)) / (unsigned) (base);	\
	_res;							\
})

#else
/*
 * unsigned long long division.  Yuck Yuck!  What is Linux coming to?
 * This is 100% disgusting
 */
#define do_div(n,base)							\
({									\
	unsigned long __low, __low2, __high, __rem;			\
	__low  = (n) & 0xffffffff;					\
	__high = (n) >> 32;						\
	if (__high) {							\
		__rem   = __high % (unsigned long)base;			\
		__high  = __high / (unsigned long)base;			\
		__low2  = __low >> 16;					\
		__low2 += __rem << 16;					\
		__rem   = __low2 % (unsigned long)base;			\
		__low2  = __low2 / (unsigned long)base;			\
		__low   = __low & 0xffff;				\
		__low  += __rem << 16;					\
		__rem   = __low  % (unsigned long)base;			\
		__low   = __low  / (unsigned long)base;			\
		n = __low  + ((long long)__low2 << 16) +		\
			((long long) __high << 32);			\
	} else {							\
		__rem = __low % (unsigned long)base;			\
		n = (__low / (unsigned long)base);			\
	}								\
	__rem;								\
})
#endif

#endif

