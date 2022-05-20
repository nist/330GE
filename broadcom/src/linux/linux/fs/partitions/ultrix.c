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
 *  fs/partitions/ultrix.c
 *
 *  Code extracted from drivers/block/genhd.c
 *
 *  Re-organised Jul 1999 Russell King
 */

#include <linux/fs.h>
#include <linux/genhd.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/blk.h>

#include "check.h"

int ultrix_partition(struct gendisk *hd, struct block_device *bdev,
                            unsigned long first_sector, int first_part_minor)
{
	int i;
	Sector sect;
	unsigned char *data;
	struct ultrix_disklabel {
		s32	pt_magic;	/* magic no. indicating part. info exits */
		s32	pt_valid;	/* set by driver if pt is current */
		struct  pt_info {
			s32		pi_nblocks; /* no. of sectors */
			u32		pi_blkoff;  /* block offset for start */
		} pt_part[8];
	} *label;

#define PT_MAGIC	0x032957	/* Partition magic number */
#define PT_VALID	1		/* Indicates if struct is valid */

	data = read_dev_sector(bdev, (16384 - sizeof(*label))/512, &sect);
	if (!data)
		return -1;
	
	label = (struct ultrix_disklabel *)(data + 512 - sizeof(*label));

	if (label->pt_magic == PT_MAGIC && label->pt_valid == PT_VALID) {
		for (i=0; i<8; i++, first_part_minor++)
			if (label->pt_part[i].pi_nblocks)
				add_gd_partition(hd, first_part_minor, 
					      label->pt_part[i].pi_blkoff,
					      label->pt_part[i].pi_nblocks);
		put_dev_sector(sect);
		printk ("\n");
		return 1;
	} else {
		put_dev_sector(sect);
		return 0;
	}
}
