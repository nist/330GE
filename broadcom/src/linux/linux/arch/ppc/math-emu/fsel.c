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
 * BK Id: SCCS/s.fsel.c 1.6 05/17/01 18:14:22 cort
 */
#include <linux/types.h>
#include <linux/errno.h>
#include <asm/uaccess.h>

#include "soft-fp.h"
#include "double.h"

int
fsel(u32 *frD, void *frA, u32 *frB, u32 *frC)
{
	FP_DECL_D(A);

#ifdef DEBUG
	printk("%s: %p %p %p %p\n", __FUNCTION__, frD, frA, frB, frC);
#endif

	__FP_UNPACK_D(A, frA);

#ifdef DEBUG
	printk("A: %ld %lu %lu %ld (%ld)\n", A_s, A_f1, A_f0, A_e, A_c);
	printk("B: %08x %08x\n", frB[0], frB[1]);
	printk("C: %08x %08x\n", frC[0], frC[1]);
#endif

	if (A_c == FP_CLS_NAN || (A_c != FP_CLS_ZERO && A_s)) {
		frD[0] = frB[0];
		frD[1] = frB[1];
	} else {
		frD[0] = frC[0];
		frD[1] = frC[1];
	}

#ifdef DEBUG
	printk("D: %08x.%08x\n", frD[0], frD[1]);
#endif

	return 0;
}
