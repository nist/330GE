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
#ifndef _ASM_IA64_ASMMACRO_H
#define _ASM_IA64_ASMMACRO_H

/*
 * Copyright (C) 2000-2001 Hewlett-Packard Co
 * Copyright (C) 2000-2001 David Mosberger-Tang <davidm@hpl.hp.com>
 */

#define ENTRY(name)				\
	.align 32;				\
	.proc name;				\
name:

#define GLOBAL_ENTRY(name)			\
	.global name;				\
	ENTRY(name)

#define END(name)				\
	.endp name

/*
 * Helper macros to make unwind directives more readable:
 */

/* prologue_gr: */
#define ASM_UNW_PRLG_RP			0x8
#define ASM_UNW_PRLG_PFS		0x4
#define ASM_UNW_PRLG_PSP		0x2
#define ASM_UNW_PRLG_PR			0x1
#define ASM_UNW_PRLG_GRSAVE(ninputs)	(32+(ninputs))

/*
 * Helper macros for accessing user memory.
 */

	.section "__ex_table", "a"		// declare section & section attributes
	.previous

#if __GNUC__ >= 3
# define EX(y,x...)					\
	.xdata4 "__ex_table", @gprel(99f), @gprel(y);	\
  [99:]	x
# define EXCLR(y,x...)					\
	.xdata4 "__ex_table", @gprel(99f), @gprel(y)+4;	\
  [99:]	x
#else
# define EX(y,x...)					\
	.xdata4 "__ex_table", @gprel(99f), @gprel(y);	\
  99:	x
# define EXCLR(y,x...)					\
	.xdata4 "__ex_table", @gprel(99f), @gprel(y)+4;	\
  99:	x
#endif

#endif /* _ASM_IA64_ASMMACRO_H */
