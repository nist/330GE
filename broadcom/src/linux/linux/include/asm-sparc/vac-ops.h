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
/* $Id: vac-ops.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $ */
#ifndef _SPARC_VAC_OPS_H
#define _SPARC_VAC_OPS_H

/* vac-ops.h: Inline assembly routines to do operations on the Sparc
 *            VAC (virtual address cache) for the sun4c.
 *
 * Copyright (C) 1994, David S. Miller (davem@caip.rutgers.edu)
 */

#include <linux/config.h>
#include <asm/sysen.h>
#include <asm/contregs.h>
#include <asm/asi.h>

/* The SUN4C models have a virtually addressed write-through
 * cache.
 *
 * The cache tags are directly accessible through an ASI and
 * each have the form:
 *
 * ------------------------------------------------------------
 * | MBZ | CONTEXT | WRITE | PRIV | VALID | MBZ | TagID | MBZ |
 * ------------------------------------------------------------
 *  31 25  24   22     21     20     19    18 16  15   2  1  0
 *
 * MBZ: These bits are either unused and/or reserved and should
 *      be written as zeroes.
 *
 * CONTEXT: Records the context to which this cache line belongs.
 *
 * WRITE: A copy of the writable bit from the mmu pte access bits.
 *
 * PRIV: A copy of the privileged bit from the pte access bits.
 *
 * VALID: If set, this line is valid, else invalid.
 *
 * TagID: Fourteen bits of tag ID.
 *
 * Every virtual address is seen by the cache like this:
 *
 * ----------------------------------------
 * |  RESV  | TagID | LINE | BYTE-in-LINE |
 * ----------------------------------------
 *  31    30 29   16 15   4 3            0
 *
 * RESV: Unused/reserved.
 *
 * TagID: Used to match the Tag-ID in that vac tags.
 *
 * LINE: Which line within the cache
 *
 * BYTE-in-LINE: Which byte within the cache line.
 */

/* Sun4c VAC Tags */
#define S4CVACTAG_CID      0x01c00000
#define S4CVACTAG_W        0x00200000
#define S4CVACTAG_P        0x00100000
#define S4CVACTAG_V        0x00080000
#define S4CVACTAG_TID      0x0000fffc

/* Sun4c VAC Virtual Address */
/* These aren't used, why bother? (Anton) */

/* The indexing of cache lines creates a problem.  Because the line
 * field of a virtual address extends past the page offset within
 * the virtual address it is possible to have what are called
 * 'bad aliases' which will create inconsistencies.  So we must make
 * sure that within a context that if a physical page is mapped
 * more than once, that 'extra' line bits are the same.  If this is
 * not the case, and thus is a 'bad alias' we must turn off the
 * cacheable bit in the pte's of all such pages.
 */

#ifdef CONFIG_SUN4
#define S4CVAC_BADBITS     0x0001e000
#else
#define S4CVAC_BADBITS    0x0000f000
#endif

/* The following is true if vaddr1 and vaddr2 would cause
 * a 'bad alias'.
 */
#define S4CVAC_BADALIAS(vaddr1, vaddr2) \
        ((((unsigned long) (vaddr1)) ^ ((unsigned long) (vaddr2))) & \
	 (S4CVAC_BADBITS))

/* The following structure describes the characteristics of a sun4c
 * VAC as probed from the prom during boot time.
 */
struct sun4c_vac_props {
	unsigned int num_bytes;     /* Size of the cache */
	unsigned int num_lines;     /* Number of cache lines */
	unsigned int do_hwflushes;  /* Hardware flushing available? */
	enum { NONE, WRITE_THROUGH,
	    WRITE_BACK } type;      /* What type of VAC? */
	unsigned int linesize;      /* Size of each line in bytes */
	unsigned int log2lsize;     /* log2(linesize) */
	unsigned int on;            /* VAC is enabled */
};

extern struct sun4c_vac_props sun4c_vacinfo;

/* sun4c_enable_vac() enables the sun4c virtual address cache. */
extern __inline__ void sun4c_enable_vac(void)
{
  __asm__ __volatile__("lduba [%0] %1, %%g1\n\t"
		       "or    %%g1, %2, %%g1\n\t"
		       "stba  %%g1, [%0] %1\n\t" : :
		       "r" ((unsigned int) AC_SENABLE),
		       "i" (ASI_CONTROL), "i" (SENABLE_CACHE) :
		       "g1");
  sun4c_vacinfo.on = 1;
}

/* sun4c_disable_vac() disables the virtual address cache. */
extern __inline__ void sun4c_disable_vac(void)
{
  __asm__ __volatile__("lduba [%0] %1, %%g1\n\t"
		       "andn  %%g1, %2, %%g1\n\t"
		       "stba  %%g1, [%0] %1\n\t" : :
		       "r" ((unsigned int) AC_SENABLE),
		       "i" (ASI_CONTROL), "i" (SENABLE_CACHE) :
		       "g1");
  sun4c_vacinfo.on = 0;
}

#endif /* !(_SPARC_VAC_OPS_H) */
