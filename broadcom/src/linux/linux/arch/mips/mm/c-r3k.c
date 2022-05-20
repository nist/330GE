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
 * r2300.c: R2000 and R3000 specific mmu/cache code.
 *
 * Copyright (C) 1996 David S. Miller (dm@engr.sgi.com)
 *
 * with a lot of changes to make this thing work for R3000s
 * Tx39XX R4k style caches added. HK
 * Copyright (C) 1998, 1999, 2000 Harald Koerfgen
 * Copyright (C) 1998 Gleb Raiko & Vladimir Roganov
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>

#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/mmu_context.h>
#include <asm/system.h>
#include <asm/isadep.h>
#include <asm/io.h>
#include <asm/bootinfo.h>
#include <asm/cpu.h>

static unsigned long icache_size, dcache_size;		/* Size in bytes */
static unsigned long icache_lsize, dcache_lsize;	/* Size in bytes */

#undef DEBUG_CACHE

unsigned long __init r3k_cache_size(unsigned long ca_flags)
{
	unsigned long flags, status, dummy, size;
	volatile unsigned long *p;

	p = (volatile unsigned long *) KSEG0;

	flags = read_c0_status();

	/* isolate cache space */
	write_c0_status((ca_flags|flags)&~ST0_IEC);

	*p = 0xa5a55a5a;
	dummy = *p;
	status = read_c0_status();

	if (dummy != 0xa5a55a5a || (status & ST0_CM)) {
		size = 0;
	} else {
		for (size = 128; size <= 0x40000; size <<= 1)
			*(p + size) = 0;
		*p = -1;
		for (size = 128;
		     (size <= 0x40000) && (*(p + size) == 0);
		     size <<= 1)
			;
		if (size > 0x40000)
			size = 0;
	}

	write_c0_status(flags);

	return size * sizeof(*p);
}

unsigned long __init r3k_cache_lsize(unsigned long ca_flags)
{
	unsigned long flags, status, lsize, i;
	volatile unsigned long *p;

	p = (volatile unsigned long *) KSEG0;

	flags = read_c0_status();

	/* isolate cache space */
	write_c0_status((ca_flags|flags)&~ST0_IEC);

	for (i = 0; i < 128; i++)
		*(p + i) = 0;
	*(volatile unsigned char *)p = 0;
	for (lsize = 1; lsize < 128; lsize <<= 1) {
		*(p + lsize);
		status = read_c0_status();
		if (!(status & ST0_CM))
			break;
	}
	for (i = 0; i < 128; i += lsize)
		*(volatile unsigned char *)(p + i) = 0;

	write_c0_status(flags);

	return lsize * sizeof(*p);
}

static void __init r3k_probe_cache(void)
{
	dcache_size = r3k_cache_size(ST0_ISC);
	if (dcache_size)
		dcache_lsize = r3k_cache_lsize(ST0_ISC);


	icache_size = r3k_cache_size(ST0_ISC|ST0_SWC);
	if (icache_size)
		icache_lsize = r3k_cache_lsize(ST0_ISC|ST0_SWC);
}

