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
 * Wrap-around code for a console using the
 * ARC io-routines.
 *
 * Copyright (c) 1998 Harald Koerfgen
 * Copyright (c) 2001 Ralf Baechle
 * Copyright (c) 2002 Thiemo Seufer
 */
#include <linux/tty.h>
#include <linux/major.h>
#include <linux/ptrace.h>
#include <linux/init.h>
#include <linux/console.h>
#include <linux/fs.h>
#include <asm/sgialib.h>

static void prom_console_write(struct console *co, const char *s,
			       unsigned count)
{
	/* Do each character */
	while (count--) {
		if (*s == '\n')
			prom_putchar('\r');
		prom_putchar(*s++);
	}
}

static kdev_t prom_console_device(struct console *co)
{
	return MKDEV(TTY_MAJOR, 64 + co->index);
}

static int __init prom_console_setup(struct console *co, char *options)
{
	return !(prom_flags & PROM_FLAG_USE_AS_CONSOLE);
}

static struct console arc_cons = {
	.name		= "arc",
	.write		= prom_console_write,
	.device		= prom_console_device,
	.setup		= prom_console_setup,
	.flags		= CON_PRINTBUFFER,
	.index		= -1,
};

/*
 *    Register console.
 */

void __init arc_console_init(void)
{
	register_console(&arc_cons);
}
