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
#ifndef _ASM_IA64_SN_IDLE_H
#define _ASM_IA64_SN_IDLE_H

/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (c) 2001-2002 Silicon Graphics, Inc.  All rights reserved.
 */

#include <linux/config.h>
#include <asm/sn/leds.h>
#include <asm/sn/simulator.h>

static __inline__ void
snidle(void) {

#ifdef CONFIG_IA64_SGI_AUTOTEST
	{
		extern int	autotest_enabled;
		if (autotest_enabled) {
			extern void llsc_main(int);
			llsc_main(smp_processor_id());
		}
	}
#endif
	
	if (pda.idle_flag == 0) {
		/* 
		 * Turn the activity LED off.
		 */
		set_led_bits(0, LED_CPU_ACTIVITY);
	}

#ifdef CONFIG_IA64_SGI_SN_SIM
	if (IS_RUNNING_ON_SIMULATOR())
		SIMULATOR_SLEEP();
#endif

	pda.idle_flag = 1;
}

static __inline__ void
snidleoff(void) {
	/* 
	 * Turn the activity LED on.
	 */
	set_led_bits(LED_CPU_ACTIVITY, LED_CPU_ACTIVITY);

	pda.idle_flag = 0;
}

#endif /* _ASM_IA64_SN_IDLE_H */
