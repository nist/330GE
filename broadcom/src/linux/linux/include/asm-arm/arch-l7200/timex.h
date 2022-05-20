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
 * linux/include/asm-arm/arch-l7200/timex.h
 *
 * Copyright (C) 2000 Rob Scott (rscott@mtrob.fdns.net)
 *                    Steve Hill (sjhill@cotw.com)
 *
 * 04-21-2000  RS Created file
 * 05-03-2000 SJH Tick rate was wrong
 *
 */

/*
 * On the ARM720T, clock ticks are set to 128 Hz.
 *
 * NOTE: The actual RTC value is set in 'time.h' which
 *       must be changed when choosing a different tick
 *       rate. The value of HZ in 'param.h' must also
 *       be changed to match below.
 */
#define CLOCK_TICK_RATE		128
