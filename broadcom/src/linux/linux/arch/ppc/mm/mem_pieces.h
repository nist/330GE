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
 * BK Id: SCCS/s.mem_pieces.h 1.5 05/17/01 18:14:23 cort
 */
/*
 *    Copyright (c) 1996 Paul Mackerras <paulus@cs.anu.edu.au>
 *      Changes to accomodate Power Macintoshes.
 *    Cort Dougan <cort@cs.nmt.edu>
 *      Rewrites.
 *    Grant Erickson <grant@lcse.umn.edu>
 *      General rework and split from mm/init.c.
 *
 *    Module name: mem_pieces.h
 *
 *    Description:
 *      Routines and data structures for manipulating and representing
 *      phyiscal memory extents (i.e. address/length pairs).
 *
 */

#ifndef __MEM_PIECES_H__
#define	__MEM_PIECES_H__

#include <asm/prom.h>

#ifdef __cplusplus
extern "C" {
#endif


/* Type Definitions */

#define	MEM_PIECES_MAX	32

struct mem_pieces {
    int n_regions;
    struct reg_property regions[MEM_PIECES_MAX];
};

/* Function Prototypes */

extern void	*mem_pieces_find(unsigned int size, unsigned int align);
extern void	 mem_pieces_remove(struct mem_pieces *mp, unsigned int start,
				   unsigned int size, int must_exist);
extern void	 mem_pieces_append(struct mem_pieces *mp, unsigned int start,
				   unsigned int size);
extern void	 mem_pieces_coalesce(struct mem_pieces *mp);
extern void	 mem_pieces_sort(struct mem_pieces *mp);

#ifdef __cplusplus
}
#endif

#endif /* __MEM_PIECES_H__ */
