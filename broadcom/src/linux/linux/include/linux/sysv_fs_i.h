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
#ifndef _SYSV_FS_I
#define _SYSV_FS_I

/*
 * SystemV/V7/Coherent FS inode data in memory
 */
struct sysv_inode_info {
	u32 i_data[10+1+1+1];	/* zone numbers: max. 10 data blocks,
				 * then 1 indirection block,
				 * then 1 double indirection block,
				 * then 1 triple indirection block.
				 */
	u32 i_dir_start_lookup;
};

#endif

