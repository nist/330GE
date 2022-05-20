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
 * linux/include/asm-arm/arch-shark/time.h
 *
 * by Alexander Schulz
 *
 * Uses the real time clock because you can't run
 * the timer with level triggered interrupts and
 * you can't run the shark with edge triggered
 * inetrrupts (loses ints and hangs).
 *
 * derived from linux/drivers/char/rtc.c and:
 * linux/include/asm-arm/arch-ebsa110/time.h
 * Copyright (c) 1996,1997,1998 Russell King.
 */

#include <asm/leds.h>
#include <linux/mc146818rtc.h>

#define IRQ_TIMER 8

extern void get_rtc_time(struct rtc_time *rtc_tm);
extern void set_rtc_irq_bit(unsigned char bit);
extern unsigned long epoch;

static void timer_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{

	CMOS_READ(RTC_INTR_FLAGS);	

	do_leds();

	{
#ifdef DIVISOR
		static unsigned int divisor;

		if (divisor-- == 0) {
			divisor = DIVISOR - 1;
#else
		{
#endif
			do_timer(regs);
		}
	}
}

/*
 * Set up timer interrupt, and return the current time in seconds.
 */
static inline void setup_timer(void)
{
        struct rtc_time r_time;
        unsigned long flags;
	int tmp = 0;
	unsigned char val;

        /*
	 * Set the clock to 128 Hz, we already have a valid
	 * vector now:
	 */

	while (HZ > (1<<tmp))
	  tmp++;

	/*
	 * Check that the input was really a power of 2.
	 */
	if (HZ != (1<<tmp))
	  panic("Please set HZ to a power of 2!");

	save_flags(flags);
	cli();
	val = CMOS_READ(RTC_FREQ_SELECT) & 0xf0;
	val |= (16 - tmp);
	CMOS_WRITE(val, RTC_FREQ_SELECT);
	restore_flags(flags);
	set_rtc_irq_bit(RTC_PIE);

	get_rtc_time(&r_time);
	xtime.tv_sec = mktime(r_time.tm_year+epoch, r_time.tm_mon+1, r_time.tm_mday,
			      r_time.tm_hour, r_time.tm_min, r_time.tm_sec);

	timer_irq.handler = timer_interrupt;
	timer_irq.flags = SA_INTERRUPT; 
	setup_arm_irq(IRQ_TIMER, &timer_irq);
}
