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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1996, 1997, 1998, 2000, 2001 by Ralf Baechle
 */
#ifndef __ASM_WATCH_H
#define __ASM_WATCH_H

#include <linux/linkage.h>

/*
 * Types of reference for watch_set()
 */
enum wref_type {
	wr_save = 1,
	wr_load = 2
};

extern char watch_available;

extern asmlinkage void __watch_set(unsigned long addr, enum wref_type ref);
extern asmlinkage void __watch_clear(void);
extern asmlinkage void __watch_reenable(void);

#define watch_set(addr, ref)					\
	if (watch_available)					\
		__watch_set(addr, ref)
#define watch_clear()						\
	if (watch_available)					\
		__watch_clear()
#define watch_reenable()					\
	if (watch_available)					\
		__watch_reenable()

#endif /* __ASM_WATCH_H */
