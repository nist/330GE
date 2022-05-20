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

#define add_ssaaaa(sh, sl, ah, al, bh, bl) ({		\
        unsigned int __sh = (ah);			\
        unsigned int __sl = (al);			\
        __asm__ ("   alr  %1,%3\n"			\
                 "   brc  12,0f\n"			\
                 "   ahi  %0,1\n"			\
                 "0: alr  %0,%2"			\
                 : "+&d" (__sh), "+d" (__sl)		\
                 : "d" (bh), "d" (bl) : "cc" );		\
        (sh) = __sh;					\
        (sl) = __sl;					\
})

#define sub_ddmmss(sh, sl, ah, al, bh, bl) ({		\
       unsigned int __sh = (ah);			\
       unsigned int __sl = (al);			\
       __asm__ ("   slr  %1,%3\n"			\
                "   brc  3,0f\n"			\
                "   ahi  %0,-1\n"			\
                "0: slr  %0,%2"				\
                : "+&d" (__sh), "+d" (__sl)		\
                : "d" (bh), "d" (bl) : "cc" );		\
       (sh) = __sh;					\
       (sl) = __sl;					\
})

/* a umul b = a mul b + (a>=2<<31) ? b<<32:0 + (b>=2<<31) ? a<<32:0 */
#define umul_ppmm(wh, wl, u, v) ({			\
        unsigned int __wh = u;				\
        unsigned int __wl = v;				\
        __asm__ ("   ltr  1,%0\n"			\
                 "   mr   0,%1\n"			\
                 "   jnm  0f\n"				\
                 "   alr  0,%1\n"			\
                 "0: ltr  %1,%1\n"			\
                 "   jnm  1f\n"				\
                 "   alr  0,%0\n"			\
                 "1: lr   %0,0\n"			\
                 "   lr   %1,1\n"			\
                 : "+d" (__wh), "+d" (__wl)		\
                 : : "0", "1", "cc" );			\
        wh = __wh;					\
        wl = __wl;					\
})

#define udiv_qrnnd(q, r, n1, n0, d)			\
  do { unsigned long __r;				\
    (q) = __udiv_qrnnd (&__r, (n1), (n0), (d));		\
    (r) = __r;						\
  } while (0)
extern unsigned long __udiv_qrnnd (unsigned long *, unsigned long,
				   unsigned long , unsigned long);

#define UDIV_NEEDS_NORMALIZATION 0

#define abort() return 0

#define __BYTE_ORDER __BIG_ENDIAN
