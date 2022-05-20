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
 * linux/include/asm-arm/arch-tbox/system.h
 *
 * Copyright (c) 1996-1999 Russell King.
 */
#ifndef __ASM_ARCH_SYSTEM_H
#define __ASM_ARCH_SYSTEM_H

static void arch_idle(void)
{
	unsigned long start_idle;

	start_idle = jiffies;

	do {
		if (current->need_resched || hlt_counter)
			goto slow_out;
		cpu_do_idle(IDLE_WAIT_FAST);
	} while (time_before(jiffies, start_idle + HZ/50));

	cpu_do_idle(IDLE_CLOCK_SLOW);

	while (!current->need_resched && !hlt_counter) {
		cpu_do_idle(IDLE_WAIT_SLOW);
	}

	cpu_do_idle(IDLE_CLOCK_FAST);
slow_out:
}

#define arch_reset(mode)	do { } while (0)

#endif
