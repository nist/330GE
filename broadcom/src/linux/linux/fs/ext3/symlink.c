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
 *  linux/fs/ext3/symlink.c
 *
 * Only fast symlinks left here - the rest is done by generic code. AV, 1999
 *
 * Copyright (C) 1992, 1993, 1994, 1995
 * Remy Card (card@masi.ibp.fr)
 * Laboratoire MASI - Institut Blaise Pascal
 * Universite Pierre et Marie Curie (Paris VI)
 *
 *  from
 *
 *  linux/fs/minix/symlink.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *
 *  ext3 symlink handling code
 */

#include <linux/fs.h>
#include <linux/jbd.h>
#include <linux/ext3_fs.h>

static int ext3_readlink(struct dentry *dentry, char *buffer, int buflen)
{
	char *s = (char *)dentry->d_inode->u.ext3_i.i_data;
	return vfs_readlink(dentry, buffer, buflen, s);
}

static int ext3_follow_link(struct dentry *dentry, struct nameidata *nd)
{
	char *s = (char *)dentry->d_inode->u.ext3_i.i_data;
	return vfs_follow_link(nd, s);
}

struct inode_operations ext3_fast_symlink_inode_operations = {
	readlink:	ext3_readlink,		/* BKL not held.  Don't need */
	follow_link:	ext3_follow_link,	/* BKL not held.  Don't need */
};
