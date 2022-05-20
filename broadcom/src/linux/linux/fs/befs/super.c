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
 * super.c
 *
 * Copyright (C) 2001-2002 Will Dyson <will_dyson@pobox.com>
 *
 * Licensed under the GNU GPL. See the file COPYING for details.
 *
 */

#include <linux/fs.h>

#include "befs_fs.h"
#include "endian.h"

/**
 * load_befs_sb -- Read from disk and properly byteswap all the fields
 * of the befs superblock
 *
 *
 *
 *
 */
int
befs_load_sb(struct super_block *sb, befs_super_block * disk_sb)
{
	befs_sb_info *befs_sb = BEFS_SB(sb);

	/* byte_order is special, no byte-swap */
	befs_sb->byte_order = disk_sb->fs_byte_order;

	befs_sb->magic1 = fs32_to_cpu(sb, disk_sb->magic1);
	befs_sb->magic2 = fs32_to_cpu(sb, disk_sb->magic2);
	befs_sb->magic3 = fs32_to_cpu(sb, disk_sb->magic3);
	befs_sb->block_size = fs32_to_cpu(sb, disk_sb->block_size);
	befs_sb->block_shift = fs32_to_cpu(sb, disk_sb->block_shift);
	befs_sb->num_blocks = fs64_to_cpu(sb, disk_sb->num_blocks);
	befs_sb->used_blocks = fs64_to_cpu(sb, disk_sb->used_blocks);
	befs_sb->inode_size = fs32_to_cpu(sb, disk_sb->inode_size);

	befs_sb->blocks_per_ag = fs32_to_cpu(sb, disk_sb->blocks_per_ag);
	befs_sb->ag_shift = fs32_to_cpu(sb, disk_sb->ag_shift);
	befs_sb->num_ags = fs32_to_cpu(sb, disk_sb->num_ags);

	befs_sb->log_blocks = fsrun_to_cpu(sb, disk_sb->log_blocks);
	befs_sb->log_start = fs64_to_cpu(sb, disk_sb->log_start);
	befs_sb->log_end = fs64_to_cpu(sb, disk_sb->log_end);

	befs_sb->root_dir = fsrun_to_cpu(sb, disk_sb->root_dir);
	befs_sb->indices = fsrun_to_cpu(sb, disk_sb->indices);
	befs_sb->nls = NULL;

	return BEFS_OK;
}

int
befs_check_sb(struct super_block *sb)
{
	befs_sb_info *befs_sb = BEFS_SB(sb);

	/* Check magic headers of super block */
	if ((befs_sb->magic1 != BEFS_SUPER_MAGIC1)
	    || (befs_sb->magic2 != BEFS_SUPER_MAGIC2)
	    || (befs_sb->magic3 != BEFS_SUPER_MAGIC3)) {
		befs_error(sb, "invalid magic header");
		return BEFS_ERR;
	}

	/*
	 * Check blocksize of BEFS.
	 *
	 * Blocksize of BEFS is 1024, 2048, 4096 or 8192.
	 */

	if ((befs_sb->block_size != 1024)
	    && (befs_sb->block_size != 2048)
	    && (befs_sb->block_size != 4096)
	    && (befs_sb->block_size != 8192)) {
		befs_error(sb, "invalid blocksize: %u", befs_sb->block_size);
		return BEFS_ERR;
	}

	if (befs_sb->block_size > PAGE_SIZE) {
		befs_error(sb, "blocksize(%u) cannot be larger"
			   "than system pagesize(%lu)", befs_sb->block_size,
			   PAGE_SIZE);
		return BEFS_ERR;
	}

	/*
	   * block_shift and block_size encode the same information
	   * in different ways as a consistency check.
	 */

	if ((1 << befs_sb->block_shift) != befs_sb->block_size) {
		befs_error(sb, "block_shift disagrees with block_size. "
			   "Corruption likely.");
		return BEFS_ERR;
	}

	if (befs_sb->log_start != befs_sb->log_end) {
		befs_error(sb, "Filesystem not clean! There are blocks in the "
			   "journal. You must boot into BeOS and mount this volume "
			   "to make it clean.");
		return BEFS_ERR;
	}

	return BEFS_OK;
}
