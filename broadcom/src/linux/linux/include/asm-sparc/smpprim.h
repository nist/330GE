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
/*  $Id: smpprim.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $
 *  smpprim.h:  SMP locking primitives on the Sparc
 *
 *  God knows we won't be actually using this code for some time
 *  but I thought I'd write it since I knew how.
 *
 *  Copyright (C) 1995 David S. Miller (davem@caip.rutgers.edu)
 */

#ifndef __SPARC_SMPPRIM_H
#define __SPARC_SMPPRIM_H

/* Test and set the unsigned byte at ADDR to 1.  Returns the previous
 * value.  On the Sparc we use the ldstub instruction since it is
 * atomic.
 */

extern __inline__ __volatile__ char test_and_set(void *addr)
{
	char state = 0;

	__asm__ __volatile__("ldstub [%0], %1         ! test_and_set\n\t"
			     "=r" (addr), "=r" (state) :
			     "0" (addr), "1" (state) : "memory");

	return state;
}

/* Initialize a spin-lock. */
extern __inline__ __volatile__ smp_initlock(void *spinlock)
{
	/* Unset the lock. */
	*((unsigned char *) spinlock) = 0;

	return;
}

/* This routine spins until it acquires the lock at ADDR. */
extern __inline__ __volatile__ smp_lock(void *addr)
{
	while(test_and_set(addr) == 0xff)
		;

	/* We now have the lock */
	return;
}

/* This routine releases the lock at ADDR. */
extern __inline__ __volatile__ smp_unlock(void *addr)
{
	*((unsigned char *) addr) = 0;
}

#endif /* !(__SPARC_SMPPRIM_H) */
