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
#ifndef _ISO_FS_I
#define _ISO_FS_I

enum isofs_file_format {
	isofs_file_normal = 0,
	isofs_file_sparse = 1,
	isofs_file_compressed = 2,
};
	
/*
 * iso fs inode data in memory
 */
struct iso_inode_info {
	unsigned int i_first_extent;
	unsigned char i_file_format;
	unsigned char i_format_parm[3];
	unsigned long i_next_section_ino;
	off_t i_section_size;
};

#endif
