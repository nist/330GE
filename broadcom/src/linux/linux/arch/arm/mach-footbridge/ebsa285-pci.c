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
 * linux/arch/arm/mach-footbridge/ebsa285-pci.c
 *
 * PCI bios-type initialisation for PCI machines
 *
 * Bits taken from various places.
 */
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/init.h>

#include <asm/irq.h>
#include <asm/mach/pci.h>
#include <asm/hardware/dec21285.h>

static int irqmap_ebsa285[] __initdata = { IRQ_IN3, IRQ_IN1, IRQ_IN0, IRQ_PCI };

static u8 __init ebsa285_swizzle(struct pci_dev *dev, u8 *pin)
{
	return PCI_SLOT(dev->devfn);
}

static int __init ebsa285_map_irq(struct pci_dev *dev, u8 slot, u8 pin)
{
	if (dev->vendor == PCI_VENDOR_ID_CONTAQ &&
	    dev->device == PCI_DEVICE_ID_CONTAQ_82C693)
		switch (PCI_FUNC(dev->devfn)) {
			case 1:	return 14;
			case 2:	return 15;
			case 3:	return 12;
		}

	return irqmap_ebsa285[(slot + pin) & 3];
}

struct hw_pci ebsa285_pci __initdata = {
	setup_resources:	dc21285_setup_resources,
	init:			dc21285_init,
	mem_offset:		DC21285_PCI_MEM,
	swizzle:		ebsa285_swizzle,
	map_irq:		ebsa285_map_irq,
};
