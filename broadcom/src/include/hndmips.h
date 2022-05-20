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
 * HND SiliconBackplane MIPS core software interface.
 *
 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: hndmips.h,v 1.1.1.1 2007/01/25 12:52:12 jiahao_jhou Exp $
 */

#ifndef _hndmips_h_
#define _hndmips_h_

extern void sb_mips_init(sb_t *sbh, uint shirq_map_base);
extern bool sb_mips_setclock(sb_t *sbh, uint32 mipsclock, uint32 sbclock, uint32 pciclock);
extern void enable_pfc(uint32 mode);
extern uint32 sb_memc_get_ncdl(sb_t *sbh);

#if defined(BCMPERFSTATS)
/* enable counting - exclusive version. Only one set of counters allowed at a time */
extern void hndmips_perf_cyclecount_enable(void);
extern void hndmips_perf_instrcount_enable(void);
extern void hndmips_perf_icachecount_enable(void);
extern void hndmips_perf_dcachecount_enable(void);
/* start and stop counting */
#define hndmips_perf_start01() \
	MTC0(C0_PERFORMANCE, 4, MFC0(C0_PERFORMANCE, 4) | 0x80008000)
#define hndmips_perf_stop01() \
	MTC0(C0_PERFORMANCE, 4, MFC0(C0_PERFORMANCE, 4) & ~0x80008000)
/* retrieve coutners - counters *decrement* */
#define hndmips_perf_read0() -(long)(MFC0(C0_PERFORMANCE, 0))
#define hndmips_perf_read1() -(long)(MFC0(C0_PERFORMANCE, 1))
#define hndmips_perf_read2() -(long)(MFC0(C0_PERFORMANCE, 2))
/* enable counting - modular version. Each counters can be enabled separately. */
extern void hndmips_perf_icache_hit_enable(void);
extern void hndmips_perf_icache_miss_enable(void);
extern uint32 hndmips_perf_read_instrcount(void);
extern uint32 hndmips_perf_read_cache_miss(void);
extern uint32 hndmips_perf_read_cache_hit(void);
#endif 

#endif /* _hndmips_h_ */
