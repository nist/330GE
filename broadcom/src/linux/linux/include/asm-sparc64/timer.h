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
/* $Id: timer.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $
 * timer.h: System timer definitions for sun5.
 *
 * Copyright (C) 1997 David S. Miller (davem@caip.rutgers.edu)
 */

#ifndef _SPARC64_TIMER_H
#define _SPARC64_TIMER_H

/* How timers work:
 *
 * On uniprocessors we just use counter zero for the system wide
 * ticker, this performs thread scheduling, clock book keeping,
 * and runs timer based events.  Previously we used the Ultra
 * %tick interrupt for this purpose.
 *
 * On multiprocessors we pick one cpu as the master level 10 tick
 * processor.  Here this counter zero tick handles clock book
 * keeping and timer events only.  Each Ultra has it's level
 * 14 %tick interrupt set to fire off as well, even the master
 * tick cpu runs this locally.  This ticker performs thread
 * scheduling, system/user tick counting for the current thread,
 * and also profiling if enabled.
 */

#include <linux/config.h>

struct sun5_timer {
	u64	count0;
	u64	limit0;
	u64	count1;
	u64	limit1;
};

#define SUN5_LIMIT_ENABLE	0x80000000
#define SUN5_LIMIT_TOZERO	0x40000000
#define SUN5_LIMIT_ZRESTART	0x20000000
#define SUN5_LIMIT_CMASK	0x1fffffff

/* Given a HZ value, set the limit register to so that the timer IRQ
 * gets delivered that often.
 */
#define SUN5_HZ_TO_LIMIT(__hz)  (1000000/(__hz))

#ifdef CONFIG_SMP
extern unsigned long timer_tick_offset;
extern void timer_tick_interrupt(struct pt_regs *);
#endif

#endif /* _SPARC64_TIMER_H */
