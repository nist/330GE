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
 * fsync.c
 *
 * PURPOSE
 *  Fsync handling routines for the OSTA-UDF(tm) filesystem.
 *
 * CONTACTS
 *  E-mail regarding any portion of the Linux UDF file system should be
 *  directed to the development team mailing list (run by majordomo):
 *      linux_udf@hpesjro.fc.hp.com
 *
 * COPYRIGHT
 *  This file is distributed under the terms of the GNU General Public
 *  License (GPL). Copies of the GPL can be obtained from:
 *      ftp://prep.ai.mit.edu/pub/gnu/GPL
 *  Each contributing author retains all rights to their own work.
 *
 *  (C) 1999-2001 Ben Fennema
 *  (C) 1999-2000 Stelias Computing Inc
 *
 * HISTORY
 *
 *  05/22/99 blf  Created.
 */

#include "udfdecl.h"

#include <linux/fs.h>
#include <linux/locks.h>
#include <linux/smp_lock.h>

/*
 *	File may be NULL when we are called. Perhaps we shouldn't
 *	even pass file to fsync ?
 */

int udf_fsync_file(struct file * file, struct dentry *dentry, int datasync)
{
	struct inode *inode = dentry->d_inode;
	return udf_fsync_inode(inode, datasync);
}

int udf_fsync_inode(struct inode *inode, int datasync)
{
	int err;

	err = fsync_inode_buffers(inode);
	err |= fsync_inode_data_buffers(inode);
	if (!(inode->i_state & I_DIRTY))
		return err;
	if (datasync && !(inode->i_state & I_DIRTY_DATASYNC))
		return err;

	err |= udf_sync_inode (inode);
	return err ? -EIO : 0;
}
