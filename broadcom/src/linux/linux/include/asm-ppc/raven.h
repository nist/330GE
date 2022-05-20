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
 * BK Id: SCCS/s.raven.h 1.7 05/17/01 18:14:25 cort
 */
/*
 *  asm-ppc/raven.h -- Raven MPIC chip.
 *
 *  Copyright (C) 1998 Johnnie Peters
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License.  See the file COPYING in the main directory of this archive
 *  for more details.
 */

#ifdef __KERNEL__
#ifndef _ASMPPC_RAVEN_H
#define _ASMPPC_RAVEN_H

#define MVME2600_INT_SIO		0
#define MVME2600_INT_FALCN_ECC_ERR	1
#define MVME2600_INT_PCI_ETHERNET	2
#define MVME2600_INT_PCI_SCSI		3
#define MVME2600_INT_PCI_GRAPHICS	4
#define MVME2600_INT_PCI_VME0		5
#define MVME2600_INT_PCI_VME1		6
#define MVME2600_INT_PCI_VME2		7
#define MVME2600_INT_PCI_VME3		8
#define MVME2600_INT_PCI_INTA		9
#define MVME2600_INT_PCI_INTB		10
#define MVME2600_INT_PCI_INTC 		11
#define MVME2600_INT_PCI_INTD 		12
#define MVME2600_INT_LM_SIG0		13
#define MVME2600_INT_LM_SIG1		14

extern struct hw_interrupt_type raven_pic;

extern int raven_init(void);
#endif /* _ASMPPC_RAVEN_H */
#endif /* __KERNEL__ */
