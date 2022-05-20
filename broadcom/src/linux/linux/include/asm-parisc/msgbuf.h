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
#ifndef _PARISC_MSGBUF_H
#define _PARISC_MSGBUF_H

/* 
 * The msqid64_ds structure for parisc architecture, copied from sparc.
 * Note extra padding because this structure is passed back and forth
 * between kernel and user space.
 *
 * Pad space is left for:
 * - 64-bit time_t to solve y2038 problem
 * - 2 miscellaneous 32-bit values
 */

struct msqid64_ds {
	struct ipc64_perm msg_perm;
#ifndef __LP64__
	unsigned int   __pad1;
#endif
	__kernel_time_t msg_stime;	/* last msgsnd time */
#ifndef __LP64__
	unsigned int   __pad2;
#endif
	__kernel_time_t msg_rtime;	/* last msgrcv time */
#ifndef __LP64__
	unsigned int   __pad3;
#endif
	__kernel_time_t msg_ctime;	/* last change time */
	unsigned int  msg_cbytes;	/* current number of bytes on queue */
	unsigned int  msg_qnum;	/* number of messages in queue */
	unsigned int  msg_qbytes;	/* max number of bytes on queue */
	__kernel_pid_t msg_lspid;	/* pid of last msgsnd */
	__kernel_pid_t msg_lrpid;	/* last receive pid */
	unsigned int  __unused1;
	unsigned int  __unused2;
};

#endif /* _PARISC_MSGBUF_H */
