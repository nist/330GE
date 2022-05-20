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
 * include/asm-parisc/cache.h
 */

#ifndef __ARCH_PARISC_CACHE_H
#define __ARCH_PARISC_CACHE_H

#include <linux/config.h>

#ifndef __ASSEMBLY__
/*
 * PA 2.0 processors have 64-byte cachelines; PA 1.1 processors have
 * 32-byte cachelines.  The default configuration is not for SMP anyway,
 * so if you're building for SMP, you should select the appropriate
 * processor type.  There is a potential livelock danger when running
 * a machine with this value set too small, but it's more probable you'll
 * just ruin performance.
 */
#ifdef CONFIG_PA20
#define L1_CACHE_BYTES 64
#else
#define L1_CACHE_BYTES 32
#endif

#define L1_CACHE_ALIGN(x)       (((x)+(L1_CACHE_BYTES-1))&~(L1_CACHE_BYTES-1))

#define SMP_CACHE_BYTES L1_CACHE_BYTES

#define __cacheline_aligned __attribute__((__aligned__(L1_CACHE_BYTES)))

extern void flush_data_cache_local(void);  /* flushes local data-cache only */
extern void flush_instruction_cache_local(void); /* flushes local code-cache only */
#ifdef CONFIG_SMP
extern void flush_data_cache(void); /* flushes data-cache only (all processors) */
#else
#define flush_data_cache flush_data_cache_local
#define flush_instruction_cache flush_instruction_cache_local
#endif

extern void cache_init(void);		/* initializes cache-flushing */
extern void flush_all_caches(void);     /* flush everything (tlb & cache) */
extern int get_cache_info(char *);
extern void flush_user_icache_range_asm(unsigned long, unsigned long);
extern void flush_kernel_icache_range_asm(unsigned long, unsigned long);
extern void flush_user_dcache_range_asm(unsigned long, unsigned long);
extern void flush_kernel_dcache_range_asm(unsigned long, unsigned long);
extern void flush_kernel_dcache_page(void *);
extern void flush_kernel_icache_page(void *);
extern void disable_sr_hashing(void);   /* turns off space register hashing */
extern void disable_sr_hashing_asm(int); /* low level support for above */
extern void free_sid(unsigned long);
unsigned long alloc_sid(void);

struct seq_file;
extern void show_cache_info(struct seq_file *m);

extern int split_tlb;
extern int dcache_stride;
extern int icache_stride;
extern struct pdc_cache_info cache_info;

#define pdtlb(addr)         asm volatile("pdtlb 0(%%sr1,%0)" : : "r" (addr));
#define pitlb(addr)         asm volatile("pitlb 0(%%sr1,%0)" : : "r" (addr));
#define pdtlb_kernel(addr)  asm volatile("pdtlb 0(%0)" : : "r" (addr));

#endif /* ! __ASSEMBLY__ */

/* Classes of processor wrt: disabling space register hashing */

#define SRHASH_PCXST    0   /* pcxs, pcxt, pcxt_ */
#define SRHASH_PCXL     1   /* pcxl */
#define SRHASH_PA20     2   /* pcxu, pcxu_, pcxw, pcxw_ */

#endif
