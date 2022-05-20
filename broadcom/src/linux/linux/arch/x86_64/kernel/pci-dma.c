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
 * Dynamic DMA mapping support. Common code
 */

#include <linux/types.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <linux/pci.h>
#include <asm/io.h>

dma_addr_t bad_dma_address = -1UL; 

/* Map a set of buffers described by scatterlist in streaming
 * mode for DMA.  This is the scather-gather version of the
 * above pci_map_single interface.  Here the scatter gather list
 * elements are each tagged with the appropriate dma address
 * and length.  They are obtained via sg_dma_{address,length}(SG).
 *
 * NOTE: An implementation may be able to use a smaller number of
 *       DMA address/length pairs than there are SG table elements.
 *       (for example via virtual mapping capabilities)
 *       The routine returns the number of addr/length pairs actually
 *       used, at most nents.
 *
 * Device ownership issues as mentioned above for pci_map_single are
 * the same here.
 */
int pci_map_sg(struct pci_dev *hwdev, struct scatterlist *sg,
			     int nents, int direction)
{
	int i;

	BUG_ON(direction == PCI_DMA_NONE);
 
 	/*
 	 * temporary 2.4 hack
 	 */
 	for (i = 0; i < nents; i++ ) {
		struct scatterlist *s = &sg[i];
 		if (s->address) {
			BUG_ON(s->page || s->offset); 
 			s->dma_address = pci_map_single(hwdev, s->address, s->length, 
							direction); 
 		} else if (s->page) { 
			s->dma_address = pci_map_page(hwdev, s->page, s->offset,
						      s->length, direction); 
		} else
			BUG(); 

		if (unlikely(s->dma_address == bad_dma_address))
			goto error; 
 	}
	return nents;

 error: 
	pci_unmap_sg(hwdev, sg, i, direction); 
	return 0; 
}

/* Unmap a set of streaming mode DMA translations.
 * Again, cpu read rules concerning calls here are the same as for
 * pci_unmap_single() above.
 */
void pci_unmap_sg(struct pci_dev *dev, struct scatterlist *sg, 
				  int nents, int dir)
{
	int i;
	for (i = 0; i < nents; i++) { 
		struct scatterlist *s = &sg[i];
		BUG_ON(s->address == NULL && s->page == NULL); 
		BUG_ON(s->dma_address == 0); 
		pci_unmap_single(dev, s->dma_address, s->length, dir); 
	} 
}
