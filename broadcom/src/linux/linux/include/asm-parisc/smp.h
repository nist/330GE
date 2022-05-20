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

#if defined(CONFIG_SMP)

/* Page Zero Location PDC will look for the address to branch to when we poke
** slave CPUs still in "Icache loop".
*/
#define PDC_OS_BOOT_RENDEZVOUS     0x10
#define PDC_OS_BOOT_RENDEZVOUS_HI  0x28

#ifndef ASSEMBLY
#include <linux/threads.h>	/* for NR_CPUS */
typedef unsigned long address_t;

extern volatile unsigned long cpu_online_map;


/*
 *	Private routines/data
 *
 *	physical and logical are equivalent until we support CPU hotplug.
 */
#define cpu_number_map(cpu)	(cpu)
#define cpu_logical_map(cpu)	(cpu)

extern void smp_send_reschedule(int cpu);

#endif /* !ASSEMBLY */

/*
 *	This magic constant controls our willingness to transfer
 *      a process across CPUs. Such a transfer incurs cache and tlb
 *      misses. The current value is inherited from i386. Still needs
 *      to be tuned for parisc.
 */
 
#define PROC_CHANGE_PENALTY	15		/* Schedule penalty */

#undef ENTRY_SYS_CPUS
#ifdef ENTRY_SYS_CPUS
#define STATE_RENDEZVOUS			0
#define STATE_STOPPED 				1 
#define STATE_RUNNING				2
#define STATE_HALTED				3
#endif

#define smp_processor_id() (current->processor)

#endif /* CONFIG_SMP */

#define NO_PROC_ID		0xFF		/* No processor magic marker */
#define ANY_PROC_ID		0xFF		/* Any processor magic marker */

#endif /*  __ASM_SMP_H */
