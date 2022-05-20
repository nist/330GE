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
#ifndef __ASM_SMP_H
#define __ASM_SMP_H

#include <linux/config.h>
#include <asm/pal.h>

/* HACK: Cabrio WHAMI return value is bogus if more than 8 bits used.. :-( */

static __inline__ unsigned char
__hard_smp_processor_id(void)
{
	register unsigned char __r0 __asm__("$0");
	__asm__ __volatile__(
		"call_pal %1 #whami"
		: "=r"(__r0)
		:"i" (PAL_whami)
		: "$1", "$22", "$23", "$24", "$25");
	return __r0;
}

#ifdef CONFIG_SMP

#include <linux/threads.h>
#include <asm/irq.h>

struct cpuinfo_alpha {
	unsigned long loops_per_jiffy;
	unsigned long last_asn;
	int need_new_asn;
	int asn_lock;
	unsigned long *pgd_cache;
	unsigned long *pmd_cache;
	unsigned long *pte_cache;
	unsigned long pgtable_cache_sz;
	unsigned long ipi_count;
	unsigned long irq_attempt[NR_IRQS];
	unsigned long prof_multiplier;
	unsigned long prof_counter;
	unsigned char mcheck_expected;
	unsigned char mcheck_taken;
	unsigned char mcheck_extra;
} __attribute__((aligned(64)));

extern struct cpuinfo_alpha cpu_data[NR_CPUS];

#define PROC_CHANGE_PENALTY     20

/* Map from cpu id to sequential logical cpu number.  This will only
   not be idempotent when cpus failed to come on-line.  */
extern int __cpu_number_map[NR_CPUS];
#define cpu_number_map(cpu)  __cpu_number_map[cpu]

/* The reverse map from sequential logical cpu number to cpu id.  */
extern int __cpu_logical_map[NR_CPUS];
#define cpu_logical_map(cpu)  __cpu_logical_map[cpu]

#define hard_smp_processor_id()	__hard_smp_processor_id()
#define smp_processor_id()	(current->processor)

extern unsigned long cpu_present_mask;
#define cpu_online_map cpu_present_mask

extern int smp_call_function_on_cpu(void (*func) (void *info), void *info,int retry, int wait, unsigned long cpu);

#else /* CONFIG_SMP */

#define smp_call_function_on_cpu(func,info,retry,wait,cpu)    ({ 0; })

#endif /* CONFIG_SMP */

#define NO_PROC_ID	(-1)

#endif
