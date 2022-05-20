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
 *	Precise Delay Loops for SuperH
 *
 *	Copyright (C) 1999 Niibe Yutaka & Kaz Kojima
 */

#include <linux/sched.h>
#include <linux/delay.h>

void __delay(unsigned long loops)
{
	__asm__ __volatile__(
		"tst	%0, %0\n\t"
		"1:\t"
		"bf/s	1b\n\t"
		" dt	%0"
		: "=r" (loops)
		: "0" (loops)
		: "t");
}

inline void __const_udelay(unsigned long xloops)
{
	__asm__("dmulu.l	%0, %2\n\t"
		"sts	mach, %0"
		: "=r" (xloops)
		: "0" (xloops), "r" (current_cpu_data.loops_per_jiffy)
		: "macl", "mach");
	__delay(xloops * HZ);
}

void __udelay(unsigned long usecs)
{
	__const_udelay(usecs * 0x000010c6);  /* 2**32 / 1000000 */
}
