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
#ifndef _INET_COMMON_H
#define _INET_COMMON_H

extern struct proto_ops		inet_stream_ops;
extern struct proto_ops		inet_dgram_ops;

/*
 *	INET4 prototypes used by INET6
 */

extern void			inet_remove_sock(struct sock *sk1);
extern void			inet_put_sock(unsigned short num, 
					      struct sock *sk);
extern int			inet_release(struct socket *sock);
extern int			inet_stream_connect(struct socket *sock,
						    struct sockaddr * uaddr,
						    int addr_len, int flags);
extern int			inet_dgram_connect(struct socket *sock, 
						   struct sockaddr * uaddr,
						   int addr_len, int flags);
extern int			inet_accept(struct socket *sock, 
					    struct socket *newsock, int flags);
extern int			inet_recvmsg(struct socket *sock, 
					     struct msghdr *ubuf, 
					     int size, int flags, struct scm_cookie *scm);
extern int			inet_sendmsg(struct socket *sock, 
					     struct msghdr *msg, 
					     int size, struct scm_cookie *scm);
extern int			inet_shutdown(struct socket *sock, int how);
extern unsigned int		inet_poll(struct file * file, struct socket *sock, struct poll_table_struct *wait);
extern int			inet_setsockopt(struct socket *sock, int level,
						int optname, char *optval, 
						int optlen);
extern int			inet_getsockopt(struct socket *sock, int level,
						int optname, char *optval, 
						int *optlen);
extern int			inet_fcntl(struct socket *sock, 
					   unsigned int cmd, 
					   unsigned long arg);
extern int			inet_listen(struct socket *sock, int backlog);

extern void			inet_sock_release(struct sock *sk);
extern void			inet_sock_destruct(struct sock *sk);
extern atomic_t			inet_sock_nr;

#endif


