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
 * Handle interrupts from the SRM, assuming no additional weirdness.
 */

#include <linux/init.h>
#include <linux/sched.h>
#include <linux/irq.h>

#include "proto.h"
#include "irq_impl.h"


/*
 * Is the palcode SMP safe? In other words: can we call cserve_ena/dis
 * at the same time in multiple CPUs? To be safe I added a spinlock
 * but it can be removed trivially if the palcode is robust against smp.
 */
spinlock_t srm_irq_lock = SPIN_LOCK_UNLOCKED;

static inline void
srm_enable_irq(unsigned int irq)
{
	spin_lock(&srm_irq_lock);
	cserve_ena(irq - 16);
	spin_unlock(&srm_irq_lock);
}

static void
srm_disable_irq(unsigned int irq)
{
	spin_lock(&srm_irq_lock);
	cserve_dis(irq - 16);
	spin_unlock(&srm_irq_lock);
}

static unsigned int
srm_startup_irq(unsigned int irq)
{
	srm_enable_irq(irq);
	return 0;
}

static void
srm_end_irq(unsigned int irq)
{
	if (!(irq_desc[irq].status & (IRQ_DISABLED|IRQ_INPROGRESS)))
		srm_enable_irq(irq);
}

/* Handle interrupts from the SRM, assuming no additional weirdness.  */
static struct hw_interrupt_type srm_irq_type = {
	typename:	"SRM",
	startup:	srm_startup_irq,
	shutdown:	srm_disable_irq,
	enable:		srm_enable_irq,
	disable:	srm_disable_irq,
	ack:		srm_disable_irq,
	end:		srm_end_irq,
};

void __init
init_srm_irqs(long max, unsigned long ignore_mask)
{
	long i;

	for (i = 16; i < max; ++i) {
		if (i < 64 && ((ignore_mask >> i) & 1))
			continue;
		irq_desc[i].status = IRQ_DISABLED | IRQ_LEVEL;
		irq_desc[i].handler = &srm_irq_type;
	}
}

void 
srm_device_interrupt(unsigned long vector, struct pt_regs * regs)
{
	int irq = (vector - 0x800) >> 4;
	handle_irq(irq, regs);
}
