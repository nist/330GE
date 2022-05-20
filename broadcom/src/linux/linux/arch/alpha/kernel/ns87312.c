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
 *	linux/arch/alpha/kernel/ns87312.c
 */

#include <linux/init.h>
#include <asm/io.h>
#include "proto.h"


/*
 * The SRM console *disables* the IDE interface, this code ensures it's
 * enabled.
 *
 * This code bangs on a control register of the 87312 Super I/O chip
 * that implements parallel port/serial ports/IDE/FDI.  Depending on
 * the motherboard, the Super I/O chip can be configured through a
 * pair of registers that are located either at I/O ports 0x26e/0x26f
 * or 0x398/0x399.  Unfortunately, autodetecting which base address is
 * in use works only once (right after a reset).  The Super I/O chip
 * has the additional quirk that configuration register data must be
 * written twice (I believe this is a safety feature to prevent
 * accidental modification---fun, isn't it?).
 */

void __init
ns87312_enable_ide(long ide_base)
{
	int data;
	unsigned long flags;

	__save_and_cli(flags);
	outb(0, ide_base);		/* set the index register for reg #0 */
	data = inb(ide_base+1);		/* read the current contents */
	outb(0, ide_base);		/* set the index register for reg #0 */
	outb(data | 0x40, ide_base+1);	/* turn on IDE */
	outb(data | 0x40, ide_base+1);	/* turn on IDE, really! */
	__restore_flags(flags);
}
