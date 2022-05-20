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
 * Platform dependent support for HP simulator.
 *
 * Copyright (C) 1998-2001 Hewlett-Packard Co
 * Copyright (C) 1998-2001 David Mosberger-Tang <davidm@hpl.hp.com>
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/irq.h>

static unsigned int
hpsim_irq_startup (unsigned int irq)
{
	return 0;
}

static void
hpsim_irq_noop (unsigned int irq)
{
}

static struct hw_interrupt_type irq_type_hp_sim = {
	typename:	"hpsim",
	startup:	hpsim_irq_startup,
	shutdown:	hpsim_irq_noop,
	enable:		hpsim_irq_noop,
	disable:	hpsim_irq_noop,
	ack:		hpsim_irq_noop,
	end:		hpsim_irq_noop,
	set_affinity:	(void (*)(unsigned int, unsigned long)) hpsim_irq_noop,
};

void __init
hpsim_irq_init (void)
{
	irq_desc_t *idesc;
	int i;

	for (i = 0; i < NR_IRQS; ++i) {
		idesc = irq_desc(i);
		if (idesc->handler == &no_irq_type)
			idesc->handler = &irq_type_hp_sim;
	}
}
