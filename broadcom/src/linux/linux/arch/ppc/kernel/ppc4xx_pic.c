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
 * BK Id: SCCS/s.ppc4xx_pic.c 1.5 05/17/01 18:14:21 cort
 */
/*
 *
 *    Copyright (c) 1999 Grant Erickson <grant@lcse.umn.edu>
 *
 *    Module name: ppc4xx_pic.c
 *
 *    Description:
 *      Interrupt controller driver for PowerPC 4xx-based processors.
 */

/*
 * The PowerPC 403 cores' Asynchronous Interrupt Controller (AIC) has
 * 32 possible interrupts, a majority of which are not implemented on
 * all cores. There are six configurable, external interrupt pins and
 * there are eight internal interrupts for the on-chip serial port
 * (SPU), DMA controller, and JTAG controller.
 *
 * The PowerPC 405 cores' Universal Interrupt Controller (UIC) has 32
 * possible interrupts as well. There are seven, configurable external
 * interrupt pins and there are 17 internal interrupts for the on-chip
 * serial port, DMA controller, on-chip Ethernet controller, PCI, etc.
 *
 */

#include <linux/init.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/stddef.h>

#include <asm/processor.h>
#include <asm/system.h>
#include <asm/irq.h>
#include <asm/ppc4xx_pic.h>

/* Global Variables */

struct hw_interrupt_type *ppc4xx_pic;


/* Function Prototypes */

static void	 ppc403_aic_enable(unsigned int irq);
static void	 ppc403_aic_disable(unsigned int irq);
static void	 ppc403_aic_disable_and_ack(unsigned int irq);

static void	 ppc405_uic_enable(unsigned int irq);
static void	 ppc405_uic_disable(unsigned int irq);
static void	 ppc405_uic_disable_and_ack(unsigned int irq);

static struct hw_interrupt_type ppc403_aic = {
	"403GC AIC",
	NULL,
	NULL,
	ppc403_aic_enable,
	ppc403_aic_disable,
	ppc403_aic_disable_and_ack,
	0
};

static struct hw_interrupt_type ppc405_uic = {
	"405GP UIC",
	NULL,
	NULL,
	ppc405_uic_enable,
	ppc405_uic_disable,
	ppc405_uic_disable_and_ack,
	0
};

/*
 * Document me.
 */
void __init
ppc4xx_pic_init(void)
{
	unsigned long ver = PVR_VER(mfspr(SPRN_PVR));

	switch (ver) {

	case PVR_VER(PVR_403GC):
		/*
		 * Disable all external interrupts until they are
		 * explicity requested.
		 */
		ppc_cached_irq_mask[0] = 0;
		mtdcr(DCRN_EXIER, 0);

		ppc4xx_pic = &ppc403_aic;
		break;

	case PVR_VER(PVR_405GP):
		ppc4xx_pic = &ppc405_uic;
		break;
	}

	return;
}

int
ppc4xx_pic_get_irq(struct pt_regs *regs)
{
	int irq;
	unsigned long bits, mask = (1 << 31);

	/*
	 * Only report the status of those interrupts that are actually
	 * enabled.
	 */

	bits = mfdcr(DCRN_EXISR) & mfdcr(DCRN_EXIER);

	/*
	 * Walk through the interrupts from highest priority to lowest, and
	 * report the first pending interrupt found.
	 */

	for (irq = 0; irq < NR_IRQS; irq++, mask >>= 1) {
		if (bits & mask)
			break;
	}

	return (irq);
}

/*
 * Document me.
 */
static void
ppc403_aic_enable(unsigned int irq)
{
	int bit, word;

	bit = irq & 0x1f;
	word = irq >> 5;

	ppc_cached_irq_mask[word] |= (1 << (31 - bit));
	mtdcr(DCRN_EXIER, ppc_cached_irq_mask[word]);
}

/*
 * Document me.
 */
static void
ppc403_aic_disable(unsigned int irq)
{
	int bit, word;

	bit = irq & 0x1f;
	word = irq >> 5;

	ppc_cached_irq_mask[word] &= ~(1 << (31 - bit));
	mtdcr(DCRN_EXIER, ppc_cached_irq_mask[word]);
}

/*
 * Document me.
 */
static void
ppc403_aic_disable_and_ack(unsigned int irq)
{
	int bit, word;

	bit = irq & 0x1f;
	word = irq >> 5;

	ppc_cached_irq_mask[word] &= ~(1 << (31 - bit));
	mtdcr(DCRN_EXIER, ppc_cached_irq_mask[word]);
	mtdcr(DCRN_EXISR, (1 << (31 - bit)));
}

/*
 * Document me.
 */
static void
ppc405_uic_enable(unsigned int irq)
{
}

/*
 * Document me.
 */
static void
ppc405_uic_disable(unsigned int irq)
{
}

/*
 * Document me.
 */
static void
ppc405_uic_disable_and_ack(unsigned int irq)
{
}