static void r3k_flush_icache_range(unsigned long start, unsigned long end)
{
	unsigned long size, i, flags;
	volatile unsigned char *p;

	size = end - start;
	if (size > icache_size || KSEGX(start) != KSEG0) {
		start = KSEG0;
		size = icache_size;
	}
	p = (char *)start;

	flags = read_c0_status();

	/* isolate cache space */
	write_c0_status((ST0_ISC|ST0_SWC|flags)&~ST0_IEC);

	for (i = 0; i < size; i += 0x080) {
		asm ( 	"sb\t$0, 0x000(%0)\n\t"
			"sb\t$0, 0x004(%0)\n\t"
			"sb\t$0, 0x008(%0)\n\t"
			"sb\t$0, 0x00c(%0)\n\t"
			"sb\t$0, 0x010(%0)\n\t"
			"sb\t$0, 0x014(%0)\n\t"
			"sb\t$0, 0x018(%0)\n\t"
			"sb\t$0, 0x01c(%0)\n\t"
		 	"sb\t$0, 0x020(%0)\n\t"
			"sb\t$0, 0x024(%0)\n\t"
			"sb\t$0, 0x028(%0)\n\t"
			"sb\t$0, 0x02c(%0)\n\t"
			"sb\t$0, 0x030(%0)\n\t"
			"sb\t$0, 0x034(%0)\n\t"
			"sb\t$0, 0x038(%0)\n\t"
			"sb\t$0, 0x03c(%0)\n\t"
			"sb\t$0, 0x040(%0)\n\t"
			"sb\t$0, 0x044(%0)\n\t"
			"sb\t$0, 0x048(%0)\n\t"
			"sb\t$0, 0x04c(%0)\n\t"
			"sb\t$0, 0x050(%0)\n\t"
			"sb\t$0, 0x054(%0)\n\t"
			"sb\t$0, 0x058(%0)\n\t"
			"sb\t$0, 0x05c(%0)\n\t"
		 	"sb\t$0, 0x060(%0)\n\t"
			"sb\t$0, 0x064(%0)\n\t"
			"sb\t$0, 0x068(%0)\n\t"
			"sb\t$0, 0x06c(%0)\n\t"
			"sb\t$0, 0x070(%0)\n\t"
			"sb\t$0, 0x074(%0)\n\t"
			"sb\t$0, 0x078(%0)\n\t"
			"sb\t$0, 0x07c(%0)\n\t"
			: : "r" (p) );
		p += 0x080;
	}

	write_c0_status(flags);
}

static void r3k_flush_dcache_range(unsigned long start, unsigned long end)
{
	unsigned long size, i, flags;
	volatile unsigned char *p;

	size = end - start;
	if (size > dcache_size || KSEGX(start) != KSEG0) {
		start = KSEG0;
		size = dcache_size;
	}
	p = (char *)start;

	flags = read_c0_status();

	/* isolate cache space */
	write_c0_status((ST0_ISC|flags)&~ST0_IEC);

	for (i = 0; i < size; i += 0x080) {
		asm ( 	"sb\t$0, 0x000(%0)\n\t"
			"sb\t$0, 0x004(%0)\n\t"
			"sb\t$0, 0x008(%0)\n\t"
			"sb\t$0, 0x00c(%0)\n\t"
		 	"sb\t$0, 0x010(%0)\n\t"
			"sb\t$0, 0x014(%0)\n\t"
			"sb\t$0, 0x018(%0)\n\t"
			"sb\t$0, 0x01c(%0)\n\t"
		 	"sb\t$0, 0x020(%0)\n\t"
			"sb\t$0, 0x024(%0)\n\t"
			"sb\t$0, 0x028(%0)\n\t"
			"sb\t$0, 0x02c(%0)\n\t"
		 	"sb\t$0, 0x030(%0)\n\t"
			"sb\t$0, 0x034(%0)\n\t"
			"sb\t$0, 0x038(%0)\n\t"
			"sb\t$0, 0x03c(%0)\n\t"
		 	"sb\t$0, 0x040(%0)\n\t"
			"sb\t$0, 0x044(%0)\n\t"
			"sb\t$0, 0x048(%0)\n\t"
			"sb\t$0, 0x04c(%0)\n\t"
		 	"sb\t$0, 0x050(%0)\n\t"
			"sb\t$0, 0x054(%0)\n\t"
			"sb\t$0, 0x058(%0)\n\t"
			"sb\t$0, 0x05c(%0)\n\t"
		 	"sb\t$0, 0x060(%0)\n\t"
			"sb\t$0, 0x064(%0)\n\t"
			"sb\t$0, 0x068(%0)\n\t"
			"sb\t$0, 0x06c(%0)\n\t"
		 	"sb\t$0, 0x070(%0)\n\t"
			"sb\t$0, 0x074(%0)\n\t"
			"sb\t$0, 0x078(%0)\n\t"
			"sb\t$0, 0x07c(%0)\n\t"
			: : "r" (p) );
		p += 0x080;
	}

	write_c0_status(flags);
}

