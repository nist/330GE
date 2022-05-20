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
 * BK Id: SCCS/s.msgbuf.h 1.5 05/17/01 18:14:25 cort
 */
#ifndef _PPC_MSGBUF_H
#define _PPC_MSGBUF_H

/* 
 * The msqid64_ds structure for the PPC architecture.
 */

struct msqid64_ds {
	struct ipc64_perm msg_perm;
	unsigned int	__unused1;
	__kernel_time_t msg_stime;	/* last msgsnd time */
	unsigned int	__unused2;
	__kernel_time_t msg_rtime;	/* last msgrcv time */
	unsigned int	__unused3;
	__kernel_time_t msg_ctime;	/* last change time */
	unsigned long  msg_cbytes;	/* current number of bytes on queue */
	unsigned long  msg_qnum;	/* number of messages in queue */
	unsigned long  msg_qbytes;	/* max number of bytes on queue */
	__kernel_pid_t msg_lspid;	/* pid of last msgsnd */
	__kernel_pid_t msg_lrpid;	/* last receive pid */
	unsigned long  __unused4;
	unsigned long  __unused5;
};

#endif /* _PPC_MSGBUF_H */
