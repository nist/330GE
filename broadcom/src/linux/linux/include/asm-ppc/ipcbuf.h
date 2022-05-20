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
 * BK Id: SCCS/s.ipcbuf.h 1.5 05/17/01 18:14:24 cort
 */
#ifndef __PPC_IPCBUF_H__
#define __PPC_IPCBUF_H__

/*
 * The ipc64_perm structure for PPC architecture.
 * Note extra padding because this structure is passed back and forth
 * between kernel and user space.
 *
 * Pad space is left for:
 * - 1 32-bit value to fill up for 8-byte alignment
 * - 2 miscellaneous 64-bit values (so that this structure matches
 *                                  PPC64 ipc64_perm)
 */

struct ipc64_perm
{
	__kernel_key_t		key;
	__kernel_uid_t		uid;
	__kernel_gid_t		gid;
	__kernel_uid_t		cuid;
	__kernel_gid_t		cgid;
	__kernel_mode_t		mode;
	unsigned long		seq;
	unsigned int		__pad2;
	unsigned long long	__unused1;
	unsigned long long	__unused2;
};

#endif /* __PPC_IPCBUF_H__ */
