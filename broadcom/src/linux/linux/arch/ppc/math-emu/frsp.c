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
 * BK Id: SCCS/s.frsp.c 1.6 05/17/01 18:14:22 cort
 */
#include <linux/types.h>
#include <linux/errno.h>
#include <asm/uaccess.h>

#include "soft-fp.h"
#include "double.h"
#include "single.h"

int
frsp(void *frD, void *frB)
{
	FP_DECL_D(B);

#ifdef DEBUG
	printk("%s: D %p, B %p\n", __FUNCTION__, frD, frB);
#endif

	__FP_UNPACK_D(B, frB);

#ifdef DEBUG
	printk("B: %ld %lu %lu %ld (%ld)\n", B_s, B_f1, B_f0, B_e, B_c);
#endif

	return __FP_PACK_DS(frD, B);
}