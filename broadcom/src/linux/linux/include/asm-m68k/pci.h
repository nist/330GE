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
#ifndef _ASM_M68K_PCI_H
#define _ASM_M68K_PCI_H

/*
 * asm-m68k/pci_m68k.h - m68k specific PCI declarations.
 *
 * Written by Wout Klaren.
 */

struct pci_ops;

/*
 * Structure with hardware dependent information and functions of the
 * PCI bus.
 */

struct pci_bus_info
{
	/*
	 * Resources of the PCI bus.
	 */

	struct resource mem_space;
	struct resource io_space;

	/*
	 * System dependent functions.
	 */

	struct pci_ops *m68k_pci_ops;

	void (*fixup)(int pci_modify);
	void (*conf_device)(unsigned char bus, unsigned char device_fn);
};

#define pcibios_assign_all_busses()	0

extern inline void pcibios_set_master(struct pci_dev *dev)
{
	/* No special bus mastering setup handling */
}

extern inline void pcibios_penalize_isa_irq(int irq)
{
	/* We don't do dynamic PCI IRQ allocation */
}

/* Return the index of the PCI controller for device PDEV. */
#define pci_controller_num(PDEV)	(0)

/* The PCI address space does equal the physical memory
 * address space.  The networking and block device layers use
 * this boolean for bounce buffer decisions.
 */
#define PCI_DMA_BUS_IS_PHYS	(1)

#endif /* _ASM_M68K_PCI_H */
