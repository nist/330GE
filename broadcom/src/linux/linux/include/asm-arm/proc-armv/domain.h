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
 *  linux/include/asm-arm/proc-armv/domain.h
 *
 *  Copyright (C) 1999 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __ASM_PROC_DOMAIN_H
#define __ASM_PROC_DOMAIN_H

/*
 * Domain numbers
 *
 *  DOMAIN_IO     - domain 2 includes all IO only
 *  DOMAIN_KERNEL - domain 1 includes all kernel memory only
 *  DOMAIN_USER   - domain 0 includes all user memory only
 */
#define DOMAIN_USER	0
#define DOMAIN_KERNEL	1
#define DOMAIN_TABLE	1
#define DOMAIN_IO	2

/*
 * Domain types
 */
#define DOMAIN_NOACCESS	0
#define DOMAIN_CLIENT	1
#define DOMAIN_MANAGER	3

#define domain_val(dom,type)	((type) << 2*(dom))

#define set_domain(x)					\
	do {						\
	__asm__ __volatile__(				\
	"mcr	p15, 0, %0, c3, c0	@ set domain"	\
	  : : "r" (x));					\
	} while (0)

#define modify_domain(dom,type)				\
	do {						\
	unsigned int domain = current->thread.domain;	\
	domain &= ~domain_val(dom, DOMAIN_MANAGER);	\
	domain |= domain_val(dom, type);		\
	current->thread.domain = domain;		\
	set_domain(current->thread.domain);		\
	} while (0)

#endif
