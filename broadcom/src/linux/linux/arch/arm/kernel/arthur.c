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
 *  linux/arch/arm/kernel/arthur.c
 *
 *  Copyright (C) 1998, 1999, 2000, 2001 Philip Blundell
 *
 * Arthur personality
 */

/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/personality.h>
#include <linux/stddef.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/init.h>

#include <asm/ptrace.h>

/* Arthur doesn't have many signals, and a lot of those that it does
   have don't map easily to any Linux equivalent.  Never mind.  */

#define ARTHUR_SIGABRT		1
#define ARTHUR_SIGFPE		2
#define ARTHUR_SIGILL		3
#define ARTHUR_SIGINT		4
#define ARTHUR_SIGSEGV		5
#define ARTHUR_SIGTERM		6
#define ARTHUR_SIGSTAK		7
#define ARTHUR_SIGUSR1		8
#define ARTHUR_SIGUSR2		9
#define ARTHUR_SIGOSERROR	10

static unsigned long arthur_to_linux_signals[32] = {
	0,	1,	2,	3,	4,	5,	6,	7,
	8,	9,	10,	11,	12,	13,	14,	15,
	16,	17,	18,	19,	20,	21,	22,	23,
	24,	25,	26,	27,	28,	29,	30,	31
};

static unsigned long linux_to_arthur_signals[32] = {
	0,		-1,		ARTHUR_SIGINT,	-1,
       	ARTHUR_SIGILL,	5,		ARTHUR_SIGABRT,	7,
	ARTHUR_SIGFPE,	9,		ARTHUR_SIGUSR1,	ARTHUR_SIGSEGV,	
	ARTHUR_SIGUSR2,	13,		14,		ARTHUR_SIGTERM,
	16,		17,		18,		19,
	20,		21,		22,		23,
	24,		25,		26,		27,
	28,		29,		30,		31
};

static void arthur_lcall7(int nr, struct pt_regs *regs)
{
	struct siginfo info;
	info.si_signo = SIGSWI;
	info.si_errno = nr;
	/* Bounce it to the emulator */
	send_sig_info(SIGSWI, &info, current);
}

static struct exec_domain arthur_exec_domain = {
	"Arthur",	/* name */
	arthur_lcall7,
	PER_RISCOS, PER_RISCOS,
	arthur_to_linux_signals,
	linux_to_arthur_signals,
	THIS_MODULE,
	NULL		/* Nothing after this in the list. */
};

/*
 * We could do with some locking to stop Arthur being removed while
 * processes are using it.
 */

int __init arthur_init(void)
{
	return register_exec_domain(&arthur_exec_domain);
}

void __exit arthur_exit(void)
{
	unregister_exec_domain(&arthur_exec_domain);
}

module_init(arthur_init);
module_exit(arthur_exit);