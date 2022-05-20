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
#ifndef _LINUX_SYS_H
#define _LINUX_SYS_H

/*
 * system call entry points ... but not all are defined
 */
#define NR_syscalls 256

/*
 * These are system calls that will be removed at some time
 * due to newer versions existing..
 * (please be careful - ibcs2 may need some of these).
 */
#ifdef notdef
#define _sys_waitpid	_sys_old_syscall	/* _sys_wait4 */
#define _sys_olduname	_sys_old_syscall	/* _sys_newuname */
#define _sys_uname	_sys_old_syscall	/* _sys_newuname */
#define _sys_stat	_sys_old_syscall	/* _sys_newstat */
#define _sys_fstat	_sys_old_syscall	/* _sys_newfstat */
#define _sys_lstat	_sys_old_syscall	/* _sys_newlstat */
#define _sys_signal	_sys_old_syscall	/* _sys_sigaction */
#define _sys_sgetmask	_sys_old_syscall	/* _sys_sigprocmask */
#define _sys_ssetmask	_sys_old_syscall	/* _sys_sigprocmask */
#endif

/*
 * These are system calls that haven't been implemented yet
 * but have an entry in the table for future expansion..
 */
#endif
