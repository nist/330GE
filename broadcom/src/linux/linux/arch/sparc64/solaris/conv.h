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
/* $Id: conv.h,v 1.1.1.1 2007/01/25 12:51:51 jiahao_jhou Exp $
 * conv.h: Utility macros for Solaris emulation
 *
 * Copyright (C) 1997 Jakub Jelinek (jj@sunsite.mff.cuni.cz)
 */
 
/* #define DEBUG_SOLARIS */
#define DEBUG_SOLARIS_KMALLOC

#ifndef __ASSEMBLY__

#include <asm/unistd.h>

/* Use this to get at 32-bit user passed pointers. */
#define A(__x)				\
({	unsigned long __ret;		\
	__asm__ ("srl	%0, 0, %0"	\
		 : "=r" (__ret)		\
		 : "0" (__x));		\
	__ret;				\
})

extern unsigned sys_call_table[];
extern unsigned sys_call_table32[];
extern unsigned sunos_sys_table[];

#define SYS(name) ((long)sys_call_table[__NR_##name])
#define SUNOS(x) ((long)sunos_sys_table[x])

#ifdef DEBUG_SOLARIS
#define SOLD(s) printk("%s,%d,%s(): %s\n",__FILE__,__LINE__,__FUNCTION__,(s))
#define SOLDD(s) printk("solaris: "); printk s
#else
#define SOLD(s)
#define SOLDD(s)
#endif

#endif /* __ASSEMBLY__ */
