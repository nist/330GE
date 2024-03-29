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
 * Copyright 2001 MontaVista Software Inc.
 * Author: jsun@mvista.com or jsun@junsun.net
 *
 * arch/mips/vr4181/osprey/prom.c
 *     prom code for osprey.
 *
 * This program is free software; you can redistribute	it and/or modify it
 * under  the terms of	the GNU General	 Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/bootmem.h>
#include <asm/bootinfo.h>
#include <asm/addrspace.h>

char arcs_cmdline[CL_SIZE];

const char *get_system_type(void)
{
	return "NEC_Vr41xx Osprey";
}

/*
 * [jsun] right now we assume it is the nec debug monitor, which does
 * not pass any arguments.
 */
void __init prom_init()
{
	strcpy(arcs_cmdline, "ip=bootp ");
	strcat(arcs_cmdline, "ether=46,0x03fe0300,eth0 ");
	// strcpy(arcs_cmdline, "ether=0,0x0300,eth0 "
	// strcat(arcs_cmdline, "video=vr4181fb:xres:240,yres:320,bpp:8 ");

	mips_machgroup = MACH_GROUP_NEC_VR41XX;
	mips_machtype = MACH_NEC_OSPREY;

	/* 16MB fixed */
	add_memory_region(0, 16 << 20, BOOT_MEM_RAM);
}

void __init prom_free_prom_memory(void)
{
}

void __init prom_fixup_mem_map(unsigned long start, unsigned long end)
{
}

