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
 * sun3ints.h -- Linux/Sun3 interrupt handling code definitions
 *
 * Erik Verbruggen (erik@bigmama.xtdnet.nl)
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive for
 * more details.
 */

#ifndef SUN3INTS_H
#define SUN3INTS_H

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/kernel_stat.h>
#include <linux/interrupt.h>
#include <asm/segment.h>
#include <asm/intersil.h>
#include <asm/oplib.h>

void sun3_enable_irq(unsigned int irq);
void sun3_disable_irq(unsigned int irq);
int sun3_request_irq(unsigned int irq,
                     void (*handler)(int, void *, struct pt_regs *),
                     unsigned long flags, const char *devname, void *dev_id
		    );
extern void sun3_init_IRQ (void);
extern void (*sun3_default_handler[]) (int, void *, struct pt_regs *);
extern void (*sun3_inthandler[]) (int, void *, struct pt_regs *);
extern void sun3_free_irq (unsigned int irq, void *dev_id);
extern void sun3_enable_interrupts (void);
extern void sun3_disable_interrupts (void);
extern int sun3_get_irq_list(char *buf);
extern void sun3_process_int(int, struct pt_regs *);
extern volatile unsigned char* sun3_intreg;


#endif /* SUN3INTS_H */
