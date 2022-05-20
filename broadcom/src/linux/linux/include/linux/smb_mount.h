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
 *  smb_mount.h
 *
 *  Copyright (C) 1995, 1996 by Paal-Kr. Engstad and Volker Lendecke
 *  Copyright (C) 1997 by Volker Lendecke
 *
 */

#ifndef _LINUX_SMB_MOUNT_H
#define _LINUX_SMB_MOUNT_H

#include <linux/types.h>

#define SMB_MOUNT_VERSION	6

struct smb_mount_data {
	int version;
	__kernel_uid_t mounted_uid; /* Who may umount() this filesystem? */
	__kernel_uid_t uid;
	__kernel_gid_t gid;
	__kernel_mode_t file_mode;
	__kernel_mode_t dir_mode;
};


#ifdef __KERNEL__

/* "vers" in big-endian */
#define SMB_MOUNT_ASCII 0x76657273

#define SMB_MOUNT_OLDVERSION	6
#undef SMB_MOUNT_VERSION
#define SMB_MOUNT_VERSION	7

/* flags */
#define SMB_MOUNT_WIN95		0x0001	/* Win 95 server */
#define SMB_MOUNT_OLDATTR	0x0002	/* Use core getattr (Win 95 speedup) */
#define SMB_MOUNT_DIRATTR	0x0004	/* Use find_first for getattr */
#define SMB_MOUNT_CASE		0x0008	/* Be case sensitive */


struct smb_mount_data_kernel {
	int version;

	__kernel_uid_t mounted_uid;	/* Who may umount() this filesystem? */
	__kernel_uid_t uid;
	__kernel_gid_t gid;
	__kernel_mode_t file_mode;
	__kernel_mode_t dir_mode;

	u32 flags;

	int ttl;	/* maximum age in jiffies (inode, dentry & dircache) */
	int timeo;	/* timeout for requests */

	struct smb_nls_codepage codepage;
};

#endif

#endif
