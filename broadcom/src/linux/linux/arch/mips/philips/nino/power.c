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
 *  arch/mips/philips/nino/power.c
 *
 *  Copyright (C) 2000 Jim Pick <jim@jimpick.com>
 *  Copyright (C) 2001 Steven J. Hill (sjhill@realitydiluted.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Power management routines for the Philips Nino
 */
#include <asm/io.h>
#include <asm/tx3912.h>

void nino_wait(void)
{
	/* We stop the CPU to conserve power */
	outl(inl(TX3912_POWER_CTRL) | TX3912_POWER_CTRL_STOPCPU,
		 TX3912_POWER_CTRL);

	/*
	 * We wait until an interrupt happens...
	 */

	/* We resume here */
	outl(inl(TX3912_POWER_CTRL) & ~TX3912_POWER_CTRL_STOPCPU,
		 TX3912_POWER_CTRL);

	/* Give ourselves a little delay */
	__asm__ __volatile__(
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t");
}
