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
extern void hp300_init_IRQ(void);
extern void (*hp300_handlers[8])(int, void *, struct pt_regs *);
extern void hp300_free_irq(unsigned int irq, void *dev_id);
extern int hp300_request_irq(unsigned int irq,
		void (*handler) (int, void *, struct pt_regs *),
		unsigned long flags, const char *devname, void *dev_id);

/* number of interrupts, includes 0 (what's that?) */
#define HP300_NUM_IRQS 8
