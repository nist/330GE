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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1994 by Waldorf GMBH, written by Ralf Baechle
 * Copyright (C) 1995, 96, 97, 98, 99, 2000, 01, 02 by Ralf Baechle
 * Copyright (C) 1999, 2000 Silicon Graphics, Inc.
 * Copyright (C) 2001 Kanoj Sarcar
 */
#ifndef _ASM_IRQ_H
#define _ASM_IRQ_H

#include <linux/config.h>
#include <asm/sn/arch.h>

#define NR_IRQS 256

/*
 * Number of levels in INT_PEND0. Can be set to 128 if we also
 * consider INT_PEND1.
 */
#define PERNODE_LEVELS	64

extern int node_level_to_irq[MAX_COMPACT_NODES][PERNODE_LEVELS];

/*
 * we need to map irq's up to at least bit 7 of the INT_MASK0_A register
 * since bits 0-6 are pre-allocated for other purposes.
 */
#define LEAST_LEVEL	7
#define FAST_IRQ_TO_LEVEL(i)	((i) + LEAST_LEVEL)
#define LEVEL_TO_IRQ(c, l) \
			(node_level_to_irq[CPUID_TO_COMPACT_NODEID(c)][(l)])

#ifdef CONFIG_I8259
static inline int irq_cannonicalize(int irq)
{
	return ((irq == 2) ? 9 : irq);
}
#else
#define irq_cannonicalize(irq) (irq)	/* Sane hardware, sane code ... */
#endif

struct irqaction;
extern int i8259_setup_irq(int irq, struct irqaction * new);

extern void disable_irq(unsigned int);
extern void disable_irq_nosync(unsigned int);
extern void enable_irq(unsigned int);

/* Machine specific interrupt initialization  */
extern void (*irq_setup)(void);

extern void init_generic_irq(void);

#endif /* _ASM_IRQ_H */
