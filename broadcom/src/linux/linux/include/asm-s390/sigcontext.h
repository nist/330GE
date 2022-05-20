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
 *  include/asm-s390/sigcontext.h
 *
 *  S390 version
 *    Copyright (C) 1999,2000 IBM Deutschland Entwicklung GmbH, IBM Corporation
 */

#ifndef _ASM_S390_SIGCONTEXT_H
#define _ASM_S390_SIGCONTEXT_H

#define __NUM_GPRS 16
#define __NUM_FPRS 16
#define __NUM_ACRS 16

/*
  Has to be at least _NSIG_WORDS from asm/signal.h
 */
#define _SIGCONTEXT_NSIG      64
#define _SIGCONTEXT_NSIG_BPW  32
/* Size of stack frame allocated when calling signal handler. */
#define __SIGNAL_FRAMESIZE	96
#define _SIGCONTEXT_NSIG_WORDS	(_SIGCONTEXT_NSIG / _SIGCONTEXT_NSIG_BPW)
#define _SIGMASK_COPY_SIZE	(sizeof(unsigned long)*_SIGCONTEXT_NSIG_WORDS)

typedef struct 
{
        unsigned long mask;
        unsigned long addr;
} __attribute__ ((aligned(8))) _psw_t;

typedef struct
{
	_psw_t psw;
	unsigned long gprs[__NUM_GPRS];
	unsigned int  acrs[__NUM_ACRS];
} _s390_regs_common;

typedef struct
{
	unsigned int fpc;
	double   fprs[__NUM_FPRS];
} _s390_fp_regs;

typedef struct
{
	_s390_regs_common regs;
	_s390_fp_regs     fpregs;
} _sigregs;

struct sigcontext
{
	unsigned long	oldmask[_SIGCONTEXT_NSIG_WORDS];
	_sigregs        *sregs;
};


#endif
