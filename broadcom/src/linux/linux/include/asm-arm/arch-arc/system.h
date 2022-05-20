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
 *  linux/include/asm-arm/arch-arc/system.h
 *
 *  Copyright (C) 1996-1999 Russell King and Dave Gilbert
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

static void arch_idle(void)
{
	while (!current->need_resched && !hlt_counter);
}

static inline void arch_reset(char mode)
{
	/*
	 * copy branch instruction to reset location and call it
	 */
	*(unsigned long *)0 = *(unsigned long *)0x03800000;
	((void(*)(void))0)();
}
