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
#ifndef _LINUX_RESOURCE_H
#define _LINUX_RESOURCE_H

#include <linux/time.h>

/*
 * Resource control/accounting header file for linux
 */

/*
 * Definition of struct rusage taken from BSD 4.3 Reno
 * 
 * We don't support all of these yet, but we might as well have them....
 * Otherwise, each time we add new items, programs which depend on this
 * structure will lose.  This reduces the chances of that happening.
 */
#define	RUSAGE_SELF	0
#define	RUSAGE_CHILDREN	(-1)
#define RUSAGE_BOTH	(-2)		/* sys_wait4() uses this */

struct	rusage {
	struct timeval ru_utime;	/* user time used */
	struct timeval ru_stime;	/* system time used */
	long	ru_maxrss;		/* maximum resident set size */
	long	ru_ixrss;		/* integral shared memory size */
	long	ru_idrss;		/* integral unshared data size */
	long	ru_isrss;		/* integral unshared stack size */
	long	ru_minflt;		/* page reclaims */
	long	ru_majflt;		/* page faults */
	long	ru_nswap;		/* swaps */
	long	ru_inblock;		/* block input operations */
	long	ru_oublock;		/* block output operations */
	long	ru_msgsnd;		/* messages sent */
	long	ru_msgrcv;		/* messages received */
	long	ru_nsignals;		/* signals received */
	long	ru_nvcsw;		/* voluntary context switches */
	long	ru_nivcsw;		/* involuntary " */
};

struct rlimit {
	unsigned long	rlim_cur;
	unsigned long	rlim_max;
};

#define	PRIO_MIN	(-20)
#define	PRIO_MAX	20

#define	PRIO_PROCESS	0
#define	PRIO_PGRP	1
#define	PRIO_USER	2

/*
 * Due to binary compatibility, the actual resource numbers
 * may be different for different linux versions..
 */
#include <asm/resource.h>

#endif
