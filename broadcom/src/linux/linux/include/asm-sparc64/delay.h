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
/* $Id: delay.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $
 * delay.h: Linux delay routines on the V9.
 *
 * Copyright (C) 1996 David S. Miller (davem@caip.rutgers.edu).
 */

#ifndef __SPARC64_DELAY_H
#define __SPARC64_DELAY_H

#include <linux/config.h>
#include <linux/param.h>

#ifndef __ASSEMBLY__

#ifdef CONFIG_SMP
#include <asm/smp.h>
#else
extern unsigned long loops_per_jiffy;
#endif 

extern __inline__ void __delay(unsigned long loops)
{
	__asm__ __volatile__(
"	b,pt	%%xcc, 1f\n"
"	 cmp	%0, 0\n"
"	.align	32\n"
"1:\n"
"	bne,pt	%%xcc, 1b\n"
"	 subcc	%0, 1, %0\n"
	: "=&r" (loops)
	: "0" (loops)
	: "cc");
}

extern __inline__ void __udelay(unsigned long usecs, unsigned long lps)
{
	usecs *= 0x00000000000010c6UL;		/* 2**32 / 1000000 */

	__asm__ __volatile__(
"	mulx	%1, %2, %0\n"
"	srlx	%0, 32, %0\n"
	: "=r" (usecs)
	: "r" (usecs), "r" (lps));

	__delay(usecs * HZ);
}

#ifdef CONFIG_SMP
#define __udelay_val cpu_data[smp_processor_id()].udelay_val
#else
#define __udelay_val loops_per_jiffy
#endif

#define udelay(usecs) __udelay((usecs),__udelay_val)

#endif /* !__ASSEMBLY__ */

#endif /* defined(__SPARC64_DELAY_H) */
