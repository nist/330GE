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

#ifndef _PPC_KERNEL_i8259_H
#define _PPC_KERNEL_i8259_H

#include <linux/irq.h>

extern struct hw_interrupt_type i8259_pic;

void i8259_init(unsigned long int_ack);
int i8259_irq(struct pt_regs *regs);
int i8259_poll(struct pt_regs *regs);

#endif /* _PPC_KERNEL_i8259_H */
