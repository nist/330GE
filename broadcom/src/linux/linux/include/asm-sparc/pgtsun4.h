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
/* $Id: pgtsun4.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $
 * pgtsun4.h:  Sun4 specific pgtable.h defines and code.
 *
 * Copyright (C) 1995 David S. Miller (davem@caip.rutgers.edu)
 * Copyright (C) 1998 Jakub Jelinek (jj@sunsite.mff.cuni.cz)
 */
#ifndef _SPARC_PGTSUN4C_H
#define _SPARC_PGTSUN4C_H

#include <asm/contregs.h>

/* PMD_SHIFT determines the size of the area a second-level page table can map */
#define SUN4C_PMD_SHIFT       23
#define SUN4C_PMD_SIZE        (1UL << SUN4C_PMD_SHIFT)
#define SUN4C_PMD_MASK        (~(SUN4C_PMD_SIZE-1))
#define SUN4C_PMD_ALIGN(addr) (((addr)+SUN4C_PMD_SIZE-1)&SUN4C_PMD_MASK)

/* PGDIR_SHIFT determines what a third-level page table entry can map */
#define SUN4C_PGDIR_SHIFT       23
#define SUN4C_PGDIR_SIZE        (1UL << SUN4C_PGDIR_SHIFT)
#define SUN4C_PGDIR_MASK        (~(SUN4C_PGDIR_SIZE-1))
#define SUN4C_PGDIR_ALIGN(addr) (((addr)+SUN4C_PGDIR_SIZE-1)&SUN4C_PGDIR_MASK)

/* To represent how the sun4c mmu really lays things out. */
#define SUN4C_REAL_PGDIR_SHIFT       18
#define SUN4C_REAL_PGDIR_SIZE        (1UL << SUN4C_REAL_PGDIR_SHIFT)
#define SUN4C_REAL_PGDIR_MASK        (~(SUN4C_REAL_PGDIR_SIZE-1))
#define SUN4C_REAL_PGDIR_ALIGN(addr) (((addr)+SUN4C_REAL_PGDIR_SIZE-1)&SUN4C_REAL_PGDIR_MASK)

/* 19 bit PFN on sun4 */
#define SUN4C_PFN_MASK 0x7ffff
 
/* Don't increase these unless the structures in sun4c.c are fixed */
#define SUN4C_MAX_SEGMAPS 256
#define SUN4C_MAX_CONTEXTS 16

/*
 * To be efficient, and not have to worry about allocating such
 * a huge pgd, we make the kernel sun4c tables each hold 1024
 * entries and the pgd similarly just like the i386 tables.
 */
#define SUN4C_PTRS_PER_PTE    1024
#define SUN4C_PTRS_PER_PMD    1
#define SUN4C_PTRS_PER_PGD    1024

/*
 * Sparc SUN4C pte fields.
 */
#define _SUN4C_PAGE_VALID        0x80000000
#define _SUN4C_PAGE_SILENT_READ  0x80000000   /* synonym */
#define _SUN4C_PAGE_DIRTY        0x40000000
#define _SUN4C_PAGE_SILENT_WRITE 0x40000000   /* synonym */
#define _SUN4C_PAGE_PRIV         0x20000000   /* privileged page */
#define _SUN4C_PAGE_NOCACHE      0x10000000   /* non-cacheable page */
#define _SUN4C_PAGE_PRESENT      0x08000000   /* implemented in software */
#define _SUN4C_PAGE_IO           0x04000000   /* I/O page */
#define _SUN4C_PAGE_READ         0x00800000   /* implemented in software */
#define _SUN4C_PAGE_WRITE        0x00400000   /* implemented in software */
#define _SUN4C_PAGE_ACCESSED     0x00200000   /* implemented in software */
#define _SUN4C_PAGE_MODIFIED     0x00100000   /* implemented in software */

#define _SUN4C_READABLE		(_SUN4C_PAGE_READ|_SUN4C_PAGE_SILENT_READ|\
				 _SUN4C_PAGE_ACCESSED)
