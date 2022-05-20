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
#ifndef _PARISC_STAT_H
#define _PARISC_STAT_H

#include <linux/types.h>

struct stat {
	dev_t		st_dev;		/* dev_t is 32 bits on parisc */
	ino_t		st_ino;		/* 32 bits */
	mode_t		st_mode;	/* 16 bits */
	nlink_t		st_nlink;	/* 16 bits */
	unsigned short	st_reserved1;	/* old st_uid */
	unsigned short	st_reserved2;	/* old st_gid */
	dev_t		st_rdev;
	off_t		st_size;
	time_t		st_atime;
	unsigned int	st_spare1;
	time_t		st_mtime;
	unsigned int	st_spare2;
	time_t		st_ctime;
	unsigned int	st_spare3;
	int		st_blksize;
	int		st_blocks;
	unsigned int	__unused1;	/* ACL stuff */
	dev_t		__unused2;	/* network */
	ino_t		__unused3;	/* network */
	unsigned int	__unused4;	/* cnodes */
	unsigned short	__unused5;	/* netsite */
	short		st_fstype;
	dev_t		st_realdev;
	unsigned short	st_basemode;
	unsigned short	st_spareshort;
	uid_t		st_uid;
	gid_t		st_gid;
	unsigned int	st_spare4[3];
};

typedef __kernel_off64_t	off64_t;

struct hpux_stat64 {
	dev_t		st_dev;		/* dev_t is 32 bits on parisc */
	ino_t           st_ino;         /* 32 bits */
	mode_t		st_mode;	/* 16 bits */
	nlink_t		st_nlink;	/* 16 bits */
	unsigned short	st_reserved1;	/* old st_uid */
	unsigned short	st_reserved2;	/* old st_gid */
	dev_t		st_rdev;
	off64_t		st_size;
	time_t		st_atime;
	unsigned int	st_spare1;
	time_t		st_mtime;
	unsigned int	st_spare2;
	time_t		st_ctime;
	unsigned int	st_spare3;
	int		st_blksize;
	__u64		st_blocks;
	unsigned int	__unused1;	/* ACL stuff */
	dev_t		__unused2;	/* network */
	ino_t           __unused3;      /* network */
	unsigned int	__unused4;	/* cnodes */
	unsigned short	__unused5;	/* netsite */
	short		st_fstype;
	dev_t		st_realdev;
	unsigned short	st_basemode;
	unsigned short	st_spareshort;
	uid_t		st_uid;
	gid_t		st_gid;
	unsigned int	st_spare4[3];
};

/* This is the struct that 32-bit userspace applications are expecting.
 * How 64-bit apps are going to be compiled, I have no idea.  But at least
 * this way, we don't have a wrapper in the kernel.
 */
struct stat64 {
	unsigned long long	st_dev;
	unsigned int		__pad1;

	unsigned int		__st_ino;	/* Not actually filled in */
	unsigned int		st_mode;
	unsigned int		st_nlink;
	unsigned int		st_uid;
	unsigned int		st_gid;
	unsigned long long	st_rdev;
	unsigned int		__pad2;
	signed long long	st_size;
	signed int		st_blksize;

	signed long long	st_blocks;
	signed int		st_atime;
	unsigned int		__unused1;
	signed int		st_mtime;
	unsigned int		__unused2;
	signed int		st_ctime;
	unsigned int		__unused3;
	unsigned long long	st_ino;
};

#endif
