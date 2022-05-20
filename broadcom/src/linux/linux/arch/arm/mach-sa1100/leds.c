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
 * linux/arch/arm/mach-sa1100/leds.c
 *
 * SA1100 LEDs dispatcher
 * 
 * Copyright (C) 2001 Nicolas Pitre
 */
#include <linux/init.h>

#include <asm/leds.h>
#include <asm/mach-types.h>

#include "leds.h"

static int __init
sa1100_leds_init(void)
{
	if (machine_is_adsbitsy())
		leds_event = adsbitsy_leds_event;
	if (machine_is_assabet())
		leds_event = assabet_leds_event;
 	if (machine_is_consus())
 	        leds_event = consus_leds_event;
	if (machine_is_brutus())
		leds_event = brutus_leds_event;
	if (machine_is_cerf())
		leds_event = cerf_leds_event;
	if (machine_is_flexanet())
		leds_event = flexanet_leds_event;
	if (machine_is_frodo())
		leds_event = frodo_leds_event;
	if (machine_is_graphicsclient())
		leds_event = graphicsclient_leds_event;
	if (machine_is_graphicsmaster())
		leds_event = graphicsmaster_leds_event;
	if (machine_is_lart())
		leds_event = lart_leds_event;
	if (machine_is_pfs168())
		leds_event = pfs168_leds_event;
	if (machine_is_pt_system3())
		leds_event = system3_leds_event;

	leds_event(led_start);
	return 0;
}

__initcall(sa1100_leds_init);
