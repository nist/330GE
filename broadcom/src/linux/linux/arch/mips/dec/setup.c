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
 * Setup the interrupt stuff.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1998 Harald Koerfgen
 * Copyright (C) 2000, 2001, 2002  Maciej W. Rozycki
 */
#include <linux/config.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/mc146818rtc.h>
#include <linux/param.h>
#include <linux/console.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/types.h>

#include <asm/cpu.h>
#include <asm/bootinfo.h>
#include <asm/irq.h>
#include <asm/irq_cpu.h>
#include <asm/mipsregs.h>
#include <asm/reboot.h>
#include <asm/traps.h>
#include <asm/wbflush.h>

#include <asm/dec/interrupts.h>
#include <asm/dec/ioasic.h>
#include <asm/dec/ioasic_addrs.h>
#include <asm/dec/ioasic_ints.h>
#include <asm/dec/kn01.h>
#include <asm/dec/kn02.h>
#include <asm/dec/kn02ba.h>
#include <asm/dec/kn02ca.h>
#include <asm/dec/kn03.h>
#include <asm/dec/kn230.h>


extern void dec_machine_restart(char *command);
extern void dec_machine_halt(void);
extern void dec_machine_power_off(void);
extern void dec_intr_halt(int irq, void *dev_id, struct pt_regs *regs);

extern asmlinkage void decstation_handle_int(void);

spinlock_t ioasic_ssr_lock;

volatile u32 *ioasic_base;
unsigned long dec_kn_slot_size;

/*
 * IRQ routing and priority tables.  Priorites are set as follows:
 *
 * 		KN01	KN230	KN02	KN02-BA	KN02-CA	KN03
 *
 * MEMORY	CPU	CPU	CPU	ASIC	CPU	CPU
 * RTC		CPU	CPU	CPU	ASIC	CPU	CPU
 * DMA		-	-	-	ASIC	ASIC	ASIC
 * SERIAL0	CPU	CPU	CSR	ASIC	ASIC	ASIC
 * SERIAL1	-	-	-	ASIC	-	ASIC
 * SCSI		CPU	CPU	CSR	ASIC	ASIC	ASIC
 * ETHERNET	CPU	*	CSR	ASIC	ASIC	ASIC
 * other	-	-	-	ASIC	-	-
 * TC2		-	-	CSR	CPU	ASIC	ASIC
 * TC1		-	-	CSR	CPU	ASIC	ASIC
 * TC0		-	-	CSR	CPU	ASIC	ASIC
 * other	-	CPU	-	CPU	ASIC	ASIC
 * other	-	-	-	-	CPU	CPU
 *
 * * -- shared with SCSI
 */

int dec_interrupt[DEC_NR_INTS] = {
	[0 ... DEC_NR_INTS - 1] = -1
};
int_ptr cpu_mask_nr_tbl[DEC_MAX_CPU_INTS][2] = {
	{ { .i = ~0 }, { .p = dec_intr_unimplemented } },
};
int_ptr asic_mask_nr_tbl[DEC_MAX_ASIC_INTS][2] = {
	{ { .i = ~0 }, { .p = asic_intr_unimplemented } },
};
int cpu_fpu_mask = DEC_CPU_IRQ_MASK(DEC_CPU_INR_FPU);

static struct irqaction ioirq = {NULL, 0, 0, "cascade", NULL, NULL};
static struct irqaction fpuirq = {NULL, 0, 0, "fpu", NULL, NULL};

static struct irqaction haltirq = {dec_intr_halt, 0, 0, "halt", NULL, NULL};


void (*board_time_init) (struct irqaction * irq);


/*
 * enable the periodic interrupts
 */
static void __init dec_time_init(struct irqaction *irq)
{
	/*
	* Here we go, enable periodic rtc interrupts.
	*/

#ifndef LOG_2_HZ
#  define LOG_2_HZ 7
#endif

	CMOS_WRITE(RTC_REF_CLCK_32KHZ | (16 - LOG_2_HZ), RTC_REG_A);
	CMOS_WRITE(CMOS_READ(RTC_REG_B) | RTC_PIE, RTC_REG_B);
	setup_irq(dec_interrupt[DEC_IRQ_RTC], irq);
}


/*
 * Bus error (DBE/IBE exceptions and memory interrupts) handling
 * setup.  Nothing for now.
 */
void __init bus_error_init(void)
{
}


void __init decstation_setup(void)
{
	board_time_init = dec_time_init;

	wbflush_setup();

	_machine_restart = dec_machine_restart;
	_machine_halt = dec_machine_halt;
	_machine_power_off = dec_machine_power_off;

#ifdef CONFIG_FB
	conswitchp = &dummy_con;
#endif

	rtc_ops = &dec_rtc_ops;
}


/*
 * Machine-specific initialisation for KN01, aka DS2100 (aka Pmin)
 * or DS3100 (aka Pmax).
 */
