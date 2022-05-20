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
 * Performance counters software interface.
 *
 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: bcmperf.h,v 1.1.1.1 2007/01/25 12:52:12 jiahao_jhou Exp $
 */
/* essai */
#ifndef _BCMPERF_H_
#define _BCMPERF_H_
/* get cache hits and misses */
#if defined(mips) && defined(BCMPERFSTATS)
#include <hndmips.h>
#define BCMPERF_ENABLE_INSTRCOUNT() hndmips_perf_instrcount_enable()
#define BCMPERF_ENABLE_ICACHE_MISS() hndmips_perf_icache_miss_enable()
#define BCMPERF_ENABLE_ICACHE_HIT() hndmips_perf_icache_hit_enable()
#define	BCMPERF_GETICACHE_MISS(x)	((x) = hndmips_perf_read_cache_miss())
#define	BCMPERF_GETICACHE_HIT(x)	((x) = hndmips_perf_read_cache_hit())
#define	BCMPERF_GETINSTRCOUNT(x)	((x) = hndmips_perf_read_instrcount())
#else
#define BCMPERF_ENABLE_INSTRCOUNT()
#define BCMPERF_ENABLE_ICACHE_MISS()
#define BCMPERF_ENABLE_ICACHE_HIT()
#define	BCMPERF_GETICACHE_MISS(x)	((x) = 0)
#define	BCMPERF_GETICACHE_HIT(x)	((x) = 0)
#define	BCMPERF_GETINSTRCOUNT(x)	((x) = 0)
#endif /* defined(mips) */
#endif /* _BCMPERF_H_ */
