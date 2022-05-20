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
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <asm/byteorder.h>
#include <asm/fpu.h>

#define add_ssaaaa(sh, sl, ah, al, bh, bl) \
  ((sl) = (al) + (bl), (sh) = (ah) + (bh) + ((sl) < (al)))

#define sub_ddmmss(sh, sl, ah, al, bh, bl) \
  ((sl) = (al) - (bl), (sh) = (ah) - (bh) - ((al) < (bl)))

#define umul_ppmm(wh, wl, u, v)			\
  __asm__ ("mulq %2,%3,%1; umulh %2,%3,%0"	\
	   : "=r" ((UDItype)(wh)),		\
	     "=&r" ((UDItype)(wl))		\
	   : "r" ((UDItype)(u)),		\
	     "r" ((UDItype)(v)))

#define udiv_qrnnd(q, r, n1, n0, d)				\
  do { unsigned long __r;					\
    (q) = __udiv_qrnnd (&__r, (n1), (n0), (d));			\
    (r) = __r;							\
  } while (0)
extern unsigned long __udiv_qrnnd (unsigned long *, unsigned long,
				   unsigned long , unsigned long);

#define UDIV_NEEDS_NORMALIZATION 1  

#define abort()			goto bad_insn

#ifndef __LITTLE_ENDIAN
#define __LITTLE_ENDIAN -1
#endif
#define __BYTE_ORDER __LITTLE_ENDIAN
