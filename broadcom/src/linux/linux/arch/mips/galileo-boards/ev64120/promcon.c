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
 * SGI PROM io-routines.
 *
 * Copyright (c) 1999 Ulf Carlsson
 *
 * Derived from DECstation promcon.c
 * Copyright (c) 1998 Harald Koerfgen
 */
#include <linux/tty.h>
#include <linux/major.h>
#include <linux/ptrace.h>
#include <linux/init.h>
#include <linux/console.h>
#include <linux/fs.h>

static void prom_console_write(struct console *co, const char *s,
			       unsigned count)
{
	extern int CONSOLE_CHANNEL;	// The default serial port
	unsigned i;

	for (i = 0; i < count; i++) {
		if (*s == 10)
			serial_putc(CONSOLE_CHANNEL, 13);
		serial_putc(CONSOLE_CHANNEL, *s++);
	}
}

int prom_getchar(void)
{
	return 0;
}

static kdev_t prom_console_device(struct console *c)
{
	return MKDEV(TTY_MAJOR, 64 + c->index);
}

static struct console sercons = {
    .name	= "ttyS",
    .write	= prom_console_write,
    .device	= prom_console_device,
    .flags	= CON_PRINTBUFFER,
    .index	= -1,
};

/*
 *    Register console.
 */

void gal_serial_console_init(void)
{
	//  serial_init();
	//serial_set(115200);

	register_console(&sercons);
}
