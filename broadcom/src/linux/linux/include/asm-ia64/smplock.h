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
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/sched.h>

#include <asm/current.h>
#include <asm/hardirq.h>

extern spinlock_t kernel_flag;

#define kernel_locked()		spin_is_locked(&kernel_flag)

/*
 * Release global kernel lock and global interrupt lock
 */
static __inline__ void
release_kernel_lock(struct task_struct *task, int cpu)
{
	if (task->lock_depth >= 0)
		spin_unlock(&kernel_flag);
	release_irqlock(cpu);
	__sti();
}

/*
 * Re-acquire the kernel lock
 */
static __inline__ void
reacquire_kernel_lock(struct task_struct *task)
{
	if (task->lock_depth >= 0)
		spin_lock(&kernel_flag);
}

/*
 * Getting the big kernel lock.
 *
 * This cannot happen asynchronously,
 * so we only need to worry about other
 * CPU's.
 */
static __inline__ void
lock_kernel(void)
{
	if (!++current->lock_depth)
		spin_lock(&kernel_flag);
}

static __inline__ void
unlock_kernel(void)
{
	if (--current->lock_depth < 0)
		spin_unlock(&kernel_flag);
}
