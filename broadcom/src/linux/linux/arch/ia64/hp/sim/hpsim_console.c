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
 * Platform dependent support for HP simulator.
 *
 * Copyright (C) 1998, 1999 Hewlett-Packard Co
 * Copyright (C) 1998, 1999 David Mosberger-Tang <davidm@hpl.hp.com>
 * Copyright (C) 1999 Vijay Chander <vijay@engr.sgi.com>
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/param.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/console.h>

#include <asm/delay.h>
#include <asm/irq.h>
#include <asm/pal.h>
#include <asm/machvec.h>
#include <asm/pgtable.h>
#include <asm/sal.h>

#include "hpsim_ssc.h"

static int simcons_init (struct console *, char *);
static void simcons_write (struct console *, const char *, unsigned);
static int simcons_wait_key (struct console *);
static kdev_t simcons_console_device (struct console *);

struct console hpsim_cons = {
	name:		"simcons",
	write:		simcons_write,
	device:		simcons_console_device,
	setup:		simcons_init,
	flags:		CON_PRINTBUFFER,
	index:		-1,
};

static int
simcons_init (struct console *cons, char *options)
{
	return 0;
}

static void
simcons_write (struct console *cons, const char *buf, unsigned count)
{
	unsigned long ch;

	while (count-- > 0) {
		ch = *buf++;
		ia64_ssc(ch, 0, 0, 0, SSC_PUTCHAR);
		if (ch == '\n')
		  ia64_ssc('\r', 0, 0, 0, SSC_PUTCHAR);
	}
}

static kdev_t
simcons_console_device (struct console *c)
{
	return MKDEV(TTY_MAJOR, 64 + c->index);
}
