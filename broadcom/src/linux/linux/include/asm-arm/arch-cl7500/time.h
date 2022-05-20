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
 * linux/include/asm-arm/arch-cl7500/time.h
 *
 * Copyright (c) 1996-2000 Russell King.
 *
 * Changelog:
 *  24-Sep-1996	RMK	Created
 *  10-Oct-1996	RMK	Brought up to date with arch-sa110eval
 *  04-Dec-1997	RMK	Updated for new arch/arm/time.c
 */

static void timer_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{
	do_timer(regs);
	do_set_rtc();
	do_profile(regs);

	{
		/* Twinkle the lights. */
		static int count, state = 0xff00;
		if (count-- == 0) {
			state ^= 0x100;
			count = 25;
			*((volatile unsigned int *)LED_ADDRESS) = state;
		}
	}
}

/*
 * Set up timer interrupt.
 */
static inline void setup_timer(void)
{
	extern void ioctime_init(void);
	ioctime_init();

	timer_irq.handler = timer_interrupt;

	setup_arm_irq(IRQ_TIMER, &timer_irq);
}
