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
 * linux/include/asm-arm/arch-nexuspci/irqs.h
 *
 * Copyright (C) 1997, 1998, 2000 Philip Blundell
 */

/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

/*
 * The hardware is capable of routing any interrupt source (except the
 * DUART) to either IRQ or FIQ.  We ignore FIQ and use IRQ exclusively
 * for simplicity.  
 */

#define IRQ_DUART		0
#define IRQ_PLX 		1
#define IRQ_PCI_D		2
#define IRQ_PCI_C		3
#define IRQ_PCI_B		4
#define IRQ_PCI_A	        5
#define IRQ_SYSERR		6	/* only from IOSLAVE rev B */

#define FIRST_IRQ		IRQ_DUART
#define LAST_IRQ		IRQ_SYSERR

/* timer is part of the DUART */
#define IRQ_TIMER		IRQ_DUART

#define irq_cannonicalize(i)	(i)
