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
 *	include/linux/bfs_fs_sb.h
 *	Copyright (C) 1999 Tigran Aivazian <tigran@veritas.com>
 */

#ifndef _LINUX_BFS_FS_SB
#define _LINUX_BFS_FS_SB

/*
 * BFS file system in-core superblock info
 */
struct bfs_sb_info {
	unsigned long si_blocks;
	unsigned long si_freeb;
	unsigned long si_freei;
	unsigned long si_lf_ioff;
	unsigned long si_lf_sblk;
	unsigned long si_lf_eblk;
	unsigned long si_lasti;
	char * si_imap;
	struct buffer_head * si_sbh;		/* buffer header w/superblock */
	struct bfs_super_block * si_bfs_sb;	/* superblock in si_sbh->b_data */
};

#endif	/* _LINUX_BFS_FS_SB */
