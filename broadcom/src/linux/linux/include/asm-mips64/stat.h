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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1995, 1999, 2000 Ralf Baechle
 * Copyright (C) 2000 Silicon Graphics, Inc.
 */
#ifndef _ASM_STAT_H
#define _ASM_STAT_H

#include <linux/types.h>

struct stat32 {
	__kernel_dev_t32    st_dev;
	int		    st_pad1[3];
	__kernel_ino_t32    st_ino;
	__kernel_mode_t32   st_mode;
	__kernel_nlink_t32  st_nlink;
	__kernel_uid_t32    st_uid;
	__kernel_gid_t32    st_gid;
	__kernel_dev_t32    st_rdev;
	int		    st_pad2[2];
	__kernel_off_t32    st_size;
	int		    st_pad3;
	__kernel_time_t32   st_atime;
	int		    reserved0;
	__kernel_time_t32   st_mtime;
	int		    reserved1;
	__kernel_time_t32   st_ctime;
	int		    reserved2;
	int		    st_blksize;
	int		    st_blocks;
	int		    st_pad4[14];
};

/* The memory layout is the same as of struct stat64 of the 32-bit kernel.  */
struct stat {
	dev_t		st_dev;
	unsigned int	st_pad0[3];	/* Reserved for st_dev expansion  */

	unsigned long	st_ino;

	mode_t		st_mode;
	nlink_t		st_nlink;

	uid_t		st_uid;
	gid_t		st_gid;

	dev_t		st_rdev;
	unsigned int	st_pad1[3];	/* Reserved for st_rdev expansion  */

	off_t		st_size;

	/*
	 * Actually this should be timestruc_t st_atime, st_mtime and st_ctime
	 * but we don't have it under Linux.
	 */
	unsigned int	st_atime;
	unsigned int	reserved0;	/* Reserved for st_atime expansion  */

	unsigned int	st_mtime;
	unsigned int	reserved1;	/* Reserved for st_mtime expansion  */

	unsigned int	st_ctime;
	unsigned int	reserved2;	/* Reserved for st_ctime expansion  */

	unsigned int	st_blksize;
	unsigned int	st_pad2;

	unsigned long	st_blocks;
};

#endif /* _ASM_STAT_H */
