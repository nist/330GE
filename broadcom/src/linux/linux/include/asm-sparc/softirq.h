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
/* softirq.h: 32-bit Sparc soft IRQ support.
 *
 * Copyright (C) 1997 David S. Miller (davem@caip.rutgers.edu)
 * Copyright (C) 1998-99 Anton Blanchard (anton@progsoc.uts.edu.au)
 */

#ifndef __SPARC_SOFTIRQ_H
#define __SPARC_SOFTIRQ_H

#include <linux/threads.h>	/* For NR_CPUS */

#include <asm/atomic.h>
#include <asm/smp.h>
#include <asm/hardirq.h>

#define local_bh_disable()	(local_bh_count(smp_processor_id())++)
#define __local_bh_enable()	(local_bh_count(smp_processor_id())--)
#define local_bh_enable()			  \
do { if (!--local_bh_count(smp_processor_id()) && \
	 softirq_pending(smp_processor_id())) {   \
		do_softirq();			  \
		__sti();			  \
     }						  \
} while (0)
#define in_softirq() (local_bh_count(smp_processor_id()) != 0)

#endif	/* __SPARC_SOFTIRQ_H */
