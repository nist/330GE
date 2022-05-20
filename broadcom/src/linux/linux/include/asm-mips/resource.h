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
 * Copyright (C) 1995, 96, 98, 2000 by Ralf Baechle
 */
#ifndef _ASM_RESOURCE_H
#define _ASM_RESOURCE_H

/*
 * Resource limits
 */
#define RLIMIT_CPU 0			/* CPU time in ms */
#define RLIMIT_FSIZE 1			/* Maximum filesize */
#define RLIMIT_DATA 2			/* max data size */
#define RLIMIT_STACK 3			/* max stack size */
#define RLIMIT_CORE 4			/* max core file size */
#define RLIMIT_NOFILE 5			/* max number of open files */
#define RLIMIT_AS 6			/* mapped memory */
#define RLIMIT_RSS 7			/* max resident set size */
#define RLIMIT_NPROC 8			/* max number of processes */
#define RLIMIT_MEMLOCK 9		/* max locked-in-memory address space */
#define RLIMIT_LOCKS	10		/* maximum file locks held */

#define RLIM_NLIMITS 11			/* Number of limit flavors.  */

#ifdef __KERNEL__

/*
 * SuS says limits have to be unsigned.
 * Which makes a ton more sense anyway.
 */
#define RLIM_INFINITY	0x7fffffffUL

#define INIT_RLIMITS					\
{							\
	{ RLIM_INFINITY, RLIM_INFINITY },		\
	{ RLIM_INFINITY, RLIM_INFINITY },		\
	{ RLIM_INFINITY, RLIM_INFINITY },		\
	{ _STK_LIM,      RLIM_INFINITY },		\
	{        0,      RLIM_INFINITY },		\
	{ INR_OPEN,      INR_OPEN      },		\
	{ RLIM_INFINITY, RLIM_INFINITY },		\
	{ RLIM_INFINITY, RLIM_INFINITY },		\
	{ 0,             0             },		\
	{ RLIM_INFINITY, RLIM_INFINITY },		\
	{ RLIM_INFINITY, RLIM_INFINITY },		\
}

#endif /* __KERNEL__ */

#endif /* _ASM_RESOURCE_H */
