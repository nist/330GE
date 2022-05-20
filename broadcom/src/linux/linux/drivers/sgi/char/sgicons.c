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
 * sgicons.c: Setting up and registering console I/O on the SGI.
 *
 * Copyright (C) 1996 David S. Miller (dm@engr.sgi.com)
 * Copyright (C) 1997 Miguel de Icaza (miguel@nuclecu.unam.mx)
 *
 * This implements a virtual console interface.
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include "gconsole.h"

/* This is the system graphics console (the first adapter found) */
struct console_ops *gconsole;
struct console_ops *real_gconsole;

void
enable_gconsole (void)
{
	if (!gconsole)
		gconsole = real_gconsole;
}

void
disable_gconsole (void)
{
	if (gconsole){
		real_gconsole = gconsole;
		gconsole = 0;
	}
}

EXPORT_SYMBOL(disable_gconsole);
EXPORT_SYMBOL(enable_gconsole);

void
register_gconsole (struct console_ops *gc)
{
	if (gconsole)
		return;
	gconsole = gc;
}
