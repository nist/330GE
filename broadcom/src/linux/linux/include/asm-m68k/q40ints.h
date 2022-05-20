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
 * contains some Q40 related interrupt definitions
 */

#define Q40_IRQ_MAX      (34)

#define Q40_IRQ_SAMPLE    (34)
#define Q40_IRQ_KEYBOARD (32)
#define Q40_IRQ_FRAME    (33)


/* masks for interrupt regiosters*/
/* internal, IIRQ_REG */
#define Q40_IRQ_KEYB_MASK    (2)
#define Q40_IRQ_SER_MASK     (1<<2)
#define Q40_IRQ_FRAME_MASK   (1<<3)
#define Q40_IRQ_EXT_MASK     (1<<4)    /* is a EIRQ */
/* eirq, EIRQ_REG */
#define Q40_IRQ3_MASK        (1)
#define Q40_IRQ4_MASK        (1<<1)
#define Q40_IRQ5_MASK        (1<<2)
#define Q40_IRQ6_MASK        (1<<3)
#define Q40_IRQ7_MASK        (1<<4)
#define Q40_IRQ10_MASK       (1<<5)
#define Q40_IRQ14_MASK       (1<<6)
#define Q40_IRQ15_MASK       (1<<7)

extern unsigned long q40_probe_irq_on (void);
extern int q40_probe_irq_off (unsigned long irqs);
