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
 *	linux/arch/alpha/kernel/irq_pyxis.c
 *
 * Based on code written by David A Rusling (david.rusling@reo.mts.dec.com).
 *
 * IRQ Code common to all PYXIS core logic chips.
 */

#include <linux/init.h>
#include <linux/sched.h>
#include <linux/irq.h>

#include <asm/io.h>
#include <asm/core_cia.h>

#include "proto.h"
#include "irq_impl.h"


/* Note mask bit is true for ENABLED irqs.  */
static unsigned long cached_irq_mask;

static inline void
pyxis_update_irq_hw(unsigned long mask)
{
	*(vulp)PYXIS_INT_MASK = mask;
	mb();
	*(vulp)PYXIS_INT_MASK;
}

static inline void
pyxis_enable_irq(unsigned int irq)
{
	pyxis_update_irq_hw(cached_irq_mask |= 1UL << (irq - 16));
}

static void
pyxis_disable_irq(unsigned int irq)
{
	pyxis_update_irq_hw(cached_irq_mask &= ~(1UL << (irq - 16)));
}

static unsigned int
pyxis_startup_irq(unsigned int irq)
{
	pyxis_enable_irq(irq);
	return 0;
}

static void
pyxis_end_irq(unsigned int irq)
{
	if (!(irq_desc[irq].status & (IRQ_DISABLED|IRQ_INPROGRESS)))
		pyxis_enable_irq(irq);
}

static void
pyxis_mask_and_ack_irq(unsigned int irq)
{
	unsigned long bit = 1UL << (irq - 16);
	unsigned long mask = cached_irq_mask &= ~bit;

	/* Disable the interrupt.  */
	*(vulp)PYXIS_INT_MASK = mask;
	wmb();
	/* Ack PYXIS PCI interrupt.  */
	*(vulp)PYXIS_INT_REQ = bit;
	mb();
	/* Re-read to force both writes.  */
	*(vulp)PYXIS_INT_MASK;
}

static struct hw_interrupt_type pyxis_irq_type = {
	typename:	"PYXIS",
	startup:	pyxis_startup_irq,
	shutdown:	pyxis_disable_irq,
	enable:		pyxis_enable_irq,
	disable:	pyxis_disable_irq,
	ack:		pyxis_mask_and_ack_irq,
	end:		pyxis_end_irq,
};

void 
pyxis_device_interrupt(unsigned long vector, struct pt_regs *regs)
{
	unsigned long pld;
	unsigned int i;

	/* Read the interrupt summary register of PYXIS */
	pld = *(vulp)PYXIS_INT_REQ;
	pld &= cached_irq_mask;

	/*
	 * Now for every possible bit set, work through them and call
	 * the appropriate interrupt handler.
	 */
	while (pld) {
		i = ffz(~pld);
		pld &= pld - 1; /* clear least bit set */
		if (i == 7)
			isa_device_interrupt(vector, regs);
		else
			handle_irq(16+i, regs);
	}
}

void __init
init_pyxis_irqs(unsigned long ignore_mask)
{
	long i;

	*(vulp)PYXIS_INT_MASK = 0;		/* disable all */
	*(vulp)PYXIS_INT_REQ  = -1;		/* flush all */
	mb();

	/* Send -INTA pulses to clear any pending interrupts ...*/
	*(vuip) CIA_IACK_SC;

	for (i = 16; i < 48; ++i) {
		if ((ignore_mask >> i) & 1)
			continue;
		irq_desc[i].status = IRQ_DISABLED | IRQ_LEVEL;
		irq_desc[i].handler = &pyxis_irq_type;
	}

	setup_irq(16+7, &isa_cascade_irqaction);
}
