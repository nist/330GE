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
 * arch/ppc/platforms/pal4_pci.c
 * 
 * PCI support for SBS Palomar IV
 *
 * Author: Dan Cox 
 *
 * Copyright 2002 MontaVista Software Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/pci.h>

#include <asm/byteorder.h>
#include <asm/machdep.h>
#include <asm/io.h>
#include <asm/pci-bridge.h>
#include <asm/uaccess.h>

#include "cpc700.h"
#include "pal4.h"

/* not much to this.... */
static inline int __init
pal4_map_irq(struct pci_dev *dev, unsigned char idsel, unsigned char pin)
{
	if (idsel == 9)
		return PAL4_ETH;
	else
		return PAL4_INTA + (idsel - 3);
}

void __init
pal4_find_bridges(void)
{
	struct pci_controller *hose;

	hose = pcibios_alloc_controller();
	if (!hose)
		return;

	hose->first_busno = 0;
	hose->last_busno = 0xff;
	hose->pci_mem_offset = 0;

	/* Could snatch these from the CPC700.... */
	pci_init_resource(&hose->io_resource,
			  0x0, 
			  0x03ffffff,
			  IORESOURCE_IO,
			  "PCI host bridge");

	pci_init_resource(&hose->mem_resources[0],
			  0x90000000,
			  0x9fffffff,
			  IORESOURCE_MEM,
			  "PCI host bridge");

	hose->io_space.start = 0x00800000;
	hose->io_space.end = 0x03ffffff;
	hose->mem_space.start = 0x90000000;
	hose->mem_space.end = 0x9fffffff;
	hose->io_base_virt = (void *) 0xf8000000;
	
	setup_indirect_pci(hose, CPC700_PCI_CONFIG_ADDR,
			   CPC700_PCI_CONFIG_DATA);
	
	hose->last_busno = pciauto_bus_scan(hose, hose->first_busno);

	ppc_md.pci_swizzle = common_swizzle;
	ppc_md.pci_map_irq = pal4_map_irq;
}