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
#ifndef _ASM_IA64_STAT_H
#define _ASM_IA64_STAT_H

/*
 * Copyright (C) 1998, 1999 Hewlett-Packard Co
 * Copyright (C) 1998, 1999 David Mosberger-Tang <davidm@hpl.hp.com>
 */

struct stat {
	unsigned long	st_dev;
	unsigned long	st_ino;
	unsigned long	st_nlink;
	unsigned int	st_mode;
	unsigned int	st_uid;
	unsigned int	st_gid;
	unsigned int	__pad0;
	unsigned long	st_rdev;
	unsigned long	st_size;
	unsigned long	st_atime;
	unsigned long	__reserved0;	/* reserved for atime.nanoseconds */
	unsigned long	st_mtime;
	unsigned long	__reserved1;	/* reserved for mtime.nanoseconds */
	unsigned long	st_ctime;
	unsigned long	__reserved2;	/* reserved for ctime.nanoseconds */
	unsigned long	st_blksize;
	long		st_blocks;
	unsigned long	__unused[3];
};

struct ia64_oldstat {
	unsigned int	st_dev;
	unsigned int	st_ino;
	unsigned int	st_mode;
	unsigned int	st_nlink;
	unsigned int	st_uid;
	unsigned int	st_gid;
	unsigned int	st_rdev;
	unsigned int	__pad1;
	unsigned long	st_size;
	unsigned long	st_atime;
	unsigned long	st_mtime;
	unsigned long	st_ctime;
	unsigned int	st_blksize;
	int		st_blocks;
	unsigned int	__unused1;
	unsigned int	__unused2;
};

#endif /* _ASM_IA64_STAT_H */
