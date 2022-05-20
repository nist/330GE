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
#ifndef _ASM_STAT_H
#define _ASM_STAT_H

#include <linux/types.h>

struct stat {
	dev_t		st_dev;
	long		st_pad1[3];		/* Reserved for network id */
	ino_t		st_ino;
	mode_t		st_mode;
	nlink_t		st_nlink;
	uid_t		st_uid;
	gid_t		st_gid;
	dev_t		st_rdev;
	long		st_pad2[2];
	off_t		st_size;
	long		st_pad3;
	/*
	 * Actually this should be timestruc_t st_atime, st_mtime and st_ctime
	 * but we don't have it under Linux.
	 */
	time_t		st_atime;
	long		reserved0;
	time_t		st_mtime;
	long		reserved1;
	time_t		st_ctime;
	long		reserved2;
	long		st_blksize;
	long		st_blocks;
	long		st_pad4[14];
};

/*
 * This matches struct stat64 in glibc2.1, hence the absolutely insane
 * amounts of padding around dev_t's.  The memory layout is the same as of
 * struct stat of the 64-bit kernel.
 */

struct stat64 {
	unsigned long	st_dev;
	unsigned long	st_pad0[3];	/* Reserved for st_dev expansion  */

	unsigned long long	st_ino;

	mode_t		st_mode;
	nlink_t		st_nlink;

	uid_t		st_uid;
	gid_t		st_gid;

	unsigned long	st_rdev;
	unsigned long	st_pad1[3];	/* Reserved for st_rdev expansion  */

	long long	st_size;

	/*
	 * Actually this should be timestruc_t st_atime, st_mtime and st_ctime
	 * but we don't have it under Linux.
	 */
	time_t		st_atime;
	unsigned long	reserved0;	/* Reserved for st_atime expansion  */

	time_t		st_mtime;
	unsigned long	reserved1;	/* Reserved for st_mtime expansion  */

	time_t		st_ctime;
	unsigned long	reserved2;	/* Reserved for st_ctime expansion  */

	unsigned long	st_blksize;
	unsigned long	st_pad2;

	long long	st_blocks;
};

#endif /* _ASM_STAT_H */
