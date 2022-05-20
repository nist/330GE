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
#ifndef __ASM_PCI_CHANNEL_H
#define __ASM_PCI_CHANNEL_H

/*
 * This file essentially defines the interface between board
 * specific PCI code and MIPS common PCI code.  Should potentially put
 * into include/asm/pci.h file.
 */

#include <linux/ioport.h>
#include <linux/pci.h>

/*
 * Each pci channel is a top-level PCI bus seem by CPU.  A machine  with
 * multiple PCI channels may have multiple PCI host controllers or a
 * single controller supporting multiple channels.
 */
struct pci_channel {
	struct pci_ops *pci_ops;
	struct resource *io_resource;
	struct resource *mem_resource;
	int first_devfn;
	int last_devfn;
};

/*
 * each board defines an array of pci_channels, that ends with all NULL entry
 */
extern struct pci_channel mips_pci_channels[];

/*
 * board supplied pci irq fixup routine
 */
extern void pcibios_fixup_irqs(void);

/*
 * board supplied pci fixup routines
 */
extern void pcibios_fixup_resources(struct pci_dev *dev);

#endif  /* __ASM_PCI_CHANNEL_H */
