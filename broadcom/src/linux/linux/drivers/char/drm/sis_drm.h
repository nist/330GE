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

#ifndef _sis_drm_public_h_
#define _sis_drm_public_h_

typedef struct {
  int context;
  unsigned int offset;
  unsigned int size;
  unsigned long free;
} drm_sis_mem_t;

typedef struct {
  unsigned int offset, size;
} drm_sis_agp_t;

typedef struct {
  unsigned int left, right;
} drm_sis_flip_t;

#ifdef __KERNEL__

int sis_fb_alloc(struct inode *inode, struct file *filp, unsigned int cmd,
		  unsigned long arg);
int sis_fb_free(struct inode *inode, struct file *filp, unsigned int cmd,
		  unsigned long arg);

int sisp_agp_init(struct inode *inode, struct file *filp, unsigned int cmd,
		  unsigned long arg);
int sisp_agp_alloc(struct inode *inode, struct file *filp, unsigned int cmd,
		  unsigned long arg);
int sisp_agp_free(struct inode *inode, struct file *filp, unsigned int cmd,
		  unsigned long arg);

#endif

#endif
