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

/***************************************************************************
 *
 *  drivers/s390/char/tapechar.h
 *    character device frontend for tape device driver
 *
 *  S390 and zSeries version
 *    Copyright (C) 2001 IBM Corporation
 *    Author(s): Carsten Otte <cotte@de.ibm.com>
 *               Tuan Ngo-Anh <ngoanh@de.ibm.com>
 *
 *
 ****************************************************************************
 */

#ifndef TAPECHAR_H
#define TAPECHAR_H
#include <linux/config.h>
#define TAPECHAR_DEFAULTMODE 0020644
#define  TAPE_MAJOR                    0        /* get dynamic major since no major officialy defined for tape */
/*
 * Prototypes for tape_fops
 */
ssize_t tape_read(struct file *, char *, size_t, loff_t *);
ssize_t tape_write(struct file *, const char *, size_t, loff_t *);
int tape_ioctl(struct inode *,struct file *,unsigned int,unsigned long);
int tape_open (struct inode *,struct file *);
int tape_release (struct inode *,struct file *);
#ifdef CONFIG_DEVFS_FS
void tapechar_mkdevfstree (tape_info_t* ti);
#endif
void tapechar_init (void);
void tapechar_uninit (void);
#endif /* TAPECHAR_H */
