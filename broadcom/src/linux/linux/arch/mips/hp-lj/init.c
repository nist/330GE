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

#include <linux/mm.h>
#include <asm/bootinfo.h>
#include <asm/addrspace.h>
#include <asm/hp-lj/asic.h>
#include <linux/bootmem.h>

#include "utils.h"


#define Delimiter "CMDLINE="
const char CommandLine[] = Delimiter
  "root=/dev/hda3                                                                                                                                                                                                                                            ";

char arcs_cmdline[CL_SIZE];

int __init prom_init(int argc, char ** argv, char **envp)
{
	ulong mem_size = get_mem_avail();
        int reserve_size = 0;

	printk("Total Memory: %ld bytes\n", mem_size);

	reserve_buffer(CommandLine, mem_size);

	reserve_size = get_reserved_buffer_size();
	mem_size -= reserve_size;

	add_memory_region(0x0,mem_size, BOOT_MEM_RAM);
        add_memory_region(mem_size,reserve_size, BOOT_MEM_RESERVED);

	printk("Main Memory: %ld bytes\n", mem_size);
	printk("Reserved Memory: %ld bytes at 0x%08x\n",
		get_reserved_buffer_size(), (ulong)get_reserved_buffer());

	printk("Detected %s ASIC\n", GetAsicName());
	mips_machgroup  = MACH_GROUP_HP_LJ;
	mips_machtype   = MACH_UNKNOWN;

	strcpy(arcs_cmdline, CommandLine+strlen(Delimiter));

	return 0;
}


void prom_free_prom_memory (void)
{
}
