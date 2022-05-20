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
 * RTC routines for DECstation style attached Dallas DS1287 chip.
 *
 * Copyright (C) 1998, 2001 by Ralf Baechle
 * Copyright (C) 1998 by Harald Koerfgen
 * Copyright (C) 2002  Maciej W. Rozycki
 */

#include <linux/mc146818rtc.h>
#include <linux/module.h>
#include <linux/types.h>

volatile u8 *dec_rtc_base;

static unsigned char dec_rtc_read_data(unsigned long addr)
{
	return dec_rtc_base[addr * 4];
}

static void dec_rtc_write_data(unsigned char data, unsigned long addr)
{
	dec_rtc_base[addr * 4] = data;
}

static int dec_rtc_bcd_mode(void)
{
	return 0;
}

struct rtc_ops dec_rtc_ops = {
	&dec_rtc_read_data,
	&dec_rtc_write_data,
	&dec_rtc_bcd_mode
};

EXPORT_SYMBOL(dec_rtc_base);
