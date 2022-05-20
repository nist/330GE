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
#ifndef __LINUX_NET_AFUNIX_H
#define __LINUX_NET_AFUNIX_H
extern void unix_proto_init(struct net_proto *pro);
extern void unix_inflight(struct file *fp);
extern void unix_notinflight(struct file *fp);
typedef struct sock unix_socket;
extern void unix_gc(void);

#define UNIX_HASH_SIZE	256

extern unix_socket *unix_socket_table[UNIX_HASH_SIZE+1];
extern rwlock_t unix_table_lock;

extern atomic_t unix_tot_inflight;


#define forall_unix_sockets(i, s) for (i=0; i<=UNIX_HASH_SIZE; i++) \
                                    for (s=unix_socket_table[i]; s; s=s->next)

struct unix_address
{
	atomic_t	refcnt;
	int		len;
	unsigned	hash;
	struct sockaddr_un name[0];
};

struct unix_skb_parms
{
	struct ucred		creds;		/* Skb credentials	*/
	struct scm_fp_list	*fp;		/* Passed files		*/
};

#define UNIXCB(skb) 	(*(struct unix_skb_parms*)&((skb)->cb))
#define UNIXCREDS(skb)	(&UNIXCB((skb)).creds)

#define unix_state_rlock(s)	read_lock(&(s)->protinfo.af_unix.lock)
#define unix_state_runlock(s)	read_unlock(&(s)->protinfo.af_unix.lock)
#define unix_state_wlock(s)	write_lock(&(s)->protinfo.af_unix.lock)
#define unix_state_wunlock(s)	write_unlock(&(s)->protinfo.af_unix.lock)

#endif
