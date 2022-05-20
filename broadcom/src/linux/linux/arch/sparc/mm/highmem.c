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
 *  highmem.c: virtual kernel memory mappings for high memory
 *
 *  Provides kernel-static versions of atomic kmap functions originally
 *  found as inlines in include/asm-sparc/highmem.h.  These became
 *  needed as kmap_atomic() and kunmap_atomic() started getting
 *  called from within modules.
 *  -- Tomas Szepe <szepe@pinerecords.com>, September 2002
 */

#include <linux/mm.h>
#include <linux/highmem.h>
#include <asm/pgalloc.h>

/*
 * The use of kmap_atomic/kunmap_atomic is discouraged -- kmap()/kunmap()
 * gives a more generic (and caching) interface.  But kmap_atomic() can
 * be used in IRQ contexts, so in some (very limited) cases we need it.
 */
void *kmap_atomic(struct page *page, enum km_type type)
{
	unsigned long idx;
	unsigned long vaddr;

	if (page < highmem_start_page)
		return page_address(page);

	idx = type + KM_TYPE_NR * smp_processor_id();
	vaddr = fix_kmap_begin + idx * PAGE_SIZE;

	flush_cache_all();

#if HIGHMEM_DEBUG
	if (!pte_none(*(kmap_pte + idx)))
		BUG();
#endif
	set_pte(kmap_pte + idx, mk_pte(page, kmap_prot));
	flush_tlb_all();

	return (void *) vaddr;
}

void kunmap_atomic(void *kvaddr, enum km_type type)
{
	unsigned long vaddr = (unsigned long) kvaddr;
	unsigned long idx = type + KM_TYPE_NR * smp_processor_id();

	if (vaddr < fix_kmap_begin) 
		return;

	if (vaddr != fix_kmap_begin + idx * PAGE_SIZE)
		BUG();

	flush_cache_all();

#ifdef HIGHMEM_DEBUG
	/*
	 *  Force other mappings to oops if they try to access
	 *  this pte without first remapping it.
	 */
	pte_clear(kmap_pte + idx);
	flush_tlb_all();
#endif
}
