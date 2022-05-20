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
/* -*- c -*- --------------------------------------------------------------- *
 *
 * linux/fs/autofs/symlink.c
 *
 *  Copyright 1997-1998 Transmeta Corporation -- All Rights Reserved
 *
 * This file is part of the Linux kernel and is made available under
 * the terms of the GNU General Public License, version 2, or at your
 * option, any later version, incorporated herein by reference.
 *
 * ------------------------------------------------------------------------- */

#include "autofs_i.h"

static int autofs4_readlink(struct dentry *dentry, char *buffer, int buflen)
{
	struct autofs_info *ino = autofs4_dentry_ino(dentry);

	return vfs_readlink(dentry, buffer, buflen, ino->u.symlink);
}

static int autofs4_follow_link(struct dentry *dentry, struct nameidata *nd)
{
	struct autofs_info *ino = autofs4_dentry_ino(dentry);

	return vfs_follow_link(nd, ino->u.symlink);
}

struct inode_operations autofs4_symlink_inode_operations = {
	readlink:	autofs4_readlink,
	follow_link:	autofs4_follow_link
};
