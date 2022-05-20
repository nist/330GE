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
 * Copyright (C) 1999,2001-2002 Silicon Graphics, Inc.  All Rights Reserved.
 * Copyright (C) 1999 by Ralf Baechle
 */
#ifndef _ASM_IA64_SN_TYPES_H
#define _ASM_IA64_SN_TYPES_H

#include <linux/types.h>

typedef unsigned long 	cpuid_t;
typedef unsigned long 	cpumask_t;
typedef signed short	nasid_t;	/* node id in numa-as-id space */
typedef signed char	partid_t;	/* partition ID type */
typedef signed short	moduleid_t;	/* user-visible module number type */
typedef signed short	cmoduleid_t;	/* kernel compact module id type */
typedef unsigned char	clusterid_t;	/* Clusterid of the cell */

typedef uint64_t __psunsigned_t;

typedef unsigned long iopaddr_t;
typedef unsigned char uchar_t;
typedef unsigned long paddr_t;
typedef unsigned long pfn_t;

#endif /* _ASM_IA64_SN_TYPES_H */
