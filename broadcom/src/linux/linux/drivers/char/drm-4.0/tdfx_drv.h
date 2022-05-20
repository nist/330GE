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
/* tdfx_drv.h -- Private header for tdfx driver -*- linux-c -*-
 * Created: Thu Oct  7 10:40:04 1999 by faith@precisioninsight.com
 *
 * Copyright 1999 Precision Insight, Inc., Cedar Park, Texas.
 * Copyright 2000 VA Linux Systems, Inc., Sunnyvale, California.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 * 
 * Authors:
 *    Rickard E. (Rik) Faith <faith@valinux.com>
 *    Daryll Strauss <daryll@valinux.com>
 * 
 */

#ifndef _TDFX_DRV_H_
#define _TDFX_DRV_H_

				/* tdfx_drv.c */
extern int  tdfx_version(struct inode *inode, struct file *filp,
			  unsigned int cmd, unsigned long arg);
extern int  tdfx_open(struct inode *inode, struct file *filp);
extern int  tdfx_release(struct inode *inode, struct file *filp);
extern int  tdfx_ioctl(struct inode *inode, struct file *filp,
			unsigned int cmd, unsigned long arg);
extern int  tdfx_lock(struct inode *inode, struct file *filp,
		       unsigned int cmd, unsigned long arg);
extern int  tdfx_unlock(struct inode *inode, struct file *filp,
			 unsigned int cmd, unsigned long arg);

				/* tdfx_context.c */

extern int  tdfx_resctx(struct inode *inode, struct file *filp,
			unsigned int cmd, unsigned long arg);
extern int  tdfx_addctx(struct inode *inode, struct file *filp,
		        unsigned int cmd, unsigned long arg);
extern int  tdfx_modctx(struct inode *inode, struct file *filp,
		        unsigned int cmd, unsigned long arg);
extern int  tdfx_getctx(struct inode *inode, struct file *filp,
		        unsigned int cmd, unsigned long arg);
extern int  tdfx_switchctx(struct inode *inode, struct file *filp,
			   unsigned int cmd, unsigned long arg);
extern int  tdfx_newctx(struct inode *inode, struct file *filp,
			unsigned int cmd, unsigned long arg);
extern int  tdfx_rmctx(struct inode *inode, struct file *filp,
		       unsigned int cmd, unsigned long arg);

extern int  tdfx_context_switch(drm_device_t *dev, int old, int new);
extern int  tdfx_context_switch_complete(drm_device_t *dev, int new);
#endif
