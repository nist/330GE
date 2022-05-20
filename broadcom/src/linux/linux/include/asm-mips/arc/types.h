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
 * Copyright 1999 Ralf Baechle (ralf@gnu.org)
 * Copyright 1999 Silicon Graphics, Inc.
 */
#ifndef _ASM_ARC_TYPES_H
#define _ASM_ARC_TYPES_H

#include <linux/config.h>

#ifdef CONFIG_ARC32

typedef char		CHAR;
typedef short		SHORT;
typedef long		LARGE_INTEGER __attribute__ ((__mode__ (__DI__)));
typedef	long		LONG __attribute__ ((__mode__ (__SI__)));
typedef unsigned char	UCHAR;
typedef unsigned short	USHORT;
typedef unsigned long	ULONG __attribute__ ((__mode__ (__SI__)));
typedef void		VOID;

/* The pointer types.  Note that we're using a 64-bit compiler but all
   pointer in the ARC structures are only 32-bit, so we need some disgusting
   workarounds.  Keep your vomit bag handy.  */
typedef LONG		_PCHAR;
typedef LONG		_PSHORT;
typedef LONG		_PLARGE_INTEGER;
typedef	LONG		_PLONG;
typedef LONG		_PUCHAR;
typedef LONG		_PUSHORT;
typedef LONG		_PULONG;
typedef LONG		_PVOID;

#endif /* CONFIG_ARC32 */

#ifdef CONFIG_ARC64

typedef char		CHAR;
typedef short		SHORT;
typedef long		LARGE_INTEGER __attribute__ ((__mode__ (__DI__)));
typedef	long		LONG __attribute__ ((__mode__ (__DI__)));
typedef unsigned char	UCHAR;
typedef unsigned short	USHORT;
typedef unsigned long	ULONG __attribute__ ((__mode__ (__DI__)));
typedef void		VOID;

/* The pointer types.  We're 64-bit and the firmware is also 64-bit, so
   live is sane ...  */
typedef CHAR		*_PCHAR;
typedef SHORT		*_PSHORT;
typedef LARGE_INTEGER	*_PLARGE_INTEGER;
typedef	LONG		*_PLONG;
typedef UCHAR		*_PUCHAR;
typedef USHORT		*_PUSHORT;
typedef ULONG		*_PULONG;
typedef VOID		*_PVOID;

#endif /* CONFIG_ARC64  */

typedef CHAR		*PCHAR;
typedef SHORT		*PSHORT;
typedef LARGE_INTEGER	*PLARGE_INTEGER;
typedef	LONG		*PLONG;
typedef UCHAR		*PUCHAR;
typedef USHORT		*PUSHORT;
typedef ULONG		*PULONG;
typedef VOID		*PVOID;

#endif /* _ASM_ARC_TYPES_H */