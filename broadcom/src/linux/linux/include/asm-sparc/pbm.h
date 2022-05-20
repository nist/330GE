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
/* $Id: pbm.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $
 *
 * pbm.h: PCI bus module pseudo driver software state
 *        Adopted from sparc64 by V. Roganov and G. Raiko
 *
 * Original header:
 * pbm.h: U2P PCI bus module pseudo driver software state.
 *
 * Copyright (C) 1997 David S. Miller (davem@caip.rutgers.edu)
 *
 * To put things into perspective, consider sparc64 with a few PCI controllers.
 * Each type would have an own structure, with instances related one to one.
 * We have only pcic on sparc, but we want to be compatible with sparc64 pbm.h.
 * All three represent different abstractions.
 *   pci_bus  - Linux PCI subsystem view of a PCI bus (including bridged buses)
 *   pbm      - Arch-specific view of a PCI bus (sparc or sparc64)
 *   pcic     - Chip-specific information for PCIC.
 */

#ifndef __SPARC_PBM_H
#define __SPARC_PBM_H

#include <linux/pci.h>
#include <asm/oplib.h>

struct linux_pbm_info {
	int		prom_node;
	char		prom_name[64];
	/* struct linux_prom_pci_ranges	pbm_ranges[PROMREG_MAX]; */
	/* int		num_pbm_ranges; */

	/* Now things for the actual PCI bus probes. */
	unsigned int	pci_first_busno;	/* Can it be nonzero? */
	struct pci_bus	*pci_bus;		/* Was inline, MJ allocs now */
};

/* PCI devices which are not bridges have this placed in their pci_dev
 * sysdata member.  This makes OBP aware PCI device drivers easier to
 * code.
 */
struct pcidev_cookie {
	struct linux_pbm_info		*pbm;
	int				prom_node;
};

#endif /* !(__SPARC_PBM_H) */
