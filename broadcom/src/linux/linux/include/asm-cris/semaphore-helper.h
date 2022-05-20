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
/* $Id: semaphore-helper.h,v 1.1.1.1 2007/01/25 12:52:04 jiahao_jhou Exp $
 *
 * SMP- and interrupt-safe semaphores helper functions. Generic versions, no
 * optimizations whatsoever... 
 *
 */

#ifndef _ASM_SEMAPHORE_HELPER_H
#define _ASM_SEMAPHORE_HELPER_H

#include <asm/atomic.h>

#define read(a) ((a)->counter)
#define inc(a) (((a)->counter)++)
#define dec(a) (((a)->counter)--)

#define count_inc(a) ((*(a))++)

/*
 * These two _must_ execute atomically wrt each other.
 */
static inline void wake_one_more(struct semaphore * sem)
{
	atomic_inc(&sem->waking);
}

static inline int waking_non_zero(struct semaphore *sem)
{
	unsigned long flags;
	int ret = 0;

	save_and_cli(flags);
	if (read(&sem->waking) > 0) {
		dec(&sem->waking);
		ret = 1;
	}
	restore_flags(flags);
	return ret;
}

static inline int waking_non_zero_interruptible(struct semaphore *sem,
						struct task_struct *tsk)
{
	int ret = 0;
	unsigned long flags;

	save_and_cli(flags);
	if (read(&sem->waking) > 0) {
		dec(&sem->waking);
		ret = 1;
	} else if (signal_pending(tsk)) {
		count_inc(&sem->count);
		ret = -EINTR;
	}
	restore_flags(flags);
	return ret;
}

static inline int waking_non_zero_trylock(struct semaphore *sem)
{
        int ret = 1;
	unsigned long flags;

	save_and_cli(flags);
	if (read(&sem->waking) <= 0)
		count_inc(&sem->count);
	else {
		dec(&sem->waking);
		ret = 0;
	}
	restore_flags(flags);
	return ret;
}

#endif /* _ASM_SEMAPHORE_HELPER_H */


