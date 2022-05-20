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
#include <asm/contregs.h>
#include <asm/sun3mmu.h>
#include <asm/io.h>
#include <asm/movs.h>

#define FC_CONTROL 3    /* This should go somewhere else... */

void sun3_leds(unsigned char byte)
{
	unsigned char dfc;
	
	GET_DFC(dfc);
        SET_DFC(FC_CONTROL);
       	SET_CONTROL_BYTE(AC_LEDS,byte);
	SET_DFC(dfc);
}
