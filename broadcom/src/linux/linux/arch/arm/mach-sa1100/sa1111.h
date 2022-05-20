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
 * linux/arch/arm/mach-sa1100/sa1111.h
 */

/*
 * These two don't really belong in here.
 */
extern void sa1110_mb_enable(void);
extern void sa1110_mb_disable(void);

/*
 * Probe for a SA1111 chip.
 */
extern int sa1111_probe(unsigned long phys);

/*
 * Wake up a SA1111 chip.
 */
extern void sa1111_wake(void);

/*
 * Doze the SA1111 chip.
 */
extern void sa1111_doze(void);

/*
 * Configure the SA1111 shared memory controller.
 */
extern void sa1111_configure_smc(int sdram, unsigned int drac, unsigned int cas_latency);


extern void sa1111_init_irq(int irq_nr);
extern void sa1111_IRQ_demux(int irq, void *dev_id, struct pt_regs *regs);

