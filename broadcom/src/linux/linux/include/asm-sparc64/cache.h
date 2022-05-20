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
 * include/asm-sparc64/cache.h
 */
#ifndef __ARCH_SPARC64_CACHE_H
#define __ARCH_SPARC64_CACHE_H

/* bytes per L1 cache line */
#define        L1_CACHE_BYTES		32 /* Two 16-byte sub-blocks per line. */

#define        L1_CACHE_ALIGN(x)       (((x)+(L1_CACHE_BYTES-1))&~(L1_CACHE_BYTES-1))

#define        SMP_CACHE_BYTES_SHIFT	6
#define        SMP_CACHE_BYTES		(1 << SMP_CACHE_BYTES_SHIFT) /* L2 cache line size. */

#ifdef MODULE
#define __cacheline_aligned __attribute__((__aligned__(SMP_CACHE_BYTES)))
#else
#define __cacheline_aligned					\
  __attribute__((__aligned__(SMP_CACHE_BYTES),			\
		 __section__(".data.cacheline_aligned")))
#endif

#endif
