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
#ifndef _MINIX_FS_SB
#define _MINIX_FS_SB

/*
 * minix super-block data in memory
 */
struct minix_sb_info {
			unsigned long s_ninodes;
			unsigned long s_nzones;
			unsigned long s_imap_blocks;
			unsigned long s_zmap_blocks;
			unsigned long s_firstdatazone;
			unsigned long s_log_zone_size;
			unsigned long s_max_size;
			int s_dirsize;
			int s_namelen;
			int s_link_max;
			struct buffer_head ** s_imap;
			struct buffer_head ** s_zmap;
			struct buffer_head * s_sbh;
			struct minix_super_block * s_ms;
			unsigned short s_mount_state;
			unsigned short s_version;
};

#endif
