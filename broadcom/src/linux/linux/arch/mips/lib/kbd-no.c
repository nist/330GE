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
 * Stub keyboard and psaux routines to keep Linux from crashing on machines
 * without a keyboard.
 *
 * Copyright (C) 1998 by Ralf Baechle
 */
#include <linux/sched.h>
#include <asm/keyboard.h>

static void no_kbd_request_region(void)
{
	/* No I/O ports are being used on the Indy.  */
}

static int no_kbd_request_irq(void (*handler)(int, void *, struct pt_regs *))
{
	return -ENODEV;
}

static int no_aux_request_irq(void (*handler)(int, void *, struct pt_regs *))
{
	return -ENODEV;
}

static void no_aux_free_irq(void)
{
}

static unsigned char no_kbd_read_input(void)
{
	return 0;
}

static void no_kbd_write_output(unsigned char val)
{
}

static void no_kbd_write_command(unsigned char val)
{
}

static unsigned char no_kbd_read_status(void)
{
	return 0;
}

struct kbd_ops no_kbd_ops = {
	no_kbd_request_region,
	no_kbd_request_irq,

	no_aux_request_irq,
	no_aux_free_irq,

	no_kbd_read_input,
	no_kbd_write_output,
	no_kbd_write_command,
	no_kbd_read_status
};
