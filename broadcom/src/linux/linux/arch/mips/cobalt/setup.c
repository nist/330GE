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
 * Setup pointers to hardware dependent routines.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1996, 1997 by Ralf Baechle
 * Copyright (C) 2001, 2002, 2003 by Liam Davies (ldavies@agile.tv)
 *
 */

#include <linux/config.h>
#include <linux/interrupt.h>
#include <linux/pci.h>
#include <linux/mc146818rtc.h>
#include <linux/init.h>
#include <linux/ide.h>

#include <asm/bootinfo.h>
#include <asm/time.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/cobalt/cobalt.h>
#include <asm/pci.h>
#include <asm/processor.h>
#include <asm/reboot.h>
#include <asm/traps.h>

extern void cobalt_machine_restart(char *command);
extern void cobalt_machine_halt(void);
extern void cobalt_machine_power_off(void);

extern struct rtc_ops std_rtc_ops;
extern struct ide_ops std_ide_ops;


char arcs_cmdline[CL_SIZE] = {
 "console=ttyS0,115200 "
#ifdef CONFIG_IP_PNP
 "ip=on "
#endif
#ifdef CONFIG_ROOT_NFS
 "root=/dev/nfs "
#else
 "root=/dev/hda1 "
#endif
 };

const char *get_system_type(void)
{
	return "MIPS Cobalt";
}


#define GALILEO_T0_VAL		0xb4000850
#define GALILEO_TIMER_CTRL	0xb4000864
#define GALILEO_CPU_MASK	0xb4000c1c

#define GALILEO_ENTC0		0x01
#define GALILEO_SELTC0		0x02

static void __init cobalt_time_init(void)
{
	rtc_ops = &std_rtc_ops;
}

static void __init cobalt_timer_setup(struct irqaction *irq)
{
	/* Load timer value for 150 Hz */
	volatile unsigned long *timer_reg = (volatile unsigned long *)GALILEO_T0_VAL;

	*timer_reg = 500000;

	/* Register our timer interrupt */
	setup_irq(COBALT_TIMER_IRQ, irq);

	/* Enable timer ints */
	*((volatile unsigned long *) GALILEO_TIMER_CTRL) =
			(unsigned long) (GALILEO_ENTC0 | GALILEO_SELTC0);
	/* Unmask timer int */
	*((volatile unsigned long *) GALILEO_CPU_MASK) = (unsigned long) 0x00000100;
}


void __init bus_error_init(void) { /* nothing */ }


void __init cobalt_setup(void)
{

	_machine_restart = cobalt_machine_restart;
	_machine_halt = cobalt_machine_halt;
	_machine_power_off = cobalt_machine_power_off;

	board_time_init = cobalt_time_init;
	board_timer_setup = cobalt_timer_setup;

#ifdef CONFIG_BLK_DEV_IDE
	ide_ops = &std_ide_ops;
#endif

        set_io_port_base(KSEG1ADDR(0x10000000));

	/*
	 * This is a prom style console. We just poke at the
	 *  UART to make it talk.
	 * Only use this console if you really screw up and can't
	 *  get to the stage of setting up a real serial console.
	 */
	/*ns16550_setup_console();*/
}

/* Prom init. We read our one and only communication with the
    firmware. Grab the amount of installed memory */
void __init prom_init(int argc)
{
	mips_machgroup = MACH_GROUP_COBALT;

	add_memory_region(0x0, argc & 0x7fffffff, BOOT_MEM_RAM);
}

void __init prom_free_prom_memory(void)
{
	/* Nothing to do! */
}
