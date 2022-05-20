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
 * Copyright (C) 1996, 1997, 1998, 2001 by Ralf Baechle
 */
#ifndef _ASM_BRANCH_H
#define _ASM_BRANCH_H

#include <asm/ptrace.h>

static inline int delay_slot(struct pt_regs *regs)
{
	return regs->cp0_cause & CAUSEF_BD;
}

static inline unsigned long exception_epc(struct pt_regs *regs)
{
	if (!delay_slot(regs))
		return regs->cp0_epc;

	return regs->cp0_epc + 4;
}

extern int __compute_return_epc(struct pt_regs *regs);

static inline int compute_return_epc(struct pt_regs *regs)
{
	if (!delay_slot(regs)) {
		regs->cp0_epc += 4;
		return 0;
	}

	return __compute_return_epc(regs);
}

#endif /* _ASM_BRANCH_H */