#define _SUN4C_WRITEABLE	(_SUN4C_PAGE_WRITE|_SUN4C_PAGE_SILENT_WRITE|\
				 _SUN4C_PAGE_MODIFIED)

#define _SUN4C_PAGE_CHG_MASK	(0xffff|_SUN4C_PAGE_ACCESSED|_SUN4C_PAGE_MODIFIED)

#define SUN4C_PAGE_NONE		__pgprot(_SUN4C_PAGE_PRESENT)
#define SUN4C_PAGE_SHARED	__pgprot(_SUN4C_PAGE_PRESENT|_SUN4C_READABLE|\
					 _SUN4C_PAGE_WRITE)
#define SUN4C_PAGE_COPY		__pgprot(_SUN4C_PAGE_PRESENT|_SUN4C_READABLE)
#define SUN4C_PAGE_READONLY	__pgprot(_SUN4C_PAGE_PRESENT|_SUN4C_READABLE)
#define SUN4C_PAGE_KERNEL	__pgprot(_SUN4C_READABLE|_SUN4C_WRITEABLE|\
					 _SUN4C_PAGE_DIRTY|_SUN4C_PAGE_PRIV)

#ifndef __ASSEMBLY__

extern __inline__ unsigned long sun4c_get_synchronous_error(void)
{
	unsigned long sync_err;

	__asm__ __volatile__("lda [%1] %2, %0\n\t" :
			     "=r" (sync_err) :
			     "r" (AC_SYNC_ERR), "i" (ASI_CONTROL));
	return sync_err;
}

extern __inline__ unsigned long sun4c_get_synchronous_address(void)
{
	unsigned long sync_addr;

	__asm__ __volatile__("lda [%1] %2, %0\n\t" :
			     "=r" (sync_addr) :
			     "r" (AC_SYNC_VA), "i" (ASI_CONTROL));
	return sync_addr;
}

/* SUN4 pte, segmap, and context manipulation */
extern __inline__ unsigned long sun4c_get_segmap(unsigned long addr)
{
  register unsigned long entry;

  __asm__ __volatile__("\n\tlduha [%1] %2, %0\n\t" : 
		       "=r" (entry) :
		       "r" (addr), "i" (ASI_SEGMAP));
  return entry;
}

extern __inline__ void sun4c_put_segmap(unsigned long addr, unsigned long entry)
{
  __asm__ __volatile__("\n\tstha %1, [%0] %2; nop; nop; nop;\n\t" : :
		       "r" (addr), "r" (entry),
		       "i" (ASI_SEGMAP));
}

extern __inline__ unsigned long sun4c_get_pte(unsigned long addr)
{
  register unsigned long entry;

  __asm__ __volatile__("\n\tlda [%1] %2, %0\n\t" : 
		       "=r" (entry) :
		       "r" (addr), "i" (ASI_PTE));
  return entry;
}

extern __inline__ void sun4c_put_pte(unsigned long addr, unsigned long entry)
{
  __asm__ __volatile__("\n\tsta %1, [%0] %2; nop; nop; nop;\n\t" : :
		       "r" (addr), 
		       "r" ((entry & ~(_SUN4C_PAGE_PRESENT))), "i" (ASI_PTE));
}

extern __inline__ int sun4c_get_context(void)
{
  register int ctx;

  __asm__ __volatile__("\n\tlduba [%1] %2, %0\n\t" :
		       "=r" (ctx) :
		       "r" (AC_CONTEXT), "i" (ASI_CONTROL));

  return ctx;
}

extern __inline__ int sun4c_set_context(int ctx)
{
  __asm__ __volatile__("\n\tstba %0, [%1] %2; nop; nop; nop;\n\t" : :
		       "r" (ctx), "r" (AC_CONTEXT), "i" (ASI_CONTROL));

  return ctx;
}

#endif /* !(__ASSEMBLY__) */

#endif /* !(_SPARC_PGTSUN4_H) */
