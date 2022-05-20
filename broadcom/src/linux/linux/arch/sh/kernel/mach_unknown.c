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
 * linux/arch/sh/kernel/mach_unknown.c
 *
 * Copyright (C) 2000 Stuart Menefy (stuart.menefy@st.com)
 *
 * May be copied or modified under the terms of the GNU General Public
 * License.  See linux/COPYING for more information.
 *
 * Machine specific code for an unknown machine (internal peripherials only)
 */

#include <linux/config.h>
#include <linux/init.h>

#include <asm/machvec.h>
#include <asm/machvec_init.h>

#include <asm/io_unknown.h>

#include <asm/rtc.h>
/*
 * The Machine Vector
 */

struct sh_machine_vector mv_unknown __initmv = {
	mv_name:		"Unknown",

#if defined(__SH4__)
	mv_nr_irqs:		48,
#elif defined(CONFIG_CPU_SUBTYPE_SH7708)
	mv_nr_irqs:		32,
#elif defined(CONFIG_CPU_SUBTYPE_SH7709)
	mv_nr_irqs:		61,
#endif

	mv_inb:			unknown_inb,
	mv_inw:			unknown_inw,
	mv_inl:			unknown_inl,
	mv_outb:		unknown_outb,
	mv_outw:		unknown_outw,
	mv_outl:		unknown_outl,

	mv_inb_p:		unknown_inb_p,
	mv_inw_p:		unknown_inw_p,
	mv_inl_p:		unknown_inl_p,
	mv_outb_p:		unknown_outb_p,
	mv_outw_p:		unknown_outw_p,
	mv_outl_p:		unknown_outl_p,

	mv_insb:		unknown_insb,
	mv_insw:		unknown_insw,
	mv_insl:		unknown_insl,
	mv_outsb:		unknown_outsb,
	mv_outsw:		unknown_outsw,
	mv_outsl:		unknown_outsl,

	mv_readb:		unknown_readb,
	mv_readw:		unknown_readw,
	mv_readl:		unknown_readl,
	mv_writeb:		unknown_writeb,
	mv_writew:		unknown_writew,
	mv_writel:		unknown_writel,

	mv_ioremap:		unknown_ioremap,
	mv_iounmap:		unknown_iounmap,

	mv_isa_port2addr:	unknown_isa_port2addr,

	mv_rtc_gettimeofday:	sh_rtc_gettimeofday,
	mv_rtc_settimeofday:	sh_rtc_settimeofday,
};
ALIAS_MV(unknown)
