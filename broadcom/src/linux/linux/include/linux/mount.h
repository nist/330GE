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
 *
 * Definitions for mount interface. This describes the in the kernel build 
 * linkedlist with mounted filesystems.
 *
 * Author:  Marco van Wieringen <mvw@planets.elm.net>
 *
 * Version: $Id: mount.h,v 1.1.1.1 2007/01/25 12:52:07 jiahao_jhou Exp $
 *
 */
#ifndef _LINUX_MOUNT_H
#define _LINUX_MOUNT_H
#ifdef __KERNEL__

#define MNT_NOSUID	1
#define MNT_NODEV	2
#define MNT_NOEXEC	4

struct vfsmount
{
	struct list_head mnt_hash;
	struct vfsmount *mnt_parent;	/* fs we are mounted on */
	struct dentry *mnt_mountpoint;	/* dentry of mountpoint */
	struct dentry *mnt_root;	/* root of the mounted tree */
	struct super_block *mnt_sb;	/* pointer to superblock */
	struct list_head mnt_mounts;	/* list of children, anchored here */
	struct list_head mnt_child;	/* and going through their mnt_child */
	atomic_t mnt_count;
	int mnt_flags;
	char *mnt_devname;		/* Name of device e.g. /dev/dsk/hda1 */
	struct list_head mnt_list;
};

static inline struct vfsmount *mntget(struct vfsmount *mnt)
{
	if (mnt)
		atomic_inc(&mnt->mnt_count);
	return mnt;
}

extern void __mntput(struct vfsmount *mnt);

static inline void mntput(struct vfsmount *mnt)
{
	if (mnt) {
		if (atomic_dec_and_test(&mnt->mnt_count))
			__mntput(mnt);
	}
}

#endif
#endif /* _LINUX_MOUNT_H */
