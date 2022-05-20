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
/* -*- linux-c -*- --------------------------------------------------------- *
 *
 * linux/fs/autofs/dir.c
 *
 *  Copyright 1997-1998 Transmeta Corporation -- All Rights Reserved
 *
 * This file is part of the Linux kernel and is made available under
 * the terms of the GNU General Public License, version 2, or at your
 * option, any later version, incorporated herein by reference.
 *
 * ------------------------------------------------------------------------- */

#include "autofs_i.h"

/*
 * No entries except for "." and "..", both of which are handled by the VFS
 * layer. So all children are negative and dcache-based versions of operations
 * are OK.
 */
static struct dentry *autofs_dir_lookup(struct inode *dir,struct dentry *dentry)
{
	d_add(dentry, NULL);
	return NULL;
}

struct inode_operations autofs_dir_inode_operations = {
	lookup:		autofs_dir_lookup,
};

