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
#ifndef _ALPHA_STATFS_H
#define _ALPHA_STATFS_H

#ifndef __KERNEL_STRICT_NAMES

#include <linux/types.h>

typedef __kernel_fsid_t	fsid_t;

#endif

struct statfs {
	int f_type;
	int f_bsize;
	int f_blocks;
	int f_bfree;
	int f_bavail;
	int f_files;
	int f_ffree;
	__kernel_fsid_t f_fsid;
	int f_namelen;
	int f_spare[6];
};

#endif
