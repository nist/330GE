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
#ifndef _M68K_SEMAPHORE_HELPER_H
#define _M68K_SEMAPHORE_HELPER_H

/*
 * SMP- and interrupt-safe semaphores helper functions.
 *
 * (C) Copyright 1996 Linus Torvalds
 *
 * m68k version by Andreas Schwab
 */

#include <linux/config.h>

/*
 * These two _must_ execute atomically wrt each other.
 */
static inline void wake_one_more(struct semaphore * sem)
{
	atomic_inc(&sem->waking);
}

#ifndef CONFIG_RMW_INSNS
extern spinlock_t semaphore_wake_lock;
#endif

static inline int waking_non_zero(struct semaphore *sem)
{
	int ret;
#ifndef CONFIG_RMW_INSNS
	unsigned long flags;

	spin_lock_irqsave(&semaphore_wake_lock, flags);
	ret = 0;
	if (atomic_read(&sem->waking) > 0) {
		atomic_dec(&sem->waking);
		ret = 1;
	}
	spin_unlock_irqrestore(&semaphore_wake_lock, flags);
#else
	int tmp1, tmp2;

	__asm__ __volatile__
	  ("1:	movel	%1,%2\n"
	   "    jle	2f\n"
	   "	subql	#1,%2\n"
	   "	casl	%1,%2,%3\n"
	   "	jne	1b\n"
	   "	moveq	#1,%0\n"
	   "2:"
	   : "=d" (ret), "=d" (tmp1), "=d" (tmp2)
	   : "m" (sem->waking), "0" (0), "1" (sem->waking));
#endif

	return ret;
}

/*
 * waking_non_zero_interruptible:
 *	1	got the lock
 *	0	go to sleep
 *	-EINTR	interrupted
 */
static inline int waking_non_zero_interruptible(struct semaphore *sem,
						struct task_struct *tsk)
{
	int ret;
#ifndef CONFIG_RMW_INSNS
	unsigned long flags;

	spin_lock_irqsave(&semaphore_wake_lock, flags);
	ret = 0;
	if (atomic_read(&sem->waking) > 0) {
		atomic_dec(&sem->waking);
		ret = 1;
	} else if (signal_pending(tsk)) {
		atomic_inc(&sem->count);
		ret = -EINTR;
	}
	spin_unlock_irqrestore(&semaphore_wake_lock, flags);
#else
	int tmp1, tmp2;

	__asm__ __volatile__
	  ("1:	movel	%1,%2\n"
	   "	jle	2f\n"
	   "	subql	#1,%2\n"
	   "	casl	%1,%2,%3\n"
	   "	jne	1b\n"
	   "	moveq	#1,%0\n"
	   "	jra	%a4\n"
	   "2:"
	   : "=d" (ret), "=d" (tmp1), "=d" (tmp2)
	   : "m" (sem->waking), "i" (&&next), "0" (0), "1" (sem->waking));
	if (signal_pending(tsk)) {
		atomic_inc(&sem->count);
		ret = -EINTR;
	}
next:
#endif

	return ret;
}

/*
 * waking_non_zero_trylock:
 *	1	failed to lock
 *	0	got the lock
 */
static inline int waking_non_zero_trylock(struct semaphore *sem)
{
	int ret;
#ifndef CONFIG_RMW_INSNS
	unsigned long flags;

	spin_lock_irqsave(&semaphore_wake_lock, flags);
	ret = 1;
	if (atomic_read(&sem->waking) > 0) {
		atomic_dec(&sem->waking);
		ret = 0;
	} else
		atomic_inc(&sem->count);
	spin_unlock_irqrestore(&semaphore_wake_lock, flags);
#else
	int tmp1, tmp2;

	__asm__ __volatile__
	  ("1:	movel	%1,%2\n"
	   "    jle	2f\n"
	   "	subql	#1,%2\n"
	   "	casl	%1,%2,%3\n"
	   "	jne	1b\n"
	   "	moveq	#0,%0\n"
	   "2:"
	   : "=d" (ret), "=d" (tmp1), "=d" (tmp2)
	   : "m" (sem->waking), "0" (1), "1" (sem->waking));
	if (ret)
		atomic_inc(&sem->count);
#endif
	return ret;
}

#endif
