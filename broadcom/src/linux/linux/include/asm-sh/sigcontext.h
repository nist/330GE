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
#ifndef __ASM_SH_SIGCONTEXT_H
#define __ASM_SH_SIGCONTEXT_H

struct sigcontext {
	unsigned long	oldmask;

	/* CPU registers */
	unsigned long sc_regs[16];
	unsigned long sc_pc;
	unsigned long sc_pr;
	unsigned long sc_sr;
	unsigned long sc_gbr;
	unsigned long sc_mach;
	unsigned long sc_macl;

#if defined(__SH4__)
	/* FPU registers */
	unsigned long sc_fpregs[16];
	unsigned long sc_xfpregs[16];
	unsigned int sc_fpscr;
	unsigned int sc_fpul;
	unsigned int sc_ownedfp;
#endif
};

#endif /* __ASM_SH_SIGCONTEXT_H */
