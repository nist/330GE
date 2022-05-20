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
 * linux/fs/fat/buffer.c
 *
 *
 */

#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/blkdev.h>
#include <linux/msdos_fs.h>
#include <linux/fat_cvf.h>

#  define PRINTK(x)

struct buffer_head *fat_bread(struct super_block *sb, int block)
{
	return MSDOS_SB(sb)->cvf_format->cvf_bread(sb,block);
}
struct buffer_head *fat_getblk(struct super_block *sb, int block)
{
	return MSDOS_SB(sb)->cvf_format->cvf_getblk(sb,block);
}
void fat_brelse (struct super_block *sb, struct buffer_head *bh)
{
	if (bh) 
		MSDOS_SB(sb)->cvf_format->cvf_brelse(sb,bh);
}
void fat_mark_buffer_dirty (
	struct super_block *sb,
	struct buffer_head *bh)
{
	MSDOS_SB(sb)->cvf_format->cvf_mark_buffer_dirty(sb,bh);
}
void fat_set_uptodate (
	struct super_block *sb,
	struct buffer_head *bh,
	int val)
{
	MSDOS_SB(sb)->cvf_format->cvf_set_uptodate(sb,bh,val);
}
int fat_is_uptodate(struct super_block *sb, struct buffer_head *bh)
{
	return MSDOS_SB(sb)->cvf_format->cvf_is_uptodate(sb,bh);
}
void fat_ll_rw_block (
	struct super_block *sb,
	int opr,
	int nbreq,
	struct buffer_head *bh[32])
{
	MSDOS_SB(sb)->cvf_format->cvf_ll_rw_block(sb,opr,nbreq,bh);
}

struct buffer_head *default_fat_bread(struct super_block *sb, int block)
{
	return sb_bread(sb, block);
}

struct buffer_head *default_fat_getblk(struct super_block *sb, int block)
{
	return sb_getblk(sb, block);
}

void default_fat_brelse(struct super_block *sb, struct buffer_head *bh)
{
	brelse (bh);
}

void default_fat_mark_buffer_dirty (
	struct super_block *sb,
	struct buffer_head *bh)
{
	mark_buffer_dirty (bh);
}

void default_fat_set_uptodate (
	struct super_block *sb,
	struct buffer_head *bh,
	int val)
{
	mark_buffer_uptodate(bh, val);
}

int default_fat_is_uptodate (struct super_block *sb, struct buffer_head *bh)
{
	return buffer_uptodate(bh);
}

void default_fat_ll_rw_block (
	struct super_block *sb,
	int opr,
	int nbreq,
	struct buffer_head *bh[32])
{
	ll_rw_block(opr,nbreq,bh);
}
