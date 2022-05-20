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
/* fpumacro.h: FPU related macros.
 *
 * Copyright (C) 1997 Jakub Jelinek (jj@sunsite.mff.cuni.cz)
 * Copyright (C) 1997 David S. Miller (davem@caip.rutgers.edu)
 */

#ifndef _SPARC64_FPUMACRO_H
#define _SPARC64_FPUMACRO_H

#include <asm/asi.h>
#include <asm/visasm.h>

struct fpustate {
	u32	regs[64];
};

#define FPUSTATE (struct fpustate *)(((unsigned long)current) + AOFF_task_fpregs)

extern __inline__ unsigned long fprs_read(void)
{
	unsigned long retval;

	__asm__ __volatile__("rd %%fprs, %0" : "=r" (retval));

	return retval;
}

extern __inline__ void fprs_write(unsigned long val)
{
	__asm__ __volatile__("wr %0, 0x0, %%fprs" : : "r" (val));
}

#endif /* !(_SPARC64_FPUMACRO_H) */
