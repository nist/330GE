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
#ifndef ASMARM_SEMAPHORE_HELPER_H
#define ASMARM_SEMAPHORE_HELPER_H

/*
 * These two _must_ execute atomically wrt each other.
 */
static inline void wake_one_more(struct semaphore * sem)
{
	unsigned long flags;

	spin_lock_irqsave(&semaphore_wake_lock, flags);
	if (atomic_read(&sem->count) <= 0)
		sem->waking++;
	spin_unlock_irqrestore(&semaphore_wake_lock, flags);
}

static inline int waking_non_zero(struct semaphore *sem)
{
	unsigned long flags;
	int ret = 0;

	spin_lock_irqsave(&semaphore_wake_lock, flags);
	if (sem->waking > 0) {
		sem->waking--;
		ret = 1;
	}
	spin_unlock_irqrestore(&semaphore_wake_lock, flags);
	return ret;
}

/*
 * waking non zero interruptible
 *	1	got the lock
 *	0	go to sleep
 *	-EINTR	interrupted
 *
 * We must undo the sem->count down_interruptible() increment while we are
 * protected by the spinlock in order to make this atomic_inc() with the
 * atomic_read() in wake_one_more(), otherwise we can race. -arca
 */
static inline int waking_non_zero_interruptible(struct semaphore *sem,
						struct task_struct *tsk)
{
	unsigned long flags;
	int ret = 0;

	spin_lock_irqsave(&semaphore_wake_lock, flags);
	if (sem->waking > 0) {
		sem->waking--;
		ret = 1;
	} else if (signal_pending(tsk)) {
		atomic_inc(&sem->count);
		ret = -EINTR;
	}
	spin_unlock_irqrestore(&semaphore_wake_lock, flags);
	return ret;	
}

/*
 * waking_non_zero_try_lock:
 *	1	failed to lock
 *	0	got the lock
 *
 * We must undo the sem->count down_interruptible() increment while we are
 * protected by the spinlock in order to make this atomic_inc() with the
 * atomic_read() in wake_one_more(), otherwise we can race. -arca
 */
static inline int waking_non_zero_trylock(struct semaphore *sem)
{
	unsigned long flags;
	int ret = 1;

	spin_lock_irqsave(&semaphore_wake_lock, flags);
	if (sem->waking <= 0)
		atomic_inc(&sem->count);
	else {
		sem->waking--;
		ret = 0;
	}
	spin_unlock_irqrestore(&semaphore_wake_lock, flags);
	return ret;
}

#endif
