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
 * IRIX prctl interface
 *
 * The IRIX kernel maps a page at PRDA_ADDRESS with the
 * contents of prda and fills it the bits on prda_sys.
 */

#ifndef __PRCTL_H__
#define __PRCTL_H__

#define PRDA_ADDRESS 0x200000L
#define PRDA ((struct prda *) PRDA_ADDRESS)

struct prda_sys {
	pid_t t_pid;
        u32   t_hint;
        u32   t_dlactseq;
        u32   t_fpflags;
        u32   t_prid;		/* processor type, $prid CP0 register */
        u32   t_dlendseq;
        u64   t_unused1[5];
        pid_t t_rpid;
        s32   t_resched;
        u32   t_unused[8];
        u32   t_cpu;		/* current/last cpu */

	u32   t_flags;		/* if true, then the sigprocmask is in userspace */
	u32   t_sigprocmask [1]; /* the sigprocmask */
};

struct prda {
	char fill [0xe00];
	struct prda_sys prda_sys;
};

#define t_sys           prda_sys

ptrdiff_t prctl (int op, int v1, int v2);

#endif
