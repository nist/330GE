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
 * linux/include/linux/lockd/debug.h
 *
 * Debugging stuff.
 *
 * Copyright (C) 1996 Olaf Kirch <okir@monad.swb.de>
 */

#ifndef LINUX_LOCKD_DEBUG_H
#define LINUX_LOCKD_DEBUG_H

#ifdef __KERNEL__

#include <linux/sunrpc/debug.h>

/*
 * Enable lockd debugging.
 * Requires RPC_DEBUG.
 */
#ifdef RPC_DEBUG
# define LOCKD_DEBUG		1
#endif

#undef ifdebug
#if defined(RPC_DEBUG) && defined(LOCKD_DEBUG)
# define ifdebug(flag)		if (nlm_debug & NLMDBG_##flag)
#else
# define ifdebug(flag)		if (0)
#endif

#ifdef LOCKD_DEBUG
char *		  nlm_procname(u32);
#endif

#endif /* __KERNEL__ */

/*
 * Debug flags
 */
#define NLMDBG_SVC		0x0001
#define NLMDBG_CLIENT		0x0002
#define NLMDBG_CLNTLOCK		0x0004
#define NLMDBG_SVCLOCK		0x0008
#define NLMDBG_MONITOR		0x0010
#define NLMDBG_CLNTSUBS		0x0020
#define NLMDBG_SVCSUBS		0x0040
#define NLMDBG_HOSTCACHE	0x0080
#define NLMDBG_XDR		0x0100
#define NLMDBG_ALL		0x7fff


#endif /* LINUX_LOCKD_DEBUG_H */
