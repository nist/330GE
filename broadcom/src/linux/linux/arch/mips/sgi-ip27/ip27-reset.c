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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Reset an IP27.
 *
 * Copyright (C) 1997, 1998, 1999, 2000 by Ralf Baechle
 * Copyright (C) 1999, 2000 Silicon Graphics, Inc.
 */
#include <linux/config.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/smp.h>
#include <linux/mmzone.h>

#include <asm/io.h>
#include <asm/irq.h>
#include <asm/reboot.h>
#include <asm/system.h>
#include <asm/sgialib.h>
#include <asm/sn/addrs.h>
#include <asm/sn/arch.h>
#include <asm/sn/gda.h>
#include <asm/sn/sn0/hub.h>

void machine_restart(char *command) __attribute__((noreturn));
void machine_halt(void) __attribute__((noreturn));
void machine_power_off(void) __attribute__((noreturn));

#define noreturn while(1);				/* Silence gcc.  */

static void ip27_machine_restart(char *command)
{

	printk("Reboot started from CPU %d\n", smp_processor_id());
#ifdef CONFIG_SMP
	smp_send_stop();
#endif
	LOCAL_HUB_S(NI_PORT_RESET, NPR_PORTRESET | NPR_LOCALRESET);
	noreturn;
}

static void ip27_machine_halt(void)
{
	int i;

#ifdef CONFIG_SMP
	smp_send_stop();
#endif
	for (i = 0; i < numnodes; i++)
		REMOTE_HUB_S(COMPACT_TO_NASID_NODEID(i), PROMOP_REG,
							PROMOP_RESTART);
	LOCAL_HUB_S(NI_PORT_RESET, NPR_PORTRESET | NPR_LOCALRESET);
	noreturn;
}

static void ip27_machine_power_off(void)
{
	/* To do ...  */
	noreturn;
}

void ip27_reboot_setup(void)
{
	_machine_restart = ip27_machine_restart;
	_machine_halt = ip27_machine_halt;
	_machine_power_off = ip27_machine_power_off;
}
