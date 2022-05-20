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
#ifndef _LINUX_LOCKS_H
#define _LINUX_LOCKS_H

#ifndef _LINUX_MM_H
#include <linux/mm.h>
#endif
#ifndef _LINUX_PAGEMAP_H
#include <linux/pagemap.h>
#endif

/*
 * Buffer cache locking - note that interrupts may only unlock, not
 * lock buffers.
 */
extern void __wait_on_buffer(struct buffer_head *);

static inline void wait_on_buffer(struct buffer_head * bh)
{
	if (test_bit(BH_Lock, &bh->b_state))
		__wait_on_buffer(bh);
}

static inline void lock_buffer(struct buffer_head * bh)
{
	while (test_and_set_bit(BH_Lock, &bh->b_state))
		__wait_on_buffer(bh);
}

extern void FASTCALL(unlock_buffer(struct buffer_head *bh));

/*
 * super-block locking. Again, interrupts may only unlock
 * a super-block (although even this isn't done right now.
 * nfs may need it).
 */

static inline void lock_super(struct super_block * sb)
{
	down(&sb->s_lock);
}

static inline void unlock_super(struct super_block * sb)
{
	up(&sb->s_lock);
}

#endif /* _LINUX_LOCKS_H */

