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
 * init.c: PROM library initialisation code.
 *
 * Copyright (C) 1998 Gleb Raiko & Vladimir Roganov
 */
#include <linux/init.h>
#include <asm/addrspace.h>
#include <asm/bootinfo.h>

char arcs_cmdline[CL_SIZE];

const char *get_system_type(void)
{
	/* Should probably return one of "BT23-201", "BT23-202" */
	return "Baget";
}

void __init prom_init(unsigned int mem_upper)
{
	mem_upper = PHYSADDR(mem_upper);

	mips_machgroup  = MACH_GROUP_UNKNOWN;
	mips_machtype   = MACH_UNKNOWN;
	arcs_cmdline[0] = 0;

	vac_memory_upper = mem_upper;

	add_memory_region(0, mem_upper, BOOT_MEM_RAM);
}

void prom_free_prom_memory (void)
{
}
