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
 *  linux/include/asm-arm/arch-ebsa110/time.h
 *
 *  Copyright (C) 1996,1997,1998 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * No real time clock on the evalulation board!
 *
 * Changelog:
 *  10-Oct-1996	RMK	Created
 *  04-Dec-1997	RMK	Updated for new arch/arm/kernel/time.c
 *  07-Aug-1998	RMK	Updated for arch/arm/kernel/leds.c
 *  28-Dec-1998	APH	Made leds code optional
 */

#include <asm/leds.h>

extern int  ebsa110_reset_timer(void);
extern void ebsa110_setup_timer(void);

static void timer_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{
	if (ebsa110_reset_timer()) {
		do_leds();
		do_timer(regs);
		do_profile(regs);
	}
}

/*
 * Set up timer interrupt.
 */
static inline void setup_timer(void)
{
	ebsa110_setup_timer();

	timer_irq.handler = timer_interrupt;

	setup_arm_irq(IRQ_EBSA110_TIMER0, &timer_irq);
}


