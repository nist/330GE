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
#ifndef __ASM_SPINLOCK_H
#define __ASM_SPINLOCK_H

#include <asm/system.h>

/* Note that PA-RISC has to use `1' to mean unlocked and `0' to mean locked
 * since it only has load-and-zero.
 */

#undef SPIN_LOCK_UNLOCKED
#define SPIN_LOCK_UNLOCKED (spinlock_t) { 1 }

#define spin_lock_init(x)	do { (x)->lock = 1; } while(0)

#define spin_is_locked(x) ((x)->lock == 0)

#define spin_unlock_wait(x)	do { barrier(); } while(((volatile spinlock_t *)(x))->lock == 0)

#define spin_lock(x) do { \
	while (__ldcw (&(x)->lock) == 0) \
		while (((x)->lock) == 0) ; } while (0)

	
#define spin_unlock(x) \
	do { (x)->lock = 1; } while(0)

#define spin_trylock(x) (__ldcw(&(x)->lock) != 0)

/*
 * Read-write spinlocks, allowing multiple readers
 * but only one writer.
 */
typedef struct {
	spinlock_t lock;
	volatile int counter;
} rwlock_t;

#define RW_LOCK_UNLOCKED (rwlock_t) { SPIN_LOCK_UNLOCKED, 0 }

#define rwlock_init(lp)	do { *(lp) = RW_LOCK_UNLOCKED; } while (0)

/* read_lock, read_unlock are pretty straightforward.  Of course it somehow
 * sucks we end up saving/restoring flags twice for read_lock_irqsave aso. */

static inline void read_lock(rwlock_t *rw)
{
	unsigned long flags;
	spin_lock_irqsave(&rw->lock, flags);

	rw->counter++;

	spin_unlock_irqrestore(&rw->lock, flags);
}

static inline void read_unlock(rwlock_t *rw)
{
	unsigned long flags;
	spin_lock_irqsave(&rw->lock, flags);

	rw->counter--;

	spin_unlock_irqrestore(&rw->lock, flags);
}

/* write_lock is less trivial.  We optimistically grab the lock and check
 * if we surprised any readers.  If so we release the lock and wait till
 * they're all gone before trying again
 *
 * Also note that we don't use the _irqsave / _irqrestore suffixes here.
 * If we're called with interrupts enabled and we've got readers (or other
 * writers) in interrupt handlers someone fucked up and we'd dead-lock
 * sooner or later anyway.   prumpf */

static inline void write_lock(rwlock_t *rw)
{
retry:
	spin_lock(&rw->lock);

	if(rw->counter != 0) {
		/* this basically never happens */
		spin_unlock(&rw->lock);

		while(rw->counter != 0);

		goto retry;
	}

	/* got it.  now leave without unlocking */
}

/* write_unlock is absolutely trivial - we don't have to wait for anything */

static inline void write_unlock(rwlock_t *rw)
{
	spin_unlock(&rw->lock);
}

#endif /* __ASM_SPINLOCK_H */
