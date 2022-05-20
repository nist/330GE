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
 * dir.h - Header file for dir.c
 *
 * Copyright (C) 1997 Régis Duchesne
 */
#define ITERATE_SPLIT_DONE      1

enum ntfs_iterate_e {
    BY_POSITION,
    BY_NAME,
    DIR_INSERT
};

/* not all fields are used for all operations */
typedef struct ntfs_iterate_s {
	enum ntfs_iterate_e type;
	ntfs_inode *dir;
	union{
		ntfs_u64 pos;
		int flags;
	}u;
	char *result;      /* pointer to entry if found */
	ntfs_u16* name;
	int namelen;
	int block;         /* current index record */
	int newblock;      /* index record created in a split */
	char *new_entry;
	int new_entry_size;
	/*ntfs_inode* new;*/
} ntfs_iterate_s;

int ntfs_getdir_unsorted(ntfs_inode *ino, ntfs_u32 *p_high, ntfs_u32* p_low,
			 int (*cb)(ntfs_u8*, void*), void *param);

int ntfs_getdir_byname(ntfs_iterate_s *walk);

int ntfs_dir_add(ntfs_inode *dir, ntfs_inode *new, ntfs_attribute *name);

int ntfs_check_index_record(ntfs_inode *ino, char *record);

int ntfs_getdir_byposition(ntfs_iterate_s *walk);

int ntfs_mkdir(ntfs_inode* dir,const char* name,int namelen, ntfs_inode *ino);

int ntfs_split_indexroot(ntfs_inode *ino);

int ntfs_add_index_root(ntfs_inode *ino, int type);

