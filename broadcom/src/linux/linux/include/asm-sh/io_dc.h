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
 *	$Id: io_dc.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $
 *	IO functions for SEGA Dreamcast
 */

#ifndef _ASM_SH_IO_DREAMCAST_H
#define _ASM_SH_IO_DREAMCAST_H

#include <asm/io_generic.h>

unsigned long dreamcast_isa_port2addr(unsigned long offset);

#ifdef __WANT_IO_DEF

# define __inb			generic_inb
# define __inw			generic_inw
# define __inl			generic_inl
# define __outb			generic_outb
# define __outw			generic_outw
# define __outl			generic_outl

# define __inb_p		generic_inb_p
# define __inw_p		generic_inw
# define __inl_p		generic_inl
# define __outb_p		generic_outb_p
# define __outw_p		generic_outw
# define __outl_p		generic_outl

# define __insb			generic_insb
# define __insw			generic_insw
# define __insl			generic_insl
# define __outsb		generic_outsb
# define __outsw		generic_outsw
# define __outsl		generic_outsl

# define __readb		generic_readb
# define __readw		generic_readw
# define __readl		generic_readl
# define __writeb		generic_writeb
# define __writew		generic_writew
# define __writel		generic_writel

# define __isa_port2addr	dreamcast_isa_port2addr
# define __ioremap		generic_ioremap
# define __iounmap		generic_iounmap

#endif

#endif /* _ASM_SH_IO_DREAMCAST_H */
