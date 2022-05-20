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
 * Copyright (C) 2002 by Ralf Baechle
 */
#ifndef __ASM_TLBDEBUG_H
#define __ASM_TLBDEBUG_H

/*
 * TLB debugging functions:
 */
extern void dump_tlb(int first, int last);
extern void dump_tlb_all(void);
extern void dump_tlb_wired(void);
extern void dump_tlb_addr(unsigned long addr);
extern void dump_tlb_nonwired(void);

#endif /* __ASM_TLBDEBUG_H */
