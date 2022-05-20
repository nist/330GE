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
/* $Id: tsunami.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $
 * tsunami.h:  Module specific definitions for Tsunami V8 Sparcs
 *
 * Copyright (C) 1995 David S. Miller (davem@caip.rutgers.edu)
 */

#ifndef _SPARC_TSUNAMI_H
#define _SPARC_TSUNAMI_H

#include <asm/asi.h>

/* The MMU control register on the Tsunami:
 *
 * -----------------------------------------------------------------------
 * | implvers |SW|AV|DV|MV| RSV |PC|ITD|ALC| RSV |PE| RC |IE|DE|RSV|NF|ME|
 * -----------------------------------------------------------------------
 *  31      24 23 22 21 20 19-18 17  16 14  13-12 11 10-9  8  7 6-2  1  0
 *
 * SW: Enable Software Table Walks  0=off 1=on
 * AV: Address View bit
 * DV: Data View bit
 * MV: Memory View bit
 * PC: Parity Control
 * ITD: ITBR disable
 * ALC: Alternate Cacheable
 * PE: Parity Enable   0=off 1=on
 * RC: Refresh Control
 * IE: Instruction cache Enable  0=off 1=on
 * DE: Data cache Enable  0=off 1=on
 * NF: No Fault, same as all other SRMMUs
 * ME: MMU Enable, same as all other SRMMUs
 */

#define TSUNAMI_SW        0x00800000
#define TSUNAMI_AV        0x00400000
#define TSUNAMI_DV        0x00200000
#define TSUNAMI_MV        0x00100000
#define TSUNAMI_PC        0x00020000
#define TSUNAMI_ITD       0x00010000
#define TSUNAMI_ALC       0x00008000
#define TSUNAMI_PE        0x00001000
#define TSUNAMI_RCMASK    0x00000C00
#define TSUNAMI_IENAB     0x00000200
#define TSUNAMI_DENAB     0x00000100
#define TSUNAMI_NF        0x00000002
#define TSUNAMI_ME        0x00000001

extern __inline__ void tsunami_flush_icache(void)
{
	__asm__ __volatile__("sta %%g0, [%%g0] %0\n\t" : :
			     "i" (ASI_M_IC_FLCLEAR) : "memory");
}

extern __inline__ void tsunami_flush_dcache(void)
{
	__asm__ __volatile__("sta %%g0, [%%g0] %0\n\t" : :
			     "i" (ASI_M_DC_FLCLEAR) : "memory");
}

#endif /* !(_SPARC_TSUNAMI_H) */
