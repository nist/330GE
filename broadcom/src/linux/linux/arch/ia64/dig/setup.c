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
 * Platform dependent support for DIG64 platforms.
 *
 * Copyright (C) 1999 Intel Corp.
 * Copyright (C) 1999, 2001 Hewlett-Packard Co
 * Copyright (C) 1999, 2001 David Mosberger-Tang <davidm@hpl.hp.com>
 * Copyright (C) 1999 VA Linux Systems
 * Copyright (C) 1999 Walt Drummond <drummond@valinux.com>
 * Copyright (C) 1999 Vijay Chander <vijay@engr.sgi.com>
 */
#include <linux/config.h>

#include <linux/init.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/kdev_t.h>
#include <linux/string.h>
#include <linux/tty.h>
#include <linux/console.h>
#include <linux/timex.h>
#include <linux/sched.h>

#include <asm/io.h>
#include <asm/machvec.h>
#include <asm/system.h>

/*
 * This is here so we can use the CMOS detection in ide-probe.c to
 * determine what drives are present.  In theory, we don't need this
 * as the auto-detection could be done via ide-probe.c:do_probe() but
 * in practice that would be much slower, which is painful when
 * running in the simulator.  Note that passing zeroes in DRIVE_INFO
 * is sufficient (the IDE driver will autodetect the drive geometry).
 */
char drive_info[4*16];
extern int pcat_compat;

void __init
dig_setup (char **cmdline_p)
{
	unsigned int orig_x, orig_y, num_cols, num_rows, font_height;

	/*
	 * Default to /dev/sda2.  This assumes that the EFI partition
	 * is physical disk 1 partition 1 and the Linux root disk is
	 * physical disk 1 partition 2.
	 */
	ROOT_DEV = to_kdev_t(0x0802);		/* default to second partition on first drive */

#ifdef CONFIG_SMP
	init_smp_config();
#endif

	memset(&screen_info, 0, sizeof(screen_info));

	if (!ia64_boot_param->console_info.num_rows
	    || !ia64_boot_param->console_info.num_cols)
	{
		printk("dig_setup: warning: invalid screen-info, guessing 80x25\n");
		orig_x = 0;
		orig_y = 0;
		num_cols = 80;
		num_rows = 25;
		font_height = 16;
	} else {
		orig_x = ia64_boot_param->console_info.orig_x;
		orig_y = ia64_boot_param->console_info.orig_y;
		num_cols = ia64_boot_param->console_info.num_cols;
		num_rows = ia64_boot_param->console_info.num_rows;
		font_height = 400 / num_rows;
	}

	screen_info.orig_x = orig_x;
	screen_info.orig_y = orig_y;
	screen_info.orig_video_cols  = num_cols;
	screen_info.orig_video_lines = num_rows;
	screen_info.orig_video_points = font_height;
	screen_info.orig_video_mode = 3;	
	screen_info.orig_video_isVGA = 1;	
	screen_info.orig_video_ega_bx = 3;	
}

void __init
dig_irq_init (void)
{
}
