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
 * dummyfs: a placeholder filesystem that sleeps forever when mounted
 *
 * Copyright 2006, ASUSTek Inc.      
 * All Rights Reserved.      
 *       
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY      
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM      
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS      
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.      
 *
 * $Id: dummy.c,v 1.1.1.1 2007/01/25 12:51:52 jiahao_jhou Exp $
 */

#include <linux/config.h>
#include <linux/module.h>

#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/stat.h>
#include <linux/errno.h>
#include <linux/major.h>
#include <linux/wait.h>
#include <linux/blk.h>
#include <linux/init.h>
#include <linux/devfs_fs_kernel.h>
#include <linux/smp_lock.h>
#include <linux/swap.h>
#include <linux/slab.h>

#include <asm/uaccess.h>

/* I don't thik anyone would mind if we stole CM206_CDROM_MAJOR */
#define DUMMY_MAJOR 0x20

static int dummy_open(struct inode *inode, struct file *file)
{
	DECLARE_WAIT_QUEUE_HEAD(wait);

	for (;;)
		sleep_on(&wait);

	return 0;
}

static struct block_device_operations dummy_fops = {
	open:		dummy_open,
};

int __init dummy_init(void) 
{
	if (devfs_register_blkdev(DUMMY_MAJOR, "dummy", &dummy_fops)) {
		printk(KERN_WARNING "Unable to get major number for dummy device\n");
		return -EIO;
	}

	register_disk(NULL, MKDEV(DUMMY_MAJOR, 0), 1, &dummy_fops, 0);

	return 0;
}

void dummy_exit(void) 
{
	if (devfs_unregister_blkdev(0, "dummy"))
		printk(KERN_WARNING "dummy: cannot unregister blkdev\n");
}

module_init(dummy_init);
module_exit(dummy_exit);
