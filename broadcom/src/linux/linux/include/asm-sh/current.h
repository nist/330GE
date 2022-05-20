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
#ifndef __ASM_SH_CURRENT_H
#define __ASM_SH_CURRENT_H

/*
 * Copyright (C) 1999 Niibe Yutaka
 *
 */

struct task_struct;

static __inline__ struct task_struct * get_current(void)
{
	struct task_struct *current;

	__asm__("stc	r7_bank, %0"
		:"=r" (current));

	return current;
}

#define current get_current()

#endif /* __ASM_SH_CURRENT_H */
