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
#ifndef _LINUX_KERNEL_STAT_H
#define _LINUX_KERNEL_STAT_H

#include <linux/config.h>
#include <asm/irq.h>
#include <linux/smp.h>
#include <linux/threads.h>

/*
 * 'kernel_stat.h' contains the definitions needed for doing
 * some kernel statistics (CPU usage, context switches ...),
 * used by rstatd/perfmeter
 */

#define DK_MAX_MAJOR 16
#define DK_MAX_DISK 16

struct kernel_stat {
	unsigned int per_cpu_user[NR_CPUS],
	             per_cpu_nice[NR_CPUS],
	             per_cpu_system[NR_CPUS];
	unsigned int dk_drive[DK_MAX_MAJOR][DK_MAX_DISK];
	unsigned int dk_drive_rio[DK_MAX_MAJOR][DK_MAX_DISK];
	unsigned int dk_drive_wio[DK_MAX_MAJOR][DK_MAX_DISK];
	unsigned int dk_drive_rblk[DK_MAX_MAJOR][DK_MAX_DISK];
	unsigned int dk_drive_wblk[DK_MAX_MAJOR][DK_MAX_DISK];
	unsigned int pgpgin, pgpgout;
	unsigned int pswpin, pswpout;
#if defined(__hppa__) 
	unsigned int irqs[NR_IRQ_REGS][IRQ_PER_REGION];
#elif !defined(CONFIG_ARCH_S390)
	unsigned int irqs[NR_CPUS][NR_IRQS];
#endif
	unsigned int context_swtch;
};

extern struct kernel_stat kstat;

extern unsigned long nr_context_switches(void);

#if defined(__hppa__) 
/*
 * Number of interrupts per specific IRQ source, since bootup
 */
static inline int kstat_irqs (int irq)
{
	return kstat.irqs[IRQ_REGION(irq)][IRQ_OFFSET(irq)];
}
#elif !defined(CONFIG_ARCH_S390)
/*
 * Number of interrupts per specific IRQ source, since bootup
 */
extern inline int kstat_irqs (int irq)
{
	int i, sum=0;

	for (i = 0 ; i < smp_num_cpus ; i++)
		sum += kstat.irqs[cpu_logical_map(i)][irq];

	return sum;
}
#endif

#endif /* _LINUX_KERNEL_STAT_H */
