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
 *  linux/arch/arm/mach-clps711x/irq.c
 *
 *  Copyright (C) 2000 Deep Blue Solutions Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <linux/init.h>

#include <asm/mach/irq.h>
#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/irq.h>

#include <asm/hardware/clps7111.h>

static void mask_irq_int1(unsigned int irq)
{
	u32 intmr1;

	intmr1 = clps_readl(INTMR1);
	intmr1 &= ~(1 << irq);
	clps_writel(intmr1, INTMR1);
}

static void mask_ack_irq_int1(unsigned int irq)
{
	u32 intmr1;

	intmr1 = clps_readl(INTMR1);
	intmr1 &= ~(1 << irq);
	clps_writel(intmr1, INTMR1);

	switch (irq) {
	case IRQ_CSINT:  clps_writel(0, COEOI);  break;
	case IRQ_TC1OI:  clps_writel(0, TC1EOI); break;
	case IRQ_TC2OI:  clps_writel(0, TC2EOI); break;
	case IRQ_RTCMI:  clps_writel(0, RTCEOI); break;
	case IRQ_TINT:   clps_writel(0, TEOI);   break;
	case IRQ_UMSINT: clps_writel(0, UMSEOI); break;
	}
}

static void unmask_irq_int1(unsigned int irq)
{
	u32 intmr1;

	intmr1 = clps_readl(INTMR1);
	intmr1 |= 1 << irq;
	clps_writel(intmr1, INTMR1);
}

static void mask_irq_int2(unsigned int irq)
{
	u32 intmr2;

	intmr2 = clps_readl(INTMR2);
	intmr2 &= ~(1 << (irq - 16));
	clps_writel(intmr2, INTMR2);
}

static void mask_ack_irq_int2(unsigned int irq)
{
	u32 intmr2;

	intmr2 = clps_readl(INTMR2);
	intmr2 &= ~(1 << (irq - 16));
	clps_writel(intmr2, INTMR2);

	switch (irq) {
	case IRQ_KBDINT: clps_writel(0, KBDEOI); break;
	}
}

static void unmask_irq_int2(unsigned int irq)
{
	u32 intmr2;

	intmr2 = clps_readl(INTMR2);
	intmr2 |= 1 << (irq - 16);
	clps_writel(intmr2, INTMR2);
}

void __init clps711x_init_irq(void)
{
	unsigned int i;

	for (i = 0; i < NR_IRQS; i++) {
	        if (INT1_IRQS & (1 << i)) {
		        irq_desc[i].valid	= 1;
			irq_desc[i].probe_ok	= 1;
			irq_desc[i].mask_ack	= (INT1_ACK_IRQS & (1 << i)) ?
						   mask_ack_irq_int1 :
						   mask_irq_int1;
			irq_desc[i].mask	= mask_irq_int1;
			irq_desc[i].unmask	= unmask_irq_int1;
		}
		if (INT2_IRQS & (1 << i)) {
		        irq_desc[i].valid	= 1;
			irq_desc[i].probe_ok	= 1;
			irq_desc[i].mask_ack	= (INT2_ACK_IRQS & (1 << i)) ?
						   mask_ack_irq_int2 :
						   mask_irq_int2;
			irq_desc[i].mask	= mask_irq_int2;
			irq_desc[i].unmask	= unmask_irq_int2;
		}			
	}

	/*
	 * Disable interrupts
	 */
	clps_writel(0, INTMR1);
	clps_writel(0, INTMR2);

	/*
	 * Clear down any pending interrupts
	 */
	clps_writel(0, COEOI);
	clps_writel(0, TC1EOI);
	clps_writel(0, TC2EOI);
	clps_writel(0, RTCEOI);
	clps_writel(0, TEOI);
	clps_writel(0, UMSEOI);
	clps_writel(0, SYNCIO);
	clps_writel(0, KBDEOI);
}
