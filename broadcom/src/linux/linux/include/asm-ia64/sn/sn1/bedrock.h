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
/* $Id: bedrock.h,v 1.1.1.1 2007/01/25 12:52:04 jiahao_jhou Exp $
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1992 - 1997, 2000-2002 Silicon Graphics, Inc. All rights reserved.
 */

#ifndef _ASM_IA64_SN_SN1_BEDROCK_H
#define _ASM_IA64_SN_SN1_BEDROCK_H


/* The secret password; used to release protection */
#define HUB_PASSWORD		0x53474972756c6573ull

#define CHIPID_HUB		0x3012
#define CHIPID_ROUTER		0x3017

#define BEDROCK_REV_1_0		1
#define BEDROCK_REV_1_1		2

#define MAX_HUB_PATH		80

#include <asm/sn/arch.h>
#include <asm/sn/sn1/addrs.h>
#include <asm/sn/sn1/hubpi.h>
#include <asm/sn/sn1/hubmd.h>
#include <asm/sn/sn1/hubio.h>
#include <asm/sn/sn1/hubni.h>
#include <asm/sn/sn1/hublb.h>
#include <asm/sn/sn1/hubxb.h>
#include <asm/sn/sn1/hubpi_next.h>
#include <asm/sn/sn1/hubmd_next.h>
#include <asm/sn/sn1/hubio_next.h>
#include <asm/sn/sn1/hubni_next.h>
#include <asm/sn/sn1/hublb_next.h>
#include <asm/sn/sn1/hubxb_next.h>

/* Translation of uncached attributes */
#define	UATTR_HSPEC	0
#define	UATTR_IO	1
#define	UATTR_MSPEC	2
#define	UATTR_UNCAC	3

#if __ASSEMBLY__

/*
 * Get nasid into register, r (uses at)
 */
#define GET_NASID_ASM(r)				\
	dli	r, LOCAL_HUB_ADDR(LB_REV_ID);	\
	ld	r, (r);					\
	and	r, LRI_NODEID_MASK;			\
	dsrl	r, LRI_NODEID_SHFT

#endif /* __ASSEMBLY__ */

#ifndef __ASSEMBLY__

#include <asm/sn/xtalk/xwidget.h>

/* hub-as-widget iograph info, labelled by INFO_LBL_XWIDGET */
typedef struct v_hub_s *v_hub_t;
typedef uint64_t      rtc_time_t;

struct nodepda_s;
int hub_check_pci_equiv(void *addra, void *addrb);
void capture_hub_stats(cnodeid_t, struct nodepda_s *);
void init_hub_stats(cnodeid_t, struct nodepda_s *);

#endif /* __ASSEMBLY__ */

#endif /* _ASM_IA64_SN_SN1_BEDROCK_H */
