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
#ifndef __ASM_APIC_H
#define __ASM_APIC_H

#include <linux/config.h>
#include <linux/pm.h>
#include <asm/apicdef.h>
#include <asm/system.h>

#ifdef CONFIG_X86_LOCAL_APIC

#ifndef CONFIG_MCOUNT
#define APIC_DEBUG 0
#endif

#if APIC_DEBUG
#define Dprintk(x...) printk(x)
#else
#define Dprintk(x...)
#endif

/*
 * Basic functions accessing APICs.
 */

static __inline void apic_write(unsigned long reg, unsigned long v)
{
	*((volatile unsigned long *)(APIC_BASE+reg)) = v;
}

static __inline void apic_write_atomic(unsigned long reg, unsigned long v)
{
	xchg((volatile unsigned long *)(APIC_BASE+reg), v);
}

static __inline unsigned long apic_read(unsigned long reg)
{
	return *((volatile unsigned long *)(APIC_BASE+reg));
}

static __inline__ void apic_wait_icr_idle(void)
{
	do { } while ( apic_read( APIC_ICR ) & APIC_ICR_BUSY );
}

#ifdef CONFIG_X86_GOOD_APIC
# define FORCE_READ_AROUND_WRITE 0
# define apic_read_around(x)
# define apic_write_around(x,y) apic_write((x),(y))
#else
# define FORCE_READ_AROUND_WRITE 1
# define apic_read_around(x) apic_read(x)
# define apic_write_around(x,y) apic_write_atomic((x),(y))
#endif

static inline void ack_APIC_irq(void)
{
	/*
	 * ack_APIC_irq() actually gets compiled as a single instruction:
	 * - a single rmw on Pentium/82489DX
	 * - a single write on P6+ cores (CONFIG_X86_GOOD_APIC)
	 * ... yummie.
	 */

	/* Docs say use 0 for future compatibility */
	apic_write_around(APIC_EOI, 0);
}

extern int get_maxlvt(void);
extern void clear_local_APIC(void);
extern void connect_bsp_APIC (void);
extern void disconnect_bsp_APIC (void);
extern void disable_local_APIC (void);
extern int verify_local_APIC (void);
extern void cache_APIC_registers (void);
extern void sync_Arb_IDs (void);
extern void init_bsp_APIC (void);
extern void setup_local_APIC (void);
extern void init_apic_mappings (void);
extern void smp_local_timer_interrupt (struct pt_regs * regs);
extern void setup_APIC_clocks (void);
extern void setup_APIC_perfctr(void);
extern int setup_profiling_timer(unsigned int);
extern int prof_multiplier[];
extern void setup_apic_nmi_watchdog (void);
extern inline void nmi_watchdog_tick (struct pt_regs * regs);
extern int APIC_init_uniprocessor (void);
extern void disable_APIC_timer(void);
extern void enable_APIC_timer(void);

extern struct pm_dev *apic_pm_register(pm_dev_t, unsigned long, pm_callback);
extern void apic_pm_unregister(struct pm_dev*);

extern unsigned int apic_timer_irqs [NR_CPUS];
extern int check_nmi_watchdog (void);

extern unsigned int nmi_watchdog;
#define NMI_NONE	0
#define NMI_IO_APIC	1
#define NMI_LOCAL_APIC	2
#define NMI_INVALID	3

#endif /* CONFIG_X86_LOCAL_APIC */

#endif /* __ASM_APIC_H */
