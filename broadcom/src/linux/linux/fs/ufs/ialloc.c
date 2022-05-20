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
 *  linux/fs/ufs/ialloc.c
 *
 * Copyright (c) 1998
 * Daniel Pirkl <daniel.pirkl@email.cz>
 * Charles University, Faculty of Mathematics and Physics
 *
 *  from
 *
 *  linux/fs/ext2/ialloc.c
 *
 * Copyright (C) 1992, 1993, 1994, 1995
 * Remy Card (card@masi.ibp.fr)
 * Laboratoire MASI - Institut Blaise Pascal
 * Universite Pierre et Marie Curie (Paris VI)
 *
 *  BSD ufs-inspired inode and directory allocation by 
 *  Stephen Tweedie (sct@dcs.ed.ac.uk), 1993
 *  Big-endian to little-endian byte-swapping/bitmaps by
 *        David S. Miller (davem@caip.rutgers.edu), 1995
 */

#include <linux/fs.h>
#include <linux/ufs_fs.h>
#include <linux/sched.h>
#include <linux/stat.h>
#include <linux/string.h>
#include <linux/locks.h>
#include <linux/quotaops.h>
#include <asm/bitops.h>
#include <asm/byteorder.h>

#include "swab.h"
#include "util.h"

#undef UFS_IALLOC_DEBUG

#ifdef UFS_IALLOC_DEBUG
#define UFSD(x) printk("(%s, %d), %s: ", __FILE__, __LINE__, __FUNCTION__); printk x;
#else
#define UFSD(x)
#endif

/*
 * NOTE! When we get the inode, we're the only people
 * that have access to it, and as such there are no
 * race conditions we have to worry about. The inode
 * is not on the hash-lists, and it cannot be reached
 * through the filesystem because the directory entry
 * has been deleted earlier.
 *
 * HOWEVER: we must make sure that we get no aliases,
 * which means that we have to call "clear_inode()"
 * _before_ we mark the inode not in use in the inode
 * bitmaps. Otherwise a newly created file might use
 * the same inode number (not actually the same pointer
 * though), and then we'd have two inodes sharing the
 * same inode number and space on the harddisk.
 */
void ufs_free_inode (struct inode * inode)
{
	struct super_block * sb;
	struct ufs_sb_private_info * uspi;
	struct ufs_super_block_first * usb1;
	struct ufs_cg_private_info * ucpi;
	struct ufs_cylinder_group * ucg;
	int is_directory;
	unsigned ino, cg, bit;
	
	UFSD(("ENTER, ino %lu\n", inode->i_ino))

	sb = inode->i_sb;
	uspi = sb->u.ufs_sb.s_uspi;
	usb1 = ubh_get_usb_first(USPI_UBH);
	
	ino = inode->i_ino;

	lock_super (sb);

	if (!((ino > 1) && (ino < (uspi->s_ncg * uspi->s_ipg )))) {
		ufs_warning(sb, "ufs_free_inode", "reserved inode or nonexistent inode %u\n", ino);
		unlock_super (sb);
		return;
	}
	
	cg = ufs_inotocg (ino);
	bit = ufs_inotocgoff (ino);
	ucpi = ufs_load_cylinder (sb, cg);
	if (!ucpi) {
		unlock_super (sb);
		return;
	}
	ucg = ubh_get_ucg(UCPI_UBH);
	if (!ufs_cg_chkmagic(sb, ucg))
		ufs_panic (sb, "ufs_free_fragments", "internal error, bad cg magic number");

	ucg->cg_time = cpu_to_fs32(sb, CURRENT_TIME);

	is_directory = S_ISDIR(inode->i_mode);

	DQUOT_FREE_INODE(inode);
	DQUOT_DROP(inode);

	clear_inode (inode);

	if (ubh_isclr (UCPI_UBH, ucpi->c_iusedoff, bit))
		ufs_error(sb, "ufs_free_inode", "bit already cleared for inode %u", ino);
	else {
		ubh_clrbit (UCPI_UBH, ucpi->c_iusedoff, bit);
		if (ino < ucpi->c_irotor)
			ucpi->c_irotor = ino;
		fs32_add(sb, &ucg->cg_cs.cs_nifree, 1);
		fs32_add(sb, &usb1->fs_cstotal.cs_nifree, 1);
		fs32_add(sb, &sb->fs_cs(cg).cs_nifree, 1);

		if (is_directory) {
			fs32_sub(sb, &ucg->cg_cs.cs_ndir, 1);
			fs32_sub(sb, &usb1->fs_cstotal.cs_ndir, 1);
			fs32_sub(sb, &sb->fs_cs(cg).cs_ndir, 1);
		}
	}

	ubh_mark_buffer_dirty (USPI_UBH);
	ubh_mark_buffer_dirty (UCPI_UBH);
	if (sb->s_flags & MS_SYNCHRONOUS) {
		ubh_ll_rw_block (WRITE, 1, (struct ufs_buffer_head **) &ucpi);
		ubh_wait_on_buffer (UCPI_UBH);
	}
	
	sb->s_dirt = 1;
	unlock_super (sb);
	UFSD(("EXIT\n"))
}

/*
 * There are two policies for allocating an inode.  If the new inode is
 * a directory, then a forward search is made for a block group with both
 * free space and a low directory-to-inode ratio; if that fails, then of
 * the groups with above-average free space, that group with the fewest
 * directories already is chosen.
 *
 * For other inodes, search forward from the parent directory's block
 * group to find a free inode.
 */
