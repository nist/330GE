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
 *	include/linux/bfs_fs_i.h
 *	Copyright (C) 1999 Tigran Aivazian <tigran@veritas.com>
 */

#ifndef _LINUX_BFS_FS_I
#define _LINUX_BFS_FS_I

/*
 * BFS file system in-core inode info
 */
struct bfs_inode_info {
	unsigned long i_dsk_ino; /* inode number from the disk, can be 0 */
	unsigned long i_sblock;
	unsigned long i_eblock;
};

#endif	/* _LINUX_BFS_FS_I */
