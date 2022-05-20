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
#ifndef _ASM_X86_64_STAT_H
#define _ASM_X86_64_STAT_H

struct stat {
	unsigned long	st_dev;
	unsigned long	st_ino;
	unsigned long	st_nlink;

	unsigned int	st_mode;
	unsigned int	st_uid;
	unsigned int	st_gid;
	unsigned int	__pad0;
	unsigned long	st_rdev;
	long		st_size;
	long		st_blksize;
	long		st_blocks;	/* Number 512-byte blocks allocated. */

	unsigned long	st_atime;
	unsigned long	__reserved0;	/* reserved for atime.nanoseconds */
	unsigned long	st_mtime;
	unsigned long	__reserved1;	/* reserved for atime.nanoseconds */
	unsigned long	st_ctime;
	unsigned long	__reserved2;	/* reserved for atime.nanoseconds */
  	long		__unused[3];
};

#endif
