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
#ifndef _PARISC_DELAY_H
#define _PARISC_DELAY_H

#include <asm/system.h>    /* for mfctl() */
#include <asm/processor.h> /* for boot_cpu_data */


/*
 * Copyright (C) 1993 Linus Torvalds
 *
 * Delay routines
 */

static __inline__ void __delay(unsigned long loops) {
	asm volatile(
	"	.balignl	64,0x34000034\n"
	"	addib,UV -1,%0,.\n"
	"	nop\n"
		: "=r" (loops) : "0" (loops));
}

static __inline__ void __cr16_delay(unsigned long clocks) {
	unsigned long start;

	/*
	 * Note: Due to unsigned math, cr16 rollovers shouldn't be
	 * a problem here. However, on 32 bit, we need to make sure
	 * we don't pass in too big a value. The current default
	 * value of MAX_UDELAY_MS should help prevent this.
	 */

	start = mfctl(16);
	while ((mfctl(16) - start) < clocks)
	    ;
}

static __inline__ void __udelay(unsigned long usecs) {
	__cr16_delay(usecs * ((unsigned long)boot_cpu_data.cpu_hz / 1000000UL));
}

#define udelay(n) __udelay(n)

#endif /* defined(_PARISC_DELAY_H) */
