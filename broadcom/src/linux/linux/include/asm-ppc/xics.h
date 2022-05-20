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
 * BK Id: SCCS/s.xics.h 1.5 05/17/01 18:14:22 cort
 */
/*
 * arch/ppc/kernel/xics.h
 *
 * Copyright 2000 IBM Corporation.
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version
 *  2 of the License, or (at your option) any later version.
 */

#ifndef _PPC_KERNEL_XICS_H
#define _PPC_KERNEL_XICS_H

extern struct hw_interrupt_type xics_pic;
extern struct hw_interrupt_type xics_8259_pic;

void xics_init_IRQ(void);
int xics_get_irq(struct pt_regs *);

#endif /* _PPC_KERNEL_XICS_H */
