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
#ifndef _LINUX_FS_STRUCT_H
#define _LINUX_FS_STRUCT_H
#ifdef __KERNEL__

struct fs_struct {
	atomic_t count;
	rwlock_t lock;
	int umask;
	struct dentry * root, * pwd, * altroot;
	struct vfsmount * rootmnt, * pwdmnt, * altrootmnt;
};

#define INIT_FS { \
	ATOMIC_INIT(1), \
	RW_LOCK_UNLOCKED, \
	0022, \
	NULL, NULL, NULL, NULL, NULL, NULL \
}

extern void exit_fs(struct task_struct *);
extern void set_fs_altroot(void);

/*
 * Replace the fs->{rootmnt,root} with {mnt,dentry}. Put the old values.
 * It can block. Requires the big lock held.
 */

static inline void set_fs_root(struct fs_struct *fs,
	struct vfsmount *mnt,
	struct dentry *dentry)
{
	struct dentry *old_root;
	struct vfsmount *old_rootmnt;
	write_lock(&fs->lock);
	old_root = fs->root;
	old_rootmnt = fs->rootmnt;
	fs->rootmnt = mntget(mnt);
	fs->root = dget(dentry);
	write_unlock(&fs->lock);
	if (old_root) {
		dput(old_root);
		mntput(old_rootmnt);
	}
}

/*
 * Replace the fs->{pwdmnt,pwd} with {mnt,dentry}. Put the old values.
 * It can block. Requires the big lock held.
 */

static inline void set_fs_pwd(struct fs_struct *fs,
	struct vfsmount *mnt,
	struct dentry *dentry)
{
	struct dentry *old_pwd;
	struct vfsmount *old_pwdmnt;
	write_lock(&fs->lock);
	old_pwd = fs->pwd;
	old_pwdmnt = fs->pwdmnt;
	fs->pwdmnt = mntget(mnt);
	fs->pwd = dget(dentry);
	write_unlock(&fs->lock);
	if (old_pwd) {
		dput(old_pwd);
		mntput(old_pwdmnt);
	}
}

struct fs_struct *copy_fs_struct(struct fs_struct *old);
void put_fs_struct(struct fs_struct *fs);

#endif
#endif
