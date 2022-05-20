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
 * linux/include/linux/hfs_fs_i.h
 *
 * Copyright (C) 1995, 1996  Paul H. Hargrove
 * This file may be distributed under the terms of the GNU General Public License.
 *
 * This file defines the type (struct hfs_inode_info) and the two
 * subordinate types hfs_extent and hfs_file.
 */

#ifndef _LINUX_HFS_FS_I_H
#define _LINUX_HFS_FS_I_H

/*
 * struct hfs_inode_info
 *
 * The HFS-specific part of a Linux (struct inode)
 */
struct hfs_inode_info {
	int				magic;     /* A magic number */

	unsigned long			mmu_private;
	struct hfs_cat_entry		*entry;

	/* For a regular or header file */
	struct hfs_fork 		*fork;
	int				convert;

	/* For a directory */
	ino_t				file_type;
	char				dir_size;

	/* For header files */
	const struct hfs_hdr_layout	*default_layout;
	struct hfs_hdr_layout		*layout;

	/* to deal with localtime ugliness */
	int                             tz_secondswest;

        /* for dentry cleanup */
        void (*d_drop_op)(struct dentry *, const ino_t);
};

#endif
