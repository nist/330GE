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
 * file.c
 *
 * Copyright (c) 1999 Al Smith
 *
 * Portions derived from work (c) 1995,1996 Christian Vogelgsang.
 */

#include <linux/efs_fs.h>

int efs_get_block(struct inode *inode, long iblock,
		  struct buffer_head *bh_result, int create)
{
	int error = -EROFS;
	long phys;

	if (create)
		return error;
	if (iblock >= inode->i_blocks) {
#ifdef DEBUG
		/*
		 * i have no idea why this happens as often as it does
		 */
		printk(KERN_WARNING "EFS: bmap(): block %d >= %ld (filesize %ld)\n",
			block,
			inode->i_blocks,
			inode->i_size);
#endif
		return 0;
	}
	phys = efs_map_block(inode, iblock);
	if (phys) {
		bh_result->b_dev = inode->i_dev;
		bh_result->b_blocknr = phys;
		bh_result->b_state |= (1UL << BH_Mapped);
	}
	return 0;
}

int efs_bmap(struct inode *inode, efs_block_t block) {

	if (block < 0) {
		printk(KERN_WARNING "EFS: bmap(): block < 0\n");
		return 0;
	}

	/* are we about to read past the end of a file ? */
	if (!(block < inode->i_blocks)) {
#ifdef DEBUG
		/*
		 * i have no idea why this happens as often as it does
		 */
		printk(KERN_WARNING "EFS: bmap(): block %d >= %ld (filesize %ld)\n",
			block,
			inode->i_blocks,
			inode->i_size);
#endif
		return 0;
	}

	return efs_map_block(inode, block);
}
