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
 *  include/asm-x86_64/bugs.h
 *
 *  Copyright (C) 1994  Linus Torvalds
 *  Copyright (C) 2000  SuSE
 *
 * This is included by init/main.c to check for architecture-dependent bugs.
 *
 * Needs:
 *	void check_bugs(void);
 */

#include <linux/config.h>
#include <asm/processor.h>
#include <asm/i387.h>
#include <asm/msr.h>
#include <asm/pda.h>

static inline void check_fpu(void)
{
	extern void __bad_fxsave_alignment(void);
	if (offsetof(struct task_struct, thread.i387.fxsave) & 15)
		__bad_fxsave_alignment();

	/* This should not be here */	
	set_in_cr4(X86_CR4_OSFXSR);
	set_in_cr4(X86_CR4_OSXMMEXCPT);
}

static void __init check_bugs(void)
{
	identify_cpu(&boot_cpu_data);
	check_fpu();
}
