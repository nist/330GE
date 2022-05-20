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
 * linux/include/asm-arm/arch-sa1100/pleb.h
 *
 * Created 2000/12/08 by Daniel Potts <danielp@cse.unsw.edu.au>
 *
 * This file contains the hardware specific definitions for the
 * PLEB board. http://www.cse.unsw.edu.au/~pleb
 */

#ifndef _INCLUDE_PLEB_H_
#define _INCLUDE_PLEB_H_

#define PLEB_ETH0_P		(0x20000300)	/* Ethernet 0 in PCMCIA0 IO */
#define PLEB_ETH0_V		(0xf6000300)

#define GPIO_ETH0_IRQ		GPIO_GPIO (21)
#define GPIO_ETH0_EN		GPIO_GPIO (26)

#define IRQ_GPIO_ETH0_IRQ	IRQ_GPIO21

#endif
