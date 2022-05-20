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
 * linux/include/asm-sparc64/timex.h
 *
 * sparc64 architecture timex specifications
 */
#ifndef _ASMsparc64_TIMEX_H
#define _ASMsparc64_TIMEX_H

#define CLOCK_TICK_RATE	1193180 /* Underlying HZ */
#define CLOCK_TICK_FACTOR	20	/* Factor of both 1000000 and CLOCK_TICK_RATE */
#define FINETUNE ((((((long)LATCH * HZ - CLOCK_TICK_RATE) << SHIFT_HZ) * \
	(1000000/CLOCK_TICK_FACTOR) / (CLOCK_TICK_RATE/CLOCK_TICK_FACTOR)) \
		<< (SHIFT_SCALE-SHIFT_HZ)) / HZ)

/* Getting on the cycle counter on sparc64. */
typedef unsigned long cycles_t;
#define get_cycles() \
({	cycles_t ret; \
	__asm__ __volatile__("rd	%%tick, %0" : "=r" (ret)); \
	ret; \
})

#define vxtime_lock()		do {} while (0)
#define vxtime_unlock()		do {} while (0)

#endif
