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
 * <asm/smplock.h>
 *
 * Default SMP lock implementation
 */
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>

extern spinlock_t kernel_flag;

#define kernel_locked()			\
	(spin_is_locked(&kernel_flag) &&\
	 (current->lock_depth >= 0))

/*
 * Release global kernel lock and global interrupt lock
 */
#define release_kernel_lock(task, cpu) \
do { \
	if (task->lock_depth >= 0) \
		spin_unlock(&kernel_flag); \
	release_irqlock(cpu); \
	__sti(); \
} while (0)

/*
 * Re-acquire the kernel lock
 */
#define reacquire_kernel_lock(task) \
do { \
	if (task->lock_depth >= 0) \
		spin_lock(&kernel_flag); \
} while (0)


/*
 * Getting the big kernel lock.
 *
 * This cannot happen asynchronously,
 * so we only need to worry about other
 * CPU's.
 */
#define lock_kernel() \
do { \
	if (!++current->lock_depth) \
		spin_lock(&kernel_flag); \
} while(0)

#define unlock_kernel() \
do { \
	if (--current->lock_depth < 0) \
		spin_unlock(&kernel_flag); \
} while(0)
