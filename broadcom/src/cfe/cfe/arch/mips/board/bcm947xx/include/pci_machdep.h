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
 * Copyright (c) 2001,2002,2003 SiByte, Inc.  All rights reserved.
 */

#ifndef _PCI_MACHDEP_H_
#define _PCI_MACHDEP_H_

/*
 * Machine-specific definitions for PCI autoconfiguration.
 *
 * See the comments in pci_machdep.c for more explanation.
 */

#include "lib_types.h"

/*
 * Address types, as integers.
 */
typedef uint32_t pci_addr_t;
typedef uint32_t phys_addr_t;

/*
 * Root of the bus hierarchy, one per host bridge.
 */
typedef struct pciroot_s *pciroot_t;

#define PCI_ROOTMAX 1

/*
 * Configuration tag; created from a {bus,device,function} triplet by
 * pci_make_tag(), and passed to pci_conf_read() and pci_conf_write().
 */
typedef uint32_t pcitag_t;

/*
 * Type of a value read from or written to a configuration register.
 * Always 32 bits.
 */
typedef uint32_t pcireg_t;

/*
 * The number of rooted bus trees to be configured (i.e., host bridges
 * with independent address spaces).
 */
#define PCI_HOST_PORTS 1
#define PCI_PORTMAX    0x00

#endif /* _PCI_MACHDEP_H_ */
