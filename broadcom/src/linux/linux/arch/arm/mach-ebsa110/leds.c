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
 *  linux/arch/arm/mach-ebsa110/leds.c
 *
 *  Copyright (C) 1998 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  EBSA-110 LED control routines.  We use the led as follows:
 *
 *   - Red - toggles state every 50 timer interrupts
 */
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/init.h>

#include <asm/hardware.h>
#include <asm/leds.h>
#include <asm/system.h>
#include <asm/mach-types.h>

static spinlock_t leds_lock;

static void ebsa110_leds_event(led_event_t ledevt)
{
	unsigned long flags;

	spin_lock_irqsave(&leds_lock, flags);

	switch(ledevt) {
	case led_timer:
		*(volatile unsigned char *)SOFT_BASE ^= 128;
		break;

	default:
		break;
	}

	spin_unlock_irqrestore(&leds_lock, flags);
}

static int __init leds_init(void)
{
	if (machine_is_ebsa110())
		leds_event = ebsa110_leds_event;

	return 0;
}

__initcall(leds_init);
