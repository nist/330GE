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
 *  Name                         : qnx4_fs_sb.h
 *  Author                       : Richard Frowijn
 *  Function                     : qnx4 superblock definitions
 *  Version                      : 1.0.2
 *  Last modified                : 2000-01-06
 * 
 *  History                      : 23-03-1998 created
 * 
 */
#ifndef _QNX4_FS_SB
#define _QNX4_FS_SB

#include <linux/qnx4_fs.h>

/*
 * qnx4 super-block data in memory
 */

struct qnx4_sb_info {
	struct buffer_head	*sb_buf;	/* superblock buffer */
	struct qnx4_super_block	*sb;		/* our superblock */
	unsigned int		Version;	/* may be useful */
	struct qnx4_inode_entry	*BitMap;	/* useful */
};

#endif
