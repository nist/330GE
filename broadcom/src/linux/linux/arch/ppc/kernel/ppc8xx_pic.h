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
 * BK Id: %F% %I% %G% %U% %#%
 */
#ifndef _PPC_KERNEL_PPC8xx_H
#define _PPC_KERNEL_PPC8xx_H

#include <linux/config.h>
#include <linux/irq.h>

extern struct hw_interrupt_type ppc8xx_pic;

void m8xx_pic_init(void);
void m8xx_do_IRQ(struct pt_regs *regs,
                 int            cpu);
int m8xx_get_irq(struct pt_regs *regs);

#ifdef CONFIG_MBX
#include <asm/i8259.h>
#include <asm/io.h>
void mbx_i8259_action(int cpl, void *dev_id, struct pt_regs *regs);
#endif

#endif /* _PPC_KERNEL_PPC8xx_H */
