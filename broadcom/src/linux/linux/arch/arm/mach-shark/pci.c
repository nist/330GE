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
 *  linux/arch/arm/mach-shark/pci.c
 *
 *  PCI bios-type initialisation for PCI machines
 *
 *  Bits taken from various places.
 */
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/init.h>

#include <asm/irq.h>
#include <asm/mach/pci.h>

static int __init shark_map_irq(struct pci_dev *dev, u8 slot, u8 pin)
{
	if (dev->bus->number == 0)
		if (dev->devfn == 0) return 255;
		else return 11;
	else return 6;
}

struct hw_pci shark_pci __initdata = {
	init: via82c505_init,
	swizzle: no_swizzle,
	map_irq: shark_map_irq
};
