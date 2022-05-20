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
 *  linux/include/asm-arm/arch-ebsa285/irq.h
 *
 *  Copyright (C) 1996-1998 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Changelog:
 *   22-Aug-1998 RMK	Restructured IRQ routines
 *   03-Sep-1998 PJB	Merged CATS support
 *   20-Jan-1998 RMK	Started merge of EBSA286, CATS and NetWinder
 *   26-Jan-1999 PJB	Don't use IACK on CATS
 *   16-Mar-1999 RMK	Added autodetect of ISA PICs
 */
#include <asm/hardware.h>
#include <asm/hardware/dec21285.h>
#include <asm/irq.h>
#include <asm/mach-types.h>

int isa_irq = -1;

static inline int fixup_irq(unsigned int irq)
{
#ifdef PCIIACK_BASE
	if (irq == isa_irq)
		irq = *(unsigned char *)PCIIACK_BASE;
#endif

	return irq;
}

