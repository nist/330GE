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
#ifndef _PARISC_SHMBUF_H
#define _PARISC_SHMBUF_H

/* 
 * The shmid64_ds structure for parisc architecture.
 * Note extra padding because this structure is passed back and forth
 * between kernel and user space.
 *
 * Pad space is left for:
 * - 64-bit time_t to solve y2038 problem
 * - 2 miscellaneous 32-bit values
 */

struct shmid64_ds {
	struct ipc64_perm	shm_perm;	/* operation perms */
#ifndef __LP64__
	unsigned int		__pad1;
#endif
	__kernel_time_t		shm_atime;	/* last attach time */
#ifndef __LP64__
	unsigned int		__pad2;
#endif
	__kernel_time_t		shm_dtime;	/* last detach time */
#ifndef __LP64__
	unsigned int		__pad3;
#endif
	__kernel_time_t		shm_ctime;	/* last change time */
#ifndef __LP64__
	unsigned int		__pad4;
#endif
	size_t			shm_segsz;	/* size of segment (bytes) */
	__kernel_pid_t		shm_cpid;	/* pid of creator */
	__kernel_pid_t		shm_lpid;	/* pid of last operator */
	unsigned int		shm_nattch;	/* no. of current attaches */
	unsigned int		__unused1;
	unsigned int		__unused2;
};

#ifdef __LP64__
#endif
struct shminfo64 {
	unsigned int	shmmax;
	unsigned int	shmmin;
	unsigned int	shmmni;
	unsigned int	shmseg;
	unsigned int	shmall;
	unsigned int	__unused1;
	unsigned int	__unused2;
	unsigned int	__unused3;
	unsigned int	__unused4;
};

#endif /* _PARISC_SHMBUF_H */
