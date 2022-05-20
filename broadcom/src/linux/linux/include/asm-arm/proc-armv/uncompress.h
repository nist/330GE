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
 *  linux/include/asm-arm/proc-armv/uncompress.h
 *
 *  Copyright (C) 1997 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

static inline void proc_decomp_setup (void)
{
	__asm__ __volatile__("
	mrc	p15, 0, r0, c0, c0
	eor	r0, r0, #0x44 << 24
	eor	r0, r0, #0x01 << 16
	eor	r0, r0, #0xA1 << 8
	movs	r0, r0, lsr #5
	mcreq	p15, 0, r0, c7, c5, 0		@ flush I cache
	mrceq	p15, 0, r0, c1, c0
	orreq	r0, r0, #1 << 12
	mcreq	p15, 0, r0, c1, c0		@ enable I cache
	mov	r0, #0
	mcreq	p15, 0, r0, c15, c1, 2		@ enable clock switching
	" : : : "r0", "cc", "memory");
}
