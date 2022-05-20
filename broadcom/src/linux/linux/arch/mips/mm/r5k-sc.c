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
 * Copyright (C) 1997, 2001 Ralf Baechle (ralf@gnu.org),
 * derived from r4xx0.c by David S. Miller (dm@engr.sgi.com).
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>

#include <asm/mipsregs.h>
#include <asm/bcache.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/system.h>
#include <asm/mmu_context.h>
#include <asm/cacheops.h>

/* Secondary cache size in bytes, if present.  */
static unsigned long scache_size;

#define SC_LINE 32
#define SC_PAGE (128*SC_LINE)

#define cache_op(base,op)                   \
__asm__ __volatile__("				\
		.set noreorder;                 \
		.set mips3;                     \
		cache %1, (%0);                 \
		.set mips0;                     \
		.set reorder"                   \
		:                               \
		: "r" (base),                   \
		  "i" (op));

static inline void blast_r5000_scache(void)
{
	unsigned long start = KSEG0;
	unsigned long end = KSEG0 + scache_size;

	while(start < end) {
		cache_op(start, R5K_Page_Invalidate_S);
		start += SC_PAGE;
	}
}

static void r5k_dma_cache_inv_sc(unsigned long addr, unsigned long size)
{
	unsigned long end, a;

	if (size >= scache_size) {
		blast_r5000_scache();
		return;
	}

	/* On the R5000 secondary cache we cannot
	 * invalidate less than a page at a time.
	 * The secondary cache is physically indexed, write-through.
	 */
	a = addr & ~(SC_PAGE - 1);
	end = (addr + size - 1) & ~(SC_PAGE - 1);
	while (a <= end) {
		cache_op(a, R5K_Page_Invalidate_S);
		a += SC_PAGE;
	}
}

static void r5k_sc_enable(void)
{
        unsigned long flags;

	local_irq_save(flags);
	change_c0_config(CONF_SE, CONF_SE);
	blast_r5000_scache();
	local_irq_restore(flags);
}

static void r5k_sc_disable(void)
{
        unsigned long flags;

	local_irq_save(flags);
	blast_r5000_scache();
	change_c0_config(CONF_SE, 0);
	local_irq_restore(flags);
}

static inline int __init r5k_sc_probe(void)
{
	unsigned long config = read_c0_config();

	if(config & CONF_SC)
		return(0);

	scache_size = (512*1024) << ((config >> 20)&3);

	printk("R5000 SCACHE size %ldK, linesize 32 bytes.\n",
			scache_size >> 10);

	return 1;
}

struct bcache_ops r5k_sc_ops = {
	r5k_sc_enable,
	r5k_sc_disable,
	r5k_dma_cache_inv_sc,
	r5k_dma_cache_inv_sc
};

void __init r5k_sc_init(void)
{
	if (r5k_sc_probe()) {
		r5k_sc_enable();
		bcops = &r5k_sc_ops;
	}
}