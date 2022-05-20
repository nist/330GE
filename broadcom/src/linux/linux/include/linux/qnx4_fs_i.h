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
 *  Name                         : qnx4_fs_i.h
 *  Author                       : Richard Frowijn
 *  Function                     : qnx4 inode definitions
 *  Version                      : 1.0.2
 *  Last modified                : 2000-01-06
 *
 *  History                      : 23-03-1998 created
 *
 */
#ifndef _QNX4_FS_I
#define _QNX4_FS_I

#include <linux/qnxtypes.h>

/*
 * qnx4 fs inode entry
 */
struct qnx4_inode_info {
	char		i_reserved[16];	/* 16 */
	qnx4_off_t	i_size;		/*  4 */
	qnx4_xtnt_t	i_first_xtnt;	/*  8 */
	__u32		i_xblk;		/*  4 */
	__s32		i_ftime;	/*  4 */
	__s32		i_mtime;	/*  4 */
	__s32		i_atime;	/*  4 */
	__s32		i_ctime;	/*  4 */
	qnx4_nxtnt_t	i_num_xtnts;	/*  2 */
	qnx4_mode_t	i_mode;		/*  2 */
	qnx4_muid_t	i_uid;		/*  2 */
	qnx4_mgid_t	i_gid;		/*  2 */
	qnx4_nlink_t	i_nlink;	/*  2 */
	__u8		i_zero[4];	/*  4 */
	qnx4_ftype_t	i_type;		/*  1 */
	__u8		i_status;	/*  1 */
	unsigned long	mmu_private;
};

#endif
