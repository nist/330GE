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
 * linux/include/asm-arm/arch-brutus/uncompress.h
 *
 * (C) 1999 Nicolas Pitre <nico@cam.org>
 *
 * Reorganised to be machine independent.
 */

#include "hardware.h"

/*
 * The following code assumes the serial port has already been
 * initialized by the bootloader.  We search for the first enabled
 * port in the most probable order.  If you didn't setup a port in
 * your bootloader then nothing will appear (which might be desired).
 */

#define UART(x)		(*(volatile unsigned long *)(serial_port + (x)))

static void puts( const char *s )
{
	unsigned long serial_port;

	do {
		serial_port = _Ser3UTCR0;
		if (UART(UTCR3) & UTCR3_TXE) break;
		serial_port = _Ser1UTCR0;
		if (UART(UTCR3) & UTCR3_TXE) break;
		serial_port = _Ser2UTCR0;
		if (UART(UTCR3) & UTCR3_TXE) break;
		return;
	} while (0);

	for (; *s; s++) {
		/* wait for space in the UART's transmiter */
		while (!(UART(UTSR1) & UTSR1_TNF))
			barrier();

		/* send the character out. */
		UART(UTDR) = *s;

		/* if a LF, also do CR... */
		if (*s == 10) {
			while (!(UART(UTSR1) & UTSR1_TNF))
				barrier();
			UART(UTDR) = 13;
		}
	}
}

/*
 * Nothing to do for these
 */
#define arch_decomp_setup()
#define arch_decomp_wdog()
