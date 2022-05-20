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
 * Copyright (C) 1995, 1999 by Ralf Baechle
 */
#ifndef _ASM_STATFS_H
#define _ASM_STATFS_H

#include <linux/posix_types.h>

#ifndef __KERNEL_STRICT_NAMES

#include <linux/types.h>

typedef __kernel_fsid_t        fsid_t;

#endif

struct statfs {
	long		f_type;
#define f_fstyp f_type
	long		f_bsize;
	long		f_frsize;	/* Fragment size - unsupported */
	long		f_blocks;
	long		f_bfree;
	long		f_files;
	long		f_ffree;

	/* Linux specials */
	long	f_bavail;
	__kernel_fsid_t	f_fsid;
	long		f_namelen;
	long		f_spare[6];
};

#endif /* _ASM_STATFS_H */
