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
#ifndef __ALPHA_COMPILER_H
#define __ALPHA_COMPILER_H

/* 
 * Herein are macros we use when describing various patterns we want to GCC.
 * In all cases we can get better schedules out of the compiler if we hide
 * as little as possible inside inline assembly.  However, we want to be
 * able to know what we'll get out before giving up inline assembly.  Thus
 * these tests and macros.
 */

#define __kernel_insbl(val, shift)					\
  ({ unsigned long __kir;						\
     __asm__("insbl %2,%1,%0" : "=r"(__kir) : "rI"(shift), "r"(val));	\
     __kir; })
#define __kernel_inswl(val, shift)					\
  ({ unsigned long __kir;						\
     __asm__("inswl %2,%1,%0" : "=r"(__kir) : "rI"(shift), "r"(val));	\
     __kir; })
#define __kernel_insql(val, shift)					\
  ({ unsigned long __kir;						\
     __asm__("insql %2,%1,%0" : "=r"(__kir) : "rI"(shift), "r"(val));	\
     __kir; })

#if 0 && (__GNUC__ > 2 || __GNUC_MINOR__ >= 92)
#define __kernel_extbl(val, shift)  (((val) >> (((shift) & 7) * 8)) & 0xfful)
#define __kernel_extwl(val, shift)  (((val) >> (((shift) & 7) * 8)) & 0xfffful)
#else
#define __kernel_extbl(val, shift)					\
  ({ unsigned long __kir;						\
     __asm__("extbl %2,%1,%0" : "=r"(__kir) : "rI"(shift), "r"(val));	\
     __kir; })
#define __kernel_extwl(val, shift)					\
  ({ unsigned long __kir;						\
     __asm__("extwl %2,%1,%0" : "=r"(__kir) : "rI"(shift), "r"(val));	\
     __kir; })
#endif


/* 
 * Beginning with EGCS 1.1, GCC defines __alpha_bwx__ when the BWX 
 * extension is enabled.  Previous versions did not define anything
 * we could test during compilation -- too bad, so sad.
 */

#if defined(__alpha_bwx__)
#define __kernel_ldbu(mem)	(mem)
#define __kernel_ldwu(mem)	(mem)
#define __kernel_stb(val,mem)	((mem) = (val))
#define __kernel_stw(val,mem)	((mem) = (val))
#else
#define __kernel_ldbu(mem)				\
  ({ unsigned char __kir;				\
     __asm__("ldbu %0,%1" : "=r"(__kir) : "m"(mem));	\
     __kir; })
#define __kernel_ldwu(mem)				\
  ({ unsigned short __kir;				\
     __asm__("ldwu %0,%1" : "=r"(__kir) : "m"(mem));	\
     __kir; })
#define __kernel_stb(val,mem) \
  __asm__("stb %1,%0" : "=m"(mem) : "r"(val))
#define __kernel_stw(val,mem) \
  __asm__("stw %1,%0" : "=m"(mem) : "r"(val))
#endif

#endif /* __ALPHA_COMPILER_H */
