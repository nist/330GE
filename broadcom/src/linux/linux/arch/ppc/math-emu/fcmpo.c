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
 * BK Id: SCCS/s.fcmpo.c 1.6 05/17/01 18:14:22 cort
 */
#include <linux/types.h>
#include <linux/errno.h>
#include <asm/uaccess.h>

#include "soft-fp.h"
#include "double.h"

int
fcmpo(u32 *ccr, int crfD, void *frA, void *frB)
{
	FP_DECL_D(A);
	FP_DECL_D(B);
	int code[4] = { (1 << 3), (1 << 1), (1 << 2), (1 << 0) };
	long cmp;
	int ret = 0;

#ifdef DEBUG
	printk("%s: %p (%08x) %d %p %p\n", __FUNCTION__, ccr, *ccr, crfD, frA, frB);
#endif

	__FP_UNPACK_D(A, frA);
	__FP_UNPACK_D(B, frB);

#ifdef DEBUG
	printk("A: %ld %lu %lu %ld (%ld)\n", A_s, A_f1, A_f0, A_e, A_c);
	printk("B: %ld %lu %lu %ld (%ld)\n", B_s, B_f1, B_f0, B_e, B_c);
#endif

	if (A_c == FP_CLS_NAN || B_c == FP_CLS_NAN)
		ret |= EFLAG_VXVC;

	FP_CMP_D(cmp, A, B, 2);
	cmp = code[(cmp + 1) & 3];

	__FPU_FPSCR &= ~(0x1f000);
	__FPU_FPSCR |= (cmp << 12);

	*ccr &= ~(15 << ((7 - crfD) << 2));
	*ccr |= (cmp << ((7 - crfD) << 2));
 
#ifdef DEBUG
	printk("CR: %08x\n", *ccr);
#endif

	return ret;
}