static int kn01_interrupt[DEC_NR_INTS] __initdata = {
	[DEC_IRQ_CASCADE]	= -1,
	[DEC_IRQ_AB_RECV]	= -1,
	[DEC_IRQ_AB_XMIT]	= -1,
	[DEC_IRQ_DZ11]		= DEC_CPU_IRQ_NR(KN01_CPU_INR_DZ11),
	[DEC_IRQ_ASC]		= -1,
	[DEC_IRQ_FLOPPY]	= -1,
	[DEC_IRQ_FPU]		= DEC_CPU_IRQ_NR(DEC_CPU_INR_FPU),
	[DEC_IRQ_HALT]		= -1,
	[DEC_IRQ_ISDN]		= -1,
	[DEC_IRQ_LANCE]		= DEC_CPU_IRQ_NR(KN01_CPU_INR_LANCE),
	[DEC_IRQ_MEMORY]	= DEC_CPU_IRQ_NR(KN01_CPU_INR_MEMORY),
	[DEC_IRQ_PSU]		= -1,
	[DEC_IRQ_RTC]		= DEC_CPU_IRQ_NR(KN01_CPU_INR_RTC),
	[DEC_IRQ_SCC0]		= -1,
	[DEC_IRQ_SCC1]		= -1,
	[DEC_IRQ_SII]		= DEC_CPU_IRQ_NR(KN01_CPU_INR_SII),
	[DEC_IRQ_TC0]		= -1,
	[DEC_IRQ_TC1]		= -1,
	[DEC_IRQ_TC2]		= -1,
	[DEC_IRQ_TIMER]		= -1,
	[DEC_IRQ_VIDEO]		= DEC_CPU_IRQ_NR(KN01_CPU_INR_VIDEO),
	[DEC_IRQ_ASC_MERR]	= -1,
	[DEC_IRQ_ASC_ERR]	= -1,
	[DEC_IRQ_ASC_DMA]	= -1,
	[DEC_IRQ_FLOPPY_ERR]	= -1,
	[DEC_IRQ_ISDN_ERR]	= -1,
	[DEC_IRQ_ISDN_RXDMA]	= -1,
	[DEC_IRQ_ISDN_TXDMA]	= -1,
	[DEC_IRQ_LANCE_MERR]	= -1,
	[DEC_IRQ_SCC0A_RXERR]	= -1,
	[DEC_IRQ_SCC0A_RXDMA]	= -1,
	[DEC_IRQ_SCC0A_TXERR]	= -1,
	[DEC_IRQ_SCC0A_TXDMA]	= -1,
	[DEC_IRQ_AB_RXERR]	= -1,
	[DEC_IRQ_AB_RXDMA]	= -1,
	[DEC_IRQ_AB_TXERR]	= -1,
	[DEC_IRQ_AB_TXDMA]	= -1,
	[DEC_IRQ_SCC1A_RXERR]	= -1,
	[DEC_IRQ_SCC1A_RXDMA]	= -1,
	[DEC_IRQ_SCC1A_TXERR]	= -1,
	[DEC_IRQ_SCC1A_TXDMA]	= -1,
};

static int_ptr kn01_cpu_mask_nr_tbl[][2] __initdata = {
	{ { .i = DEC_CPU_IRQ_MASK(KN01_CPU_INR_MEMORY) },
		{ .i = DEC_CPU_IRQ_NR(KN01_CPU_INR_MEMORY) } },
	{ { .i = DEC_CPU_IRQ_MASK(KN01_CPU_INR_RTC) },
		{ .i = DEC_CPU_IRQ_NR(KN01_CPU_INR_RTC) } },
	{ { .i = DEC_CPU_IRQ_MASK(KN01_CPU_INR_DZ11) },
		{ .i = DEC_CPU_IRQ_NR(KN01_CPU_INR_DZ11) } },
	{ { .i = DEC_CPU_IRQ_MASK(KN01_CPU_INR_SII) },
		{ .i = DEC_CPU_IRQ_NR(KN01_CPU_INR_SII) } },
	{ { .i = DEC_CPU_IRQ_MASK(KN01_CPU_INR_LANCE) },
		{ .i = DEC_CPU_IRQ_NR(KN01_CPU_INR_LANCE) } },
	{ { .i = DEC_CPU_IRQ_ALL },
		{ .p = cpu_all_int } },
};

void __init dec_init_kn01(void)
{
	/* IRQ routing. */
	memcpy(&dec_interrupt, &kn01_interrupt,
		sizeof(kn01_interrupt));

	/* CPU IRQ priorities. */
	memcpy(&cpu_mask_nr_tbl, &kn01_cpu_mask_nr_tbl,
		sizeof(kn01_cpu_mask_nr_tbl));

	mips_cpu_irq_init(DEC_CPU_IRQ_BASE);

}				/* dec_init_kn01 */


/*
 * Machine-specific initialisation for KN230, aka DS5100, aka MIPSmate.
 */
static int kn230_interrupt[DEC_NR_INTS] __initdata = {
	[DEC_IRQ_CASCADE]	= -1,
	[DEC_IRQ_AB_RECV]	= -1,
	[DEC_IRQ_AB_XMIT]	= -1,
	[DEC_IRQ_DZ11]		= DEC_CPU_IRQ_NR(KN230_CPU_INR_DZ11),
	[DEC_IRQ_ASC]		= -1,
	[DEC_IRQ_FLOPPY]	= -1,
	[DEC_IRQ_FPU]		= DEC_CPU_IRQ_NR(DEC_CPU_INR_FPU),
	[DEC_IRQ_HALT]		= DEC_CPU_IRQ_NR(KN230_CPU_INR_HALT),
	[DEC_IRQ_ISDN]		= -1,
	[DEC_IRQ_LANCE]		= DEC_CPU_IRQ_NR(KN230_CPU_INR_LANCE),
	[DEC_IRQ_MEMORY]	= DEC_CPU_IRQ_NR(KN230_CPU_INR_MEMORY),
	[DEC_IRQ_PSU]		= -1,
	[DEC_IRQ_RTC]		= DEC_CPU_IRQ_NR(KN230_CPU_INR_RTC),
	[DEC_IRQ_SCC0]		= -1,
	[DEC_IRQ_SCC1]		= -1,
	[DEC_IRQ_SII]		= DEC_CPU_IRQ_NR(KN230_CPU_INR_SII),
	[DEC_IRQ_TC0]		= -1,
	[DEC_IRQ_TC1]		= -1,
	[DEC_IRQ_TC2]		= -1,
	[DEC_IRQ_TIMER]		= -1,
	[DEC_IRQ_VIDEO]		= -1,
	[DEC_IRQ_ASC_MERR]	= -1,
	[DEC_IRQ_ASC_ERR]	= -1,
	[DEC_IRQ_ASC_DMA]	= -1,
	[DEC_IRQ_FLOPPY_ERR]	= -1,
	[DEC_IRQ_ISDN_ERR]	= -1,
	[DEC_IRQ_ISDN_RXDMA]	= -1,
	[DEC_IRQ_ISDN_TXDMA]	= -1,
	[DEC_IRQ_LANCE_MERR]	= -1,
	[DEC_IRQ_SCC0A_RXERR]	= -1,
	[DEC_IRQ_SCC0A_RXDMA]	= -1,
	[DEC_IRQ_SCC0A_TXERR]	= -1,
	[DEC_IRQ_SCC0A_TXDMA]	= -1,
	[DEC_IRQ_AB_RXERR]	= -1,
	[DEC_IRQ_AB_RXDMA]	= -1,
	[DEC_IRQ_AB_TXERR]	= -1,
	[DEC_IRQ_AB_TXDMA]	= -1,
	[DEC_IRQ_SCC1A_RXERR]	= -1,
	[DEC_IRQ_SCC1A_RXDMA]	= -1,
	[DEC_IRQ_SCC1A_TXERR]	= -1,
	[DEC_IRQ_SCC1A_TXDMA]	= -1,
};

