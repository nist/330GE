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
#ifndef _LINUX_THREADS_H
#define _LINUX_THREADS_H

#include <linux/config.h>

/*
 * The default limit for the nr of threads is now in
 * /proc/sys/kernel/threads-max.
 */
 
#ifdef CONFIG_SMP
#ifdef __mips__
#define NR_CPUS _MIPS_SZLONG
#else
#define NR_CPUS	64		/* Max processors that can be running in SMP */
#endif
#else
#define NR_CPUS 1
#endif

#define MIN_THREADS_LEFT_FOR_ROOT 4

/*
 * This controls the maximum pid allocated to a process
 */
#define PID_MAX 0x8000

#endif
