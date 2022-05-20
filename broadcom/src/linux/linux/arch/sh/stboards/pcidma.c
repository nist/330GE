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
 * Copyright (C) 2001 David J. Mckay (david.mckay@st.com)
 *
 * May be copied or modified under the terms of the GNU General Public
 * License.  See linux/COPYING for more information.                            
 *
 * Dynamic DMA mapping support.
 */

#include <linux/types.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <linux/pci.h>
#include <asm/io.h>
#include <asm/addrspace.h>


void *pci_alloc_consistent(struct pci_dev *hwdev, size_t size,
			   dma_addr_t * dma_handle)
{
	void *ret;
	int gfp = GFP_ATOMIC;

	ret = (void *) __get_free_pages(gfp, get_order(size));

	if (ret != NULL) {
	        /* Is it neccessary to do the memset? */
		memset(ret, 0, size);
		*dma_handle = virt_to_bus(ret);
	}
	/* We must flush the cache before we pass it on to the device */
	flush_cache_all();
	return  P2SEGADDR(ret);
}

void pci_free_consistent(struct pci_dev *hwdev, size_t size,
			 void *vaddr, dma_addr_t dma_handle)
{
        unsigned long p1addr=P1SEGADDR((unsigned long)vaddr);

	free_pages(p1addr, get_order(size));
}