static int_ptr kn230_cpu_mask_nr_tbl[][2] __initdata = {
	{ { .i = DEC_CPU_IRQ_MASK(KN230_CPU_INR_MEMORY) },
		{ .i = DEC_CPU_IRQ_NR(KN230_CPU_INR_MEMORY) } },
	{ { .i = DEC_CPU_IRQ_MASK(KN230_CPU_INR_RTC) },
		{ .i = DEC_CPU_IRQ_NR(KN230_CPU_INR_RTC) } },
	{ { .i = DEC_CPU_IRQ_MASK(KN230_CPU_INR_DZ11) },
		{ .i = DEC_CPU_IRQ_NR(KN230_CPU_INR_DZ11) } },
	{ { .i = DEC_CPU_IRQ_MASK(KN230_CPU_INR_SII) },
		{ .i = DEC_CPU_IRQ_NR(KN230_CPU_INR_SII) } },
	{ { .i = DEC_CPU_IRQ_ALL },
		{ .p = cpu_all_int } },
};

void __init dec_init_kn230(void)
{
	/* IRQ routing. */
	memcpy(&dec_interrupt, &kn230_interrupt,
		sizeof(kn230_interrupt));

	/* CPU IRQ priorities. */
	memcpy(&cpu_mask_nr_tbl, &kn230_cpu_mask_nr_tbl,
		sizeof(kn230_cpu_mask_nr_tbl));

	mips_cpu_irq_init(DEC_CPU_IRQ_BASE);

}				/* dec_init_kn230 */


/*
 * Machine-specific initialisation for KN02, aka DS5000/200, aka 3max.
 */
static int kn02_interrupt[DEC_NR_INTS] __initdata = {
	[DEC_IRQ_CASCADE]	= DEC_CPU_IRQ_NR(KN02_CPU_INR_CASCADE),
	[DEC_IRQ_AB_RECV]	= -1,
	[DEC_IRQ_AB_XMIT]	= -1,
	[DEC_IRQ_DZ11]		= KN02_IRQ_NR(KN02_CSR_INR_DZ11),
	[DEC_IRQ_ASC]		= KN02_IRQ_NR(KN02_CSR_INR_ASC),
	[DEC_IRQ_FLOPPY]	= -1,
	[DEC_IRQ_FPU]		= DEC_CPU_IRQ_NR(DEC_CPU_INR_FPU),
	[DEC_IRQ_HALT]		= -1,
	[DEC_IRQ_ISDN]		= -1,
	[DEC_IRQ_LANCE]		= KN02_IRQ_NR(KN02_CSR_INR_LANCE),
	[DEC_IRQ_MEMORY]	= DEC_CPU_IRQ_NR(KN02_CPU_INR_MEMORY),
	[DEC_IRQ_PSU]		= -1,
	[DEC_IRQ_RTC]		= DEC_CPU_IRQ_NR(KN02_CPU_INR_RTC),
	[DEC_IRQ_SCC0]		= -1,
	[DEC_IRQ_SCC1]		= -1,
	[DEC_IRQ_SII]		= -1,
	[DEC_IRQ_TC0]		= KN02_IRQ_NR(KN02_CSR_INR_TC0),
	[DEC_IRQ_TC1]		= KN02_IRQ_NR(KN02_CSR_INR_TC1),
	[DEC_IRQ_TC2]		= KN02_IRQ_NR(KN02_CSR_INR_TC2),
	[DEC_IRQ_TIMER]		= -1,
	[DEC_IRQ_VIDEO]		= -1,
	[DEC_IRQ_ASC_MERR]	= -1,
	[DEC_IRQ_ASC_ERR]	= -1,
	[DEC_IRQ_ASC_DMA]	= -1,
	[DEC_IRQ_FLOPPY_ERR]	= -1,
	[DEC_IRQ_ISDN_ERR]	= -1,
	[DEC_IRQ_ISDN_RXDMA]	= -1,
	[DEC_IRQ_ISDN_TXDMA]	= -1,
	[DEC_IRQ_LANCE_MERR]	= -1,
	[DEC_IRQ_SCC0A_RXERR]	= -1,
	[DEC_IRQ_SCC0A_RXDMA]	= -1,
	[DEC_IRQ_SCC0A_TXERR]	= -1,
	[DEC_IRQ_SCC0A_TXDMA]	= -1,
	[DEC_IRQ_AB_RXERR]	= -1,
	[DEC_IRQ_AB_RXDMA]	= -1,
	[DEC_IRQ_AB_TXERR]	= -1,
	[DEC_IRQ_AB_TXDMA]	= -1,
	[DEC_IRQ_SCC1A_RXERR]	= -1,
	[DEC_IRQ_SCC1A_RXDMA]	= -1,
	[DEC_IRQ_SCC1A_TXERR]	= -1,
	[DEC_IRQ_SCC1A_TXDMA]	= -1,
};

