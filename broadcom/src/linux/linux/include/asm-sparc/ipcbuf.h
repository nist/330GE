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
#ifndef _SPARC_IPCBUF_H
#define _SPARC_IPCBUF_H

/* 
 * The ipc64_perm structure for sparc architecture.
 * Note extra padding because this structure is passed back and forth
 * between kernel and user space.
 *
 * Pad space is left for:
 * - 32-bit mode
 * - 32-bit seq
 * - 2 miscellaneous 64-bit values (so that this structure matches
 *				    sparc64 ipc64_perm)
 */

struct ipc64_perm
{
	__kernel_key_t		key;
	__kernel_uid32_t	uid;
	__kernel_gid32_t	gid;
	__kernel_uid32_t	cuid;
	__kernel_gid32_t	cgid;
	unsigned short		__pad1;
	__kernel_mode_t		mode;
	unsigned short		__pad2;
	unsigned short		seq;
	unsigned long long	__unused1;
	unsigned long long	__unused2;
};

#endif /* _SPARC_IPCBUF_H */
