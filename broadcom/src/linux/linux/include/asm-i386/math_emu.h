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
#ifndef _I386_MATH_EMU_H
#define _I386_MATH_EMU_H

#include <asm/sigcontext.h>

int restore_i387_soft(void *s387, struct _fpstate *buf);
int save_i387_soft(void *s387, struct _fpstate * buf);

/* This structure matches the layout of the data saved to the stack
   following a device-not-present interrupt, part of it saved
   automatically by the 80386/80486.
   */
struct info {
	long ___orig_eip;
	long ___ebx;
	long ___ecx;
	long ___edx;
	long ___esi;
	long ___edi;
	long ___ebp;
	long ___eax;
	long ___ds;
	long ___es;
	long ___orig_eax;
	long ___eip;
	long ___cs;
	long ___eflags;
	long ___esp;
	long ___ss;
	long ___vm86_es; /* This and the following only in vm86 mode */
	long ___vm86_ds;
	long ___vm86_fs;
	long ___vm86_gs;
};
#endif
