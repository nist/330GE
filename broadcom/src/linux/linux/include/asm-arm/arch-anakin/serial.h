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
 *  linux/include/asm-arm/arch-anakin/serial.h
 *
 *  Copyright (C) 2001 Aleph One Ltd. for Acunia N.V.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Changelog:
 *   11-Apr-2001 TTC	Created
 */

#ifndef __ASM_ARCH_SERIAL_H
#define __ASM_ARCH_SERIAL_H

#include <asm/io.h>
#include <asm/irq.h>

/*
 * UART3 and UART4 are not supported yet
 */
#define RS_TABLE_SIZE		3
#define STD_SERIAL_PORT_DEFNS	\
	{ 0, 0, IO_BASE + UART0, IRQ_UART0, 0 }, \
	{ 0, 0, IO_BASE + UART1, IRQ_UART1, 0 }, \
	{ 0, 0, IO_BASE + UART2, IRQ_UART2, 0 }
#define EXTRA_SERIAL_PORT_DEFNS

#endif
