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
 * linux/arch/arm/mach-sa1100/leds-cerf.c
 *
 * Author: ???
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

#ifdef CONFIG_SA1100_CERF_CPLD

#define LED_D0		GPIO_GPIO(0)
#define LED_D1		GPIO_GPIO(0)
#define LED_D2		GPIO_GPIO(0)
#define LED_D3		GPIO_GPIO(0)
#define LED_MASK	(LED_D0|LED_D1|LED_D2|LED_D3)

#else

#define LED_D0          GPIO_GPIO(0)
#define LED_D1          GPIO_GPIO(1)
#define LED_D2          GPIO_GPIO(2)
#define LED_D3          GPIO_GPIO(3)
#define LED_MASK        (LED_D0|LED_D1|LED_D2|LED_D3)

#endif

void cerf_leds_event(led_event_t evt)
{
        unsigned long flags;

	local_irq_save(flags);

        switch (evt) {
        case led_start:
                hw_led_state = LED_MASK;
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
                        hw_led_state ^= LED_D0;
                break;
#endif

#ifdef CONFIG_LEDS_CPU
        case led_idle_start:
                if (!(led_state & LED_STATE_CLAIMED))
                        hw_led_state &= ~LED_D1;
                break;

        case led_idle_end:
                if (!(led_state & LED_STATE_CLAIMED))
                        hw_led_state |= LED_D1;
                break;
#endif
        case led_green_on:
                if (!(led_state & LED_STATE_CLAIMED))
                        hw_led_state &= ~LED_D2;
                break;

        case led_green_off:
                if (!(led_state & LED_STATE_CLAIMED))
                        hw_led_state |= LED_D2;
                break;

        case led_amber_on:
                if (!(led_state & LED_STATE_CLAIMED))
                        hw_led_state &= ~LED_D3;
                break;

        case led_amber_off:
                if (!(led_state & LED_STATE_CLAIMED))
                        hw_led_state |= LED_D3;
                break;

        case led_red_on:
                if (!(led_state & LED_STATE_CLAIMED))
                        hw_led_state &= ~LED_D1;
                break;

        case led_red_off:
                if (!(led_state & LED_STATE_CLAIMED))
                        hw_led_state |= LED_D1;
                break;

        default:
                break;
        }

        if  (led_state & LED_STATE_ENABLED) {
                GPSR = hw_led_state;
                GPCR = hw_led_state ^ LED_MASK;
        }

	local_irq_restore(flags);
}
