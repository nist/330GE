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
 * include/asm-sh/serial-bigsur.h
 *
 * Configuration details for Big Sur 16550 based serial ports 
 * i.e. HD64465, PCMCIA, etc.
 */

#ifndef _ASM_SERIAL_BIGSUR_H
#define _ASM_SERIAL_BIGSUR_H
#include <asm/hd64465.h>

#define BASE_BAUD (3379200 / 16)

/* Leave 2 spare for possible PCMCIA serial cards */
#define RS_TABLE_SIZE  3

#define STD_COM_FLAGS (ASYNC_BOOT_AUTOCONF | ASYNC_SKIP_TEST)


#define STD_SERIAL_PORT_DEFNS                   \
        /* UART CLK   PORT IRQ     FLAGS        */                      \
        { 0, BASE_BAUD, 0x3F8, HD64465_IRQ_UART, STD_COM_FLAGS } /* ttyS0 */ 


#define SERIAL_PORT_DFNS STD_SERIAL_PORT_DEFNS

#define irq_cannonicalize(x) (x)

#endif /* _ASM_SERIAL_BIGSUR_H */
