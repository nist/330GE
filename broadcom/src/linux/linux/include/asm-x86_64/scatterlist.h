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
#ifndef _X8664_SCATTERLIST_H
#define _X8664_SCATTERLIST_H

/*
 * Drivers must set either ->address or (preferred) ->page and ->offset
 * to indicate where data must be transferred to/from.
 *
 * Using ->page is recommended since it handles highmem data as well as
 * low mem. ->address is restricted to data which has a virtual mapping, and
 * it will go away in the future. Updating to ->page can be automated very
 * easily -- something like
 *
 * sg->address = some_ptr;
 *
 * can be rewritten as
 *
 * sg->page = virt_to_page(some_ptr);
 * sg->offset = (unsigned long) some_ptr & ~PAGE_MASK;
 *
 * and that's it. There's no excuse for not highmem enabling YOUR driver. /jens
 */
struct scatterlist {
    char *  address;    /* Location data is to be transferred to, NULL for
			 * highmem page */
    struct page * page; /* Location for highmem page, if any */
    unsigned int offset;/* for highmem, page offset */

    dma_addr_t dma_address;
    unsigned int length;
};

#define ISA_DMA_THRESHOLD (0x00ffffff)

#endif 
