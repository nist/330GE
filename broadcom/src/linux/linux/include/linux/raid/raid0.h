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
#ifndef _RAID0_H
#define _RAID0_H

#include <linux/raid/md.h>

struct strip_zone
{
	unsigned long zone_offset;	/* Zone offset in md_dev */
	unsigned long dev_offset;	/* Zone offset in real dev */
	unsigned long size;		/* Zone size */
	int nb_dev;			/* # of devices attached to the zone */
	mdk_rdev_t *dev[MD_SB_DISKS]; /* Devices attached to the zone */
};

struct raid0_hash
{
	struct strip_zone *zone0, *zone1;
};

struct raid0_private_data
{
	struct raid0_hash *hash_table; /* Dynamically allocated */
	struct strip_zone *strip_zone; /* This one too */
	int nr_strip_zones;
	struct strip_zone *smallest;
	int nr_zones;
};

typedef struct raid0_private_data raid0_conf_t;

#define mddev_to_conf(mddev) ((raid0_conf_t *) mddev->private)

#endif
