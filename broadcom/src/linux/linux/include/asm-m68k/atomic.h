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
#ifndef __ARCH_M68K_ATOMIC__
#define __ARCH_M68K_ATOMIC__

/*
 * Atomic operations that C can't guarantee us.  Useful for
 * resource counting etc..
 */

/*
 * We do not have SMP m68k systems, so we don't have to deal with that.
 */

typedef struct { int counter; } atomic_t;
#define ATOMIC_INIT(i)	{ (i) }

#define atomic_read(v)		((v)->counter)
#define atomic_set(v, i)	(((v)->counter) = i)

static __inline__ void atomic_add(int i, atomic_t *v)
{
	__asm__ __volatile__("addl %1,%0" : "=m" (*v) : "id" (i), "0" (*v));
}

static __inline__ void atomic_sub(int i, atomic_t *v)
{
	__asm__ __volatile__("subl %1,%0" : "=m" (*v) : "id" (i), "0" (*v));
}

static __inline__ void atomic_inc(volatile atomic_t *v)
{
	__asm__ __volatile__("addql #1,%0" : "=m" (*v): "0" (*v));
}

static __inline__ void atomic_dec(volatile atomic_t *v)
{
	__asm__ __volatile__("subql #1,%0" : "=m" (*v): "0" (*v));
}

static __inline__ int atomic_dec_and_test(volatile atomic_t *v)
{
	char c;
	__asm__ __volatile__("subql #1,%1; seq %0" : "=d" (c), "=m" (*v): "1" (*v));
	return c != 0;
}

#define atomic_clear_mask(mask, v) \
	__asm__ __volatile__("andl %1,%0" : "=m" (*v) : "id" (~(mask)),"0"(*v))

#define atomic_set_mask(mask, v) \
	__asm__ __volatile__("orl %1,%0" : "=m" (*v) : "id" (mask),"0"(*v))

/* Atomic operations are already serializing */
#define smp_mb__before_atomic_dec()	barrier()
#define smp_mb__after_atomic_dec()	barrier()
#define smp_mb__before_atomic_inc()	barrier()
#define smp_mb__after_atomic_inc()	barrier()

#endif /* __ARCH_M68K_ATOMIC __ */
