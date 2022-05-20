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
 *  arch/s390/lib/misaligned.c
 *    S390 misalignment panic stubs
 *
 *  S390 version
 *    Copyright (C) 2001 IBM Deutschland Entwicklung GmbH, IBM Corporation
 *    Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com).
 *
 * xchg wants to panic if the pointer is not aligned. To avoid multiplying
 * the panic message over and over again, the panic is done in the helper
 * functions __misaligned_u64, __misaligned_u32 and __misaligned_u16.
 */
 
#include <linux/module.h>
#include <linux/kernel.h>

void __misaligned_u16(void)
{
	panic("misaligned (__u16 *) in __xchg\n");
}

void __misaligned_u32(void)
{
	panic("misaligned (__u32 *) in __xchg\n");
}

void __misaligned_u64(void)
{
	panic("misaligned (__u64 *) in __xchg\n");
}

EXPORT_SYMBOL(__misaligned_u16);
EXPORT_SYMBOL(__misaligned_u32);
EXPORT_SYMBOL(__misaligned_u64);
