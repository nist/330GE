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
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (c) 1992 - 1997,2001 Silicon Graphics, Inc.  All rights reserved.
 */

#ifndef _ASM_IA64_SN_SN2_SLOTNUM_H
#define _ASM_IA64_SN_SN2_SLOTNUM_H

#define SLOTNUM_MAXLENGTH	16

/*
 * This file defines IO widget to slot/device assignments.
 */


/* This determines module to pnode mapping. */

#define NODESLOTS_PER_MODULE		1
#define NODESLOTS_PER_MODULE_SHFT	1

#define SLOTNUM_NODE_CLASS	0x00	/* Node   */
#define SLOTNUM_ROUTER_CLASS	0x10	/* Router */
#define SLOTNUM_XTALK_CLASS	0x20	/* Xtalk  */
#define SLOTNUM_MIDPLANE_CLASS	0x30	/* Midplane */
#define SLOTNUM_XBOW_CLASS	0x40	/* Xbow  */
#define SLOTNUM_KNODE_CLASS	0x50	/* Kego node */
#define SLOTNUM_PCI_CLASS	0x60	/* PCI widgets on XBridge */
#define SLOTNUM_INVALID_CLASS	0xf0	/* Invalid */

#define SLOTNUM_CLASS_MASK	0xf0
#define SLOTNUM_SLOT_MASK	0x0f

#define SLOTNUM_GETCLASS(_sn)	((_sn) & SLOTNUM_CLASS_MASK)
#define SLOTNUM_GETSLOT(_sn)	((_sn) & SLOTNUM_SLOT_MASK)


#endif /* _ASM_IA64_SN_SN2_SLOTNUM_H */
