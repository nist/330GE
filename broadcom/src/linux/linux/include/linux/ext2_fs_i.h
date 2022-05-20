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
 *  linux/include/linux/ext2_fs_i.h
 *
 * Copyright (C) 1992, 1993, 1994, 1995
 * Remy Card (card@masi.ibp.fr)
 * Laboratoire MASI - Institut Blaise Pascal
 * Universite Pierre et Marie Curie (Paris VI)
 *
 *  from
 *
 *  linux/include/linux/minix_fs_i.h
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

#ifndef _LINUX_EXT2_FS_I
#define _LINUX_EXT2_FS_I

/*
 * second extended file system inode data in memory
 */
struct ext2_inode_info {
	__u32	i_data[15];
	__u32	i_flags;
	__u32	i_faddr;
	__u8	i_frag_no;
	__u8	i_frag_size;
	__u32	i_file_acl;
	__u32	i_dir_acl;
	__u32	i_dtime;
	__u32	i_block_group;
	__u32	i_next_alloc_block;
	__u32	i_next_alloc_goal;
	__u32	i_prealloc_block;
	__u32	i_prealloc_count;
	__u32	i_dir_start_lookup;
	int	i_new_inode:1;	/* Is a freshly allocated inode */
};

#endif	/* _LINUX_EXT2_FS_I */