static int_ptr kn02_cpu_mask_nr_tbl[][2] __initdata = {
	{ { .i = DEC_CPU_IRQ_MASK(KN02_CPU_INR_MEMORY) },
		{ .i = DEC_CPU_IRQ_NR(KN02_CPU_INR_MEMORY) } },
	{ { .i = DEC_CPU_IRQ_MASK(KN02_CPU_INR_RTC) },
		{ .i = DEC_CPU_IRQ_NR(KN02_CPU_INR_RTC) } },
	{ { .i = DEC_CPU_IRQ_MASK(KN02_CPU_INR_CASCADE) },
		{ .p = kn02_io_int } },
	{ { .i = DEC_CPU_IRQ_ALL },
		{ .p = cpu_all_int } },
};

static int_ptr kn02_asic_mask_nr_tbl[][2] __initdata = {
	{ { .i = KN02_IRQ_MASK(KN02_CSR_INR_DZ11) },
		{ .i = KN02_IRQ_NR(KN02_CSR_INR_DZ11) } },
	{ { .i = KN02_IRQ_MASK(KN02_CSR_INR_ASC) },
		{ .i = KN02_IRQ_NR(KN02_CSR_INR_ASC) } },
	{ { .i = KN02_IRQ_MASK(KN02_CSR_INR_LANCE) },
		{ .i = KN02_IRQ_NR(KN02_CSR_INR_LANCE) } },
	{ { .i = KN02_IRQ_MASK(KN02_CSR_INR_TC2) },
		{ .i = KN02_IRQ_NR(KN02_CSR_INR_TC2) } },
	{ { .i = KN02_IRQ_MASK(KN02_CSR_INR_TC1) },
		{ .i = KN02_IRQ_NR(KN02_CSR_INR_TC1) } },
	{ { .i = KN02_IRQ_MASK(KN02_CSR_INR_TC0) },
		{ .i = KN02_IRQ_NR(KN02_CSR_INR_TC0) } },
	{ { .i = KN02_IRQ_ALL },
		{ .p = kn02_all_int } },
};

void __init dec_init_kn02(void)
{
	/* IRQ routing. */
	memcpy(&dec_interrupt, &kn02_interrupt,
		sizeof(kn02_interrupt));

	/* CPU IRQ priorities. */
	memcpy(&cpu_mask_nr_tbl, &kn02_cpu_mask_nr_tbl,
		sizeof(kn02_cpu_mask_nr_tbl));

	/* KN02 CSR IRQ priorities. */
	memcpy(&asic_mask_nr_tbl, &kn02_asic_mask_nr_tbl,
		sizeof(kn02_asic_mask_nr_tbl));

	mips_cpu_irq_init(DEC_CPU_IRQ_BASE);
	init_kn02_irqs(KN02_IRQ_BASE);

}				/* dec_init_kn02 */


/*
 * Machine-specific initialisation for KN02-BA, aka DS5000/1xx
 * (xx = 20, 25, 33), aka 3min.  Also applies to KN04(-BA), aka
 * DS5000/150, aka 4min.
 */
