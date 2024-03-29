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
 *  fs/partitions/atari.c
 *
 *  Code extracted from drivers/block/genhd.c
 *
 *  Copyright (C) 1991-1998  Linus Torvalds
 *  Re-organised Feb 1998 Russell King
 */

#include <linux/fs.h>
#include <linux/genhd.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/string.h>
#include <linux/blk.h>
#include <linux/ctype.h>

#include <asm/byteorder.h>
#include <asm/system.h>

#include "check.h"
#include "atari.h"

/* ++guenther: this should be settable by the user ("make config")?.
 */
#define ICD_PARTS

/* check if a partition entry looks valid -- Atari format is assumed if at
   least one of the primary entries is ok this way */
#define	VALID_PARTITION(pi,hdsiz)					     \
    (((pi)->flg & 1) &&							     \
     isalnum((pi)->id[0]) && isalnum((pi)->id[1]) && isalnum((pi)->id[2]) && \
     be32_to_cpu((pi)->st) <= (hdsiz) &&				     \
     be32_to_cpu((pi)->st) + be32_to_cpu((pi)->siz) <= (hdsiz))

static inline int OK_id(char *s)
{
	return  memcmp (s, "GEM", 3) == 0 || memcmp (s, "BGM", 3) == 0 ||
		memcmp (s, "LNX", 3) == 0 || memcmp (s, "SWP", 3) == 0 ||
		memcmp (s, "RAW", 3) == 0 ;
}

int atari_partition (struct gendisk *hd, struct block_device *bdev,
		     unsigned long first_sector, int minor)
{
	int m_lim = minor + hd->max_p;
	Sector sect;
	struct rootsector *rs;
	struct partition_info *pi;
	u32 extensect;
	u32 hd_size;
#ifdef ICD_PARTS
	int part_fmt = 0; /* 0:unknown, 1:AHDI, 2:ICD/Supra */
#endif

	rs = (struct rootsector *) read_dev_sector(bdev, 0, &sect);
	if (!rs)
		return -1;

	/* Verify this is an Atari rootsector: */
	hd_size = hd->part[minor - 1].nr_sects;
	if (!VALID_PARTITION(&rs->part[0], hd_size) &&
	    !VALID_PARTITION(&rs->part[1], hd_size) &&
	    !VALID_PARTITION(&rs->part[2], hd_size) &&
	    !VALID_PARTITION(&rs->part[3], hd_size)) {
		/*
		 * if there's no valid primary partition, assume that no Atari
		 * format partition table (there's no reliable magic or the like
	         * :-()
		 */
		put_dev_sector(sect);
		return 0;
	}

	pi = &rs->part[0];
	printk (" AHDI");
	for (; pi < &rs->part[4] && minor < m_lim; minor++, pi++) {
		struct rootsector *xrs;
		Sector sect2;
		ulong partsect;

		if ( !(pi->flg & 1) )
			continue;
		/* active partition */
		if (memcmp (pi->id, "XGM", 3) != 0) {
			/* we don't care about other id's */
			add_gd_partition (hd, minor, be32_to_cpu(pi->st),
					be32_to_cpu(pi->siz));
			continue;
		}
		/* extension partition */
#ifdef ICD_PARTS
		part_fmt = 1;
#endif
		printk(" XGM<");
		partsect = extensect = be32_to_cpu(pi->st);
		while (1) {
			xrs = (struct rootsector *)read_dev_sector(bdev, partsect, &sect2);
			if (!xrs) {
				printk (" block %ld read failed\n", partsect);
				put_dev_sector(sect);
				return 0;
			}

			/* ++roman: sanity check: bit 0 of flg field must be set */
			if (!(xrs->part[0].flg & 1)) {
				printk( "\nFirst sub-partition in extended partition is not valid!\n" );
				put_dev_sector(sect2);
				break;
			}

			add_gd_partition(hd, minor,
				   partsect + be32_to_cpu(xrs->part[0].st),
				   be32_to_cpu(xrs->part[0].siz));

			if (!(xrs->part[1].flg & 1)) {
				/* end of linked partition list */
				put_dev_sector(sect2);
				break;
			}
			if (memcmp( xrs->part[1].id, "XGM", 3 ) != 0) {
				printk("\nID of extended partition is not XGM!\n");
				put_dev_sector(sect2);
				break;
			}

			partsect = be32_to_cpu(xrs->part[1].st) + extensect;
			put_dev_sector(sect2);
			minor++;
			if (minor >= m_lim) {
				printk( "\nMaximum number of partitions reached!\n" );
				break;
			}
		}
		printk(" >");
	}
#ifdef ICD_PARTS
	if ( part_fmt!=1 ) { /* no extended partitions -> test ICD-format */
		pi = &rs->icdpart[0];
		/* sanity check: no ICD format if first partition invalid */
		if (OK_id(pi->id)) {
			printk(" ICD<");
			for (; pi < &rs->icdpart[8] && minor < m_lim; minor++, pi++) {
				/* accept only GEM,BGM,RAW,LNX,SWP partitions */
				if (!((pi->flg & 1) && OK_id(pi->id)))
					continue;
				part_fmt = 2;
				add_gd_partition (hd, minor,
						be32_to_cpu(pi->st),
						be32_to_cpu(pi->siz));
			}
			printk(" >");
		}
	}
#endif
	put_dev_sector(sect);

	printk ("\n");

	return 1;
}

