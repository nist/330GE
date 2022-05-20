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
/* $Id: stat.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $ */
#ifndef _SPARC_STAT_H
#define _SPARC_STAT_H

#include <linux/types.h>

struct __old_kernel_stat {
	unsigned short st_dev;
	unsigned short st_ino;
	unsigned short st_mode;
	unsigned short st_nlink;
	unsigned short st_uid;
	unsigned short st_gid;
	unsigned short st_rdev;
	unsigned long  st_size;
	unsigned long  st_atime;
	unsigned long  st_mtime;
	unsigned long  st_ctime;
};

struct stat {
	unsigned short	st_dev;
	unsigned long	st_ino;
	unsigned short	st_mode;
	short		st_nlink;
	unsigned short	st_uid;
	unsigned short	st_gid;
	unsigned short	st_rdev;
	long		st_size;
	long		st_atime;
	unsigned long	__unused1;
	long		st_mtime;
	unsigned long	__unused2;
	long		st_ctime;
	unsigned long	__unused3;
	long		st_blksize;
	long		st_blocks;
	unsigned long	__unused4[2];
};

struct stat64 {
	unsigned char	__pad0[6];
	unsigned short	st_dev;

	unsigned long long	st_ino;

	unsigned int	st_mode;
	unsigned int	st_nlink;

	unsigned int	st_uid;
	unsigned int	st_gid;

	unsigned char	__pad2[6];
	unsigned short	st_rdev;

	unsigned char	__pad3[8];

	long long	st_size;
	unsigned int	st_blksize;

	unsigned char	__pad4[8];
	unsigned int	st_blocks;

	unsigned int	st_atime;
	unsigned int	__unused1;

	unsigned int	st_mtime;
	unsigned int	__unused2;

	unsigned int	st_ctime;
	unsigned int	__unused3;

	unsigned int	__unused4;
	unsigned int	__unused5;
};

#endif