static int kn02ba_interrupt[DEC_NR_INTS] __initdata = {
	[DEC_IRQ_CASCADE]	= DEC_CPU_IRQ_NR(KN02BA_CPU_INR_CASCADE),
	[DEC_IRQ_AB_RECV]	= -1,
	[DEC_IRQ_AB_XMIT]	= -1,
	[DEC_IRQ_DZ11]		= -1,
	[DEC_IRQ_ASC]		= IO_IRQ_NR(KN02BA_IO_INR_ASC),
	[DEC_IRQ_FLOPPY]	= -1,
	[DEC_IRQ_FPU]		= DEC_CPU_IRQ_NR(DEC_CPU_INR_FPU),
	[DEC_IRQ_HALT]		= DEC_CPU_IRQ_NR(KN02BA_CPU_INR_HALT),
	[DEC_IRQ_ISDN]		= -1,
	[DEC_IRQ_LANCE]		= IO_IRQ_NR(KN02BA_IO_INR_LANCE),
	[DEC_IRQ_MEMORY]	= IO_IRQ_NR(KN02BA_IO_INR_MEMORY),
	[DEC_IRQ_PSU]		= IO_IRQ_NR(KN02BA_IO_INR_PSU),
	[DEC_IRQ_RTC]		= IO_IRQ_NR(KN02BA_IO_INR_RTC),
	[DEC_IRQ_SCC0]		= IO_IRQ_NR(KN02BA_IO_INR_SCC0),
	[DEC_IRQ_SCC1]		= IO_IRQ_NR(KN02BA_IO_INR_SCC1),
	[DEC_IRQ_SII]		= -1,
	[DEC_IRQ_TC0]		= DEC_CPU_IRQ_NR(KN02BA_CPU_INR_TC0),
	[DEC_IRQ_TC1]		= DEC_CPU_IRQ_NR(KN02BA_CPU_INR_TC1),
	[DEC_IRQ_TC2]		= DEC_CPU_IRQ_NR(KN02BA_CPU_INR_TC2),
	[DEC_IRQ_TIMER]		= -1,
	[DEC_IRQ_VIDEO]		= -1,
	[DEC_IRQ_ASC_MERR]	= IO_IRQ_NR(IO_INR_ASC_MERR),
	[DEC_IRQ_ASC_ERR]	= IO_IRQ_NR(IO_INR_ASC_ERR),
	[DEC_IRQ_ASC_DMA]	= IO_IRQ_NR(IO_INR_ASC_DMA),
	[DEC_IRQ_FLOPPY_ERR]	= -1,
	[DEC_IRQ_ISDN_ERR]	= -1,
	[DEC_IRQ_ISDN_RXDMA]	= -1,
	[DEC_IRQ_ISDN_TXDMA]	= -1,
	[DEC_IRQ_LANCE_MERR]	= IO_IRQ_NR(IO_INR_LANCE_MERR),
	[DEC_IRQ_SCC0A_RXERR]	= IO_IRQ_NR(IO_INR_SCC0A_RXERR),
	[DEC_IRQ_SCC0A_RXDMA]	= IO_IRQ_NR(IO_INR_SCC0A_RXDMA),
	[DEC_IRQ_SCC0A_TXERR]	= IO_IRQ_NR(IO_INR_SCC0A_TXERR),
	[DEC_IRQ_SCC0A_TXDMA]	= IO_IRQ_NR(IO_INR_SCC0A_TXDMA),
	[DEC_IRQ_AB_RXERR]	= -1,
	[DEC_IRQ_AB_RXDMA]	= -1,
	[DEC_IRQ_AB_TXERR]	= -1,
	[DEC_IRQ_AB_TXDMA]	= -1,
	[DEC_IRQ_SCC1A_RXERR]	= IO_IRQ_NR(IO_INR_SCC1A_RXERR),
	[DEC_IRQ_SCC1A_RXDMA]	= IO_IRQ_NR(IO_INR_SCC1A_RXDMA),
	[DEC_IRQ_SCC1A_TXERR]	= IO_IRQ_NR(IO_INR_SCC1A_TXERR),
	[DEC_IRQ_SCC1A_TXDMA]	= IO_IRQ_NR(IO_INR_SCC1A_TXDMA),
};

static int_ptr kn02ba_cpu_mask_nr_tbl[][2] __initdata = {
	{ { .i = DEC_CPU_IRQ_MASK(KN02BA_CPU_INR_CASCADE) },
		{ .p = kn02xa_io_int } },
	{ { .i = DEC_CPU_IRQ_MASK(KN02BA_CPU_INR_TC2) },
		{ .i = DEC_CPU_IRQ_NR(KN02BA_CPU_INR_TC2) } },
	{ { .i = DEC_CPU_IRQ_MASK(KN02BA_CPU_INR_TC1) },
		{ .i = DEC_CPU_IRQ_NR(KN02BA_CPU_INR_TC1) } },
	{ { .i = DEC_CPU_IRQ_MASK(KN02BA_CPU_INR_TC0) },
		{ .i = DEC_CPU_IRQ_NR(KN02BA_CPU_INR_TC0) } },
	{ { .i = DEC_CPU_IRQ_ALL },
		{ .p = cpu_all_int } },
};

static int_ptr kn02ba_asic_mask_nr_tbl[][2] __initdata = {
	{ { .i = IO_IRQ_MASK(KN02BA_IO_INR_MEMORY) },
		{ .i = IO_IRQ_NR(KN02BA_IO_INR_MEMORY) } },
	{ { .i = IO_IRQ_MASK(KN02BA_IO_INR_RTC) },
		{ .i = IO_IRQ_NR(KN02BA_IO_INR_RTC) } },
	{ { .i = IO_IRQ_DMA },
		{ .p = asic_dma_int } },
	{ { .i = IO_IRQ_MASK(KN02BA_IO_INR_SCC0) },
		{ .i = IO_IRQ_NR(KN02BA_IO_INR_SCC0) } },
	{ { .i = IO_IRQ_MASK(KN02BA_IO_INR_SCC1) },
		{ .i = IO_IRQ_NR(KN02BA_IO_INR_SCC1) } },
	{ { .i = IO_IRQ_MASK(KN02BA_IO_INR_ASC) },
		{ .i = IO_IRQ_NR(KN02BA_IO_INR_ASC) } },
	{ { .i = IO_IRQ_MASK(KN02BA_IO_INR_LANCE) },
		{ .i = IO_IRQ_NR(KN02BA_IO_INR_LANCE) } },
	{ { .i = IO_IRQ_ALL },
		{ .p = asic_all_int } },
};

void __init dec_init_kn02ba(void)
{
	/* IRQ routing. */
	memcpy(&dec_interrupt, &kn02ba_interrupt,
		sizeof(kn02ba_interrupt));

	/* CPU IRQ priorities. */
	memcpy(&cpu_mask_nr_tbl, &kn02ba_cpu_mask_nr_tbl,
		sizeof(kn02ba_cpu_mask_nr_tbl));

	/* I/O ASIC IRQ priorities. */
	memcpy(&asic_mask_nr_tbl, &kn02ba_asic_mask_nr_tbl,
		sizeof(kn02ba_asic_mask_nr_tbl));

	mips_cpu_irq_init(DEC_CPU_IRQ_BASE);
	init_ioasic_irqs(IO_IRQ_BASE);

}				/* dec_init_kn02ba */


/*
 * Machine-specific initialisation for KN02-CA, aka DS5000/xx,
 * (xx = 20, 25, 33), aka MAXine.  Also applies to KN04(-CA), aka
 * DS5000/50, aka 4MAXine.
 */
