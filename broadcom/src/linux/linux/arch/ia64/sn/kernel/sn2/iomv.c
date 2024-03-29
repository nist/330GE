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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2000-2002 Silicon Graphics, Inc. All rights reserved.
 */

#include <linux/pci.h>
#include <asm/io.h>
#include <asm/delay.h>
#include <asm/sn/simulator.h>
#include <asm/sn/pda.h>
#include <asm/sn/sn_cpuid.h>

/**
 * sn_io_addr - convert an in/out port to an i/o address
 * @port: port to convert
 *
 * Legacy in/out instructions are converted to ld/st instructions
 * on IA64.  This routine will convert a port number into a valid 
 * SN i/o address.  Used by sn_in*() and sn_out*().
 */
void *
sn_io_addr(unsigned long port)
{
	if (!IS_RUNNING_ON_SIMULATOR()) {
		return( (void *)  (port | __IA64_UNCACHED_OFFSET));
	} else {
		unsigned long io_base;
		unsigned long addr;
 
		/*
 		 * word align port, but need more than 10 bits
 		 * for accessing registers in bedrock local block
 		 * (so we don't do port&0xfff)
 		 */
		if ((port >= 0x1f0 && port <= 0x1f7) ||
			port == 0x3f6 || port == 0x3f7) {
			io_base = (0xc000000fcc000000 | ((unsigned long)get_nasid() << 38));
			addr = io_base | ((port >> 2) << 12) | (port & 0xfff);
		} else {
			addr = __ia64_get_io_port_base() | ((port >> 2) << 2);
		}
		return(void *) addr;
	}
}

/**
 * sn2_mmiob - I/O space memory barrier
 *
 * Acts as a memory mapped I/O barrier for platforms that queue writes to 
 * I/O space.  This ensures that subsequent writes to I/O space arrive after
 * all previous writes.  For most ia64 platforms, this is a simple
 * 'mf.a' instruction.  For other platforms, mmiob() may have to read
 * a chipset register to ensure ordering.
 *
 * On SN2, we wait for the PIO_WRITE_STATUS SHub register to clear.
 */
void
sn2_mmiob (void)
{
	while ( !((volatile unsigned long) (*pda.pio_write_status_addr)) & 0x8000000000000000)
		udelay(5);
}
