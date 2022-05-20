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
 * linux/include/asm-arm/arch-l7200/param.h
 *
 * Copyright (C) 2000 Rob Scott (rscott@mtrob.fdns.net)
 *                    Steve Hill (sjhill@cotw.com)
 *
 * This file contains the hardware definitions for the
 * LinkUp Systems L7200 SOC development board.
 *
 * Changelog:
 *   04-21-2000 RS      Created L7200 version
 *   04-25-2000 SJH     Cleaned up file
 *   05-03-2000 SJH     Change comments and rate
 */
#ifndef __ASM_ARCH_PARAM_H
#define __ASM_ARCH_PARAM_H

/*
 * See 'time.h' for how the RTC HZ rate is set
 */
#define HZ 128

/*
 * Define hz_to_std, since we have a non 100Hz define
 * (see include/asm-arm/param.h)
 */

#if defined(__KERNEL__)
#define hz_to_std(a) ((a * HZ)/100)
#endif

#endif
