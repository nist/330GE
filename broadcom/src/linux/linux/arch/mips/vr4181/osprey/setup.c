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
 * linux/arch/mips/vr4181/setup.c
 *
 * VR41xx setup routines
 *
 * Copyright (C) 1999 Bradley D. LaRonde
 * Copyright (C) 1999, 2000 Michael Klar
 *
 * Copyright 2001 MontaVista Software Inc.
 * Author: jsun@mvista.com or jsun@junsun.net
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 */

#include <linux/config.h>
#include <linux/console.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/reboot.h>
#include <asm/vr4181/vr4181.h>
#include <asm/io.h>


extern void nec_osprey_restart(char* c);
extern void nec_osprey_halt(void);
extern void nec_osprey_power_off(void);

extern void vr4181_init_serial(void);
extern void vr4181_init_time(void);

void __init bus_error_init(void)
{
}

void __init nec_osprey_setup(void)
{
	set_io_port_base(VR4181_PORT_BASE);
	isa_slot_offset = VR4181_ISAMEM_BASE;

	vr4181_init_serial();
	vr4181_init_time();

#ifdef CONFIG_FB
	conswitchp = &dummy_con;
#endif

	_machine_restart = nec_osprey_restart;
	_machine_halt = nec_osprey_halt;
	_machine_power_off = nec_osprey_power_off;

	/* setup resource limit */
	ioport_resource.end = 0xffffffff;
	iomem_resource.end = 0xffffffff;

	/* [jsun] hack */
	/*
	printk("[jsun] hack to change external ISA control register, %x -> %x\n",
		(*VR4181_XISACTL),
		(*VR4181_XISACTL) | 0x2);
	*VR4181_XISACTL |= 0x2;
	*/

	// *VR4181_GPHIBSTH = 0x2000;
	// *VR4181_GPMD0REG = 0x00c0;
	// *VR4181_GPINTEN	 = 1<<6;

	/* [jsun] I believe this will get the interrupt type right
	 * for the ether port.
	 */
	*VR4181_GPINTTYPL = 0x3000;
}
