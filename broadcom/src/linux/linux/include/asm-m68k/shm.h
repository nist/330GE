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
#ifndef _M68K_SHM_H
#define _M68K_SHM_H

#include <linux/config.h>

/* format of page table entries that correspond to shared memory pages
   currently out in swap space (see also mm/swap.c):
   bits 0-1 (PAGE_PRESENT) is  = 0
   bits 8..2 (SWP_TYPE) are = SHM_SWP_TYPE
   bits 31..9 are used like this:
   bits 15..9 (SHM_ID) the id of the shared memory segment
   bits 30..16 (SHM_IDX) the index of the page within the shared memory segment
                    (actually only bits 25..16 get used since SHMMAX is so low)
   bit 31 (SHM_READ_ONLY) flag whether the page belongs to a read-only attach
*/
/* on the m68k both bits 0 and 1 must be zero */
/* format on the sun3 is similar, but bits 30, 31 are set to zero and all
   others are reduced by 2. --m */

#ifndef CONFIG_SUN3
#define SHM_ID_SHIFT	9
#else
#define SHM_ID_SHIFT	7
#endif
#define _SHM_ID_BITS	7
#define SHM_ID_MASK	((1<<_SHM_ID_BITS)-1)

#define SHM_IDX_SHIFT	(SHM_ID_SHIFT+_SHM_ID_BITS)
#define _SHM_IDX_BITS	15
#define SHM_IDX_MASK	((1<<_SHM_IDX_BITS)-1)

#endif /* _M68K_SHM_H */
