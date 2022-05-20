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
#ifndef IRQ_HANDLER_
#define IRQ_HANDLER_

#define INT_CAUSE_MAIN 0
#define INT_CAUSE_HIGH 1

#define MAX_CAUSE_REGS 4
#define MAX_CAUSE_REG_WIDTH 32

void hook_irq_handler (int int_cause , int bit_num , void *isr_ptr);
int disable_galileo_irq (int int_cause , int bit_num);
int enable_galileo_irq (int int_cause , int bit_num);

extern struct tq_struct irq_handlers[MAX_CAUSE_REGS][MAX_CAUSE_REG_WIDTH];

/*
  PCI interrupts will come in on either the INTA or
  INTD interrups lines, which are mapped to the #2 and
  #5 interrupt pins of the MIPS.  On our boards, they
  all either come in on IntD or they all come in on
  IntA, they aren't mixed. There can be numerous PCI
  interrupts, so we keep a list of the "requested"
  interrupt numbers and go through the list whenever
  we get an IntA/D.

  All PCI interrupts have numbers >= 20 by arbitrary convention.  Any
  interrupt < 8 is an interrupt that is maskable on the
  MIPS.
*/

#define TIMER 4
#define INTA 2
#define INTD 5


#endif /* IRQ_HANDLER_ */
