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
 * Copyright (C) 1999 Ralf Baechle
 * Copyright (C) 1999 Silicon Graphics, Inc.
 *
 * TLB debugging routines.  These perform horribly slow but can easily be
 * modified for debugging purposes.
 */
#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/ptrace.h>
#include <asm/system.h>

asmlinkage void do_page_fault(struct pt_regs *regs, unsigned long write,
                              unsigned long address);

asmlinkage void tlb_refill_debug(struct pt_regs regs)
{
	show_regs(&regs);
	panic(__FUNCTION__ " called.  This Does Not Happen (TM).");
}

asmlinkage void xtlb_refill_debug(struct pt_regs *regs)
{
	unsigned long addr;
	pgd_t *pgd;
	pmd_t *pmd;
	pte_t *pte;

	addr = regs->cp0_badvaddr & ~((PAGE_SIZE << 1) - 1);
	pgd = pgd_offset(current->active_mm, addr);
	pmd = pmd_offset(pgd, addr);
	pte = pte_offset(pmd, addr);

	write_c0_entrylo0(pte_val(pte[0]) >> 6);
	write_c0_entrylo1(pte_val(pte[1]) >> 6);
	__asm__ __volatile__("nop;nop;nop");

	tlb_write_random();
}

asmlinkage void xtlb_mod_debug(struct pt_regs *regs)
{
	unsigned long addr;

	addr = regs->cp0_badvaddr;
	do_page_fault(regs, 1, addr);
}

asmlinkage void xtlb_tlbl_debug(struct pt_regs *regs)
{
	unsigned long addr;

	addr = regs->cp0_badvaddr;
	do_page_fault(regs, 0, addr);
}

asmlinkage void xtlb_tlbs_debug(struct pt_regs *regs)
{
	unsigned long addr;

	addr = regs->cp0_badvaddr;
	do_page_fault(regs, 1, addr);
}
