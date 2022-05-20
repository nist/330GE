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
/* $Id: nosun4c.c,v 1.1.1.1 2007/01/25 12:51:51 jiahao_jhou Exp $
 * nosun4c.c: This file is a bunch of dummies for SMP compiles, 
 *         so that it does not need sun4c and avoid ifdefs.
 *
 * Copyright (C) 1998 Jakub Jelinek (jj@sunsite.mff.cuni.cz)
 */

#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <asm/pgtable.h>

static char shouldnothappen[] __initdata = "32bit SMP kernel only supports sun4m and sun4d\n";

/* Dummies */
struct sun4c_mmu_ring {
	unsigned long xxx1[3];
	unsigned char xxx2[2];
	int xxx3;
};
struct sun4c_mmu_ring sun4c_kernel_ring;
struct sun4c_mmu_ring sun4c_kfree_ring;
unsigned long sun4c_kernel_faults;
unsigned long *sun4c_memerr_reg;

static void __init should_not_happen(void)
{
	prom_printf(shouldnothappen);
	prom_halt();
}

unsigned long __init sun4c_paging_init(unsigned long start_mem, unsigned long end_mem)
{
	should_not_happen();
	return 0;
}

void __init ld_mmu_sun4c(void)
{
	should_not_happen();
}

void sun4c_mapioaddr(unsigned long physaddr, unsigned long virt_addr, int bus_type, int rdonly)
{
}

void sun4c_unmapioaddr(unsigned long virt_addr)
{
}

void sun4c_complete_all_stores(void)
{
}

pte_t *sun4c_pte_offset(pmd_t * dir, unsigned long address)
{
	return NULL;
}

void sun4c_update_mmu_cache(struct vm_area_struct *vma, unsigned long address, pte_t pte)
{
}

void __init sun4c_probe_vac(void)
{
	should_not_happen();
}

void __init sun4c_probe_memerr_reg(void)
{
	should_not_happen();
}
