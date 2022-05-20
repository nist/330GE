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
 *  linux/arch/arm/kernel/leds-ftvpci.c
 *
 *  Copyright (C) 1999 FutureTV Labs Ltd
 */

#include <linux/module.h>

#include <asm/hardware.h>
#include <asm/leds.h>
#include <asm/system.h>
#include <asm/io.h>

static void ftvpci_leds_event(led_event_t ledevt)
{
	static int led_state = 0;

	switch(ledevt) {
	case led_timer:
		led_state ^= 1;
		raw_writeb(0x1a | led_state, INTCONT_BASE);
		break;

	default:
		break;
	}
}

void (*leds_event)(led_event_t) = ftvpci_leds_event;

EXPORT_SYMBOL(leds_event);
