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
#ifndef _ASM_PARISC_SCATTERLIST_H
#define _ASM_PARISC_SCATTERLIST_H

#include <asm/page.h>

struct scatterlist {
	/* This will disappear in 2.5.x */
	char *address;

	/* page/offset only valid if ADDRESS member is NULL.
	** Needed to support CONFIG_HIGHMEM on x386.
	** I still think davem is a dork for forcing other
	** arches to add this to 2.4.x. -ggg
	*/
	struct page *page;
	unsigned int offset;

	unsigned int length;

	/* an IOVA can be 64-bits on some PA-Risc platforms. */
	dma_addr_t iova;	/* I/O Virtual Address */
	__u32      iova_length; /* bytes mapped */
};

#define sg_virt_addr(sg) (((sg)->address) ? ((sg)->address) : \
		((sg)->page->virtual + (sg)->offset))

#define sg_dma_address(sg) ((sg)->iova)
#define sg_dma_len(sg)     ((sg)->iova_length)

#define ISA_DMA_THRESHOLD (~0UL)

#endif /* _ASM_PARISC_SCATTERLIST_H */
