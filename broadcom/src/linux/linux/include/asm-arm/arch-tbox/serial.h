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
 * linux/include/asm-arm/arch-tbox/serial.h
 *
 * Copyright (c) 1996 Russell King.
 * Copyright (c) 1998 Phil Blundell
 *
 * Changelog:
 *  15-10-1996	RMK	Created
 *  09-06-1998  PJB	tbox version
 */
#ifndef __ASM_ARCH_SERIAL_H
#define __ASM_ARCH_SERIAL_H

/*
 * This assumes you have a 1.8432 MHz clock for your UART.
 *
 * It'd be nice if someone built a serial card with a 24.576 MHz
 * clock, since the 16550A is capable of handling a top speed of 1.5
 * megabits/second; but this requires the faster clock.
 */
#define BASE_BAUD (1843200 / 16)

#define RS_TABLE_SIZE	2

#define STD_COM_FLAGS (ASYNC_BOOT_AUTOCONF | ASYNC_SKIP_TEST)

     /* UART CLK        PORT  IRQ     FLAGS        */
#define STD_SERIAL_PORT_DEFNS \
	{ 0, BASE_BAUD, 0xffff4000 >> 2, 6, STD_COM_FLAGS }, /* ttyS0 */ \
	{ 0, BASE_BAUD, 0xffff5000 >> 2, 7, STD_COM_FLAGS }, /* ttyS1 */

#define EXTRA_SERIAL_PORT_DEFNS

#endif