static int kn02ca_interrupt[DEC_NR_INTS] __initdata = {
	[DEC_IRQ_CASCADE]	= DEC_CPU_IRQ_NR(KN02CA_CPU_INR_CASCADE),
	[DEC_IRQ_AB_RECV]	= IO_IRQ_NR(KN02CA_IO_INR_AB_RECV),
	[DEC_IRQ_AB_XMIT]	= IO_IRQ_NR(KN02CA_IO_INR_AB_XMIT),
	[DEC_IRQ_DZ11]		= -1,
	[DEC_IRQ_ASC]		= IO_IRQ_NR(KN02CA_IO_INR_ASC),
	[DEC_IRQ_FLOPPY]	= IO_IRQ_NR(KN02CA_IO_INR_FLOPPY),
	[DEC_IRQ_FPU]		= DEC_CPU_IRQ_NR(DEC_CPU_INR_FPU),
	[DEC_IRQ_HALT]		= DEC_CPU_IRQ_NR(KN02CA_CPU_INR_HALT),
	[DEC_IRQ_ISDN]		= IO_IRQ_NR(KN02CA_IO_INR_ISDN),
	[DEC_IRQ_LANCE]		= IO_IRQ_NR(KN02CA_IO_INR_LANCE),
	[DEC_IRQ_MEMORY]	= DEC_CPU_IRQ_NR(KN02CA_CPU_INR_MEMORY),
	[DEC_IRQ_PSU]		= -1,
	[DEC_IRQ_RTC]		= DEC_CPU_IRQ_NR(KN02CA_CPU_INR_RTC),
	[DEC_IRQ_SCC0]		= IO_IRQ_NR(KN02CA_IO_INR_SCC0),
	[DEC_IRQ_SCC1]		= -1,
	[DEC_IRQ_SII]		= -1,
	[DEC_IRQ_TC0]		= IO_IRQ_NR(KN02CA_IO_INR_TC0),
	[DEC_IRQ_TC1]		= IO_IRQ_NR(KN02CA_IO_INR_TC1),
	[DEC_IRQ_TC2]		= -1,
	[DEC_IRQ_TIMER]		= DEC_CPU_IRQ_NR(KN02CA_CPU_INR_TIMER),
	[DEC_IRQ_VIDEO]		= IO_IRQ_NR(KN02CA_IO_INR_VIDEO),
	[DEC_IRQ_ASC_MERR]	= IO_IRQ_NR(IO_INR_ASC_MERR),
	[DEC_IRQ_ASC_ERR]	= IO_IRQ_NR(IO_INR_ASC_ERR),
	[DEC_IRQ_ASC_DMA]	= IO_IRQ_NR(IO_INR_ASC_DMA),
	[DEC_IRQ_FLOPPY_ERR]	= IO_IRQ_NR(IO_INR_FLOPPY_ERR),
	[DEC_IRQ_ISDN_ERR]	= IO_IRQ_NR(IO_INR_ISDN_ERR),
	[DEC_IRQ_ISDN_RXDMA]	= IO_IRQ_NR(IO_INR_ISDN_RXDMA),
	[DEC_IRQ_ISDN_TXDMA]	= IO_IRQ_NR(IO_INR_ISDN_TXDMA),
	[DEC_IRQ_LANCE_MERR]	= IO_IRQ_NR(IO_INR_LANCE_MERR),
	[DEC_IRQ_SCC0A_RXERR]	= IO_IRQ_NR(IO_INR_SCC0A_RXERR),
	[DEC_IRQ_SCC0A_RXDMA]	= IO_IRQ_NR(IO_INR_SCC0A_RXDMA),
	[DEC_IRQ_SCC0A_TXERR]	= IO_IRQ_NR(IO_INR_SCC0A_TXERR),
	[DEC_IRQ_SCC0A_TXDMA]	= IO_IRQ_NR(IO_INR_SCC0A_TXDMA),
	[DEC_IRQ_AB_RXERR]	= IO_IRQ_NR(IO_INR_AB_RXERR),
	[DEC_IRQ_AB_RXDMA]	= IO_IRQ_NR(IO_INR_AB_RXDMA),
	[DEC_IRQ_AB_TXERR]	= IO_IRQ_NR(IO_INR_AB_TXERR),
	[DEC_IRQ_AB_TXDMA]	= IO_IRQ_NR(IO_INR_AB_TXDMA),
	[DEC_IRQ_SCC1A_RXERR]	= -1,
	[DEC_IRQ_SCC1A_RXDMA]	= -1,
	[DEC_IRQ_SCC1A_TXERR]	= -1,
	[DEC_IRQ_SCC1A_TXDMA]	= -1,
};

static int_ptr kn02ca_cpu_mask_nr_tbl[][2] __initdata = {
	{ { .i = DEC_CPU_IRQ_MASK(KN02CA_CPU_INR_MEMORY) },
		{ .i = DEC_CPU_IRQ_NR(KN02CA_CPU_INR_MEMORY) } },
	{ { .i = DEC_CPU_IRQ_MASK(KN02CA_CPU_INR_RTC) },
		{ .i = DEC_CPU_IRQ_NR(KN02CA_CPU_INR_RTC) } },
	{ { .i = DEC_CPU_IRQ_MASK(KN02CA_CPU_INR_CASCADE) },
		{ .p = kn02xa_io_int } },
	{ { .i = DEC_CPU_IRQ_ALL },
		{ .p = cpu_all_int } },
};

