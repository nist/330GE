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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1996, 1999, 2001 Ralf Baechle
 * Copyright (C) 1999 Silicon Graphics, Inc.
 * Copyright (C) 2001 MIPS Technologies, Inc.
 */
#ifndef __ASM_SGIDEFS_H
#define __ASM_SGIDEFS_H

/*
 * Using a Linux compiler for building Linux seems logic but not to
 * everybody.
 */
#ifndef __linux__
#error Use a Linux compiler or give up.
#endif

/*
 * Definitions for the ISA levels
 *
 * With the introduction of MIPS32 / MIPS64 instruction sets definitions
 * MIPS ISAs are no longer subsets of each other.  Therefore comparisons
 * on these symbols except with == may result in unexpected results and
 * are forbidden!
 */
#define _MIPS_ISA_MIPS1		1
#define _MIPS_ISA_MIPS2		2
#define _MIPS_ISA_MIPS3		3
#define _MIPS_ISA_MIPS4		4
#define _MIPS_ISA_MIPS5		5
#define _MIPS_ISA_MIPS32	6
#define _MIPS_ISA_MIPS64	7

/*
 * Subprogram calling convention
 */
#define _MIPS_SIM_ABI32		1
#define _MIPS_SIM_NABI32	2
#define _MIPS_SIM_ABI64		3

#endif /* __ASM_SGIDEFS_H */
