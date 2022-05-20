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
 *  linux/include/asm-arm/proc-armo/page.h
 *
 *  Copyright (C) 1995, 1996 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __ASM_PROC_PAGE_H
#define __ASM_PROC_PAGE_H

#include <linux/config.h>

/* PAGE_SHIFT determines the page size.  This is configurable. */
#if defined(CONFIG_PAGESIZE_16)
#define PAGE_SHIFT	14		/* 16K */
#else		/* default */
#define PAGE_SHIFT	15		/* 32K */
#endif

#define EXEC_PAGESIZE   32768

#endif /* __ASM_PROC_PAGE_H */
