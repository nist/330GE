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
 * linux/include/asm-arm/arch-tbox/irqs.h
 *
 * Copyright (C) 1998, 2000 Philip Blundell
 */

/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#define IRQ_MPEGDMA		0
#define IRQ_ASHTX		1
#define IRQ_ASHRX		2
#define IRQ_VSYNC		3
#define IRQ_HSYNC		4
#define IRQ_MPEG		5
#define IRQ_UART2		6
#define IRQ_UART1		7
#define IRQ_ETHERNET		8
#define IRQ_TIMER		9
#define IRQ_AUDIODMA		10
/* bit 11 used for video field ident */
#define IRQ_EXPMODCS0		12
#define IRQ_EXPMODCS1		13

#define irq_cannonicalize(i)	(i)
