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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright 1994 - 2000 Ralf Baechle (ralf@gnu.org)
 * Copyright 2000 Silicon Graphics, Inc.
 */
#ifndef _ASM_PARAM_H
#define _ASM_PARAM_H

#ifndef HZ

#ifdef __KERNEL__

/* Safeguard against user stupidity  */
#ifdef _SYS_PARAM_H
#error Do not include <asm/param.h> with __KERNEL__ defined!
#endif

#include <linux/config.h>

#ifdef CONFIG_DECSTATION
   /*
    * log2(HZ), change this here if you want another HZ value. This is also
    * used in dec_time_init.  Minimum is 1, Maximum is 15.
    */
#  define LOG_2_HZ 7
#  define HZ (1 << LOG_2_HZ)
   /*
    * Ye olde division-by-multiplication trick.
    * This works only if 100 / HZ <= 1
    */
#  define QUOTIENT ((1UL << (64 - LOG_2_HZ)) * 100)
#  define hz_to_std(a)				\
   ({ unsigned long __res;			\
      unsigned long __lo;			\
	__asm__("dmultu\t%2,%3\n\t"		\
		:"=h" (__res), "=l" (__lo)	\
		:"r" (a), "r" (QUOTIENT));	\
	(__typeof__(a)) __res;})

#else /* Not a DECstation  */

/* This is the internal value of HZ, that is the rate at which the jiffies
   counter is increasing.  This value is independent from the external value
   and can be changed in order to suit the hardware and application
   requirements.  */
#  define HZ 100
#  define hz_to_std(a) (a)

#endif /* Not a DECstation  */

#else /* defined(__KERNEL__)  */

/* This is the external value of HZ as seen by user programs.  Don't change
   unless you know what you're doing - changing breaks binary compatibility.  */
#define HZ 100

#endif /* defined(__KERNEL__)  */
#endif /* defined(HZ)  */

#define EXEC_PAGESIZE	4096

#ifndef NGROUPS
#define NGROUPS		32
#endif

#ifndef NOGROUP
#define NOGROUP		(-1)
#endif

#define MAXHOSTNAMELEN	64	/* max length of hostname */

#ifdef __KERNEL__
# define CLOCKS_PER_SEC	100	/* frequency at which times() counts */
#endif

#endif /* _ASM_PARAM_H */
