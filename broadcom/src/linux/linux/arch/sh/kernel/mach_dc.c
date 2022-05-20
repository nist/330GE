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
 *	$Id: mach_dc.c,v 1.1.1.1 2007/01/25 12:51:51 jiahao_jhou Exp $
 *	SEGA Dreamcast machine vector
 */

#include <linux/config.h>
#include <linux/init.h>
#include <linux/time.h>

#include <asm/machvec.h>
#include <asm/machvec_init.h>

#include <asm/io_generic.h>
#include <asm/io_dc.h>
#include <asm/irq.h>

void __init setup_dreamcast(void);
void __init dreamcast_pcibios_init(void);

/* Custom Dreamcast RTC routines */
void aica_rtc_gettimeofday(struct timeval *tv);
int aica_rtc_settimeofday(const struct timeval *tv);

/*
 * The Machine Vector
 */

struct sh_machine_vector mv_dreamcast __initmv = {
	mv_name:		"dreamcast",

	mv_nr_irqs:		NR_IRQS,

	mv_inb:			generic_inb,
	mv_inw:			generic_inw,
	mv_inl:			generic_inl,
	mv_outb:		generic_outb,
	mv_outw:		generic_outw,
	mv_outl:		generic_outl,

	mv_inb_p:		generic_inb_p,
	mv_inw_p:		generic_inw,
	mv_inl_p:		generic_inl,
	mv_outb_p:		generic_outb_p,
	mv_outw_p:		generic_outw,
	mv_outl_p:		generic_outl,

	mv_insb:		generic_insb,
	mv_insw:		generic_insw,
	mv_insl:		generic_insl,
	mv_outsb:		generic_outsb,
	mv_outsw:		generic_outsw,
	mv_outsl:		generic_outsl,

	mv_readb:		generic_readb,
	mv_readw:		generic_readw,
	mv_readl:		generic_readl,
	mv_writeb:		generic_writeb,
	mv_writew:		generic_writew,
	mv_writel:		generic_writel,

	mv_ioremap:		generic_ioremap,
	mv_iounmap:		generic_iounmap,

	mv_init_arch:		setup_dreamcast,
	mv_isa_port2addr:	dreamcast_isa_port2addr,
	mv_irq_demux:		systemasic_irq_demux,

	mv_rtc_gettimeofday:	aica_rtc_gettimeofday,
	mv_rtc_settimeofday:	aica_rtc_settimeofday,

	mv_hw_dreamcast:	1,
};
ALIAS_MV(dreamcast)
