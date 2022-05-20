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
 *  linux/fs/sysv/symlink.c
 *
 *  Handling of System V filesystem fast symlinks extensions.
 *  Aug 2001, Christoph Hellwig (hch@infradead.org)
 */

#include <linux/fs.h>

static int sysv_readlink(struct dentry *dentry, char *buffer, int buflen)
{
	char *s = (char *)dentry->d_inode->u.sysv_i.i_data;
	return vfs_readlink(dentry, buffer, buflen, s);
}

static int sysv_follow_link(struct dentry *dentry, struct nameidata *nd)
{
	char *s = (char *)dentry->d_inode->u.sysv_i.i_data;
	return vfs_follow_link(nd, s);
}

struct inode_operations sysv_fast_symlink_inode_operations = {
	readlink:	sysv_readlink,
	follow_link:	sysv_follow_link,
};
