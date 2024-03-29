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
 * User address space access functions.
 *
 * Copyright 1997 Andi Kleen <ak@muc.de>
 * Copyright 1997 Linus Torvalds
 * Copyright 2002 Andi Kleen <ak@suse.de>
 */
#include <asm/uaccess.h>

/*
 * Copy a null terminated string from userspace.
 */

#define __do_strncpy_from_user(dst,src,count,res)			   \
do {									   \
	long __d0, __d1, __d2;						   \
	__asm__ __volatile__(						   \
		"	testq %1,%1\n"					   \
		"	jz 2f\n"					   \
		"0:	lodsb\n"					   \
		"	stosb\n"					   \
		"	testb %%al,%%al\n"				   \
		"	jz 1f\n"					   \
		"	decq %1\n"					   \
		"	jnz 0b\n"					   \
		"1:	subq %1,%0\n"					   \
		"2:\n"							   \
		".section .fixup,\"ax\"\n"				   \
		"3:	movq %5,%0\n"					   \
		"	jmp 2b\n"					   \
		".previous\n"						   \
		".section __ex_table,\"a\"\n"				   \
		"	.align 4\n"					   \
		"	.quad 0b,3b\n"					   \
		".previous"						   \
		: "=r"(res), "=c"(count), "=&a" (__d0), "=&S" (__d1),	   \
		  "=&D" (__d2)						   \
		: "i"(-EFAULT), "0"(count), "1"(count), "3"(src), "4"(dst) \
		: "memory");						   \
} while (0)

long
__strncpy_from_user(char *dst, const char *src, long count)
{
	long res;
	__do_strncpy_from_user(dst, src, count, res);
	return res;
}

long
strncpy_from_user(char *dst, const char *src, long count)
{
	long res = -EFAULT;
	if (access_ok(VERIFY_READ, src, 1))
		__do_strncpy_from_user(dst, src, count, res);
	return res;
}

/*
 * Zero Userspace
 */

unsigned long __clear_user(void *addr, unsigned long size)
{
	long __d0;
	/* no memory constraint because it doesn't change any memory gcc knows
	   about */
	asm volatile(
		"	testq  %[size8],%[size8]\n"
		"	jz     4f\n"
		"0:	movnti %[zero],(%[dst])\n"
		"	addq   %[eight],%[dst]\n"
		"	decl %%ecx ; jnz   0b\n"
		"4:	movq  %[size1],%%rcx\n"
		"	testl %%ecx,%%ecx\n"
		"	jz     2f\n"
		"1:	movb   %b[zero],(%[dst])\n"
		"	incq   %[dst]\n"
		"	decl %%ecx ; jnz  1b\n"
		"2:	sfence\n"
		".section .fixup,\"ax\"\n"
		"3:	lea 0(%[size1],%[size8],8),%[size8]\n"
		"	jmp 2b\n"
		".previous\n"
		".section __ex_table,\"a\"\n"
		"       .align 8\n"
		"	.quad 0b,3b\n"
		"	.quad 1b,2b\n"
		".previous"
		: [size8] "=c"(size), [dst] "=&D" (__d0)
		: [size1] "r"(size & 7), "[size8]" (size / 8), "[dst] "(addr),
		  [zero] "r" (0UL), [eight] "r" (8UL));
	return size;
}


unsigned long clear_user(void *to, unsigned long n)
{
	if (access_ok(VERIFY_WRITE, to, n))
		return __clear_user(to, n);
	return n;
}

/*
 * Return the size of a string (including the ending 0)
 *
 * Return 0 on exception, a value greater than N if too long
 */

long strnlen_user(const char *s, long n)
{
	unsigned long res = 0;
	char c;

	if (!access_ok(VERIFY_READ, s, n))
		return 0;

	while (1) {
		if (get_user(c, s))
			return 0;
		if (!c)
			return res+1;
		if (res>n)
			return n+1;
		res++;
		s++;
	}
}
