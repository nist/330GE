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
 *  arch/mips/ddb5074/prom.c -- NEC DDB Vrc-5074 PROM routines
 *
 *  Copyright (C) 2000 Geert Uytterhoeven <geert@sonycom.com>
 *                     Sony Software Development Center Europe (SDCE), Brussels
 */
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/bootmem.h>

#include <asm/addrspace.h>
#include <asm/bootinfo.h>


char arcs_cmdline[CL_SIZE];

const char *get_system_type(void)
{
	return "NEC DDB Vrc-5074";
}

void __init prom_init(const char *s)
{
	int i = 0;

	if (s != (void *) -1)
		while (*s && i < sizeof(arcs_cmdline) - 1)
			arcs_cmdline[i++] = *s++;
	arcs_cmdline[i] = '\0';

	mips_machgroup = MACH_GROUP_NEC_DDB;
	mips_machtype = MACH_NEC_DDB5074;

	/* 64 MB non-upgradable */
	add_memory_region(0, 64 << 20, BOOT_MEM_RAM);
}

void __init prom_free_prom_memory(void)
{
}
