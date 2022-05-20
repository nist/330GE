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
 * include/asm-parisc/serial.h
 */

#include <linux/config.h>
#include <asm/gsc.h>

/*
 * This assumes you have a 7.272727 MHz clock for your UART.
 * The documentation implies a 40Mhz clock, and elsewhere a 7Mhz clock
 * Clarified: 7.2727MHz on LASI. Not yet clarified for DINO
 */

#define LASI_BASE_BAUD ( 7272727 / 16 )
#define BASE_BAUD  LASI_BASE_BAUD

#ifdef CONFIG_SERIAL_DETECT_IRQ
#define STD_COM_FLAGS (ASYNC_BOOT_AUTOCONF | ASYNC_SKIP_TEST | ASYNC_AUTO_IRQ)
#define STD_COM4_FLAGS (ASYNC_BOOT_AUTOCONF | ASYNC_AUTO_IRQ)
#else
#define STD_COM_FLAGS (ASYNC_BOOT_AUTOCONF | ASYNC_SKIP_TEST)
#define STD_COM4_FLAGS ASYNC_BOOT_AUTOCONF
#endif

#ifdef CONFIG_SERIAL_MANY_PORTS
#define FOURPORT_FLAGS ASYNC_FOURPORT
#define ACCENT_FLAGS 0
#define BOCA_FLAGS 0
#define HUB6_FLAGS 0
#define RS_TABLE_SIZE  64
#else
#define RS_TABLE_SIZE  4
#endif
	
/*
 *	The base is relative to the LASI base. We can fix that
 *	up later. We could also virtually map LASI so that we get
 *	nice constants all over our kernel...
 */
 
#define STD_SERIAL_PORT_DEFNS			\
	/* UART CLK   PORT IRQ     FLAGS        */			\
	{ 0, LASI_BASE_BAUD, -1, 4, ASYNC_SKIP_TEST, 0, PORT_UNKNOWN,}, /* ttyS0 */

#define SERIAL_PORT_DFNS		\
	STD_SERIAL_PORT_DEFNS

