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
 *  smb_fs_i.h
 *
 *  Copyright (C) 1995 by Paal-Kr. Engstad and Volker Lendecke
 *  Copyright (C) 1997 by Volker Lendecke
 *
 */

#ifndef _LINUX_SMB_FS_I
#define _LINUX_SMB_FS_I

#ifdef __KERNEL__
#include <linux/types.h>

/*
 * smb fs inode data (in memory only)
 */
struct smb_inode_info {

	/*
	 * file handles are local to a connection. A file is open if
	 * (open == generation).
	 */
        unsigned int open;	/* open generation */
	__u16 fileid;		/* What id to handle a file with? */
	__u16 attr;		/* Attribute fields, DOS value */

	__u16 access;		/* Access mode */
	__u16 flags;
	unsigned long oldmtime;	/* last time refreshed */
	unsigned long closed;	/* timestamp when closed */
	unsigned openers;	/* number of fileid users */
};

#endif
#endif
