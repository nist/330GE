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
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 * Copyright (C) 1997, 2001 Ralf Baechle
 * Copyright 2001 MontaVista Software Inc.
 * Author: jsun@mvista.com or jsun@junsun.net
 *
 * Copyright (C) 2002 Momentum Computer Inc.
 * Author: Matthew Dharm <mdharm@momenco.com>
 */
#include <linux/sched.h>
#include <linux/mm.h>
#include <asm/io.h>
#include <asm/pgtable.h>
#include <asm/processor.h>
#include <asm/reboot.h>
#include <asm/system.h>
#include <linux/delay.h>

void momenco_ocelot_restart(char *command)
{
	/* base address of timekeeper portion of part */
	void *nvram = (void*) 0xfc807000;

 	/* Ask the NVRAM/RTC/watchdog chip to assert reset in 1/16 second */
	writeb(0x84, nvram + 0xff7);

	/* wait for the watchdog to go off */
	mdelay(100+(1000/16));

	/* if the watchdog fails for some reason, let people know */
	printk(KERN_NOTICE "Watchdog reset failed\n");
}

void momenco_ocelot_halt(void)
{
	printk(KERN_NOTICE "\n** You can safely turn off the power\n");
	while (1)
		__asm__(".set\tmips3\n\t"
	                "wait\n\t"
			".set\tmips0");
}

void momenco_ocelot_power_off(void)
{
	momenco_ocelot_halt();
}