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
#include <linux/module.h>
#include <linux/spinlock.h>
#include <asm/atomic.h>

/*
 * This is an architecture-neutral, but slow,
 * implementation of the notion of "decrement
 * a reference count, and return locked if it
 * decremented to zero".
 *
 * NOTE NOTE NOTE! This is _not_ equivalent to
 *
 *	if (atomic_dec_and_test(&atomic)) {
 *		spin_lock(&lock);
 *		return 1;
 *	}
 *	return 0;
 *
 * because the spin-lock and the decrement must be
 * "atomic".
 *
 * This slow version gets the spinlock unconditionally,
 * and releases it if it isn't needed. Architectures
 * are encouraged to come up with better approaches,
 * this is trivially done efficiently using a load-locked
 * store-conditional approach, for example.
 */

#ifndef ATOMIC_DEC_AND_LOCK
int atomic_dec_and_lock(atomic_t *atomic, spinlock_t *lock)
{
	spin_lock(lock);
	if (atomic_dec_and_test(atomic))
		return 1;
	spin_unlock(lock);
	return 0;
}

EXPORT_SYMBOL(atomic_dec_and_lock);
#endif
