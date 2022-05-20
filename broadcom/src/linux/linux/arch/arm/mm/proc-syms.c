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
 *  linux/arch/arm/mm/proc-syms.c
 *
 *  Copyright (C) 2000 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/module.h>
#include <asm/proc-fns.h>

#ifndef MULTI_CPU
EXPORT_SYMBOL(cpu_cache_clean_invalidate_all);
EXPORT_SYMBOL(cpu_cache_clean_invalidate_range);
EXPORT_SYMBOL(cpu_flush_ram_page);
EXPORT_SYMBOL(cpu_dcache_clean_page);
EXPORT_SYMBOL(cpu_dcache_clean_entry);
EXPORT_SYMBOL(cpu_dcache_clean_range);
EXPORT_SYMBOL(cpu_dcache_invalidate_range);
EXPORT_SYMBOL(cpu_icache_invalidate_range);
EXPORT_SYMBOL(cpu_icache_invalidate_page);
EXPORT_SYMBOL(cpu_tlb_invalidate_all);
EXPORT_SYMBOL(cpu_tlb_invalidate_range);
EXPORT_SYMBOL(cpu_tlb_invalidate_page);
EXPORT_SYMBOL(cpu_set_pgd);
EXPORT_SYMBOL(cpu_set_pmd);
EXPORT_SYMBOL(cpu_set_pte);
#else
EXPORT_SYMBOL(processor);
#endif
