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
 * File created by Kanoj Sarcar 06/06/00.
 * Copyright 2000 Silicon Graphics, Inc.
 */
#ifndef __ASM_SN_MAPPED_KERNEL_H
#define __ASM_SN_MAPPED_KERNEL_H

/*
 * Note on how mapped kernels work: the text and data section is
 * compiled at cksseg segment (LOADADDR = 0xc001c000), and the
 * init/setup/data section gets a 16M virtual address bump in the
 * ld.script file (so that tlblo0 and tlblo1 maps the sections).
 * The vmlinux.64 section addresses are put in the xkseg range
 * using the change-addresses makefile option. Use elfdump -of
 * on IRIX to see where the sections go. The Origin loader loads
 * the two sections contiguously in physical memory. The loader
 * sets the entry point into kernel_entry using a xkphys address,
 * but instead of using 0xa800000001160000, it uses the address
 * 0xa800000000160000, which is where it physically loaded that
 * code. So no jumps can be done before we have switched to using
 * cksseg addresses.
 */
#include <linux/config.h>
#include <asm/addrspace.h>

#ifdef CONFIG_MAPPED_KERNEL

#define MAPPED_ADDR_RO_TO_PHYS(x)	(x - CKSSEG)
#define MAPPED_ADDR_RW_TO_PHYS(x)	(x - CKSSEG - 16777216)

#define MAPPED_KERN_RO_PHYSBASE(n) \
			(PLAT_NODE_DATA(n)->kern_vars.kv_ro_baseaddr)
#define MAPPED_KERN_RW_PHYSBASE(n) \
			(PLAT_NODE_DATA(n)->kern_vars.kv_rw_baseaddr)

#define MAPPED_KERN_RO_TO_PHYS(x) \
				((unsigned long)MAPPED_ADDR_RO_TO_PHYS(x) | \
				MAPPED_KERN_RO_PHYSBASE(get_compact_nodeid()))
#define MAPPED_KERN_RW_TO_PHYS(x) \
				((unsigned long)MAPPED_ADDR_RW_TO_PHYS(x) | \
				MAPPED_KERN_RW_PHYSBASE(get_compact_nodeid()))
#define MAPPED_OFFSET			16777216

#else /* CONFIG_MAPPED_KERNEL */

#define MAPPED_KERN_RO_TO_PHYS(x)	(x - CKSEG0)
#define MAPPED_KERN_RW_TO_PHYS(x)	(x - CKSEG0)
#define MAPPED_OFFSET			0

#endif /* CONFIG_MAPPED_KERNEL */

#define MAPPED_KERN_RO_TO_K0(x)	PHYS_TO_K0(MAPPED_KERN_RO_TO_PHYS(x))
#define MAPPED_KERN_RW_TO_K0(x)	PHYS_TO_K0(MAPPED_KERN_RW_TO_PHYS(x))

#endif /* __ASM_SN_MAPPED_KERNEL_H  */
