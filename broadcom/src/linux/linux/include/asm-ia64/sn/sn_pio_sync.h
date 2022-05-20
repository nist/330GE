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
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2001-2002 Silicon Graphics, Inc. All rights reserved.
 */


#ifndef _ASM_IA64_SN_SN_PIO_WRITE_SYNC_H
#define _ASM_IA64_SN_SN_PIO_WRITE_SYNC_H

#include <linux/config.h>
#ifdef CONFIG_IA64_SGI_SN2
#include <asm/sn/sn_cpuid.h>
#include <asm/sn/sn2/addrs.h>
#include <asm/sn/sn2/shub_mmr.h>
#include <asm/sn/sn2/shub_mmr_t.h>

/*
 * This macro flushes all outstanding PIOs performed by this cpu to the 
 * intended destination SHUB.  This in essence ensures that all PIO's
 * issues by this cpu has landed at it's destination.
 *
 * This macro expects the caller:
 *	1.  The thread is locked.
 *	2.  All prior PIO operations has been fenced with __ia64_mf_a().
 *
 * The expectation is that get_slice() will return either 0 or 2.
 * When we have multi-core cpu's, the expectation is get_slice() will 
 * return either 0,1 or 2,3.
 */

#define SN_PIO_WRITE_SYNC \
	{ \
		volatile unsigned long sn_pio_writes_done; \
	do { \
		sn_pio_writes_done = (volatile unsigned long) (SH_PIO_WRITE_STATUS_0_WRITES_OK_MASK & HUB_L( (unsigned long *)GLOBAL_MMR_ADDR(get_nasid(), (get_slice() < 2) ? SH_PIO_WRITE_STATUS_0 : SH_PIO_WRITE_STATUS_1 ))); \
	} while (!sn_pio_writes_done); \
	__ia64_mf_a(); \
	}
#else

/*
 * For all ARCHITECTURE type, this is a NOOP.
 */

#define SN_PIO_WRITE_SYNC

#endif

#endif /* _ASM_IA64_SN_SN_PIO_WRITE_SYNC_H */
