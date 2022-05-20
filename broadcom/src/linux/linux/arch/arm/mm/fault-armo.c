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
 *  linux/arch/arm/mm/fault-armo.c
 *
 *  Copyright (C) 1995  Linus Torvalds
 *  Modifications for ARM processor (c) 1995-1999 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/ptrace.h>
#include <linux/mman.h>
#include <linux/mm.h>
#include <linux/interrupt.h>

#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/pgtable.h>

#define FAULT_CODE_LDRSTRPOST	0x80
#define FAULT_CODE_LDRSTRPRE	0x40
#define FAULT_CODE_LDRSTRREG	0x20
#define FAULT_CODE_LDMSTM	0x10
#define FAULT_CODE_LDCSTC	0x08
#define FAULT_CODE_PREFETCH	0x04
#define FAULT_CODE_WRITE	0x02
#define FAULT_CODE_FORCECOW	0x01

#define DO_COW(m)		((m) & (FAULT_CODE_WRITE|FAULT_CODE_FORCECOW))
#define READ_FAULT(m)		(!((m) & FAULT_CODE_WRITE))

extern int do_page_fault(unsigned long addr, int mode, struct pt_regs *regs);
extern void show_pte(struct mm_struct *mm, unsigned long addr);

/*
 * Handle a data abort.  Note that we have to handle a range of addresses
 * on ARM2/3 for ldm.  If both pages are zero-mapped, then we have to force
 * a copy-on-write.  However, on the second page, we always force COW.
 */
asmlinkage void
do_DataAbort(unsigned long min_addr, unsigned long max_addr, int mode, struct pt_regs *regs)
{
	do_page_fault(min_addr, mode, regs);

	if ((min_addr ^ max_addr) >> PAGE_SHIFT)
		do_page_fault(max_addr, mode | FAULT_CODE_FORCECOW, regs);
}

asmlinkage int
do_PrefetchAbort(unsigned long addr, struct pt_regs *regs)
{
	do_page_fault(addr, FAULT_CODE_PREFETCH, regs);
	return 1;
}
