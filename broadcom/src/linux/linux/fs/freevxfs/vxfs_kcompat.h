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
#ifndef _VXFS_KCOMPAT_H
#define _VXFS_KCOMPAT_H

#include <linux/version.h>

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0))

#include <linux/blkdev.h>

typedef long sector_t;

/* Dito.  */
static inline void map_bh(struct buffer_head *bh, struct super_block *sb, int block)
{
	bh->b_state |= 1 << BH_Mapped;
	bh->b_dev = sb->s_dev;
	bh->b_blocknr = block;
}

#endif /* Kernel 2.4 */
#endif /* _VXFS_KCOMPAT_H */
