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
#ifndef __ASM_SOFTIRQ_H
#define __ASM_SOFTIRQ_H

#include <asm/atomic.h>
#include <asm/hardirq.h>

#define __cpu_bh_enable(cpu) \
		do { barrier(); local_bh_count(cpu)--; } while (0)
#define cpu_bh_disable(cpu) \
		do { local_bh_count(cpu)++; barrier(); } while (0)

#define local_bh_disable()	cpu_bh_disable(smp_processor_id())
#define __local_bh_enable()	__cpu_bh_enable(smp_processor_id())

#define in_softirq() (local_bh_count(smp_processor_id()) != 0)

/*
 * NOTE: this assembly code assumes:
 *
 *    (char *)&local_bh_count - 8 == (char *)&softirq_pending
 *
 * If you change the offsets in irq_stat then you have to
 * update this code as well.
 */
#define local_bh_enable()						\
do {									\
	unsigned int *ptr = &local_bh_count(smp_processor_id());	\
									\
	barrier();							\
	if (!--*ptr)							\
		__asm__ __volatile__ (					\
			"cmpl $0, -8(%0);"				\
			"jnz 2f;"					\
			"1:;"						\
									\
			LOCK_SECTION_START("")				\
			"2: pushl %%eax; pushl %%ecx; pushl %%edx;"	\
			"call %c1;"					\
			"popl %%edx; popl %%ecx; popl %%eax;"		\
			"jmp 1b;"					\
			LOCK_SECTION_END				\
									\
		: /* no output */					\
		: "r" (ptr), "i" (do_softirq)				\
		/* no registers clobbered */ );				\
} while (0)

#endif	/* __ASM_SOFTIRQ_H */
