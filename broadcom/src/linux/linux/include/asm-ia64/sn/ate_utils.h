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
#ifndef _ASM_IA64_SN_ATE_UTILS_H
#define _ASM_IA64_SN_ATE_UTILS_H

/* $Id: ate_utils.h,v 1.1.1.1 2007/01/25 12:52:04 jiahao_jhou Exp $
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1992 - 1997, 2000-2002 Silicon Graphics, Inc. All rights reserved.
 */

/*
 *	struct map	X[]	.m_size		.m_addr
 *			---	------------	-----------
 *			[0]	mapsize(X)	unused
 *				# X[] unused
 *			[1]	map lock *	mapwant sv_t *
 *				map access	wait for free map space
 *
 *	  mapstart(X)-> [2]	# units		unit number
 *			 :	    :		  :
 *			[ ]	    0
 */

#include <linux/types.h>

#define ulong_t uint64_t

struct map
{
	unsigned long m_size;	/* number of units available */
	unsigned long m_addr;	/* address of first available unit */
};

#define mapstart(X)		&X[2]		/* start of map array */

#define mapsize(X)		X[0].m_size	/* number of empty slots */
						/* remaining in map array */
#define maplock(X)		(((spinlock_t *) X[1].m_size))

#define mapout(X)		((sv_t *) X[1].m_addr)


extern ulong_t atealloc(struct map *, size_t);
extern struct map *atemapalloc(ulong_t);
extern void atefree(struct map *, size_t, ulong_t);
extern void atemapfree(struct map *);

#endif /* _ASM_IA64_SN_ATE_UTILS_H  */

