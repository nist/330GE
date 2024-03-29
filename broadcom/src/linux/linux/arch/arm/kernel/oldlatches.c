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
 *  linux/arch/arm/kernel/oldlatches.c
 *
 *  Copyright (C) David Alan Gilbert 1995/1996,2000
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Support for the latches on the old Archimedes which control the floppy,
 *  hard disc and printer
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>

#include <asm/io.h>
#include <asm/hardware.h>
#include <asm/mach-types.h>
#include <asm/arch/oldlatches.h>

static unsigned char latch_a_copy;
static unsigned char latch_b_copy;

/* newval=(oldval & ~mask)|newdata */
void oldlatch_aupdate(unsigned char mask,unsigned char newdata)
{
	if (machine_is_archimedes()) {
		unsigned long flags;

		local_save_flags(flags);
		latch_a_copy = (latch_a_copy & ~mask) | newdata;
		__raw_writeb(latch_a_copy, LATCHA_BASE);
		local_restore_flags(flags);

		printk("Latch: A = 0x%02x\n", latch_a_copy);
	} else
		BUG();
}


/* newval=(oldval & ~mask)|newdata */
void oldlatch_bupdate(unsigned char mask,unsigned char newdata)
{
	if (machine_is_archimedes()) {
		unsigned long flags;

		local_save_flags(flags);
		latch_b_copy = (latch_b_copy & ~mask) | newdata;
		__raw_writeb(latch_b_copy, LATCHB_BASE);
		local_restore_flags(flags);

		printk("Latch: B = 0x%02x\n", latch_b_copy);
	} else
		BUG();
}

static int __init oldlatch_init(void)
{
	if (machine_is_archimedes()) {
		oldlatch_aupdate(0xff, 0xff);
		/* Thats no FDC reset...*/
		oldlatch_bupdate(0xff, LATCHB_FDCRESET);
	}
	return 0;
}

__initcall(oldlatch_init);

EXPORT_SYMBOL(oldlatch_aupdate);
EXPORT_SYMBOL(oldlatch_bupdate);