static int_ptr kn02ca_asic_mask_nr_tbl[][2] __initdata = {
	{ { .i = IO_IRQ_DMA },
		{ .p = asic_dma_int } },
	{ { .i = IO_IRQ_MASK(KN02CA_IO_INR_SCC0) },
		{ .i = IO_IRQ_NR(KN02CA_IO_INR_SCC0) } },
	{ { .i = IO_IRQ_MASK(KN02CA_IO_INR_ASC) },
		{ .i = IO_IRQ_NR(KN02CA_IO_INR_ASC) } },
	{ { .i = IO_IRQ_MASK(KN02CA_IO_INR_LANCE) },
		{ .i = IO_IRQ_NR(KN02CA_IO_INR_LANCE) } },
	{ { .i = IO_IRQ_MASK(KN02CA_IO_INR_TC1) },
		{ .i = IO_IRQ_NR(KN02CA_IO_INR_TC1) } },
	{ { .i = IO_IRQ_MASK(KN02CA_IO_INR_TC0) },
		{ .i = IO_IRQ_NR(KN02CA_IO_INR_TC0) } },
	{ { .i = IO_IRQ_ALL },
		{ .p = asic_all_int } },
};

void __init dec_init_kn02ca(void)
{
	/* IRQ routing. */
	memcpy(&dec_interrupt, &kn02ca_interrupt,
		sizeof(kn02ca_interrupt));

	/* CPU IRQ priorities. */
	memcpy(&cpu_mask_nr_tbl, &kn02ca_cpu_mask_nr_tbl,
		sizeof(kn02ca_cpu_mask_nr_tbl));

	/* I/O ASIC IRQ priorities. */
	memcpy(&asic_mask_nr_tbl, &kn02ca_asic_mask_nr_tbl,
		sizeof(kn02ca_asic_mask_nr_tbl));

	mips_cpu_irq_init(DEC_CPU_IRQ_BASE);
	init_ioasic_irqs(IO_IRQ_BASE);

}				/* dec_init_kn02ca */


/*
 * Machine-specific initialisation for KN03, aka DS5000/240,
 * aka 3max+ and DS5900, aka BIGmax.  Also applies to KN05, aka
 * DS5000/260, aka 4max+ and DS5900-260.
 */
static int kn03_interrupt[DEC_NR_INTS] __initdata = {
	[DEC_IRQ_CASCADE]	= DEC_CPU_IRQ_NR(KN03_CPU_INR_CASCADE),
	[DEC_IRQ_AB_RECV]	= -1,
	[DEC_IRQ_AB_XMIT]	= -1,
	[DEC_IRQ_DZ11]		= -1,
	[DEC_IRQ_ASC]		= IO_IRQ_NR(KN03_IO_INR_ASC),
	[DEC_IRQ_FLOPPY]	= -1,
	[DEC_IRQ_FPU]		= DEC_CPU_IRQ_NR(DEC_CPU_INR_FPU),
	[DEC_IRQ_HALT]		= DEC_CPU_IRQ_NR(KN03_CPU_INR_HALT),
	[DEC_IRQ_ISDN]		= -1,
	[DEC_IRQ_LANCE]		= IO_IRQ_NR(KN03_IO_INR_LANCE),
	[DEC_IRQ_MEMORY]	= DEC_CPU_IRQ_NR(KN03_CPU_INR_MEMORY),
	[DEC_IRQ_PSU]		= IO_IRQ_NR(KN03_IO_INR_PSU),
	[DEC_IRQ_RTC]		= DEC_CPU_IRQ_NR(KN03_CPU_INR_RTC),
	[DEC_IRQ_SCC0]		= IO_IRQ_NR(KN03_IO_INR_SCC0),
	[DEC_IRQ_SCC1]		= IO_IRQ_NR(KN03_IO_INR_SCC1),
	[DEC_IRQ_SII]		= -1,
	[DEC_IRQ_TC0]		= IO_IRQ_NR(KN03_IO_INR_TC0),
	[DEC_IRQ_TC1]		= IO_IRQ_NR(KN03_IO_INR_TC1),
	[DEC_IRQ_TC2]		= IO_IRQ_NR(KN03_IO_INR_TC2),
	[DEC_IRQ_TIMER]		= -1,
	[DEC_IRQ_VIDEO]		= -1,
	[DEC_IRQ_ASC_MERR]	= IO_IRQ_NR(IO_INR_ASC_MERR),
	[DEC_IRQ_ASC_ERR]	= IO_IRQ_NR(IO_INR_ASC_ERR),
	[DEC_IRQ_ASC_DMA]	= IO_IRQ_NR(IO_INR_ASC_DMA),
	[DEC_IRQ_FLOPPY_ERR]	= -1,
	[DEC_IRQ_ISDN_ERR]	= -1,
	[DEC_IRQ_ISDN_RXDMA]	= -1,
	[DEC_IRQ_ISDN_TXDMA]	= -1,
	[DEC_IRQ_LANCE_MERR]	= IO_IRQ_NR(IO_INR_LANCE_MERR),
	[DEC_IRQ_SCC0A_RXERR]	= IO_IRQ_NR(IO_INR_SCC0A_RXERR),
	[DEC_IRQ_SCC0A_RXDMA]	= IO_IRQ_NR(IO_INR_SCC0A_RXDMA),
	[DEC_IRQ_SCC0A_TXERR]	= IO_IRQ_NR(IO_INR_SCC0A_TXERR),
	[DEC_IRQ_SCC0A_TXDMA]	= IO_IRQ_NR(IO_INR_SCC0A_TXDMA),
	[DEC_IRQ_AB_RXERR]	= -1,
	[DEC_IRQ_AB_RXDMA]	= -1,
	[DEC_IRQ_AB_TXERR]	= -1,
	[DEC_IRQ_AB_TXDMA]	= -1,
	[DEC_IRQ_SCC1A_RXERR]	= IO_IRQ_NR(IO_INR_SCC1A_RXERR),
	[DEC_IRQ_SCC1A_RXDMA]	= IO_IRQ_NR(IO_INR_SCC1A_RXDMA),
	[DEC_IRQ_SCC1A_TXERR]	= IO_IRQ_NR(IO_INR_SCC1A_TXERR),
	[DEC_IRQ_SCC1A_TXDMA]	= IO_IRQ_NR(IO_INR_SCC1A_TXDMA),
};

