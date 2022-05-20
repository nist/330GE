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
 * Copyright (C) 1994, 1995, 1996, 1999 by Ralf Baechle
 * Copyright (C) 1999 Silicon Graphics, Inc.
 */
#ifndef _ASM_TYPES_H
#define _ASM_TYPES_H

#include <linux/config.h>

typedef unsigned short umode_t;

/*
 * __xx is ok: it doesn't pollute the POSIX namespace. Use these in the
 * header files exported to user space
 */

typedef __signed__ char __s8;
typedef unsigned char __u8;

typedef __signed__ short __s16;
typedef unsigned short __u16;

typedef __signed__ int __s32;
typedef unsigned int __u32;

#if _MIPS_SZLONG == 64

typedef __signed__ long __s64;
typedef unsigned long __u64;

#else

#if defined(__GNUC__) && !defined(__STRICT_ANSI__)
typedef __signed__ long long __s64;
typedef unsigned long long __u64;
#endif

#endif

/*
 * These aren't exported outside the kernel to avoid name space clashes
 */
#ifdef __KERNEL__

typedef __signed char s8;
typedef unsigned char u8;

typedef __signed short s16;
typedef unsigned short u16;

typedef __signed int s32;
typedef unsigned int u32;

#if _MIPS_SZLONG == 64

typedef __signed__ long s64;
typedef unsigned long u64;

#else

#if defined(__GNUC__) && !defined(__STRICT_ANSI__)
typedef __signed__ long long s64;
typedef unsigned long long u64;
#endif

#endif

#define BITS_PER_LONG _MIPS_SZLONG

#if defined(CONFIG_HIGHMEM) && defined(CONFIG_64BIT_PHYS_ADDR)
typedef u64 dma_addr_t;
#else
typedef u32 dma_addr_t;
#endif
typedef u64 dma64_addr_t;

#ifdef CONFIG_64BIT_PHYS_ADDR
typedef unsigned long long phys_t;
#else
typedef unsigned long phys_t;
#endif

#endif /* __KERNEL__ */

#endif /* _ASM_TYPES_H */
