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
 * Symlink inode operations for Coda filesystem
 * Original version: (C) 1996 P. Braam and M. Callahan
 * Rewritten for Linux 2.1. (C) 1997 Carnegie Mellon University
 * 
 * Carnegie Mellon encourages users to contribute improvements to
 * the Coda project. Contact Peter Braam (coda@cs.cmu.edu).
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/stat.h>
#include <linux/errno.h>
#include <linux/locks.h>
#include <linux/smp_lock.h>

#include <linux/coda.h>
#include <linux/coda_linux.h>
#include <linux/coda_psdev.h>
#include <linux/coda_fs_i.h>
#include <linux/coda_proc.h>

static int coda_symlink_filler(struct file *file, struct page *page)
{
	struct inode *inode = page->mapping->host;
	int error;
	struct coda_inode_info *cii;
	unsigned int len = PAGE_SIZE;
	char *p = kmap(page);

	lock_kernel();
	cii = ITOC(inode);
	coda_vfs_stat.follow_link++;

	error = venus_readlink(inode->i_sb, &cii->c_fid, p, &len);
	unlock_kernel();
	if (error)
		goto fail;
	SetPageUptodate(page);
	kunmap(page);
	UnlockPage(page);
	return 0;

fail:
	SetPageError(page);
	kunmap(page);
	UnlockPage(page);
	return error;
}

struct address_space_operations coda_symlink_aops = {
	readpage:	coda_symlink_filler
};
