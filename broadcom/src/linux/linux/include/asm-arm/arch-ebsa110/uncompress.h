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
 *  linux/include/asm-arm/arch-ebsa110/uncompress.h
 *
 *  Copyright (C) 1996,1997,1998 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/*
 * This does not append a newline
 */
static void puts(const char *s)
{
	unsigned long tmp1, tmp2;

	__asm__ __volatile__("
	ldrb	%0, [%2], #1
	teq	%0, #0
	beq	3f
1:	strb	%0, [%3]
2:	ldrb	%1, [%3, #0x14]
	and	%1, %1, #0x60
	teq	%1, #0x60
	bne	2b
	teq	%0, #'\n'
	moveq	%0, #'\r'
	beq	1b
	ldrb	%0, [%2], #1
	teq	%0, #0
	bne	1b
3:	ldrb	%1, [%3, #0x14]
	and	%1, %1, #0x60
	teq	%1, #0x60
	bne	3b
	"
	 : "=&r" (tmp1), "=&r" (tmp2)
	 : "0" (0), "1" (0), "r" (s), "r" (0xf0000be0) : "cc");
}

/*
 * nothing to do
 */
#define arch_decomp_setup()
#define arch_decomp_wdog()
