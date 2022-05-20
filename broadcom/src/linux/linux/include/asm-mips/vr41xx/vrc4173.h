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
 * FILE NAME
 *	include/asm-mips/vr41xx/vrc4173.h
 *
 * BRIEF MODULE DESCRIPTION
 *	Include file for NEC VRC4173.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2000 by Michael R. McDonald
 *
 * Copyright 2001,2002 Montavista Software Inc.
 * Author: Yoichi Yuasa
 *         yyuasa@mvista.com or source@mvista.com
 */
#ifndef __NEC_VRC4173_H 
#define __NEC_VRC4173_H 

#include <asm/io.h>

/*
 * Interrupt Number
 */
#define VRC4173_IRQ_BASE	72
#define VRC4173_USB_IRQ		(VRC4173_IRQ_BASE + 0)
#define VRC4173_PCMCIA2_IRQ	(VRC4173_IRQ_BASE + 1)
#define VRC4173_PCMCIA1_IRQ	(VRC4173_IRQ_BASE + 2)
#define VRC4173_PS2CH2_IRQ	(VRC4173_IRQ_BASE + 3)
#define VRC4173_PS2CH1_IRQ	(VRC4173_IRQ_BASE + 4)
#define VRC4173_PIU_IRQ		(VRC4173_IRQ_BASE + 5)
#define VRC4173_AIU_IRQ		(VRC4173_IRQ_BASE + 6)
#define VRC4173_KIU_IRQ		(VRC4173_IRQ_BASE + 7)
#define VRC4173_GIU_IRQ		(VRC4173_IRQ_BASE + 8)
#define VRC4173_AC97_IRQ	(VRC4173_IRQ_BASE + 9)
#define VRC4173_AC97INT1_IRQ	(VRC4173_IRQ_BASE + 10)
#define VRC4173_DOZEPIU_IRQ	(VRC4173_IRQ_BASE + 13)
#define VRC4173_IRQ_LAST	VRC4173_DOZEPIU_IRQ

/*
 * PCI I/O accesses
 */
extern unsigned long vrc4173_io_offset;

#define set_vrc4173_io_offset(offset)	do { vrc4173_io_offset = (offset); } while (0)

#define vrc4173_outb(val,port)		outb((val), vrc4173_io_offset+(port))
#define vrc4173_outw(val,port)		outw((val), vrc4173_io_offset+(port))
#define vrc4173_outl(val,port)		outl((val), vrc4173_io_offset+(port))
#define vrc4173_outb_p(val,port)	outb_p((val), vrc4173_io_offset+(port))
#define vrc4173_outw_p(val,port)	outw_p((val), vrc4173_io_offset+(port))
#define vrc4173_outl_p(val,port)	outl_p((val), vrc4173_io_offset+(port))

#define vrc4173_inb(port)		inb(vrc4173_io_offset+(port))
#define vrc4173_inw(port)		inw(vrc4173_io_offset+(port))
#define vrc4173_inl(port)		inl(vrc4173_io_offset+(port))
#define vrc4173_inb_p(port)		inb_p(vrc4173_io_offset+(port))
#define vrc4173_inw_p(port)		inw_p(vrc4173_io_offset+(port))
#define vrc4173_inl_p(port)		inl_p(vrc4173_io_offset+(port))

#define vrc4173_outsb(port,addr,count)	outsb(vrc4173_io_offset+(port),(addr),(count))
#define vrc4173_outsw(port,addr,count)	outsw(vrc4173_io_offset+(port),(addr),(count))
#define vrc4173_outsl(port,addr,count)	outsl(vrc4173_io_offset+(port),(addr),(count))

#define vrc4173_insb(port,addr,count)	insb(vrc4173_io_offset+(port),(addr),(count))
#define vrc4173_insw(port,addr,count)	insw(vrc4173_io_offset+(port),(addr),(count))
#define vrc4173_insl(port,addr,count)	insl(vrc4173_io_offset+(port),(addr),(count))

/*
 * Clock Mask Unit
 */
extern void vrc4173_clock_supply(u16 mask);
extern void vrc4173_clock_mask(u16 mask);

/*
 * General-Purpose I/O Unit
 */
enum {
	PS2CH1_SELECT,
	PS2CH2_SELECT,
	TOUCHPANEL_SELECT,
	KIU8_SELECT,
	KIU10_SELECT,
	KIU12_SELECT,
	GPIO_SELECT
};

extern void vrc4173_select_function(int func);

#endif /* __NEC_VRC4173_H */
