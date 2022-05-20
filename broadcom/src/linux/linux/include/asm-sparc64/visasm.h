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
/* $Id: visasm.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $ */
#ifndef _SPARC64_VISASM_H
#define _SPARC64_VISASM_H

/* visasm.h:  FPU saving macros for VIS routines
 *
 * Copyright (C) 1998 Jakub Jelinek (jj@ultra.linux.cz)
 */
 
#include <asm/pstate.h>
#include <asm/ptrace.h>

#define AOFF_task_fpregs	(((ASIZ_task) + (64 - 1)) & ~(64 - 1))
 
/* Clobbers %o5, %g1, %g2, %g3, %g7, %icc, %xcc */

#define VISEntry					\
	rd		%fprs, %o5;			\
	andcc		%o5, (FPRS_FEF|FPRS_DU), %g0;	\
	be,pt		%icc, 297f;			\
	 sethi		%hi(297f), %g7;			\
	ba,pt		%xcc, VISenter;			\
	 or		%g7, %lo(297f), %g7;		\
297:	wr		%g0, FPRS_FEF, %fprs;		\

#define VISExit						\
	wr		%g0, 0, %fprs;

/* Clobbers %o5, %g1, %g2, %g3, %g7, %icc, %xcc.
 * Must preserve %o5 between VISEntryHalf and VISExitHalf */

#define VISEntryHalf					\
	rd		%fprs, %o5;			\
	andcc		%o5, FPRS_FEF, %g0;		\
	be,pt		%icc, 297f;			\
	 sethi		%hi(298f), %g7;			\
	ba,pt		%xcc, VISenterhalf;		\
	 or		%g7, %lo(298f), %g7;		\
	clr		%o5;				\
297:	wr		%o5, FPRS_FEF, %fprs;		\
298:

#define VISExitHalf					\
	wr		%o5, 0, %fprs;

#ifndef __ASSEMBLY__	
extern __inline__ void save_and_clear_fpu(void) {
	__asm__ __volatile__ (
"		rd %%fprs, %%o5\n"
"		andcc %%o5, %0, %%g0\n"
"		be,pt %%icc, 299f\n"
"		 sethi %%hi(298f), %%g7\n"
"		ba VISenter	! Note. This cannot be bp, as it may be too far from VISenter.\n"
"		 or %%g7, %%lo(298f), %%g7\n"
"	298:	wr %%g0, 0, %%fprs\n"
"	299:\n"
"		" : : "i" (FPRS_FEF|FPRS_DU) :
		"o5", "g1", "g2", "g3", "g7", "cc");
}
#endif

#endif /* _SPARC64_ASI_H */
