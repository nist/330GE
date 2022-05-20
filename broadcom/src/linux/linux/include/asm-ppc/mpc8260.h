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

/* This is the single file included by all MPC8260 build options.
 * Since there are many different boards and no standard configuration,
 * we have a unique include file for each.  Rather than change every
 * file that has to include MPC8260 configuration, they all include
 * this one and the configuration switching is done here.
 */
#ifdef __KERNEL__
#ifndef __CONFIG_8260_DEFS
#define __CONFIG_8260_DEFS

#include <linux/config.h>

#ifdef CONFIG_8260

#ifdef CONFIG_EST8260
#include <platforms/est8260.h>
#endif

/* I don't yet have the ISA or PCI stuff done....no 8260 with
 * such thing.....
 */
#define _IO_BASE        0
#define _ISA_MEM_BASE   0
#define PCI_DRAM_OFFSET 0

/* The "residual" data board information structure the boot loader
 * hands to us.
 */
extern unsigned char __res[];

/* I need this to get pt_regs.......
*/
#include <asm/ptrace.h>

extern int request_8xxirq(unsigned int irq,
		       void (*handler)(int, void *, struct pt_regs *),
		       unsigned long flags, 
		       const char *device,
		       void *dev_id);

#endif /* CONFIG_8260 */
#endif /* !__CONFIG_8260_DEFS */
#endif /* __KERNEL__ */
