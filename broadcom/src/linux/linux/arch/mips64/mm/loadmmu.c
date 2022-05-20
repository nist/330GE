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
 * Copyright (C) 1996 David S. Miller (dm@engr.sgi.com)
 * Copyright (C) 1997, 1999 Ralf Baechle (ralf@gnu.org)
 * Copyright (C) 1999 Silicon Graphics, Inc.
 */
#include <linux/config.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>

#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/system.h>
#include <asm/bootinfo.h>
#include <asm/cpu.h>

/* memory functions */
void (*_clear_page)(void * page);
void (*_copy_page)(void * to, void * from);

/* Cache operations. */
void (*_flush_cache_all)(void);
void (*___flush_cache_all)(void);
void (*_flush_cache_mm)(struct mm_struct *mm);
void (*_flush_cache_range)(struct mm_struct *mm, unsigned long start,
                           unsigned long end);
void (*_flush_cache_page)(struct vm_area_struct *vma, unsigned long page);
void (*_flush_cache_sigtramp)(unsigned long addr);
void (*_flush_icache_range)(unsigned long start, unsigned long end);
void (*_flush_icache_page)(struct vm_area_struct *vma, struct page *page);
void (*_flush_page_to_ram)(struct page * page);
void (*_flush_icache_all)(void);

/* MIPS specific cache operations */
void (*_flush_cache_l2)(void);
void (*_flush_cache_l1)(void);


/* DMA cache operations. */
void (*_dma_cache_wback_inv)(unsigned long start, unsigned long size);
void (*_dma_cache_wback)(unsigned long start, unsigned long size);
void (*_dma_cache_inv)(unsigned long start, unsigned long size);

/* Miscellaneous. */
void (*_update_mmu_cache)(struct vm_area_struct * vma,
	unsigned long address, pte_t pte);

extern void ld_mmu_r4xx0(void);
extern void ld_mmu_andes(void);
extern void ld_mmu_sb1(void);
extern void sb1_tlb_init(void);
extern void ld_mmu_mips64(void);
extern void r4k_tlb_init(void);

void __init load_mmu(void)
{
	if (mips_cpu.options & MIPS_CPU_4KTLB) {
#if defined(CONFIG_CPU_R4300)						\
    || defined (CONFIG_CPU_R4X00)					\
    || defined (CONFIG_CPU_R5000)					\
    || defined (CONFIG_CPU_NEVADA)
		printk(KERN_INFO "Loading R4000 MMU routines.\n");
		ld_mmu_r4xx0();
		r4k_tlb_init();
#endif
#if defined(CONFIG_CPU_MIPS64)
		printk(KERN_INFO "Loading MIPS64 MMU routines.\n");
		ld_mmu_mips64();
		r4k_tlb_init();
#endif

	} else switch(mips_cpu.cputype) {
#ifdef CONFIG_CPU_R10000
	case CPU_R10000:
	case CPU_R12000:
		printk(KERN_INFO "Loading R10000 MMU routines.\n");
		ld_mmu_andes();
		andes_tlb_init();
		break;
#endif
#if defined CONFIG_CPU_SB1
	case CPU_SB1:
		printk(KERN_INFO "Loading SB1 MMU routines.\n");
		ld_mmu_sb1();
		sb1_tlb_init();
		break;
#endif

	case CPU_R8000:
		panic("R8000 is unsupported");
		break;

	default:
		panic("Yeee, unsupported mmu/cache architecture or "
		      "wrong compiletime kernel configuration.");
	}
}
