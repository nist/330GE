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
#ifndef _MSDOS_FS_I
#define _MSDOS_FS_I

/*
 * MS-DOS file system inode data in memory
 */

struct msdos_inode_info {
	unsigned long mmu_private;
	int i_start;	/* first cluster or 0 */
	int i_logstart;	/* logical first cluster */
	int i_attrs;	/* unused attribute bits */
	int i_ctime_ms;	/* unused change time in milliseconds */
	int i_location;	/* on-disk position of directory entry or 0 */
	struct inode *i_fat_inode;	/* struct inode of this one */
	struct list_head i_fat_hash;	/* hash by i_location */
};

#endif
