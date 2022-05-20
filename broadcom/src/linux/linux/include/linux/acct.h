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
 *  BSD Process Accounting for Linux - Definitions
 *
 *  Author: Marco van Wieringen (mvw@planets.elm.net)
 *
 *  This header file contains the definitions needed to implement
 *  BSD-style process accounting. The kernel accounting code and all
 *  user-level programs that try to do something useful with the
 *  process accounting log must include this file.
 *
 *  Copyright (C) 1995 - 1997 Marco van Wieringen - ELM Consultancy B.V.
 *
 */

#ifndef _LINUX_ACCT_H
#define _LINUX_ACCT_H

#include <linux/types.h>

/* 
 *  comp_t is a 16-bit "floating" point number with a 3-bit base 8
 *  exponent and a 13-bit fraction. See linux/kernel/acct.c for the
 *  specific encoding system used.
 */

typedef __u16	comp_t;

/*
 *   accounting file record
 *
 *   This structure contains all of the information written out to the
 *   process accounting file whenever a process exits.
 */

#define ACCT_COMM	16

struct acct
{
	char		ac_flag;		/* Accounting Flags */
/*
 *	No binary format break with 2.0 - but when we hit 32bit uid we'll
 *	have to bite one
 */
	__u16		ac_uid;			/* Accounting Real User ID */
	__u16		ac_gid;			/* Accounting Real Group ID */
	__u16		ac_tty;			/* Accounting Control Terminal */
	__u32		ac_btime;		/* Accounting Process Creation Time */
	comp_t		ac_utime;		/* Accounting User Time */
	comp_t		ac_stime;		/* Accounting System Time */
	comp_t		ac_etime;		/* Accounting Elapsed Time */
	comp_t		ac_mem;			/* Accounting Average Memory Usage */
	comp_t		ac_io;			/* Accounting Chars Transferred */
	comp_t		ac_rw;			/* Accounting Blocks Read or Written */
	comp_t		ac_minflt;		/* Accounting Minor Pagefaults */
	comp_t		ac_majflt;		/* Accounting Major Pagefaults */
	comp_t		ac_swaps;		/* Accounting Number of Swaps */
	__u32		ac_exitcode;		/* Accounting Exitcode */
	char		ac_comm[ACCT_COMM + 1];	/* Accounting Command Name */
	char		ac_pad[10];		/* Accounting Padding Bytes */
};

/*
 *  accounting flags
 */
				/* bit set when the process ... */
#define AFORK		0x01	/* ... executed fork, but did not exec */
#define ASU		0x02	/* ... used super-user privileges */
#define ACOMPAT		0x04	/* ... used compatibility mode (VAX only not used) */
#define ACORE		0x08	/* ... dumped core */
#define AXSIG		0x10	/* ... was killed by a signal */

#define AHZ		100

#ifdef __KERNEL__

#include <linux/config.h>

#ifdef CONFIG_BSD_PROCESS_ACCT
extern void acct_auto_close(kdev_t dev);
extern int acct_process(long exitcode);
#else
#define acct_auto_close(x)	do { } while (0)
#define acct_process(x)		do { } while (0)
#endif

#endif	/* __KERNEL */

#endif	/* _LINUX_ACCT_H */
