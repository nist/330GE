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
 * Copyright (C) 1992 Linus Torvalds
 * Copyright (C) 1994 - 2001 Ralf Baechle
 */
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>

#include <asm/io.h>
#include <asm/jazz.h>

extern asmlinkage void jazz_handle_int(void);

/*
 * On systems with i8259-style interrupt controllers we assume for
 * driver compatibility reasons interrupts 0 - 15 to be the i8295
 * interrupts even if the hardware uses a different interrupt numbering.
 */
void __init init_IRQ (void)
{
	int i;

	set_except_vector(0, jazz_handle_int);

	init_generic_irq();
	init_i8259_irqs();			/* Integrated i8259  */
}
