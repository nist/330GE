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
 *  linux/fs/ioctl.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

#include <linux/mm.h>
#include <linux/smp_lock.h>
#include <linux/file.h>

#include <asm/uaccess.h>
#include <asm/ioctls.h>

static int file_ioctl(struct file *filp,unsigned int cmd,unsigned long arg)
{
	int error;
	int block;
	struct inode * inode = filp->f_dentry->d_inode;

	switch (cmd) {
		case FIBMAP:
		{
			struct address_space *mapping = inode->i_mapping;
			int res;
			/* do we support this mess? */
			if (!mapping->a_ops->bmap)
				return -EINVAL;
			if (!capable(CAP_SYS_RAWIO))
				return -EPERM;
			if ((error = get_user(block, (int *) arg)) != 0)
				return error;

			res = mapping->a_ops->bmap(mapping, block);
			return put_user(res, (int *) arg);
		}
		case FIGETBSZ:
			if (inode->i_sb == NULL)
				return -EBADF;
			return put_user(inode->i_sb->s_blocksize, (int *) arg);
		case FIONREAD:
			return put_user(inode->i_size - filp->f_pos, (int *) arg);
	}
	if (filp->f_op && filp->f_op->ioctl)
		return filp->f_op->ioctl(inode, filp, cmd, arg);
	return -ENOTTY;
}


asmlinkage long sys_ioctl(unsigned int fd, unsigned int cmd, unsigned long arg)
{	
	struct file * filp;
	unsigned int flag;
	int on, error = -EBADF;

	filp = fget(fd);
	if (!filp)
		goto out;
	error = 0;
	lock_kernel();
	switch (cmd) {
		case FIOCLEX:
			set_close_on_exec(fd, 1);
			break;

		case FIONCLEX:
			set_close_on_exec(fd, 0);
			break;

		case FIONBIO:
			if ((error = get_user(on, (int *)arg)) != 0)
				break;
			flag = O_NONBLOCK;
#ifdef __sparc__
			/* SunOS compatibility item. */
			if(O_NONBLOCK != O_NDELAY)
				flag |= O_NDELAY;
#endif
			if (on)
				filp->f_flags |= flag;
			else
				filp->f_flags &= ~flag;
			break;

		case FIOASYNC:
			if ((error = get_user(on, (int *)arg)) != 0)
				break;
			flag = on ? FASYNC : 0;

			/* Did FASYNC state change ? */
			if ((flag ^ filp->f_flags) & FASYNC) {
				if (filp->f_op && filp->f_op->fasync)
					error = filp->f_op->fasync(fd, filp, on);
				else error = -ENOTTY;
			}
			if (error != 0)
				break;

			if (on)
				filp->f_flags |= FASYNC;
			else
				filp->f_flags &= ~FASYNC;
			break;

		default:
			error = -ENOTTY;
			if (S_ISREG(filp->f_dentry->d_inode->i_mode))
				error = file_ioctl(filp, cmd, arg);
			else if (filp->f_op && filp->f_op->ioctl)
				error = filp->f_op->ioctl(filp->f_dentry->d_inode, filp, cmd, arg);
	}
	unlock_kernel();
	fput(filp);

out:
	return error;
}
