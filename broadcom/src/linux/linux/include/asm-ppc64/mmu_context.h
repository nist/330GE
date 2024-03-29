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
#ifndef __PPC64_MMU_CONTEXT_H
#define __PPC64_MMU_CONTEXT_H

#include <linux/spinlock.h>	
#include <linux/kernel.h>	
#include <linux/mm.h>	
#include <asm/mmu.h>	
#include <asm/ppcdebug.h>	

/*
 * Copyright (C) 2001 PPC 64 Team, IBM Corp
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#define NO_CONTEXT		0
#define FIRST_USER_CONTEXT	0x10    /* First 16 reserved for kernel */
#define LAST_USER_CONTEXT	0x8000  /* Same as PID_MAX for now... */
#define NUM_USER_CONTEXT	(LAST_USER_CONTEXT-FIRST_USER_CONTEXT)

/* Choose whether we want to implement our context
 * number allocator as a LIFO or FIFO queue.
 */
#define MMU_CONTEXT_LIFO

struct mmu_context_queue_t {
	spinlock_t lock;
	long head;
	long tail;
	long size;
	mm_context_t elements[LAST_USER_CONTEXT];
};

extern struct mmu_context_queue_t mmu_context_queue;

static inline void
enter_lazy_tlb(struct mm_struct *mm, struct task_struct *tsk, unsigned cpu)
{
}

extern void flush_stab(void);

/*
 * The context number queue has underflowed.
 * Meaning: we tried to push a context number that was freed
 * back onto the context queue and the queue was already full.
 */
static inline void
mmu_context_underflow(void)
{
	printk(KERN_DEBUG "mmu_context_underflow\n");
	panic("mmu_context_underflow");
}


/*
 * Set up the context for a new address space.
 */
static inline int
init_new_context(struct task_struct *tsk, struct mm_struct *mm)
{
	long head, size;

	spin_lock( &mmu_context_queue.lock );

	if ( (size = mmu_context_queue.size) <= 0 ) {
		spin_unlock( &mmu_context_queue.lock );
		return -ENOMEM;
	}

	head = mmu_context_queue.head;
	mm->context = mmu_context_queue.elements[head];

	head = (head < LAST_USER_CONTEXT-1) ? head+1 : 0;
	mmu_context_queue.head = head;
	mmu_context_queue.size = size-1;

	spin_unlock( &mmu_context_queue.lock );

	return 0;
}

/*
 * We're finished using the context for an address space.
 */
static inline void
destroy_context(struct mm_struct *mm)
{
	long index, size = mmu_context_queue.size;

	spin_lock( &mmu_context_queue.lock );

	if ( (size = mmu_context_queue.size) >= NUM_USER_CONTEXT ) {
		spin_unlock( &mmu_context_queue.lock );
		mmu_context_underflow();
	}

#ifdef MMU_CONTEXT_LIFO
	index = mmu_context_queue.head;
	index = (index > 0) ? index-1 : LAST_USER_CONTEXT-1;
	mmu_context_queue.head = index;
#else
	index = mmu_context_queue.tail;
	index = (index < LAST_USER_CONTEXT-1) ? index+1 : 0;
	mmu_context_queue.tail = index;
#endif

	mmu_context_queue.size = size+1;
	mmu_context_queue.elements[index] = mm->context;

	spin_unlock( &mmu_context_queue.lock );
}


/*
 * switch_mm is the entry point called from the architecture independent
 * code in kernel/sched.c
 */
static inline void
switch_mm(struct mm_struct *prev, struct mm_struct *next,
	  struct task_struct *tsk, int cpu)
{
	tsk->thread.pgdir = next->pgd;	/* cache the pgdir in the thread 
					   maybe not needed any more */
	flush_stab();
}

/*
 * After we have set current->mm to a new value, this activates
 * the context for the new mm so we see the new mappings.
 */
static inline void
activate_mm(struct mm_struct *active_mm, struct mm_struct *mm)
{
	current->thread.pgdir = mm->pgd;
	flush_stab();
}


#define VSID_RANDOMIZER 42470972311
#define VSID_MASK	0xfffffffff


/* This is only valid for kernel (including vmalloc, imalloc and bolted) EA's
 */
static inline unsigned long
get_kernel_vsid( unsigned long ea )
{
	unsigned long ordinal, vsid;
	
	ordinal = (((ea >> 28) & 0x1fffff) * LAST_USER_CONTEXT) | (ea >> 60);
	vsid = (ordinal * VSID_RANDOMIZER) & VSID_MASK;

	ifppcdebug(PPCDBG_HTABSTRESS) {
		/* For debug, this path creates a very poor vsid distribuition.
		 * A user program can access virtual addresses in the form
		 * 0x0yyyyxxxx000 where yyyy = xxxx to cause multiple mappings
		 * to hash to the same page table group.
		 */ 
		ordinal = ((ea >> 28) & 0x1fff) | (ea >> 44);
		vsid = ordinal & VSID_MASK;
	}

	return vsid;
} 

/* This is only valid for user EA's (user EA's do not exceed 2^41 (EADDR_SIZE))
 */
static inline unsigned long
get_vsid( unsigned long context, unsigned long ea )
{
	unsigned long ordinal, vsid;

	ordinal = (((ea >> 28) & 0x1fffff) * LAST_USER_CONTEXT) | context;
	vsid = (ordinal * VSID_RANDOMIZER) & VSID_MASK;

	ifppcdebug(PPCDBG_HTABSTRESS) {
		/* See comment above. */
		ordinal = ((ea >> 28) & 0x1fff) | (context << 16);
		vsid = ordinal & VSID_MASK;
	}

	return vsid;
}

#endif /* __PPC64_MMU_CONTEXT_H */
