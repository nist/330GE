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
 *  linux/include/asm-arm/arch-anakin/time.h
 *
 *  Copyright (C) 2001 Aleph One Ltd. for Acunia N.V.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Changelog:
 *   10-Apr-2001 TTC	Created
 */

#ifndef __ASM_ARCH_TIME_H
#define __ASM_ARCH_TIME_H

static void
anakin_timer_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{
	do_timer(regs);
}

static inline void
setup_timer(void)
{
	timer_irq.handler = anakin_timer_interrupt;
	timer_irq.flags = SA_INTERRUPT;
	setup_arm_irq(IRQ_TICK, &timer_irq);
}

#endif
