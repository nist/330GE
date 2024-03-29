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
 * linux/arch/sh/kernel/led_se.c
 *
 * Copyright (C) 2000 Stuart Menefy <stuart.menefy@st.com>
 *
 * May be copied or modified under the terms of the GNU General Public
 * License.  See linux/COPYING for more information.
 *
 * This file contains Solution Engine specific LED code.
 */

#include <linux/config.h>
#include <asm/hitachi_7751se.h>

static void mach_led(int position, int value)
{
	volatile unsigned short* p = (volatile unsigned short*)PA_LED;

	if (value) {
		*p |= (1<<8);
	} else {
		*p &= ~(1<<8);
	}
}

#ifdef CONFIG_HEARTBEAT

#include <linux/sched.h>

/* Cycle the LED's in the clasic Knightrider/Sun pattern */
void heartbeat_7751se(void)
{
	static unsigned int cnt = 0, period = 0;
	volatile unsigned short* p = (volatile unsigned short*)PA_LED;
	static unsigned bit = 0, up = 1;

	cnt += 1;
	if (cnt < period) {
		return;
	}

	cnt = 0;

	/* Go through the points (roughly!):
	 * f(0)=10, f(1)=16, f(2)=20, f(5)=35,f(inf)->110
	 */
	period = 110 - ( (300<<FSHIFT)/
			 ((avenrun[0]/5) + (3<<FSHIFT)) );

	if (up) {
		if (bit == 7) {
			bit--;
			up=0;
		} else {
			bit ++;
		}
	} else {
		if (bit == 0) {
			bit++;
			up=1;
		} else {
			bit--;
		}
	}
	*p = 1<<(bit+8);

}
#endif /* CONFIG_HEARTBEAT */
