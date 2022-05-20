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
#ifndef __CRIS_MMU_CONTEXT_H
#define __CRIS_MMU_CONTEXT_H

extern int init_new_context(struct task_struct *tsk, struct mm_struct *mm);
extern void get_mmu_context(struct mm_struct *mm);
extern void destroy_context(struct mm_struct *mm);
extern void switch_mm(struct mm_struct *prev, struct mm_struct *next,
		      struct task_struct *tsk, int cpu);

#define activate_mm(prev,next) switch_mm((prev),(next),NULL,smp_processor_id())

/* current active pgd - this is similar to other processors pgd 
 * registers like cr3 on the i386
 */

extern volatile pgd_t *current_pgd;   /* defined in arch/cris/mm/fault.c */

static inline void enter_lazy_tlb(struct mm_struct *mm, struct task_struct *tsk, unsigned cpu)
{
}

#endif
