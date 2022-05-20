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
 * linux/arch/sh/kernel/io_unknown.c
 *
 * Copyright (C) 2000 Stuart Menefy (stuart.menefy@st.com)
 *
 * May be copied or modified under the terms of the GNU General Public
 * License.  See linux/COPYING for more information.
 *
 * I/O routine for unknown hardware.
 */

static unsigned int unknown_handler(void)
{
	return 0;
}

#define UNKNOWN_ALIAS(fn) \
	void unknown_##fn(void) __attribute__ ((alias ("unknown_handler")));

UNKNOWN_ALIAS(inb)
UNKNOWN_ALIAS(inw)
UNKNOWN_ALIAS(inl)
UNKNOWN_ALIAS(outb)
UNKNOWN_ALIAS(outw)
UNKNOWN_ALIAS(outl)
UNKNOWN_ALIAS(inb_p)
UNKNOWN_ALIAS(inw_p)
UNKNOWN_ALIAS(inl_p)
UNKNOWN_ALIAS(outb_p)
UNKNOWN_ALIAS(outw_p)
UNKNOWN_ALIAS(outl_p)
UNKNOWN_ALIAS(insb)
UNKNOWN_ALIAS(insw)
UNKNOWN_ALIAS(insl)
UNKNOWN_ALIAS(outsb)
UNKNOWN_ALIAS(outsw)
UNKNOWN_ALIAS(outsl)
UNKNOWN_ALIAS(readb)
UNKNOWN_ALIAS(readw)
UNKNOWN_ALIAS(readl)
UNKNOWN_ALIAS(writeb)
UNKNOWN_ALIAS(writew)
UNKNOWN_ALIAS(writel)
UNKNOWN_ALIAS(isa_port2addr)
UNKNOWN_ALIAS(ioremap)
UNKNOWN_ALIAS(iounmap)
