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
 * Stub RTC routines to keep Linux from crashing on machine which don't
 * have a RTC chip.
 *
 * Copyright (C) 1998, 2001 by Ralf Baechle
 */
#include <linux/kernel.h>
#include <linux/mc146818rtc.h>

static unsigned char no_rtc_read_data(unsigned long addr)
{
	panic("no_rtc_read_data called - shouldn't happen.");
}

static void no_rtc_write_data(unsigned char data, unsigned long addr)
{
	panic("no_rtc_write_data called - shouldn't happen.");
}

static int no_rtc_bcd_mode(void)
{
	panic("no_rtc_bcd_mode called - shouldn't happen.");
}

struct rtc_ops no_rtc_ops = {
	&no_rtc_read_data,
	&no_rtc_write_data,
	&no_rtc_bcd_mode
};
