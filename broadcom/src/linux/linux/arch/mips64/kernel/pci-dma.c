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
 * Copyright (C) 2000  Ani Joshi <ajoshi@unixbox.com>
 * Copyright (C) 2000, 2001  Ralf Baechle <ralf@gnu.org>
 * swiped from i386, and cloned for MIPS by Geert, polished by Ralf.
 */
#include <linux/config.h>
#include <linux/types.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/pci.h>

#include <asm/io.h>

void *pci_alloc_consistent(struct pci_dev *hwdev, size_t size,
			   dma_addr_t * dma_handle)
{
	void *ret;
	int gfp = GFP_ATOMIC;

#ifdef CONFIG_ISA
	if (hwdev == NULL || hwdev->dma_mask != 0xffffffff)
		gfp |= GFP_DMA;
#endif
	ret = (void *) __get_free_pages(gfp, get_order(size));

	if (ret != NULL) {
		memset(ret, 0, size);
		*dma_handle = bus_to_baddr(hwdev->bus, __pa(ret));
#ifdef CONFIG_NONCOHERENT_IO
		dma_cache_wback_inv((unsigned long) ret, size);
		ret = UNCAC_ADDR(ret);
#endif
	}

	return ret;
}

void pci_free_consistent(struct pci_dev *hwdev, size_t size,
			 void *vaddr, dma_addr_t dma_handle)
{
	unsigned long addr = (unsigned long) vaddr;

#ifdef CONFIG_NONCOHERENT_IO
	addr = CAC_ADDR(addr);
#endif
	free_pages(addr, get_order(size));
}

EXPORT_SYMBOL(pci_alloc_consistent);
EXPORT_SYMBOL(pci_free_consistent);
