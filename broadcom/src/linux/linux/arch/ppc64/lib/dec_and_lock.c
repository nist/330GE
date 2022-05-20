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
 * ppc64 version of atomic_dec_and_lock() using cmpxchg
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/spinlock.h>
#include <asm/system.h>
#include <asm/atomic.h>

int atomic_dec_and_lock(atomic_t *atomic, spinlock_t *lock)
{
	int counter;
	int newcount;

repeat:
	counter = atomic_read(atomic);
	newcount = counter-1;

	if (!newcount)
		goto slow_path;

	newcount = cmpxchg(&atomic->counter, counter, newcount);

	if (newcount != counter)
		goto repeat;
	return 0;

slow_path:
	spin_lock(lock);
	if (atomic_dec_and_test(atomic))
		return 1;
	spin_unlock(lock);
	return 0;
}
