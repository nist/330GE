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
#ifndef _LINEAR_H
#define _LINEAR_H

#include <linux/raid/md.h>

struct dev_info {
	kdev_t		dev;
	unsigned long	size;
	unsigned long	offset;
};

typedef struct dev_info dev_info_t;

struct linear_hash
{
	dev_info_t *dev0, *dev1;
};

struct linear_private_data
{
	struct linear_hash	*hash_table;
	dev_info_t		disks[MD_SB_DISKS];
	dev_info_t		*smallest;
	int			nr_zones;
};


typedef struct linear_private_data linear_conf_t;

#define mddev_to_conf(mddev) ((linear_conf_t *) mddev->private)

#endif
