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
#ifndef _ISOFS_FS_SB
#define _ISOFS_FS_SB

/*
 * iso9660 super-block data in memory
 */
struct isofs_sb_info {
	unsigned long s_ninodes;
	unsigned long s_nzones;
	unsigned long s_firstdatazone;
	unsigned long s_log_zone_size;
	unsigned long s_max_size;
	
	unsigned char s_high_sierra; /* A simple flag */
	unsigned char s_mapping;
	int           s_rock_offset; /* offset of SUSP fields within SU area */
	unsigned char s_rock;
	unsigned char s_joliet_level;
	unsigned char s_utf8;
	unsigned char s_cruft; /* Broken disks with high
				  byte of length containing
				  junk */
	unsigned char s_unhide;
	unsigned char s_nosuid;
	unsigned char s_nodev;
	unsigned char s_nocompress;

	mode_t s_mode;
	gid_t s_gid;
	uid_t s_uid;
	struct nls_table *s_nls_iocharset; /* Native language support table */
};

#endif
