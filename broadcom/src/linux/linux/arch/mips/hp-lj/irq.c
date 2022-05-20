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
 * Code to handle x86 style IRQs plus some generic interrupt stuff.
 *
 * Copyright (C) 1992 Linus Torvalds
 * Copyright (C) 1994 - 2000 Ralf Baechle
 */

#include <linux/config.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <asm/mipsregs.h>
#include <asm/system.h>
#include <asm/gdb-stub.h>


/* install the handler for exception 0 */
void __init init_IRQ(void)
{
    extern void hpIRQ(void);
    extern void mips_cpu_irq_init(u32 base);
    mips_cpu_irq_init(0);
    set_except_vector(0, hpIRQ);

#ifdef CONFIG_REMOTE_DEBUG
    {
       extern void breakpoint(void);
       extern int remote_debug;

       if (remote_debug) {
          set_debug_traps();
          breakpoint();
       }
    }
#endif

}

