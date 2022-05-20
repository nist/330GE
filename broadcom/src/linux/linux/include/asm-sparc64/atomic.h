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
/* $Id: atomic.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $
 * atomic.h: Thankfully the V9 is at least reasonable for this
 *           stuff.
 *
 * Copyright (C) 1996, 1997, 2000 David S. Miller (davem@redhat.com)
 */

#ifndef __ARCH_SPARC64_ATOMIC__
#define __ARCH_SPARC64_ATOMIC__

typedef struct { volatile int counter; } atomic_t;
#define ATOMIC_INIT(i)	{ (i) }

#define atomic_read(v)		((v)->counter)
#define atomic_set(v, i)	(((v)->counter) = i)

extern int __atomic_add(int, atomic_t *);
extern int __atomic_sub(int, atomic_t *);

#define atomic_add(i, v) ((void)__atomic_add(i, v))
#define atomic_sub(i, v) ((void)__atomic_sub(i, v))

#define atomic_add_return(i, v) __atomic_add(i, v)
#define atomic_sub_return(i, v) __atomic_sub(i, v)

#define atomic_dec_return(v) __atomic_sub(1, v)
#define atomic_inc_return(v) __atomic_add(1, v)

#define atomic_sub_and_test(i, v) (__atomic_sub(i, v) == 0)
#define atomic_dec_and_test(v) (__atomic_sub(1, v) == 0)

#define atomic_inc(v) ((void)__atomic_add(1, v))
#define atomic_dec(v) ((void)__atomic_sub(1, v))

/* Atomic operations are already serializing */
#define smp_mb__before_atomic_dec()	barrier()
#define smp_mb__after_atomic_dec()	barrier()
#define smp_mb__before_atomic_inc()	barrier()
#define smp_mb__after_atomic_inc()	barrier()

#endif /* !(__ARCH_SPARC64_ATOMIC__) */
