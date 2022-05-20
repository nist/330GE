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
 *  linux/include/asm-arm/arch-ebsa110/system.h
 *
 *  Copyright (C) 1996-2000 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __ASM_ARCH_SYSTEM_H
#define __ASM_ARCH_SYSTEM_H

/*
 * EBSA110 idling methodology:
 *
 * We can not execute the "wait for interrupt" instruction since that
 * will stop our MCLK signal (which provides the clock for the glue
 * logic, and therefore the timer interrupt).
 *
 * Instead, we spin, waiting for either hlt_counter or need_resched
 * to be set.  If we have been spinning for 2cs, then we drop the
 * core clock down to the memory clock.
 */
static void arch_idle(void)
{
	unsigned long start_idle;

	start_idle = jiffies;

	do {
		if (current->need_resched || hlt_counter)
			goto slow_out;
	} while (time_before(jiffies, start_idle + HZ/50));

	cpu_do_idle(IDLE_CLOCK_SLOW);

	while (!current->need_resched && !hlt_counter) {
		/* do nothing slowly */
	}

	cpu_do_idle(IDLE_CLOCK_FAST);
slow_out:
}

#define arch_reset(mode)	cpu_reset(0x80000000)

#endif
