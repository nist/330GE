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
 * super.h - Header file for super.c
 *
 * Copyright (C) 1995-1997 Martin von Löwis
 * Copyright (C) 1996-1997 Régis Duchesne
 * Copyright (c) 2001 Anton Altaparmakov
 */

int ntfs_get_free_cluster_count(ntfs_inode *bitmap);

int ntfs_get_volumesize(ntfs_volume *vol, __s64 *vol_size);

int ntfs_init_volume(ntfs_volume *vol, char *boot);

int ntfs_load_special_files(ntfs_volume *vol);

int ntfs_release_volume(ntfs_volume *vol);

int ntfs_insert_fixups(unsigned char *rec, int rec_size);

int ntfs_fixup_record(char *record, char *magic, int size);

int ntfs_allocate_clusters(ntfs_volume *vol, ntfs_cluster_t *location,
		ntfs_cluster_t *count, ntfs_runlist **rl, int *rl_len,
		const NTFS_CLUSTER_ALLOCATION_ZONES zone);

int ntfs_deallocate_cluster_run(const ntfs_volume *vol,
		const ntfs_cluster_t lcn, const ntfs_cluster_t len);

int ntfs_deallocate_clusters(const ntfs_volume *vol, const ntfs_runlist *rl,
		const int rl_len);

