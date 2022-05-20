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
 *  linux/include/asm-arm/arch-arc/time.h
 *
 *  Copyright (C) 1996-2000 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Changelog:
 *   24-Sep-1996 RMK	Created
 *   10-Oct-1996 RMK	Brought up to date with arch-sa110eval
 *   04-Dec-1997 RMK	Updated for new arch/arm/time.c
 */
extern void ioctime_init(void);

static void timer_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{
	do_timer(regs);
	do_set_rtc();
	do_profile(regs);
}

/*
 * Set up timer interrupt.
 */
static inline void setup_timer(void)
{
	ioctime_init();

	timer_irq.handler = timer_interrupt;

	setup_arm_irq(IRQ_TIMER, &timer_irq);
}
