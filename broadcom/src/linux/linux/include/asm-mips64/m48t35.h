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
 *  Registers for the SGS-Thomson M48T35 Timekeeper RAM chip
 */
#ifndef _ASM_M48T35_H
#define _ASM_M48T35_H

#include <linux/spinlock.h>

extern spinlock_t rtc_lock;

struct m48t35_rtc {
	volatile u8	pad[0x7ff8];    /* starts at 0x7ff8 */
	volatile u8	control;
	volatile u8	sec;
	volatile u8	min;
	volatile u8	hour;
	volatile u8	day;
	volatile u8	date;
	volatile u8	month;
	volatile u8	year;
};

#define M48T35_RTC_SET		0x80
#define M48T35_RTC_STOPPED	0x80
#define M48T35_RTC_READ		0x40

#define BCD_TO_BIN(x)		((x)=((x)&15) + ((x)>>4)*10)
#define BIN_TO_BCD(x)		((x)=(((x)/10)<<4) + (x)%10)

#endif /* _ASM_M48T35_H */
