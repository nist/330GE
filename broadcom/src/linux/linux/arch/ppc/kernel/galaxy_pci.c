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
 * BK Id: %F% %I% %G% %U% %#%
 */
/*
 *
 *    Copyright (c) 2000 Grant Erickson <grant@borg.umn.edu>
 *    All rights reserved.
 *
 *    Module name: galaxy_pci.c
 *
 *    Description:
 *      PCI interface code for the IBM PowerPC 405GP on-chip PCI bus
 *      interface.
 *
 *      Why is this file called "galaxy_pci"? Because on the original
 *      IBM "Walnut" evaluation board schematic I have, the 405GP is
 *      is labeled "GALAXY".
 *
 */

#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/string.h>
#include <linux/init.h>

#include <asm/processor.h>
#include <asm/system.h>
#include <asm/io.h>
#include <asm/machdep.h>
#include <asm/pci-bridge.h>


/* Preprocessor Defines */

#define	PCICFGADDR	(volatile unsigned int *)(0xEEC00000)
#define	PCICFGDATA	(volatile unsigned int *)(0xEEC00004)


/* Function Prototypes */

void __init
galaxy_pcibios_fixup(void)
{

}

static int
galaxy_pcibios_read_config_byte(struct pci_controller* hose,
				  u8 bus, u8 dev, u8 offset, u8 *val)
{

	return (PCIBIOS_SUCCESSFUL);
}

static int
galaxy_pcibios_read_config_word(struct pci_controller* hose,
				  u8 bus, u8 dev, u8 offset, u16 *val)
{

	return (PCIBIOS_SUCCESSFUL);
}

static int
galaxy_pcibios_read_config_dword(struct pci_controller* hose,
				  u8 bus, u8 dev, u8 offset, u32 *val)
{

	return (PCIBIOS_SUCCESSFUL);
}

static int
galaxy_pcibios_write_config_byte(struct pci_controller* hose,
				  u8 bus, u8 dev, u8 offset, u8 val)
{

	return (PCIBIOS_SUCCESSFUL);
}

static int
galaxy_pcibios_write_config_word(struct pci_controller* hose,
				  u8 bus, u8 dev, u8 offset, u16 val)
{

	return (PCIBIOS_SUCCESSFUL);
}

static int
galaxy_pcibios_write_config_dword(struct pci_controller* hose,
				  u8 bus, u8 dev, u8 offset, u32 val)
{

	return (PCIBIOS_SUCCESSFUL);
}

static struct pci_controller_ops galaxy_pci_ops =
{
	galaxy_pcibios_read_config_byte,
	galaxy_pcibios_read_config_word,
	galaxy_pcibios_read_config_dword,
	galaxy_pcibios_write_config_byte,
	galaxy_pcibios_write_config_word,
	galaxy_pcibios_write_config_dword
};

void __init
galaxy_find_bridges(void)
{
	struct pci_controller* hose;

	set_config_access_method(galaxy);

	ppc_md.pcibios_fixup = galaxy_pcibios_fixup;
	hose = pcibios_alloc_controller();
	if (!hose)
		return;
	hose->ops = &galaxy_pci_ops;
	/* Todo ...
	hose->cfg_data = ioremap(PCICFGDATA, ...);
	hose->cfg_addr = ioremap(PCICFGADDR, ...);
	*/
}
