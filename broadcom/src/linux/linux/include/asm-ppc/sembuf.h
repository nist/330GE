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
 * BK Id: SCCS/s.sembuf.h 1.5 05/17/01 18:14:25 cort
 */
#ifndef _PPC_SEMBUF_H
#define _PPC_SEMBUF_H

/* 
 * The semid64_ds structure for PPC architecture.
 */

struct semid64_ds {
	struct ipc64_perm sem_perm;		/* permissions .. see ipc.h */
	unsigned int	__unused1;
	__kernel_time_t	sem_otime;		/* last semop time */
	unsigned int	__unused2;
	__kernel_time_t	sem_ctime;		/* last change time */
	unsigned long	sem_nsems;		/* no. of semaphores in array */
	unsigned long	__unused3;
	unsigned long	__unused4;
};

#endif /* _PPC_SEMBUF_H */
