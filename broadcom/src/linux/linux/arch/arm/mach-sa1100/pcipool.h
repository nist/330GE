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

struct pci_pool *pci_pool_create (const char *name, struct pci_dev *dev,
		size_t size, size_t align, size_t allocation, int flags);
void pci_pool_destroy (struct pci_pool *pool);

void *pci_pool_alloc (struct pci_pool *pool, int flags, dma_addr_t *handle);
void pci_pool_free (struct pci_pool *pool, void *vaddr, dma_addr_t addr);

