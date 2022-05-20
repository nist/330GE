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
 * Broadcom SiliconBackplane MIPS definitions
 *
 * SB MIPS cores are custom MIPS32 processors with SiliconBackplane
 * OCP interfaces. The CP0 processor ID is 0x00024000, where bits
 * 23:16 mean Broadcom and bits 15:8 mean a MIPS core with an OCP
 * interface. The core revision is stored in the SB ID register in SB
 * configuration space.
 *
 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: sbhndmips.h,v 1.1.1.1 2007/01/25 12:52:12 jiahao_jhou Exp $
 */

#ifndef	_sbhndmips_h_
#define	_sbhndmips_h_

#include <mipsinc.h>

#ifndef _LANGUAGE_ASSEMBLY

/* cpp contortions to concatenate w/arg prescan */
#ifndef PAD
#define	_PADLINE(line)	pad ## line
#define	_XSTR(line)	_PADLINE(line)
#define	PAD		_XSTR(__LINE__)
#endif	/* PAD */

typedef volatile struct {
	uint32	corecontrol;
	uint32	PAD[2];
	uint32	biststatus;
	uint32	PAD[4];
	uint32	intstatus;
	uint32	intmask;
	uint32	timer;
} mipsregs_t;

#endif	/* _LANGUAGE_ASSEMBLY */

#endif	/* _sbhndmips_h_ */
