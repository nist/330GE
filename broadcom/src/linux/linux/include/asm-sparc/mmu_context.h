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
#ifndef __SPARC_MMU_CONTEXT_H
#define __SPARC_MMU_CONTEXT_H

#include <asm/btfixup.h>

#ifndef __ASSEMBLY__

static inline void enter_lazy_tlb(struct mm_struct *mm, struct task_struct *tsk, unsigned cpu)
{
}

/*
 * Initialize a new mmu context.  This is invoked when a new
 * address space instance (unique or shared) is instantiated.
 */
#define init_new_context(tsk, mm) (((mm)->context = NO_CONTEXT), 0)

/*
 * Destroy a dead context.  This occurs when mmput drops the
 * mm_users count to zero, the mmaps have been released, and
 * all the page tables have been flushed.  Our job is to destroy
 * any remaining processor-specific state.
 */
BTFIXUPDEF_CALL(void, destroy_context, struct mm_struct *)

#define destroy_context(mm) BTFIXUP_CALL(destroy_context)(mm)

/* Switch the current MM context. */
BTFIXUPDEF_CALL(void, switch_mm, struct mm_struct *, struct mm_struct *, struct task_struct *, int)

#define switch_mm(old_mm, mm, tsk, cpu) BTFIXUP_CALL(switch_mm)(old_mm, mm, tsk, cpu)

/* Activate a new MM instance for the current task. */
#define activate_mm(active_mm, mm) switch_mm((active_mm), (mm), NULL, smp_processor_id())

#endif /* !(__ASSEMBLY__) */

#endif /* !(__SPARC_MMU_CONTEXT_H) */
