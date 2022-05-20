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
#ifndef _ASM_IA64_TIMEX_H
#define _ASM_IA64_TIMEX_H

/*
 * Copyright (C) 1998-2001 Hewlett-Packard Co
 * Copyright (C) 1998-2001 David Mosberger-Tang <davidm@hpl.hp.com>
 */
/*
 * 2001/01/18 davidm	Removed CLOCK_TICK_RATE.  It makes no sense on IA-64.
 *			Also removed cacheflush_time as it's entirely unused.
 */

typedef unsigned long cycles_t;

static inline cycles_t
get_cycles (void)
{
	cycles_t ret;

	__asm__ __volatile__ ("mov %0=ar.itc" : "=r"(ret));
	return ret;
}

#define vxtime_lock()		do {} while (0)
#define vxtime_unlock()		do {} while (0)

#endif /* _ASM_IA64_TIMEX_H */
