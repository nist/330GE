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
 *  ioctl.c
 *
 *  Copyright (C) 1995, 1996 by Volker Lendecke
 *  Copyright (C) 1997 by Volker Lendecke
 *
 *  Please add a note about your changes to smbfs in the ChangeLog file.
 */

#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/highuid.h>

#include <linux/smb_fs.h>
#include <linux/smb_mount.h>

#include <asm/uaccess.h>

#include "proto.h"

int
smb_ioctl(struct inode *inode, struct file *filp,
	  unsigned int cmd, unsigned long arg)
{
	struct smb_sb_info *server = server_from_inode(inode);
	struct smb_conn_opt opt;
	int result = -EINVAL;

	switch (cmd) {
	case SMB_IOC_GETMOUNTUID:
		result = put_user(NEW_TO_OLD_UID(server->mnt->mounted_uid),
				  (uid16_t *) arg);
		break;
	case SMB_IOC_GETMOUNTUID32:
		result = put_user(server->mnt->mounted_uid, (uid_t *) arg);
		break;

	case SMB_IOC_NEWCONN:
		/* arg is smb_conn_opt, or NULL if no connection was made */
		if (!arg) {
			result = smb_wakeup(server);
			break;
		}

		result = -EFAULT;
		if (!copy_from_user(&opt, (void *)arg, sizeof(opt)))
			result = smb_newconn(server, &opt);
		break;
	default:
		break;
	}

	return result;
}
