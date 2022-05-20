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
#ifndef _ASMAXP_PTRACE_H
#define _ASMAXP_PTRACE_H


/*
 * This struct defines the way the registers are stored on the
 * kernel stack during a system call or other kernel entry
 *
 * NOTE! I want to minimize the overhead of system calls, so this
 * struct has as little information as possible.  I does not have
 *
 *  - floating point regs: the kernel doesn't change those
 *  - r9-15: saved by the C compiler
 *
 * This makes "fork()" and "exec()" a bit more complex, but should
 * give us low system call latency.
 */

struct pt_regs {
	unsigned long r0;
	unsigned long r1;
	unsigned long r2;
	unsigned long r3;
	unsigned long r4;
	unsigned long r5;
	unsigned long r6;
	unsigned long r7;
	unsigned long r8;
	unsigned long r19;
	unsigned long r20;
	unsigned long r21;
	unsigned long r22;
	unsigned long r23;
	unsigned long r24;
	unsigned long r25;
	unsigned long r26;
	unsigned long r27;
	unsigned long r28;
	unsigned long hae;
/* JRP - These are the values provided to a0-a2 by PALcode */
	unsigned long trap_a0;
	unsigned long trap_a1;
	unsigned long trap_a2;
/* These are saved by PAL-code: */
	unsigned long ps;
	unsigned long pc;
	unsigned long gp;
	unsigned long r16;
	unsigned long r17;
	unsigned long r18;
};

/*
 * This is the extended stack used by signal handlers and the context
 * switcher: it's pushed after the normal "struct pt_regs".
 */
struct switch_stack {
	unsigned long r9;
	unsigned long r10;
	unsigned long r11;
	unsigned long r12;
	unsigned long r13;
	unsigned long r14;
	unsigned long r15;
	unsigned long r26;
	unsigned long fp[32];	/* fp[31] is fpcr */
};

#ifdef __KERNEL__
#define user_mode(regs) (((regs)->ps & 8) != 0)
#define instruction_pointer(regs) ((regs)->pc)
extern void show_regs(struct pt_regs *);
#endif

#endif
