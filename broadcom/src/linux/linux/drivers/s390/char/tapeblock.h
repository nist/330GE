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

#ifndef TAPEBLOCK_H
#define TAPEBLOCK_H
#include <linux/config.h>
#define PARTN_BITS 0

#define TAPEBLOCK_READAHEAD 30
#define TAPEBLOCK_MAJOR 0

#define TAPEBLOCK_DEFAULTMODE 0060644

int tapeblock_open(struct inode *, struct file *);
int tapeblock_release(struct inode *, struct file *);
void tapeblock_setup(tape_info_t* ti);
void schedule_tapeblock_exec_IO (tape_info_t *ti);
int tapeblock_mediumdetect(tape_info_t* ti);
#ifdef CONFIG_DEVFS_FS
void tapeblock_mkdevfstree (tape_info_t* ti);
#endif
int tapeblock_init (void);
void tapeblock_uninit (void);
#endif