static inline unsigned long get_phys_page (unsigned long addr,
					   struct mm_struct *mm)
{
	pgd_t *pgd;
	pmd_t *pmd;
	pte_t *pte;
	unsigned long physpage;

	pgd = pgd_offset(mm, addr);
	pmd = pmd_offset(pgd, addr);
	pte = pte_offset(pmd, addr);

	if ((physpage = pte_val(*pte)) & _PAGE_VALID)
		return KSEG0ADDR(physpage & PAGE_MASK);

	return 0;
}

static inline void r3k_flush_cache_all(void)
{
}

static inline void r3k___flush_cache_all(void)
{
	r3k_flush_icache_range(KSEG0, KSEG0 + icache_size);
}

static void r3k_flush_cache_mm(struct mm_struct *mm)
{
}

static void r3k_flush_cache_range(struct mm_struct *mm, unsigned long start,
				  unsigned long end)
{
}

static void r3k_flush_cache_page(struct vm_area_struct *vma,
				   unsigned long page)
{
}

static void r3k_flush_page_to_ram(struct page * page)
{
	/*
	 * Nothing to be done
	 */
}

static void r3k_flush_icache_page(struct vm_area_struct *vma, struct page *page)
{
	struct mm_struct *mm = vma->vm_mm;
	unsigned long physpage;

	if (mm->context == 0)
		return;

	if (!(vma->vm_flags & VM_EXEC))
		return;

#ifdef DEBUG_CACHE
	printk("cpage[%d,%08lx]", (int)mm->context, page);
#endif

	physpage = (unsigned long) page_address(page);
	if (physpage)
		r3k_flush_icache_range(physpage, physpage + PAGE_SIZE);
}

static void r3k_flush_cache_sigtramp(unsigned long addr)
{
	unsigned long flags;

#ifdef DEBUG_CACHE
	printk("csigtramp[%08lx]", addr);
#endif

	flags = read_c0_status();

	write_c0_status(flags&~ST0_IEC);

	/* Fill the TLB to avoid an exception with caches isolated. */
	asm ( 	"lw\t$0, 0x000(%0)\n\t"
		"lw\t$0, 0x004(%0)\n\t"
		: : "r" (addr) );

	write_c0_status((ST0_ISC|ST0_SWC|flags)&~ST0_IEC);

	asm ( 	"sb\t$0, 0x000(%0)\n\t"
		"sb\t$0, 0x004(%0)\n\t"
		: : "r" (addr) );

	write_c0_status(flags);
}

static void r3k_dma_cache_wback_inv(unsigned long start, unsigned long size)
{
	iob();
	r3k_flush_dcache_range(start, start + size);
}

void __init ld_mmu_r23000(void)
{
	unsigned long config;

	_clear_page = r3k_clear_page;
	_copy_page = r3k_copy_page;

	r3k_probe_cache();

	_flush_cache_all = r3k_flush_cache_all;
	___flush_cache_all = r3k___flush_cache_all;
	_flush_cache_mm = r3k_flush_cache_mm;
	_flush_cache_range = r3k_flush_cache_range;
	_flush_cache_page = r3k_flush_cache_page;
	_flush_cache_sigtramp = r3k_flush_cache_sigtramp;
	_flush_page_to_ram = r3k_flush_page_to_ram;
	_flush_icache_page = r3k_flush_icache_page;
	_flush_icache_range = r3k_flush_icache_range;

	_dma_cache_wback_inv = r3k_dma_cache_wback_inv;

	printk("Primary instruction cache %dkb, linesize %d bytes\n",
		(int) (icache_size >> 10), (int) icache_lsize);
	printk("Primary data cache %dkb, linesize %d bytes\n",
		(int) (dcache_size >> 10), (int) dcache_lsize);

}
