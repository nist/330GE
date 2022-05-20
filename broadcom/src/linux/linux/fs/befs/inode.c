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
 * inode.c
 * 
 * Copyright (C) 2001 Will Dyson <will_dyson@pobox.com>
 */

#include <linux/fs.h>

#include "befs_fs.h"
#include "endian.h"

/*
	Validates the correctness of the befs inode
	Returns BEFS_OK if the inode should be used, otherwise
	returns BEFS_BAD_INODE
*/
int
befs_check_inode(struct super_block *sb, befs_inode * raw_inode,
		 befs_blocknr_t inode)
{
	__u32 magic1 = fs32_to_cpu(sb, raw_inode->magic1);
	befs_inode_addr ino_num = fsrun_to_cpu(sb, raw_inode->inode_num);
	__u32 flags = fs32_to_cpu(sb, raw_inode->flags);

	/* check magic header. */
	if (magic1 != BEFS_INODE_MAGIC1) {
		befs_error(sb,
			   "Inode has a bad magic header - inode = %lu", inode);
		return BEFS_BAD_INODE;
	}

	/*
	 * Sanity check2: inodes store their own block address. Check it.
	 */
	if (inode != iaddr2blockno(sb, &ino_num)) {
		befs_error(sb, "inode blocknr field disagrees with vfs "
			   "VFS: %lu, Inode %lu",
			   inode, iaddr2blockno(sb, &ino_num));
		return BEFS_BAD_INODE;
	}

	/*
	 * check flag
	 */

	if (!(flags & BEFS_INODE_IN_USE)) {
		befs_error(sb, "inode is not used - inode = %lu", inode);
		return BEFS_BAD_INODE;
	}

	return BEFS_OK;
}
