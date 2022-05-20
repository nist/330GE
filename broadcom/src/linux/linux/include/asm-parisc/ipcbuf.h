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
#ifndef __PARISC_IPCBUF_H__
#define __PARISC_IPCBUF_H__

/*
 * The ipc64_perm structure for PA-RISC is almost identical to
 * kern_ipc_perm as we have always had 32-bit UIDs and GIDs in the kernel.
 * 'seq' has been changed from long to int so that it's the same size
 * on 64-bit kernels as on 32-bit ones.
 */

struct ipc64_perm
{
	key_t           key;
	uid_t           uid;
	gid_t           gid;
	uid_t           cuid;
	gid_t           cgid;
	unsigned short int	__pad1;
	mode_t          mode;
	unsigned short int	__pad2;
	unsigned short int	seq;
	unsigned int	__pad3;
	unsigned long long int __unused1;
	unsigned long long int __unused2;
};

#endif /* __PARISC_IPCBUF_H__ */
