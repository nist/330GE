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
 * linux/arch/sh/kernel/io_ec3104.c
 *  EC3104 companion chip support
 *
 * Copyright (C) 2000 Philipp Rumpf <prumpf@tux.org>
 *
 */
/* EC3104 note:
 * This code was written without any documentation about the EC3104 chip.  While
 * I hope I got most of the basic functionality right, the register names I use
 * are most likely completely different from those in the chip documentation.
 *
 * If you have any further information about the EC3104, please tell me
 * (prumpf@tux.org).
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <asm/io.h>
#include <asm/page.h>
#include <asm/ec3104.h>

/*
 * EC3104 has a real ISA bus which we redirect low port accesses to (the
 * actual device on mine is a ESS 1868, and I don't want to hack the driver
 * more than strictly necessary).  I am not going to duplicate the
 * hard coding of PC addresses (for the 16550s aso) here though;  it's just
 * too ugly.
 */

#define low_port(port) ((port) < 0x10000)

static inline unsigned long port2addr(unsigned long port)
{
	switch(port >> 16) {
	case 0:
		return EC3104_ISA_BASE + port * 2;

	case 1:
		return EC3104_BASE + (port&0xffff) * 4;

	default:
		return 0;
	}
}

unsigned char ec3104_inb(unsigned long port)
{
	u8 ret;

	ret = *(volatile u8 *)port2addr(port);

	return ret;
}

unsigned short ec3104_inw(unsigned long port)
{
	BUG();
}

unsigned long ec3104_inl(unsigned long port)
{
	BUG();
}

void ec3104_outb(unsigned char data, unsigned long port)
{
	*(volatile u8 *)port2addr(port) = data;
}

void ec3104_outw(unsigned short data, unsigned long port)
{
	BUG();
}

void ec3104_outl(unsigned long data, unsigned long port)
{
	BUG();
}
