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
 * highmem.h: virtual kernel memory mappings for high memory
 *
 * Used in CONFIG_HIGHMEM systems for memory pages which
 * are not addressable by direct kernel virtual addresses.
 *
 * Copyright (C) 1999 Gerhard Wichert, Siemens AG
 *		      Gerhard.Wichert@pdb.siemens.de
 *
 *
 * Redesigned the x86 32-bit VM architecture to deal with 
 * up to 16 Terabyte physical memory. With current x86 CPUs
 * we now support up to 64 Gigabytes physical RAM.
 *
 * Copyright (C) 1999 Ingo Molnar <mingo@redhat.com>
 */

#ifndef _ASM_HIGHMEM_H
#define _ASM_HIGHMEM_H

#ifdef __KERNEL__

#include <linux/config.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <asm/kmap_types.h>
#include <asm/pgtable.h>

#ifdef CONFIG_DEBUG_HIGHMEM
#define HIGHMEM_DEBUG 1
#else
#define HIGHMEM_DEBUG 0
#endif

/* declarations for highmem.c */
extern unsigned long highstart_pfn, highend_pfn;

extern pte_t *kmap_pte;
extern pgprot_t kmap_prot;
extern pte_t *pkmap_page_table;

extern void kmap_init(void) __init;

/*
 * Right now we initialize only a single pte table. It can be extended
 * easily, subsequent pte tables have to be allocated in one physical
 * chunk of RAM.
 */
#define PKMAP_BASE (0xfe000000UL)
#ifdef CONFIG_X86_PAE
#define LAST_PKMAP 512
#else
#define LAST_PKMAP 1024
#endif
#define LAST_PKMAP_MASK (LAST_PKMAP-1)
#define PKMAP_NR(virt)  ((virt-PKMAP_BASE) >> PAGE_SHIFT)
#define PKMAP_ADDR(nr)  (PKMAP_BASE + ((nr) << PAGE_SHIFT))

extern void * FASTCALL(kmap_high(struct page *page));
extern void FASTCALL(kunmap_high(struct page *page));

static inline void *kmap(struct page *page)
{
	if (in_interrupt())
		out_of_line_bug();
	if (page < highmem_start_page)
		return page_address(page);
	return kmap_high(page);
}

static inline void kunmap(struct page *page)
{
	if (in_interrupt())
		out_of_line_bug();
	if (page < highmem_start_page)
		return;
	kunmap_high(page);
}

/*
 * The use of kmap_atomic/kunmap_atomic is discouraged - kmap/kunmap
 * gives a more generic (and caching) interface. But kmap_atomic can
 * be used in IRQ contexts, so in some (very limited) cases we need
 * it.
 */
static inline void *kmap_atomic(struct page *page, enum km_type type)
{
	enum fixed_addresses idx;
	unsigned long vaddr;

	if (page < highmem_start_page)
		return page_address(page);

	idx = type + KM_TYPE_NR*smp_processor_id();
	vaddr = __fix_to_virt(FIX_KMAP_BEGIN + idx);
#if HIGHMEM_DEBUG
	if (!pte_none(*(kmap_pte-idx)))
		out_of_line_bug();
#endif
	set_pte(kmap_pte-idx, mk_pte(page, kmap_prot));
	__flush_tlb_one(vaddr);

	return (void*) vaddr;
}

static inline void kunmap_atomic(void *kvaddr, enum km_type type)
{
#if HIGHMEM_DEBUG
	unsigned long vaddr = (unsigned long) kvaddr;
	enum fixed_addresses idx = type + KM_TYPE_NR*smp_processor_id();

	if (vaddr < FIXADDR_START) // FIXME
		return;

	if (vaddr != __fix_to_virt(FIX_KMAP_BEGIN+idx))
		out_of_line_bug();

	/*
	 * force other mappings to Oops if they'll try to access
	 * this pte without first remap it
	 */
	pte_clear(kmap_pte-idx);
	__flush_tlb_one(vaddr);
#endif
}

#endif /* __KERNEL__ */

#endif /* _ASM_HIGHMEM_H */
