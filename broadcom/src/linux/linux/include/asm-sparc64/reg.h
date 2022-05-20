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
/* $Id: reg.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $
 * linux/asm-sparc64/reg.h
 * Layout of the registers as expected by gdb on the Sparc
 * we should replace the user.h definitions with those in
 * this file, we don't even use the other 
 * -miguel
 *
 * The names of the structures, constants and aliases in this file
 * have the same names as the sunos ones, some programs rely on these
 * names (gdb for example).
 *
 */

#ifndef __SPARC64_REG_H
#define __SPARC64_REG_H

struct regs {
        unsigned long r_g1;
        unsigned long r_g2;
        unsigned long r_g3;
        unsigned long r_g4;
        unsigned long r_g5;
        unsigned long r_g6;
        unsigned long r_g7;
        unsigned long r_o0;
        unsigned long r_o1;
        unsigned long r_o2;
        unsigned long r_o3;
        unsigned long r_o4;
        unsigned long r_o5;
        unsigned long r_o6;
        unsigned long r_o7;
        unsigned long __pad;
        unsigned long r_tstate;
        unsigned long r_tpc;
        unsigned long r_tnpc;
        unsigned int  r_y;
        unsigned int  r_fprs;
};

#define FPU_REGS_TYPE unsigned int
#define FPU_FSR_TYPE unsigned long

struct fp_status {
        unsigned long fpu_fr[32];
        unsigned long Fpu_fsr;
};

struct fpu {
	struct fp_status f_fpstatus;
};

#define fpu_regs  f_fpstatus.fpu_fr
#define fpu_fsr   f_fpstatus.Fpu_fsr

#endif /* __SPARC64_REG_H */
