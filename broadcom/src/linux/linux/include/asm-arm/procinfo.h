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
 *  linux/include/asm-arm/procinfo.h
 *
 *  Copyright (C) 1996-1999 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __ASM_PROCINFO_H
#define __ASM_PROCINFO_H

#ifndef __ASSEMBLY__

#include <asm/proc-fns.h>

struct proc_info_item {
	const char	 *manufacturer;
	const char	 *cpu_name;
};

/*
 * Note!  struct processor is always defined if we're
 * using MULTI_CPU, otherwise this entry is unused,
 * but still exists.
 *
 * NOTE! The following structure is defined by assembly
 * language, NOT C code.  For more information, check:
 *  arch/arm/mm/proc-*.S and arch/arm/kernel/head-armv.S
 */
struct proc_info_list {
	unsigned int	 cpu_val;
	unsigned int	 cpu_mask;
	unsigned long	 __cpu_mmu_flags;	/* used by head-armv.S */
	unsigned long	 __cpu_flush;		/* used by head-armv.S */
	const char	 *arch_name;
	const char	 *elf_name;
	unsigned int	 elf_hwcap;
	struct proc_info_item *info;
#ifdef MULTI_CPU
	struct processor *proc;
#else
	void		 *unused;
#endif
};

#endif	/* __ASSEMBLY__ */

#define HWCAP_SWP	 1
#define HWCAP_HALF	 2
#define HWCAP_THUMB	 4
#define HWCAP_26BIT	 8	/* Play it safe */
#define HWCAP_FAST_MULT	 16
#define HWCAP_FPA        32
#define HWCAP_VFP        64
#define HWCAP_EDSP	 128

#endif
