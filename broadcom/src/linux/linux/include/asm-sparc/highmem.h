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
 * up to 16 Terrabyte physical memory. With current x86 CPUs
 * we now support up to 64 Gigabytes physical RAM.
 *
 * Copyright (C) 1999 Ingo Molnar <mingo@redhat.com>
 */

#ifndef _ASM_HIGHMEM_H
#define _ASM_HIGHMEM_H

#ifdef __KERNEL__

#include <linux/interrupt.h>
#include <asm/kmap_types.h>

/* undef for production */
#define HIGHMEM_DEBUG 1

/* in mm/highmem.c */
extern void *kmap_high(struct page *page);
extern void kunmap_high(struct page *page);

/* declarations for highmem.c */
extern unsigned long highstart_pfn, highend_pfn;

extern pte_t *kmap_pte;
extern pgprot_t kmap_prot;
extern pte_t *pkmap_page_table;

/* This gets set in {srmmu,sun4c}_paging_init() */
extern unsigned long fix_kmap_begin;

/* Only used and set with srmmu? */
extern unsigned long pkmap_base;

extern void kmap_init(void) __init;

/*
 * Right now we initialize only a single pte table. It can be extended
 * easily, subsequent pte tables have to be allocated in one physical
 * chunk of RAM.
 */
#define LAST_PKMAP 1024

#define LAST_PKMAP_MASK		(LAST_PKMAP - 1)
#define PKMAP_NR(virt)		((virt - pkmap_base) >> PAGE_SHIFT)
#define PKMAP_ADDR(nr)		(pkmap_base + ((nr) << PAGE_SHIFT))

/* in arch/sparc/mm/highmem.c */
void *kmap_atomic(struct page *page, enum km_type type);
void kunmap_atomic(void *kvaddr, enum km_type type);

static inline void *kmap(struct page *page)
{
	if (in_interrupt())
		BUG();
	if (page < highmem_start_page)
		return page_address(page);
	return kmap_high(page);
}

static inline void kunmap(struct page *page)
{
	if (in_interrupt())
		BUG();
	if (page < highmem_start_page)
		return;
	kunmap_high(page);
}

#endif /* __KERNEL__ */

#endif /* _ASM_HIGHMEM_H */
