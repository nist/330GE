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
#ifndef _ASM_IA64_PCI_H
#define _ASM_IA64_PCI_H

#include <linux/slab.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/spinlock.h>

#include <asm/io.h>
#include <asm/scatterlist.h>

/*
 * Can be used to override the logic in pci_scan_bus for skipping already-configured bus
 * numbers - to be used for buggy BIOSes or architectures with incomplete PCI setup by the
 * loader.
 */
#define pcibios_assign_all_busses()     0

#define PCIBIOS_MIN_IO		0x1000
#define PCIBIOS_MIN_MEM		0x10000000

void pcibios_config_init(void);
struct pci_bus *pcibios_scan_root(void *acpi_handle, int segment, int bus);
extern int (*pci_config_read)(int seg, int bus, int dev, int fn, int reg, int len, u32 *value);
extern int (*pci_config_write)(int seg, int bus, int dev, int fn, int reg, int len, u32 value);

struct pci_dev;

static inline void
pcibios_set_master (struct pci_dev *dev)
{
	/* No special bus mastering setup handling */
}

static inline void
pcibios_penalize_isa_irq (int irq)
{
	/* We don't do dynamic PCI IRQ allocation */
}

/*
 * Dynamic DMA mapping API.  See Documentation/DMA-mapping.txt for details.
 */
#define pci_alloc_consistent		platform_pci_alloc_consistent
#define pci_free_consistent		platform_pci_free_consistent
#define pci_map_single			platform_pci_map_single
#define pci_unmap_single		platform_pci_unmap_single
#define pci_map_sg			platform_pci_map_sg
#define pci_unmap_sg			platform_pci_unmap_sg
#define pci_dma_sync_single		platform_pci_dma_sync_single
#define pci_dma_sync_sg			platform_pci_dma_sync_sg
#define sg_dma_address			platform_pci_dma_address
#define pci_dma_supported		platform_pci_dma_supported

/* pci_unmap_{single,page} is not a nop, thus... */
#define DECLARE_PCI_UNMAP_ADDR(addr_name)	dma_addr_t addr_name;
#define DECLARE_PCI_UNMAP_LEN(len_name)		__u32 len_name;
#define pci_unmap_addr(ptr, addr_name)		((ptr)->addr_name)
#define pci_unmap_addr_set(ptr, addr_name, val)	(((ptr)->addr_name) = (val))
#define pci_unmap_len(ptr, len_name)		((ptr)->len_name)
#define pci_unmap_len_set(ptr, len_name, val)	(((ptr)->len_name) = (val))

#define pci_map_page(dev,pg,off,size,dir)				\
	pci_map_single((dev), page_address(pg) + (off), (size), (dir))
#define pci_unmap_page(dev,dma_addr,size,dir)				\
	pci_unmap_single((dev), (dma_addr), (size), (dir))

/* The ia64 platform always supports 64-bit addressing. */
#define pci_dac_dma_supported(pci_dev, mask)	(1)

#define pci_dac_page_to_dma(dev,pg,off,dir)	((dma_addr_t) page_to_bus(pg) + (off))
#define pci_dac_dma_to_page(dev,dma_addr)	(virt_to_page(bus_to_virt(dma_addr)))
#define pci_dac_dma_to_offset(dev,dma_addr)	((dma_addr) & ~PAGE_MASK)
#define pci_dac_dma_sync_single(dev,dma_addr,len,dir)	do { /* nothing */ } while (0)

/* Return the index of the PCI controller for device PDEV. */
#define pci_controller_num(PDEV)	(0)

#define sg_dma_len(sg)		((sg)->length)

#define HAVE_PCI_MMAP
extern int pci_mmap_page_range (struct pci_dev *dev, struct vm_area_struct *vma,
				enum pci_mmap_state mmap_state, int write_combine);

struct pci_controller {
	void *acpi_handle;
	void *iommu;
	int segment;

	u64 mem_offset;

	void *platform_data;
};

#define PCI_CONTROLLER(dev) ((struct pci_controller *) dev->sysdata)
#define PCI_SEGMENT(dev)    (PCI_CONTROLLER(dev)->segment)

#endif /* _ASM_IA64_PCI_H */
