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
 * BK Id: SCCS/s.atomic.h 1.15 10/28/01 10:37:22 trini
 */
/*
 * PowerPC atomic operations
 */

#ifndef _ASM_PPC_ATOMIC_H_ 
#define _ASM_PPC_ATOMIC_H_

typedef struct { volatile int counter; } atomic_t;

#ifdef __KERNEL__

#define ATOMIC_INIT(i)	{ (i) }

#define atomic_read(v)		((v)->counter)
#define atomic_set(v,i)		(((v)->counter) = (i))

extern void atomic_clear_mask(unsigned long mask, unsigned long *addr);
extern void atomic_set_mask(unsigned long mask, unsigned long *addr);

#ifdef CONFIG_SMP
#define SMP_ISYNC	"\n\tisync"
#else
#define SMP_ISYNC
#endif

static __inline__ void atomic_add(int a, atomic_t *v)
{
	int t;

	__asm__ __volatile__(
"1:	lwarx	%0,0,%3		# atomic_add\n\
	add	%0,%2,%0\n\
	stwcx.	%0,0,%3\n\
	bne-	1b"
	: "=&r" (t), "=m" (v->counter)
	: "r" (a), "r" (&v->counter), "m" (v->counter)
	: "cc");
}

static __inline__ int atomic_add_return(int a, atomic_t *v)
{
	int t;

	__asm__ __volatile__(
"1:	lwarx	%0,0,%2		# atomic_add_return\n\
	add	%0,%1,%0\n\
	stwcx.	%0,0,%2\n\
	bne-	1b"
	SMP_ISYNC
	: "=&r" (t)
	: "r" (a), "r" (&v->counter)
	: "cc", "memory");

	return t;
}

static __inline__ void atomic_sub(int a, atomic_t *v)
{
	int t;

	__asm__ __volatile__(
"1:	lwarx	%0,0,%3		# atomic_sub\n\
	subf	%0,%2,%0\n\
	stwcx.	%0,0,%3\n\
	bne-	1b"
	: "=&r" (t), "=m" (v->counter)
	: "r" (a), "r" (&v->counter), "m" (v->counter)
	: "cc");
}

static __inline__ int atomic_sub_return(int a, atomic_t *v)
{
	int t;

	__asm__ __volatile__(
"1:	lwarx	%0,0,%2		# atomic_sub_return\n\
	subf	%0,%1,%0\n\
	stwcx.	%0,0,%2\n\
	bne-	1b"
	SMP_ISYNC
	: "=&r" (t)
	: "r" (a), "r" (&v->counter)
	: "cc", "memory");

	return t;
}

static __inline__ void atomic_inc(atomic_t *v)
{
	int t;

	__asm__ __volatile__(
"1:	lwarx	%0,0,%2		# atomic_inc\n\
	addic	%0,%0,1\n\
	stwcx.	%0,0,%2\n\
	bne-	1b"
	: "=&r" (t), "=m" (v->counter)
	: "r" (&v->counter), "m" (v->counter)
	: "cc");
}

static __inline__ int atomic_inc_return(atomic_t *v)
{
	int t;

	__asm__ __volatile__(
"1:	lwarx	%0,0,%1		# atomic_inc_return\n\
	addic	%0,%0,1\n\
	stwcx.	%0,0,%1\n\
	bne-	1b"
	SMP_ISYNC
	: "=&r" (t)
	: "r" (&v->counter)
	: "cc", "memory");

	return t;
}

static __inline__ void atomic_dec(atomic_t *v)
{
	int t;

	__asm__ __volatile__(
"1:	lwarx	%0,0,%2		# atomic_dec\n\
	addic	%0,%0,-1\n\
	stwcx.	%0,0,%2\n\
	bne-	1b"
	: "=&r" (t), "=m" (v->counter)
	: "r" (&v->counter), "m" (v->counter)
	: "cc");
}

static __inline__ int atomic_dec_return(atomic_t *v)
{
	int t;

	__asm__ __volatile__(
"1:	lwarx	%0,0,%1		# atomic_dec_return\n\
	addic	%0,%0,-1\n\
	stwcx.	%0,0,%1\n\
	bne-	1b"
	SMP_ISYNC
	: "=&r" (t)
	: "r" (&v->counter)
	: "cc", "memory");

	return t;
}

#define atomic_sub_and_test(a, v)	(atomic_sub_return((a), (v)) == 0)
#define atomic_dec_and_test(v)		(atomic_dec_return((v)) == 0)

/*
 * Atomically test *v and decrement if it is greater than 0.
 * The function returns the old value of *v minus 1.
 */
static __inline__ int atomic_dec_if_positive(atomic_t *v)
{
	int t;

	__asm__ __volatile__(
"1:	lwarx	%0,0,%1		# atomic_dec_if_positive\n\
	addic.	%0,%0,-1\n\
	blt-	2f\n\
	stwcx.	%0,0,%1\n\
	bne-	1b"
	SMP_ISYNC
	"\n\
2:"	: "=&r" (t)
	: "r" (&v->counter)
	: "cc", "memory");

	return t;
}

#define smp_mb__before_atomic_dec()	smp_mb()
#define smp_mb__after_atomic_dec()	smp_mb()
#define smp_mb__before_atomic_inc()	smp_mb()
#define smp_mb__after_atomic_inc()	smp_mb()

#endif /* __KERNEL__ */
#endif /* _ASM_PPC_ATOMIC_H_ */