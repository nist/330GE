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
 *		INETPEER - A storage for permanent information about peers
 *
 *  Version:	$Id: inetpeer.h,v 1.1.1.1 2007/01/25 12:52:07 jiahao_jhou Exp $
 *
 *  Authors:	Andrey V. Savochkin <saw@msu.ru>
 */

#ifndef _NET_INETPEER_H
#define _NET_INETPEER_H

#include <linux/types.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <asm/atomic.h>

struct inet_peer
{
	struct inet_peer	*avl_left, *avl_right;
	struct inet_peer	*unused_next, **unused_prevp;
	atomic_t		refcnt;
	unsigned long		dtime;		/* the time of last use of not
						 * referenced entries */
	__u32			v4daddr;	/* peer's address */
	__u16			avl_height;
	__u16			ip_id_count;	/* IP ID for the next packet */
	__u32			tcp_ts;
	unsigned long		tcp_ts_stamp;
};

void			inet_initpeers(void) __init;

/* can be called with or without local BH being disabled */
struct inet_peer	*inet_getpeer(__u32 daddr, int create);

extern spinlock_t inet_peer_unused_lock;
extern struct inet_peer *inet_peer_unused_head;
extern struct inet_peer **inet_peer_unused_tailp;
/* can be called from BH context or outside */
static inline void	inet_putpeer(struct inet_peer *p)
{
	spin_lock_bh(&inet_peer_unused_lock);
	if (atomic_dec_and_test(&p->refcnt)) {
		p->unused_prevp = inet_peer_unused_tailp;
		p->unused_next = NULL;
		*inet_peer_unused_tailp = p;
		inet_peer_unused_tailp = &p->unused_next;
		p->dtime = jiffies;
	}
	spin_unlock_bh(&inet_peer_unused_lock);
}

extern spinlock_t inet_peer_idlock;
/* can be called with or without local BH being disabled */
static inline __u16	inet_getid(struct inet_peer *p)
{
	__u16 id;

	spin_lock_bh(&inet_peer_idlock);
	id = p->ip_id_count++;
	spin_unlock_bh(&inet_peer_idlock);
	return id;
}

#endif /* _NET_INETPEER_H */
