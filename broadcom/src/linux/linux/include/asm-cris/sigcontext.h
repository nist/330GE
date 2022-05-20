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
/* $Id: sigcontext.h,v 1.1.1.1 2007/01/25 12:52:04 jiahao_jhou Exp $ */

#ifndef _ASM_CRIS_SIGCONTEXT_H
#define _ASM_CRIS_SIGCONTEXT_H

#include <asm/ptrace.h>

/* This struct is saved by setup_frame in signal.c, to keep the current context while
   a signal handler is executed. It's restored by sys_sigreturn.
   
   To keep things simple, we use pt_regs here even though normally you just specify
   the list of regs to save. Then we can use copy_from_user on the entire regs instead
   of a bunch of get_user's as well...

*/

struct sigcontext {
	struct pt_regs regs;  /* needs to be first */
	unsigned long oldmask;
	unsigned long usp;    /* usp before stacking this gunk on it */
};

#endif

