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
#ifndef __ASM_SCATTERLIST_H
#define __ASM_SCATTERLIST_H

struct scatterlist {
	char * address;		/* Location data is to be transferred to,
				   NULL for highmem page */
	struct page * page;	/* Location for highmem page, if any */
	unsigned int offset;
	dma_addr_t dma_address;
	unsigned long length;
};

#define ISA_DMA_THRESHOLD (0x00ffffff)

#endif /* __ASM_SCATTERLIST_H */
