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
 * Copyright (C) 1997, 98, 99, 2000 Ralf Baechle
 * Copyright (C) 1999 Silicon Graphics, Inc.
 */
#ifndef _ASM_CACHE_H
#define _ASM_CACHE_H

#include <linux/config.h>

#ifndef __ASSEMBLY__
/*
 * Descriptor for a cache
 */
struct cache_desc {
	int linesz;
	int sets;
	int ways;
	int flags;	/* Details like write thru/back, coherent, etc. */
};
#endif /* !__ASSEMBLY__ */

/*
 * Flag definitions
 */
#define MIPS_CACHE_NOT_PRESENT 0x00000001
#define MIPS_CACHE_VTAG_CACHE  0x00000002 /* Virtually tagged cache. */

#if defined(CONFIG_CPU_R3000) || defined(CONFIG_CPU_R6000) || defined(CONFIG_CPU_TX39XX)
#define L1_CACHE_BYTES		16
#else
#define L1_CACHE_BYTES 		32	/* A guess */
#endif

#define SMP_CACHE_BYTES		L1_CACHE_BYTES

#endif /* _ASM_CACHE_H */
