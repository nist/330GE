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
 *  linux/arch/arm/mach-sa1100/leds-graphicsclient.c
 *
 * GraphicsClient Plus LEDs support
 * Woojung Huh, Feb 13, 2001
 */
#include <linux/config.h>
#include <linux/init.h>

#include <asm/hardware.h>
#include <asm/leds.h>
#include <asm/system.h>

#include "leds.h"


#define LED_STATE_ENABLED	1
#define LED_STATE_CLAIMED	2

static unsigned int led_state;
static unsigned int hw_led_state;

#define LED_TIMER       ADS_LED0  /* green heartbeat */
#define LED_USER        ADS_LED1  /* amber, boots to on */
#define LED_IDLE        ADS_LED2  /* red has the idle led, if any */

#define LED_MASK	(ADS_LED0|ADS_LED1|ADS_LED2)

void graphicsclient_leds_event(led_event_t evt)
{
	unsigned long flags;

	save_flags_cli(flags);

	switch (evt) {
	case led_start:
		hw_led_state = 0;        /* gc leds are positive logic */
		led_state = LED_STATE_ENABLED;
		break;

	case led_stop:
		led_state &= ~LED_STATE_ENABLED;
		break;

	case led_claim:
		led_state |= LED_STATE_CLAIMED;
		hw_led_state = LED_MASK;
		break;

	case led_release:
		led_state &= ~LED_STATE_CLAIMED;
		hw_led_state = LED_MASK;
		break;

#ifdef CONFIG_LEDS_TIMER
	case led_timer:
		if (!(led_state & LED_STATE_CLAIMED))
			hw_led_state ^= LED_TIMER;
		break;
#endif

#ifdef CONFIG_LEDS_CPU
	case led_idle_start:
		if (!(led_state & LED_STATE_CLAIMED))
			hw_led_state &= ~LED_IDLE;
		break;

	case led_idle_end:
		if (!(led_state & LED_STATE_CLAIMED))
			hw_led_state |= LED_IDLE;
		break;
#endif

	case led_green_on:
		break;

	case led_green_off:
		break;

	case led_amber_on:
		hw_led_state |= LED_USER;
		break;

	case led_amber_off:
		hw_led_state &= ~LED_USER;
		break;

	case led_red_on:
		break;

	case led_red_off:
		break;

	default:
		break;
	}

	if  (led_state & LED_STATE_ENABLED) {
		GPSR = hw_led_state;
		GPCR = hw_led_state ^ LED_MASK;
	}

	restore_flags(flags);
}
