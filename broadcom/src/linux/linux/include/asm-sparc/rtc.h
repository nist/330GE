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
/* $Id: rtc.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $
 *
 * rtc.h: Definitions for access to the Mostek real time clock
 *
 * Copyright (C) 1996 Thomas K. Dyas (tdyas@eden.rutgers.edu)
 */

#ifndef _RTC_H
#define _RTC_H

#include <linux/ioctl.h>

struct rtc_time
{
	int	sec;	/* Seconds (0-59) */
	int	min;	/* Minutes (0-59) */
	int	hour;	/* Hour (0-23) */
	int	dow;	/* Day of the week (1-7) */
	int	dom;	/* Day of the month (1-31) */
	int	month;	/* Month of year (1-12) */
	int	year;	/* Year (0-99) */
};

#define RTCGET _IOR('p', 20, struct rtc_time)
#define RTCSET _IOW('p', 21, struct rtc_time)

#endif
