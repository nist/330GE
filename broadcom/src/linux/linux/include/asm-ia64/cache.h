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
#ifndef _ASM_IA64_CACHE_H
#define _ASM_IA64_CACHE_H

#include <linux/config.h>

/*
 * Copyright (C) 1998-2000 Hewlett-Packard Co
 *	David Mosberger-Tang <davidm@hpl.hp.com>
 */

/* Bytes per L1 (data) cache line.  */
#define L1_CACHE_SHIFT		CONFIG_IA64_L1_CACHE_SHIFT
#define L1_CACHE_BYTES		(1 << L1_CACHE_SHIFT)

#ifdef CONFIG_SMP
# define SMP_CACHE_SHIFT	L1_CACHE_SHIFT
# define SMP_CACHE_BYTES	L1_CACHE_BYTES
#else
  /*
   * The "aligned" directive can only _increase_ alignment, so this is
   * safe and provides an easy way to avoid wasting space on a
   * uni-processor:
   */
# define SMP_CACHE_SHIFT	3
# define SMP_CACHE_BYTES	(1 << 3)
#endif

#endif /* _ASM_IA64_CACHE_H */
