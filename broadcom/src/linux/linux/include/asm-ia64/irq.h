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
#ifndef _ASM_IA64_IRQ_H
#define _ASM_IA64_IRQ_H

/*
 * Copyright (C) 1999-2000, 2002 Hewlett-Packard Co
 *	David Mosberger-Tang <davidm@hpl.hp.com>
 *	Stephane Eranian <eranian@hpl.hp.com>
 *
 * 11/24/98	S.Eranian 	updated TIMER_IRQ and irq_cannonicalize
 * 01/20/99	S.Eranian	added keyboard interrupt
 * 02/29/00     D.Mosberger	moved most things into hw_irq.h
 */

#define NR_IRQS		256

static __inline__ int
irq_cannonicalize (int irq)
{
	/*
	 * We do the legacy thing here of pretending that irqs < 16
	 * are 8259 irqs.  This really shouldn't be necessary at all,
	 * but we keep it here as serial.c still uses it...
	 */
	return ((irq == 2) ? 9 : irq);
}

extern void disable_irq (unsigned int);
extern void disable_irq_nosync (unsigned int);
extern void enable_irq (unsigned int);
extern void set_irq_affinity_info (int irq, int dest, int redir);

#endif /* _ASM_IA64_IRQ_H */
