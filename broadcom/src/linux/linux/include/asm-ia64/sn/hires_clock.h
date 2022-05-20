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
 * Copyright (C) 2001 Silicon Graphics, Inc. All rights reserved.
 *
 * SGI Hi Resolution Clock
 *
 * SGI SN platforms provide a high resolution clock that is
 * synchronized across all nodes. The clock can be memory mapped
 * and directly read from user space.
 *
 * Access to the clock is thru the following:
 *       (error checking not shown)
 *
 * (Note: should library routines be provided to encapsulate this??)
 *
 *	int		fd:
 *	volatile long	*clk;
 *
 *	fd = open (HIRES_FULLNAME, O_RDONLY);
 *	clk = mmap(0, getpagesize(), PROT_READ, MAP_SHARED, fd, 0);
 *	clk += ioctl(fd, HIRES_IOCQGETOFFSET, 0);
 *
 * At this point, clk is a pointer to the high resolution clock.
 *
 * The clock period can be obtained via:
 *
 *	long	picosec_per_tick;
 *	picosec_per_tick = ioctl(fd, HIRES_IOCQGETPICOSEC, 0);
 */

#ifndef _ASM_IA64_SN_HIRES_CLOCK_H
#define _ASM_IA64_SN_HIRES_CLOCK_H


#define HIRES_BASENAME	"sgi_hires_clock"
#define HIRES_FULLNAME  "/dev/sgi_hires_clock"
#define HIRES_IOC_BASE	's'


/* Get page offset of hires timer */
#define HIRES_IOCQGETOFFSET	_IO( HIRES_IOC_BASE, 0 )

/* get clock period in picoseconds per tick */
#define HIRES_IOCQGETPICOSEC	_IO( HIRES_IOC_BASE, 1 )

/* get number of significant bits in clock counter */
#define HIRES_IOCQGETCLOCKBITS	_IO( HIRES_IOC_BASE, 2 )

#endif /* _ASM_IA64_SN_HIRES_CLOCK_H */
