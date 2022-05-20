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
#ifndef _PARISC_CURRENT_H
#define _PARISC_CURRENT_H

#include <asm/processor.h>

struct task_struct;

static inline struct task_struct * get_current(void)
{
	register unsigned long cr;

	__asm__ __volatile__("mfctl %%cr30,%0" : "=r" (cr) );
	return (struct task_struct *)cr;
}
 
#define current get_current()

#endif /* !(_PARISC_CURRENT_H) */
