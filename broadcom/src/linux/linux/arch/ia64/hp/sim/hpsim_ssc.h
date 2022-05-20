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
 * Platform dependent support for HP simulator.
 *
 * Copyright (C) 1998, 1999 Hewlett-Packard Co
 * Copyright (C) 1998, 1999 David Mosberger-Tang <davidm@hpl.hp.com>
 * Copyright (C) 1999 Vijay Chander <vijay@engr.sgi.com>
 */
#ifndef _IA64_PLATFORM_HPSIM_SSC_H
#define _IA64_PLATFORM_HPSIM_SSC_H

/* Simulator system calls: */

#define SSC_CONSOLE_INIT		20
#define SSC_GETCHAR			21
#define SSC_PUTCHAR			31
#define SSC_CONNECT_INTERRUPT		58
#define SSC_GENERATE_INTERRUPT		59
#define SSC_SET_PERIODIC_INTERRUPT	60
#define SSC_GET_RTC			65
#define SSC_EXIT			66
#define SSC_LOAD_SYMBOLS		69
#define SSC_GET_TOD			74
#define SSC_CTL_TRACE			76

#define SSC_NETDEV_PROBE		100
#define SSC_NETDEV_SEND			101
#define SSC_NETDEV_RECV			102
#define SSC_NETDEV_ATTACH		103
#define SSC_NETDEV_DETACH		104

/*
 * Simulator system call.
 */
extern long ia64_ssc (long arg0, long arg1, long arg2, long arg3, int nr);

#endif /* _IA64_PLATFORM_HPSIM_SSC_H */