static int_ptr kn03_cpu_mask_nr_tbl[][2] __initdata = {
	{ { .i = DEC_CPU_IRQ_MASK(KN03_CPU_INR_MEMORY) },
		{ .i = DEC_CPU_IRQ_NR(KN03_CPU_INR_MEMORY) } },
	{ { .i = DEC_CPU_IRQ_MASK(KN03_CPU_INR_RTC) },
		{ .i = DEC_CPU_IRQ_NR(KN03_CPU_INR_RTC) } },
	{ { .i = DEC_CPU_IRQ_MASK(KN03_CPU_INR_CASCADE) },
		{ .p = kn03_io_int } },
	{ { .i = DEC_CPU_IRQ_ALL },
		{ .p = cpu_all_int } },
};

static int_ptr kn03_asic_mask_nr_tbl[][2] __initdata = {
	{ { .i = IO_IRQ_DMA },
		{ .p = asic_dma_int } },
	{ { .i = IO_IRQ_MASK(KN03_IO_INR_SCC0) },
		{ .i = IO_IRQ_NR(KN03_IO_INR_SCC0) } },
	{ { .i = IO_IRQ_MASK(KN03_IO_INR_SCC1) },
		{ .i = IO_IRQ_NR(KN03_IO_INR_SCC1) } },
	{ { .i = IO_IRQ_MASK(KN03_IO_INR_ASC) },
		{ .i = IO_IRQ_NR(KN03_IO_INR_ASC) } },
	{ { .i = IO_IRQ_MASK(KN03_IO_INR_LANCE) },
		{ .i = IO_IRQ_NR(KN03_IO_INR_LANCE) } },
	{ { .i = IO_IRQ_MASK(KN03_IO_INR_TC2) },
		{ .i = IO_IRQ_NR(KN03_IO_INR_TC2) } },
	{ { .i = IO_IRQ_MASK(KN03_IO_INR_TC1) },
		{ .i = IO_IRQ_NR(KN03_IO_INR_TC1) } },
	{ { .i = IO_IRQ_MASK(KN03_IO_INR_TC0) },
		{ .i = IO_IRQ_NR(KN03_IO_INR_TC0) } },
	{ { .i = IO_IRQ_ALL },
		{ .p = asic_all_int } },
};

void __init dec_init_kn03(void)
{
	/* IRQ routing. */
	memcpy(&dec_interrupt, &kn03_interrupt,
		sizeof(kn03_interrupt));

	/* CPU IRQ priorities. */
	memcpy(&cpu_mask_nr_tbl, &kn03_cpu_mask_nr_tbl,
		sizeof(kn03_cpu_mask_nr_tbl));

	/* I/O ASIC IRQ priorities. */
	memcpy(&asic_mask_nr_tbl, &kn03_asic_mask_nr_tbl,
		sizeof(kn03_asic_mask_nr_tbl));

	mips_cpu_irq_init(DEC_CPU_IRQ_BASE);
	init_ioasic_irqs(IO_IRQ_BASE);

}				/* dec_init_kn03 */


void __init init_IRQ(void)
{
	switch (mips_machtype) {
	case MACH_DS23100:	/* DS2100/DS3100 Pmin/Pmax */
		dec_init_kn01();
		break;
	case MACH_DS5100:	/* DS5100 MIPSmate */
		dec_init_kn230();
		break;
	case MACH_DS5000_200:	/* DS5000/200 3max */
		dec_init_kn02();
		break;
	case MACH_DS5000_1XX:	/* DS5000/1xx 3min */
		dec_init_kn02ba();
		break;
	case MACH_DS5000_2X0:	/* DS5000/240 3max+ */
	case MACH_DS5900:	/* DS5900 bigmax */
		dec_init_kn03();
		break;
	case MACH_DS5000_XX:	/* Personal DS5000/xx */
		dec_init_kn02ca();
		break;
	case MACH_DS5800:	/* DS5800 Isis */
		panic("Don't know how to set this up!");
		break;
	case MACH_DS5400:	/* DS5400 MIPSfair */
		panic("Don't know how to set this up!");
		break;
	case MACH_DS5500:	/* DS5500 MIPSfair-2 */
		panic("Don't know how to set this up!");
		break;
	}
	set_except_vector(0, decstation_handle_int);

	/* Free the FPU interrupt if the exception is present. */
	if (!(mips_cpu.options & MIPS_CPU_NOFPUEX)) {
		cpu_fpu_mask = 0;
		dec_interrupt[DEC_IRQ_FPU] = -1;
	}

	/* Register board interrupts: FPU and cascade. */
	if (dec_interrupt[DEC_IRQ_FPU] >= 0)
		setup_irq(dec_interrupt[DEC_IRQ_FPU], &fpuirq);
	if (dec_interrupt[DEC_IRQ_CASCADE] >= 0)
		setup_irq(dec_interrupt[DEC_IRQ_CASCADE], &ioirq);

	/* Register the HALT interrupt. */
	if (dec_interrupt[DEC_IRQ_HALT] >= 0)
		setup_irq(dec_interrupt[DEC_IRQ_HALT], &haltirq);
}

EXPORT_SYMBOL(ioasic_base);
EXPORT_SYMBOL(dec_kn_slot_size);
EXPORT_SYMBOL(dec_interrupt);
