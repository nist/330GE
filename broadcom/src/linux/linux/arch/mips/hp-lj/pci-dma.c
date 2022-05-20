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
 * Copyright (C) 2000   Ani Joshi <ajoshi@unixbox.com>
 *
 *
 * Dynamic DMA mapping support.
 *
 * swiped from i386, and cloned for MIPS by Geert.
 *
 */

#include <linux/types.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <linux/pci.h>
#include <asm/io.h>

void *pci_alloc_consistent(struct pci_dev *hwdev, size_t size,
			   dma_addr_t *dma_handle)
{
	void *ret;
	int gfp = GFP_ATOMIC;

	if (hwdev == NULL || hwdev->dma_mask != 0xffffffff)
		gfp |= GFP_DMA;
	ret = (void *)__get_free_pages(gfp, get_order(size));
	if (ret != NULL) {
		memset(ret, 0, size);
		*dma_handle = virt_to_bus(ret);


                // REVISIT  this needs reviewal as mentioned in bug report
                // currently we bump kseg0 allocates to kseg1 uncacheable space

                if ((((unsigned int) ret) & 0xe0000000) == 0x80000000) {
	           //flush the cache to eliminate coherency problems
	           // and assure dirty lines won't later get written over any dma, etc.
                   flush_cache_all();
		   ret = (void*)((unsigned int)ret | 0x20000000);
                }


	}
	return ret;
}


void pci_free_consistent(struct pci_dev *hwdev, size_t size,
			 void *vaddr, dma_addr_t dma_handle)
{
	free_pages((unsigned long)vaddr, get_order(size));
}
