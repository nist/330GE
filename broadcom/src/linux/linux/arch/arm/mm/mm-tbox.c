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
 *  linux/arch/arm/mm/mm-tbox.c
 *
 *  Copyright (C) 1998, 1999, 2000 Phil Blundell
 *  Copyright (C) 1998-1999 Russell King
 *
 *  Extra MM routines for the Tbox architecture
 */
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/init.h>

#include <asm/io.h>
#include <asm/pgtable.h>
#include <asm/page.h>

#include <asm/mach/map.h>
 
static struct map_desc tbox_io_desc[] __initdata = {
	/* See hardware.h for details */
	{ IO_BASE,	IO_START,	0x00100000, DOMAIN_IO, 0, 1, 0, 0 },
	LAST_DESC
};

void __init tbox_map_io(void)
{
	iotable_init(tbox_io_desc);
}
