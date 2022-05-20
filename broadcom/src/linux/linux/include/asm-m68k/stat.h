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
#ifndef _M68K_STAT_H
#define _M68K_STAT_H

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
	unsigned short st_dev;
	unsigned short __pad1;
	unsigned long st_ino;
	unsigned short st_mode;
	unsigned short st_nlink;
	unsigned short st_uid;
	unsigned short st_gid;
	unsigned short st_rdev;
	unsigned short __pad2;
	unsigned long  st_size;
	unsigned long  st_blksize;
	unsigned long  st_blocks;
	unsigned long  st_atime;
	unsigned long  __unused1;
	unsigned long  st_mtime;
	unsigned long  __unused2;
	unsigned long  st_ctime;
	unsigned long  __unused3;
	unsigned long  __unused4;
	unsigned long  __unused5;
};

/* This matches struct stat64 in glibc2.1, hence the absolutely
 * insane amounts of padding around dev_t's.
 */
struct stat64 {
	unsigned char	__pad0[6];
	unsigned short	st_dev;
	unsigned char	__pad1[2];

#define STAT64_HAS_BROKEN_ST_INO	1
	unsigned long	__st_ino;

	unsigned int	st_mode;
	unsigned int	st_nlink;

	unsigned long	st_uid;
	unsigned long	st_gid;

	unsigned char	__pad2[6];
	unsigned short	st_rdev;
	unsigned char	__pad3[2];

	long long	st_size;
	unsigned long	st_blksize;

	unsigned long	__pad4;		/* future possible st_blocks high bits */
	unsigned long	st_blocks;	/* Number 512-byte blocks allocated. */

	unsigned long	st_atime;
	unsigned long	__pad5;

	unsigned long	st_mtime;
	unsigned long	__pad6;

	unsigned long	st_ctime;
	unsigned long	__pad7;		/* will be high 32 bits of ctime someday */

	unsigned long long	st_ino;
};

#endif /* _M68K_STAT_H */
