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
#ifndef _ASM_IA64_BREAK_H
#define _ASM_IA64_BREAK_H

/*
 * IA-64 Linux break numbers.
 *
 * Copyright (C) 1999 Hewlett-Packard Co
 * Copyright (C) 1999 David Mosberger-Tang <davidm@hpl.hp.com>
 */

/*
 * OS-specific debug break numbers:
 */
#define __IA64_BREAK_KDB		0x80100

/*
 * OS-specific break numbers:
 */
#define __IA64_BREAK_SYSCALL		0x100000

#endif /* _ASM_IA64_BREAK_H */