struct inode * ufs_new_inode (const struct inode * dir,	int mode)
{
	struct super_block * sb;
	struct ufs_sb_private_info * uspi;
	struct ufs_super_block_first * usb1;
	struct ufs_cg_private_info * ucpi;
	struct ufs_cylinder_group * ucg;
	struct inode * inode;
	unsigned cg, bit, i, j, start;

	UFSD(("ENTER\n"))
	
	/* Cannot create files in a deleted directory */
	if (!dir || !dir->i_nlink)
		return ERR_PTR(-EPERM);
	sb = dir->i_sb;
	inode = new_inode(sb);
	if (!inode)
		return ERR_PTR(-ENOMEM);
	uspi = sb->u.ufs_sb.s_uspi;
	usb1 = ubh_get_usb_first(USPI_UBH);

	lock_super (sb);

	/*
	 * Try to place the inode in its parent directory
	 */
	i = ufs_inotocg(dir->i_ino);
	if (sb->fs_cs(i).cs_nifree) {
		cg = i;
		goto cg_found;
	}

	/*
	 * Use a quadratic hash to find a group with a free inode
	 */
	for ( j = 1; j < uspi->s_ncg; j <<= 1 ) {
		i += j;
		if (i >= uspi->s_ncg)
			i -= uspi->s_ncg;
		if (sb->fs_cs(i).cs_nifree) {
			cg = i;
			goto cg_found;
		}
	}

	/*
	 * That failed: try linear search for a free inode
	 */
	i = ufs_inotocg(dir->i_ino) + 1;
	for (j = 2; j < uspi->s_ncg; j++) {
		i++;
		if (i >= uspi->s_ncg)
			i = 0;
		if (sb->fs_cs(i).cs_nifree) {
			cg = i;
			goto cg_found;
		}
	}
	
	goto failed;

cg_found:
	ucpi = ufs_load_cylinder (sb, cg);
	if (!ucpi)
		goto failed;
	ucg = ubh_get_ucg(UCPI_UBH);
	if (!ufs_cg_chkmagic(sb, ucg)) 
		ufs_panic (sb, "ufs_new_inode", "internal error, bad cg magic number");

	start = ucpi->c_irotor;
	bit = ubh_find_next_zero_bit (UCPI_UBH, ucpi->c_iusedoff, uspi->s_ipg, start);
	if (!(bit < uspi->s_ipg)) {
		bit = ubh_find_first_zero_bit (UCPI_UBH, ucpi->c_iusedoff, start);
		if (!(bit < start)) {
			ufs_error (sb, "ufs_new_inode",
			    "cylinder group %u corrupted - error in inode bitmap\n", cg);
			goto failed;
		}
	}
	UFSD(("start = %u, bit = %u, ipg = %u\n", start, bit, uspi->s_ipg))
	if (ubh_isclr (UCPI_UBH, ucpi->c_iusedoff, bit))
		ubh_setbit (UCPI_UBH, ucpi->c_iusedoff, bit);
	else {
		ufs_panic (sb, "ufs_new_inode", "internal error");
		goto failed;
	}
	
	fs32_sub(sb, &ucg->cg_cs.cs_nifree, 1);
	fs32_sub(sb, &usb1->fs_cstotal.cs_nifree, 1);
	fs32_sub(sb, &sb->fs_cs(cg).cs_nifree, 1);
	
	if (S_ISDIR(mode)) {
		fs32_add(sb, &ucg->cg_cs.cs_ndir, 1);
		fs32_add(sb, &usb1->fs_cstotal.cs_ndir, 1);
		fs32_add(sb, &sb->fs_cs(cg).cs_ndir, 1);
	}

	ubh_mark_buffer_dirty (USPI_UBH);
	ubh_mark_buffer_dirty (UCPI_UBH);
	if (sb->s_flags & MS_SYNCHRONOUS) {
		ubh_ll_rw_block (WRITE, 1, (struct ufs_buffer_head **) &ucpi);
		ubh_wait_on_buffer (UCPI_UBH);
	}
	sb->s_dirt = 1;

	inode->i_mode = mode;
	inode->i_uid = current->fsuid;
	if (dir->i_mode & S_ISGID) {
		inode->i_gid = dir->i_gid;
		if (S_ISDIR(mode))
			inode->i_mode |= S_ISGID;
	} else
		inode->i_gid = current->fsgid;

	inode->i_ino = cg * uspi->s_ipg + bit;
	inode->i_blksize = PAGE_SIZE;	/* This is the optimal IO size (for stat), not the fs block size */
	inode->i_blocks = 0;
	inode->i_mtime = inode->i_atime = inode->i_ctime = CURRENT_TIME;
	inode->u.ufs_i.i_flags = dir->u.ufs_i.i_flags;
	inode->u.ufs_i.i_lastfrag = 0;

	insert_inode_hash(inode);
	mark_inode_dirty(inode);

	unlock_super (sb);

	if (DQUOT_ALLOC_INODE(inode)) {
		DQUOT_DROP(inode);
		inode->i_flags |= S_NOQUOTA;
		inode->i_nlink = 0;
		iput(inode);
		return ERR_PTR(-EDQUOT);
	}

	UFSD(("allocating inode %lu\n", inode->i_ino))
	UFSD(("EXIT\n"))
	return inode;

failed:
	unlock_super (sb);
	make_bad_inode(inode);
	iput (inode);
	UFSD(("EXIT (FAILED)\n"))
	return ERR_PTR(-ENOSPC);
}