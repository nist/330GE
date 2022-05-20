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
/* $Id: nosrmmu.c,v 1.1.1.1 2007/01/25 12:51:51 jiahao_jhou Exp $
 * nosrmmu.c: This file is a bunch of dummies for sun4 compiles, 
 *         so that it does not need srmmu and avoid ifdefs.
 *
 * Copyright (C) 1998 Jakub Jelinek (jj@sunsite.mff.cuni.cz)
 */

#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <asm/mbus.h>

static char shouldnothappen[] __initdata = "SUN4 kernel can only run on SUN4\n";

enum mbus_module srmmu_modtype;

int vac_cache_size = 0;

static void __init should_not_happen(void)
{
	prom_printf(shouldnothappen);
	prom_halt();
}

void __init srmmu_frob_mem_map(unsigned long start_mem)
{
	should_not_happen();
}

unsigned long __init srmmu_paging_init(unsigned long start_mem, unsigned long end_mem)
{
	should_not_happen();
	return 0;
}

void __init ld_mmu_srmmu(void)
{
	should_not_happen();
}

void srmmu_mapioaddr(unsigned long physaddr, unsigned long virt_addr, int bus_type, int rdonly)
{
}

void srmmu_unmapioaddr(unsigned long virt_addr)
{
}

void __init srmmu_end_memory(unsigned long memory_size, unsigned long *mem_end_p)
{
	return 0;
}

__u32 iounit_map_dma_init(struct sbus_bus *sbus, int size)
{
	return 0;
}

__u32 iounit_map_dma_page(__u32 vaddr, void *addr, struct sbus_bus *sbus)
{
	return 0;
}
