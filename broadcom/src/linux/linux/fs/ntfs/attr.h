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
 * attr.h -  Header file for attr.c
 *
 * Copyright (C) 1997 Régis Duchesne
 * Copyright (c) 2001 Anton Altaparmakov (AIA)
 */
#include <linux/nls.h>

ntfs_u8* ntfs_find_attr_in_mft_rec(ntfs_volume *vol, ntfs_u8 *m, __u32 type,
			wchar_t *name, __u32 name_len, int ic, __u16 instance);

int ntfs_extend_attr(ntfs_inode *ino, ntfs_attribute *attr, const __s64 len);

int ntfs_resize_attr(ntfs_inode *ino, ntfs_attribute *attr, __s64 newsize);

int ntfs_insert_attribute(ntfs_inode *ino, unsigned char* attrdata);

int ntfs_read_compressed(ntfs_inode *ino, ntfs_attribute *attr, __s64 offset,
			 ntfs_io *dest);

int ntfs_write_compressed(ntfs_inode *ino, ntfs_attribute *attr, __s64 offset,
			  ntfs_io *dest);

int ntfs_create_attr(ntfs_inode *ino, int anum, char *aname, void *data,
		     int dsize, ntfs_attribute **rattr);

int ntfs_read_zero(ntfs_io *dest, int size);

int ntfs_make_attr_nonresident(ntfs_inode *ino, ntfs_attribute *attr);

int ntfs_attr_allnonresident(ntfs_inode *ino);

int ntfs_new_attr(ntfs_inode *ino, int type, void *name, int namelen,
		  void *value, int value_len, int *pos, int *found);

int ntfs_insert_run(ntfs_attribute *attr, int cnum, ntfs_cluster_t cluster,
		     int len);

