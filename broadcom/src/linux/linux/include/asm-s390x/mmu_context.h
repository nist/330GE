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
 *  include/asm-s390/mmu_context.h
 *
 *  S390 version
 *
 *  Derived from "include/asm-i386/mmu_context.h"
 */

#ifndef __S390_MMU_CONTEXT_H
#define __S390_MMU_CONTEXT_H

/*
 * get a new mmu context.. S390 don't know about contexts.
 */
#define init_new_context(tsk,mm)        0

#define destroy_context(mm)             flush_tlb_mm(mm)

static inline void enter_lazy_tlb(struct mm_struct *mm,
                                  struct task_struct *tsk, unsigned cpu)
{
}

static inline void switch_mm(struct mm_struct *prev, struct mm_struct *next,
                             struct task_struct *tsk, unsigned cpu)
{
        unsigned long pgd;

        if (prev != next) {
                pgd = (__pa(next->pgd)&PAGE_MASK) | (_REGION_TABLE|USER_STD_MASK);
                /* Load page tables */
                asm volatile("    lctlg 7,7,%0\n"   /* secondary space */
                             "    lctlg 13,13,%0\n" /* home space */
                             : : "m" (pgd) );
        }
	set_bit(cpu, &next->cpu_vm_mask);
}

extern inline void activate_mm(struct mm_struct *prev,
                               struct mm_struct *next)
{
        switch_mm(prev, next, current, smp_processor_id());
}

#endif
