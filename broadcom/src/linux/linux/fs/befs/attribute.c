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
 * linux/fs/befs/attribute.c
 *
 * Copyright (C) 2002 Will Dyson <will_dyson@pobox.com>
 *
 * Many thanks to Dominic Giampaolo, author of "Practical File System
 * Design with the Be File System", for such a helpful book.
 *
 */

#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/string.h>

#include "befs_fs.h"
#include "endian.h"

#define SD_DATA(sd)\
	(void*)((char*)sd + sizeof(*sd) + (sd->name_size - sizeof(sd->name)))

#define SD_NEXT(sd)\
	(befs_small_data*)((char*)sd + sizeof(*sd) + (sd->name_size - \
	sizeof(sd->name) + sd->data_size))

int
 list_small_data(struct super_block *sb, befs_inode * inode, filldir_t filldir);

befs_small_data *find_small_data(struct super_block *sb, befs_inode * inode,
				 const char *name);

int
 read_small_data(struct super_block *sb, befs_inode * inode,
		 befs_small_data * sdata, void *buf, size_t bufsize);

/**
 *
 *
 *
 *
 *
 */
befs_small_data *
find_small_data(struct super_block *sb, befs_inode * inode, const char *name)
{
	befs_small_data *sdata = inode->small_data;

	while (sdata->type != 0) {
		if (strcmp(name, sdata->name) != 0) {
			return sdata;
		}
		sdata = SD_NEXT(sdata);
	}
	return NULL;
}

/**
 *
 *
 *
 *
 *
 */
int
read_small_data(struct super_block *sb, befs_inode * inode,
		const char *name, void *buf, size_t bufsize)
{
	befs_small_data *sdata;

	sdata = find_small_data(sb, inode, name);
	if (sdata == NULL)
		return BEFS_ERR;
	else if (sdata->data_size > bufsize)
		return BEFS_ERR;

	memcpy(buf, SD_DATA(sdata), sdata->data_size);

	return BEFS_OK;
}

/**
 *
 *
 *
 *
 *
 */
int
list_small_data(struct super_block *sb, befs_inode * inode)
{

}

/**
 *
 *
 *
 *
 *
 */
int
list_attr(struct super_block *sb, befs_inode * inode)
{

}

/**
 *
 *
 *
 *
 *
 */
int
read_attr(struct super_block *sb, befs_inode * inode)
{

}
