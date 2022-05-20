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
 *  linux/drivers/acorn/char/pcf8583.h
 *
 *  Copyright (C) 2000 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
struct rtc_tm {
	unsigned char	cs;
	unsigned char	secs;
	unsigned char	mins;
	unsigned char	hours;
	unsigned char	mday;
	unsigned char	mon;
	unsigned char	year_off;
	unsigned char	wday;
};

struct mem {
	unsigned int	loc;
	unsigned int	nr;
	unsigned char	*data;
};

#define RTC_GETDATETIME	0
#define RTC_SETTIME	1
#define RTC_SETDATETIME	2
#define RTC_GETCTRL	3
#define RTC_SETCTRL	4
#define MEM_READ	5
#define MEM_WRITE	6

#define CTRL_STOP	0x80
#define CTRL_HOLD	0x40
#define CTRL_32KHZ	0x00
#define CTRL_MASK	0x08
#define CTRL_ALARMEN	0x04
#define CTRL_ALARM	0x02
#define CTRL_TIMER	0x01
