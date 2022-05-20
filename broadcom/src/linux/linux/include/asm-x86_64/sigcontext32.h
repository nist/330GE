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
#ifndef _SIGCONTEXT32_H
#define _SIGCONTEXT32_H 1

/* signal context for 32bit programs. */

#define X86_FXSR_MAGIC		0x0000

struct _fpreg {
	unsigned short significand[4];
	unsigned short exponent;
};

struct _fpxreg {
	unsigned short significand[4];
	unsigned short exponent;
	unsigned short padding[3];
};

struct _xmmreg {
	__u32	element[4];
};

/* FSAVE frame with extensions */
struct _fpstate_ia32 {
	/* Regular FPU environment */
	__u32 	cw;
	__u32	sw;
	__u32	tag;	/* not compatible to 64bit twd */
	__u32	ipoff;			
	__u32	cssel;
	__u32	dataoff;
	__u32	datasel;
	struct _fpreg	_st[8];
	unsigned short	status;
	unsigned short	magic;		/* 0xffff = regular FPU data only */

	/* FXSR FPU environment */
	__u32	_fxsr_env[6];
	__u32	mxcsr;
	__u32	reserved;
	struct _fpxreg	_fxsr_st[8];
	struct _xmmreg	_xmm[8];	/* It's actually 16 */ 
	__u32	padding[56];
};

struct sigcontext_ia32 {
       unsigned short gs, __gsh;
       unsigned short fs, __fsh;
       unsigned short es, __esh;
       unsigned short ds, __dsh;
       unsigned int edi;
       unsigned int esi;
       unsigned int ebp;
       unsigned int esp;
       unsigned int ebx;
       unsigned int edx;
       unsigned int ecx;
       unsigned int eax;
       unsigned int trapno;
       unsigned int err;
       unsigned int eip;
       unsigned short cs, __csh;
       unsigned int eflags;
       unsigned int esp_at_signal;
       unsigned short ss, __ssh;
       unsigned int fpstate;		/* really (struct _fpstate_ia32 *) */
       unsigned int oldmask;
       unsigned int cr2;
};

#endif
