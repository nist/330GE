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
 * arch/alpha/lib/dec_and_lock.c
 *
 * ll/sc version of atomic_dec_and_lock()
 * 
 */

#include <linux/spinlock.h>
#include <asm/atomic.h>

  asm (".text					\n\
	.global atomic_dec_and_lock		\n\
	.ent atomic_dec_and_lock		\n\
	.align	4				\n\
atomic_dec_and_lock:				\n\
	.prologue 0				\n\
1:	ldl_l	$1, 0($16)			\n\
	subl	$1, 1, $1			\n\
	beq	$1, 2f				\n\
	stl_c	$1, 0($16)			\n\
	beq	$1, 4f				\n\
	mb					\n\
	clr	$0				\n\
	ret					\n\
2:	br	$29, 3f				\n\
3:	ldgp	$29, 0($29)			\n\
	br	$atomic_dec_and_lock_1..ng	\n\
	.subsection 2				\n\
4:	br	1b				\n\
	.previous				\n\
	.end atomic_dec_and_lock");

static int __attribute__((unused))
atomic_dec_and_lock_1(atomic_t *atomic, spinlock_t *lock)
{
	/* Slow path */
	spin_lock(lock);
	if (atomic_dec_and_test(atomic))
		return 1;
	spin_unlock(lock);
	return 0;
}
