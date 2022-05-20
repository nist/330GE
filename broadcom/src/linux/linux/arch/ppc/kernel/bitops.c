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
 * BK Id: SCCS/s.bitops.c 1.7 05/17/01 18:14:21 cort
 */
/*
 * Copyright (C) 1996 Paul Mackerras.
 */

#include <linux/kernel.h>
#include <asm/bitops.h>

/*
 * If the bitops are not inlined in bitops.h, they are defined here.
 *  -- paulus
 */
#if !__INLINE_BITOPS
void set_bit(int nr, volatile void * addr)
{
	unsigned long old;
	unsigned long mask = 1 << (nr & 0x1f);
	unsigned long *p = ((unsigned long *)addr) + (nr >> 5);
	
	__asm__ __volatile__(SMP_WMB "\n\
1:	lwarx	%0,0,%3 \n\
	or	%0,%0,%2 \n\
	stwcx.	%0,0,%3 \n\
	bne	1b"
	SMP_MB
	: "=&r" (old), "=m" (*p)
	: "r" (mask), "r" (p), "m" (*p)
	: "cc" );
}

void clear_bit(int nr, volatile void *addr)
{
	unsigned long old;
	unsigned long mask = 1 << (nr & 0x1f);
	unsigned long *p = ((unsigned long *)addr) + (nr >> 5);

	__asm__ __volatile__(SMP_WMB "\n\
1:	lwarx	%0,0,%3 \n\
	andc	%0,%0,%2 \n\
	stwcx.	%0,0,%3 \n\
	bne	1b"
	SMP_MB
	: "=&r" (old), "=m" (*p)
	: "r" (mask), "r" (p), "m" (*p)
	: "cc");
}

void change_bit(int nr, volatile void *addr)
{
	unsigned long old;
	unsigned long mask = 1 << (nr & 0x1f);
	unsigned long *p = ((unsigned long *)addr) + (nr >> 5);

	__asm__ __volatile__(SMP_WMB "\n\
1:	lwarx	%0,0,%3 \n\
	xor	%0,%0,%2 \n\
	stwcx.	%0,0,%3 \n\
	bne	1b"
	SMP_MB
	: "=&r" (old), "=m" (*p)
	: "r" (mask), "r" (p), "m" (*p)
	: "cc");
}

int test_and_set_bit(int nr, volatile void *addr)
{
	unsigned int old, t;
	unsigned int mask = 1 << (nr & 0x1f);
	volatile unsigned int *p = ((volatile unsigned int *)addr) + (nr >> 5);

	__asm__ __volatile__(SMP_WMB "\n\
1:	lwarx	%0,0,%4 \n\
	or	%1,%0,%3 \n\
	stwcx.	%1,0,%4 \n\
	bne	1b"
	SMP_MB
	: "=&r" (old), "=&r" (t), "=m" (*p)
	: "r" (mask), "r" (p), "m" (*p)
	: "cc");

	return (old & mask) != 0;
}

int test_and_clear_bit(int nr, volatile void *addr)
{
	unsigned int old, t;
	unsigned int mask = 1 << (nr & 0x1f);
	volatile unsigned int *p = ((volatile unsigned int *)addr) + (nr >> 5);

	__asm__ __volatile__(SMP_WMB "\n\
1:	lwarx	%0,0,%4 \n\
	andc	%1,%0,%3 \n\
	stwcx.	%1,0,%4 \n\
	bne	1b"
	SMP_MB
	: "=&r" (old), "=&r" (t), "=m" (*p)
	: "r" (mask), "r" (p), "m" (*p)
	: "cc");

	return (old & mask) != 0;
}

int test_and_change_bit(int nr, volatile void *addr)
{
	unsigned int old, t;
	unsigned int mask = 1 << (nr & 0x1f);
	volatile unsigned int *p = ((volatile unsigned int *)addr) + (nr >> 5);

	__asm__ __volatile__(SMP_WMB "\n\
1:	lwarx	%0,0,%4 \n\
	xor	%1,%0,%3 \n\
	stwcx.	%1,0,%4 \n\
	bne	1b"
	SMP_MB
	: "=&r" (old), "=&r" (t), "=m" (*p)
	: "r" (mask), "r" (p), "m" (*p)
	: "cc");

	return (old & mask) != 0;
}
#endif /* !__INLINE_BITOPS */
