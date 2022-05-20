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
#include <linux/fs.h>
#include <linux/minix_fs.h>
#include <linux/locks.h>
#include <linux/smp_lock.h>

enum {DEPTH = 3, DIRECT = 7};	/* Only double indirect */

typedef u16 block_t;	/* 16 bit, host order */

static inline unsigned long block_to_cpu(block_t n)
{
	return n;
}

static inline block_t cpu_to_block(unsigned long n)
{
	return n;
}

static inline block_t *i_data(struct inode *inode)
{
	return (block_t *)inode->u.minix_i.u.i1_data;
}

static int block_to_path(struct inode * inode, long block, int offsets[DEPTH])
{
	int n = 0;

	if (block < 0) {
		printk("minix_bmap: block<0");
	} else if (block >= (inode->i_sb->u.minix_sb.s_max_size/BLOCK_SIZE)) {
		printk("minix_bmap: block>big");
	} else if (block < 7) {
		offsets[n++] = block;
	} else if ((block -= 7) < 512) {
		offsets[n++] = 7;
		offsets[n++] = block;
	} else {
		block -= 512;
		offsets[n++] = 8;
		offsets[n++] = block>>9;
		offsets[n++] = block & 511;
	}
	return n;
}

#include "itree_common.c"

int V1_minix_get_block(struct inode * inode, long block,
			struct buffer_head *bh_result, int create)
{
	return get_block(inode, block, bh_result, create);
}

void V1_minix_truncate(struct inode * inode)
{
	truncate(inode);
}
