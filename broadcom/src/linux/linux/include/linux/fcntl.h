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
#ifndef _LINUX_FCNTL_H
#define _LINUX_FCNTL_H

#include <asm/fcntl.h>

#define F_SETLEASE	(F_LINUX_SPECIFIC_BASE+0)
#define F_GETLEASE	(F_LINUX_SPECIFIC_BASE+1)

/*
 * Request nofications on a directory.
 * See below for events that may be notified.
 */
#define F_NOTIFY	(F_LINUX_SPECIFIC_BASE+2)

/*
 * Types of directory notifications that may be requested.
 */
#define DN_ACCESS	0x00000001	/* File accessed */
#define DN_MODIFY	0x00000002	/* File modified */
#define DN_CREATE	0x00000004	/* File created */
#define DN_DELETE	0x00000008	/* File removed */
#define DN_RENAME	0x00000010	/* File renamed */
#define DN_ATTRIB	0x00000020	/* File changed attibutes */
#define DN_MULTISHOT	0x80000000	/* Don't remove notifier */

#ifdef __KERNEL__

#if BITS_PER_LONG == 32
#define IS_GETLK32(cmd)		((cmd) == F_GETLK)
#define IS_SETLK32(cmd)		((cmd) == F_SETLK)
#define IS_SETLKW32(cmd)	((cmd) == F_SETLKW)
#define IS_GETLK64(cmd)		((cmd) == F_GETLK64)
#define IS_SETLK64(cmd)		((cmd) == F_SETLK64)
#define IS_SETLKW64(cmd)	((cmd) == F_SETLKW64)
#else
#define IS_GETLK32(cmd)		(0)
#define IS_SETLK32(cmd)		(0)
#define IS_SETLKW32(cmd)	(0)
#define IS_GETLK64(cmd)		((cmd) == F_GETLK)
#define IS_SETLK64(cmd)		((cmd) == F_SETLK)
#define IS_SETLKW64(cmd)	((cmd) == F_SETLKW)
#endif /* BITS_PER_LONG == 32 */

#define IS_GETLK(cmd)	(IS_GETLK32(cmd)  || IS_GETLK64(cmd))
#define IS_SETLK(cmd)	(IS_SETLK32(cmd)  || IS_SETLK64(cmd))
#define IS_SETLKW(cmd)	(IS_SETLKW32(cmd) || IS_SETLKW64(cmd))

#endif /* __KERNEL__ */

#endif
