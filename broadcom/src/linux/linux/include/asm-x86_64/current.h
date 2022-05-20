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
#ifndef _X86_64_CURRENT_H
#define _X86_64_CURRENT_H

#if !defined(__ASSEMBLY__) 
struct task_struct;

#include <asm/pda.h>

static inline struct task_struct *get_current(void) 
{ 
	struct task_struct *t = read_pda(pcurrent); 
	return t;
} 


static inline struct task_struct *stack_current(void)
{
	struct task_struct *current;
	__asm__("andq %%rsp,%0; ":"=r" (current) 
		: "0" (~(unsigned long)(THREAD_SIZE-1)));
	return current;
}


#define current get_current()

#else

#ifndef ASM_OFFSET_H
#include <asm/offset.h> 
#endif

#define GET_CURRENT(reg) movq %gs:(pda_pcurrent),reg

#endif

#endif /* !(_X86_64_CURRENT_H) */
