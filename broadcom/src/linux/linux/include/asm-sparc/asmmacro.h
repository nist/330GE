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
/* asmmacro.h: Assembler macros.
 *
 * Copyright (C) 1996 David S. Miller (davem@caipfs.rutgers.edu)
 */

#ifndef _SPARC_ASMMACRO_H
#define _SPARC_ASMMACRO_H

#include <linux/config.h>
#include <asm/btfixup.h>
#include <asm/asi.h>

#define GET_PROCESSOR4M_ID(reg) \
	rd	%tbr, %reg; \
	srl	%reg, 12, %reg; \
	and	%reg, 3, %reg;

#define GET_PROCESSOR4D_ID(reg) \
	lda	[%g0] ASI_M_VIKING_TMP1, %reg;

/* Blackbox */
#define GET_PROCESSOR_ID(reg) \
	sethi	%hi(___b_smp_processor_id), %reg; \
	sethi	%hi(boot_cpu_id), %reg; \
	ldub	[%reg + %lo(boot_cpu_id)], %reg;

#define GET_PROCESSOR_MID(reg, tmp) \
	rd	%tbr, %reg; \
	sethi	%hi(C_LABEL(mid_xlate)), %tmp; \
	srl	%reg, 12, %reg; \
	or	%tmp, %lo(C_LABEL(mid_xlate)), %tmp; \
	and	%reg, 3, %reg; \
	ldub	[%tmp + %reg], %reg;

#define GET_PROCESSOR_OFFSET(reg, tmp) \
	GET_PROCESSOR_ID(reg) \
	sethi	%hi(C_LABEL(cpu_offset)), %tmp; \
	sll	%reg, 2, %reg; \
	or	%tmp, %lo(C_LABEL(cpu_offset)), %tmp; \
	ld	[%tmp + %reg], %reg;

/* All trap entry points _must_ begin with this macro or else you
 * lose.  It makes sure the kernel has a proper window so that
 * c-code can be called.
 */
#define SAVE_ALL_HEAD \
	sethi	%hi(trap_setup), %l4; \
	jmpl	%l4 + %lo(trap_setup), %l6;
#define SAVE_ALL \
	SAVE_ALL_HEAD \
	 nop;

/* All traps low-level code here must end with this macro. */
#define RESTORE_ALL b ret_trap_entry; clr %l6;

/* sun4 probably wants half word accesses to ASI_SEGMAP, while sun4c+
   likes byte accesses. These are to avoid ifdef mania. */

#ifdef CONFIG_SUN4
#define lduXa	lduha
#define stXa	stha
#else
#define lduXa	lduba
#define stXa	stba
#endif

#endif /* !(_SPARC_ASMMACRO_H) */
