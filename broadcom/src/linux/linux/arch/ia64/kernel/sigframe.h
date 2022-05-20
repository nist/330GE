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
struct sigscratch {
	unsigned long scratch_unat;	/* ar.unat for the general registers saved in pt */
	unsigned long pad;
	struct pt_regs pt;
};

struct sigframe {
	/*
	 * Place signal handler args where user-level unwinder can find them easily.
	 * DO NOT MOVE THESE.  They are part of the IA-64 Linux ABI and there is
	 * user-level code that depends on their presence!
	 */
	unsigned long arg0;		/* signum */
	unsigned long arg1;		/* siginfo pointer */
	unsigned long arg2;		/* sigcontext pointer */
	/*
	 * End of architected state.
	 */

	void *handler;			/* pointer to the plabel of the signal handler */
	struct siginfo info;
	struct sigcontext sc;
};
