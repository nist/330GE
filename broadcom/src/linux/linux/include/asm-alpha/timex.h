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
 * linux/include/asm-alpha/timex.h
 *
 * ALPHA architecture timex specifications
 */
#ifndef _ASMALPHA_TIMEX_H
#define _ASMALPHA_TIMEX_H

#define CLOCK_TICK_RATE	1193180 /* Underlying HZ */

/*
 * Standard way to access the cycle counter.
 * Currently only used on SMP for scheduling.
 *
 * Only the low 32 bits are available as a continuously counting entity. 
 * But this only means we'll force a reschedule every 8 seconds or so,
 * which isn't an evil thing.
 */

typedef unsigned int cycles_t;
extern cycles_t cacheflush_time;

static inline cycles_t get_cycles (void)
{
	cycles_t ret;
	__asm__ __volatile__ ("rpcc %0" : "=r"(ret));
	return ret;
}

#define vxtime_lock()		do {} while (0)
#define vxtime_unlock()		do {} while (0)

#endif
