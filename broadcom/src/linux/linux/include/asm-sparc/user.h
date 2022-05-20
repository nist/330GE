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
/* $Id: user.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $
 * asm-sparc/user.h: Core file definitions for the Sparc.
 *
 * Keep in sync with reg.h.  Actually, we could get rid of this
 * one, since we won't a.out core dump that much anyways - miguel.
 * Copyright (C) 1995 (davem@caip.rutgers.edu)
 */
#ifndef _SPARC_USER_H
#define _SPARC_USER_H

#include <asm/a.out.h>
struct sunos_regs {
	unsigned long psr, pc, npc, y;
	unsigned long regs[15];
};

struct sunos_fpqueue {
	unsigned long *addr;
	unsigned long inst;
};

struct sunos_fp {
	union {
		unsigned long regs[32];
		double reg_dbls[16];
	} fregs;
	unsigned long fsr;
	unsigned long flags;
	unsigned long extra;
	unsigned long fpq_count;
	struct sunos_fpqueue fpq[16];
};

struct sunos_fpu {
	struct sunos_fp fpstatus;
};

/* The SunOS core file header layout. */
struct user {
	unsigned long magic;
	unsigned long len;
	struct sunos_regs regs;
	struct exec uexec;
	int           signal;
	size_t        u_tsize; /* all of these in bytes! */
	size_t        u_dsize;
	size_t        u_ssize;
	char          u_comm[17];
	struct sunos_fpu fpu;
	unsigned long sigcode;   /* Special sigcontext subcode, if any */
};

#define NBPG                   0x2000
#define UPAGES                 1
#define HOST_TEXT_START_ADDR   (u.start_code)
#define HOST_DATA_START_ADDR   (u.uexec.a_data)
#define HOST_STACK_END_ADDR    (- u.u_ssize * NBPG)
#define SUNOS_CORE_MAGIC       0x080456

#endif /* !(_SPARC_USER_H) */
