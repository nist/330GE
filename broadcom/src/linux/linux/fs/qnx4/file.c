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
 * QNX4 file system, Linux implementation.
 *
 * Version : 0.2.1
 *
 * Using parts of the xiafs filesystem.
 *
 * History :
 *
 * 25-05-1998 by Richard Frowijn : first release.
 * 21-06-1998 by Frank Denis : wrote qnx4_readpage to use generic_file_read.
 * 27-06-1998 by Frank Denis : file overwriting.
 */

#include <linux/config.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/qnx4_fs.h>

/*
 * We have mostly NULL's here: the current defaults are ok for
 * the qnx4 filesystem.
 */
struct file_operations qnx4_file_operations =
{
	llseek:			generic_file_llseek,
	read:			generic_file_read,
#ifdef CONFIG_QNX4FS_RW
	write:			generic_file_write,
#endif
	mmap:			generic_file_mmap,
#ifdef CONFIG_QNX4FS_RW
	fsync:			qnx4_sync_file,
#endif
};

struct inode_operations qnx4_file_inode_operations =
{
#ifdef CONFIG_QNX4FS_RW
	truncate:		qnx4_truncate,
#endif
};
