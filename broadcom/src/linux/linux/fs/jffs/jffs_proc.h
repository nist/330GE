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
 * JFFS -- Journaling Flash File System, Linux implementation.
 *
 * Copyright (C) 2000  Axis Communications AB.
 *
 * Created by Simon Kagstrom <simonk@axis.com>.
 *
 * This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * $Id: jffs_proc.h,v 1.1.1.1 2007/01/25 12:52:02 jiahao_jhou Exp $
 */

/* jffs_proc.h defines a structure for inclusion in the proc-file system.  */
#ifndef __LINUX_JFFS_PROC_H__
#define __LINUX_JFFS_PROC_H__

#include <linux/proc_fs.h>

/* The proc_dir_entry for jffs (defined in jffs_proc.c).  */
extern struct proc_dir_entry *jffs_proc_root;

int jffs_register_jffs_proc_dir(kdev_t dev, struct jffs_control *c);
int jffs_unregister_jffs_proc_dir(struct jffs_control *c);

#endif /* __LINUX_JFFS_PROC_H__ */
