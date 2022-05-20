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
 * RTC routines for Indy style attached Dallas chip.
 *
 * Copyright (C) 1998, 2001 by Ralf Baechle
 */
#include <asm/ds1286.h>
#include <asm/sgi/sgihpc.h>

static unsigned char indy_rtc_read_data(unsigned long addr)
{
	volatile unsigned int *rtcregs = (void *)INDY_CLOCK_REGS;

	return rtcregs[addr];
}

static void indy_rtc_write_data(unsigned char data, unsigned long addr)
{
	volatile unsigned int *rtcregs = (void *)INDY_CLOCK_REGS;

	rtcregs[addr] = data;
}

static int indy_rtc_bcd_mode(void)
{
	return 0;
}

struct rtc_ops indy_rtc_ops = {
	&indy_rtc_read_data,
	&indy_rtc_write_data,
	&indy_rtc_bcd_mode
};
