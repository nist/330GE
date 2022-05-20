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
 *  linux/include/asm-arm/proc-armv/locks.h
 *
 *  Copyright (C) 2000 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Interrupt safe locking assembler. 
 */
#ifndef __ASM_PROC_LOCKS_H
#define __ASM_PROC_LOCKS_H

#define __down_op(ptr,fail)			\
	({					\
	__asm__ __volatile__(			\
	"@ down_op\n"				\
"	mrs	ip, cpsr\n"			\
"	orr	lr, ip, #128\n"			\
"	msr	cpsr_c, lr\n"			\
"	ldr	lr, [%0]\n"			\
"	subs	lr, lr, %1\n"			\
"	str	lr, [%0]\n"			\
"	msr	cpsr_c, ip\n"			\
"	movmi	ip, %0\n"			\
"	blmi	" SYMBOL_NAME_STR(fail)		\
	:					\
	: "r" (ptr), "I" (1)			\
	: "ip", "lr", "cc");			\
	})

#define __down_op_ret(ptr,fail)			\
	({					\
		unsigned int ret;		\
	__asm__ __volatile__(			\
	"@ down_op_ret\n"			\
"	mrs	ip, cpsr\n"			\
"	orr	lr, ip, #128\n"			\
"	msr	cpsr_c, lr\n"			\
"	ldr	lr, [%1]\n"			\
"	subs	lr, lr, %2\n"			\
"	str	lr, [%1]\n"			\
"	msr	cpsr_c, ip\n"			\
"	movmi	ip, %1\n"			\
"	movpl	ip, #0\n"			\
"	blmi	" SYMBOL_NAME_STR(fail) "\n"	\
"	mov	%0, ip"				\
	: "=&r" (ret)				\
	: "r" (ptr), "I" (1)			\
	: "ip", "lr", "cc");			\
	ret;					\
	})

#define __up_op(ptr,wake)			\
	({					\
	__asm__ __volatile__(			\
	"@ up_op\n"				\
"	mrs	ip, cpsr\n"			\
"	orr	lr, ip, #128\n"			\
"	msr	cpsr_c, lr\n"			\
"	ldr	lr, [%0]\n"			\
"	adds	lr, lr, %1\n"			\
"	str	lr, [%0]\n"			\
"	msr	cpsr_c, ip\n"			\
"	movle	ip, %0\n"			\
"	blle	" SYMBOL_NAME_STR(wake)		\
	:					\
	: "r" (ptr), "I" (1)			\
	: "ip", "lr", "cc");			\
	})

/*
 * The value 0x01000000 supports up to 128 processors and
 * lots of processes.  BIAS must be chosen such that sub'ing
 * BIAS once per CPU will result in the long remaining
 * negative.
 */
#define RW_LOCK_BIAS      0x01000000
#define RW_LOCK_BIAS_STR "0x01000000"

#define __down_op_write(ptr,fail)		\
	({					\
	__asm__ __volatile__(			\
	"@ down_op_write\n"			\
"	mrs	ip, cpsr\n"			\
"	orr	lr, ip, #128\n"			\
"	msr	cpsr_c, lr\n"			\
"	ldr	lr, [%0]\n"			\
"	subs	lr, lr, %1\n"			\
"	str	lr, [%0]\n"			\
"	msr	cpsr_c, ip\n"			\
"	movne	ip, %0\n"			\
"	blne	" SYMBOL_NAME_STR(fail)		\
	:					\
	: "r" (ptr), "I" (RW_LOCK_BIAS)		\
	: "ip", "lr", "cc");			\
	})

#define __up_op_write(ptr,wake)			\
	({					\
	__asm__ __volatile__(			\
	"@ up_op_read\n"			\
"	mrs	ip, cpsr\n"			\
"	orr	lr, ip, #128\n"			\
"	msr	cpsr_c, lr\n"			\
"	ldr	lr, [%0]\n"			\
"	adds	lr, lr, %1\n"			\
"	str	lr, [%0]\n"			\
"	msr	cpsr_c, ip\n"			\
"	movcs	ip, %0\n"			\
"	blcs	" SYMBOL_NAME_STR(wake)		\
	:					\
	: "r" (ptr), "I" (RW_LOCK_BIAS)		\
	: "ip", "lr", "cc");			\
	})

#define __down_op_read(ptr,fail)		\
	__down_op(ptr, fail)

#define __up_op_read(ptr,wake)			\
	({					\
	__asm__ __volatile__(			\
	"@ up_op_read\n"			\
"	mrs	ip, cpsr\n"			\
"	orr	lr, ip, #128\n"			\
"	msr	cpsr_c, lr\n"			\
"	ldr	lr, [%0]\n"			\
"	adds	lr, lr, %1\n"			\
"	str	lr, [%0]\n"			\
"	msr	cpsr_c, ip\n"			\
"	moveq	ip, %0\n"			\
"	bleq	" SYMBOL_NAME_STR(wake)		\
	:					\
	: "r" (ptr), "I" (1)			\
	: "ip", "lr", "cc");			\
	})

#endif
