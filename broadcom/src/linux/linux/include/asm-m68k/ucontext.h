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
#ifndef _M68K_UCONTEXT_H
#define _M68K_UCONTEXT_H

typedef int greg_t;
#define NGREG 18
typedef greg_t gregset_t[NGREG];

typedef struct fpregset {
	int f_pcr;
	int f_psr;
	int f_fpiaddr;
	int f_fpregs[8][3];
} fpregset_t;

struct mcontext {
	int version;
	gregset_t gregs;
	fpregset_t fpregs;
};

#define MCONTEXT_VERSION 2

struct ucontext {
	unsigned long	  uc_flags;
	struct ucontext  *uc_link;
	stack_t		  uc_stack;
	struct mcontext	  uc_mcontext;
	unsigned long	  uc_filler[80];
	sigset_t	  uc_sigmask;	/* mask last for extensibility */
};

#endif
