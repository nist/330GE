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
 *  linux/fs/ext2/file.c
 *
 * Copyright (C) 1992, 1993, 1994, 1995
 * Remy Card (card@masi.ibp.fr)
 * Laboratoire MASI - Institut Blaise Pascal
 * Universite Pierre et Marie Curie (Paris VI)
 *
 *  from
 *
 *  linux/fs/minix/file.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *
 *  ext2 fs regular file handling primitives
 *
 *  64-bit file support on 64-bit platforms by Jakub Jelinek
 * 	(jj@sunsite.ms.mff.cuni.cz)
 */

#include <linux/fs.h>
#include <linux/ext2_fs.h>
#include <linux/sched.h>

/*
 * Called when an inode is released. Note that this is different
 * from ext2_open_file: open gets called at every open, but release
 * gets called only when /all/ the files are closed.
 */
static int ext2_release_file (struct inode * inode, struct file * filp)
{
	if (filp->f_mode & FMODE_WRITE)
		ext2_discard_prealloc (inode);
	return 0;
}

/*
 * We have mostly NULL's here: the current defaults are ok for
 * the ext2 filesystem.
 */
struct file_operations ext2_file_operations = {
	llseek:		generic_file_llseek,
	read:		generic_file_read,
	write:		generic_file_write,
	ioctl:		ext2_ioctl,
	mmap:		generic_file_mmap,
	open:		generic_file_open,
	release:	ext2_release_file,
	fsync:		ext2_sync_file,
};

struct inode_operations ext2_file_inode_operations = {
	truncate:	ext2_truncate,
};
