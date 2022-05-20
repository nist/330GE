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
#ifndef _LINUX_DIRENT_H
#define _LINUX_DIRENT_H

struct dirent {
	long		d_ino;
	__kernel_off_t	d_off;
	unsigned short	d_reclen;
	char		d_name[256]; /* We must not include limits.h! */
};

struct dirent64 {
	__u64		d_ino;
	__s64		d_off;
	unsigned short	d_reclen;
	unsigned char	d_type;
	char		d_name[256];
};

#endif
