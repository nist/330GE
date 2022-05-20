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
 * Copyright (C) by Ralf Baechle
 */
#ifndef _MIPS_BYTEORDER_H
#define _MIPS_BYTEORDER_H

#include <asm/types.h>

#ifdef __GNUC__

#if !defined(__STRICT_ANSI__) || defined(__KERNEL__)
#  define __BYTEORDER_HAS_U64__
#  define __SWAB_64_THRU_32__
#endif

#endif /* __GNUC__ */

#if defined(__MIPSEB__)
#  include <linux/byteorder/big_endian.h>
#elif defined(__MIPSEL__)
#  include <linux/byteorder/little_endian.h>
#else
#  error "MIPS, but neither __MIPSEB__, nor __MIPSEL__???"
#endif

#endif /* _MIPS_BYTEORDER_H */
