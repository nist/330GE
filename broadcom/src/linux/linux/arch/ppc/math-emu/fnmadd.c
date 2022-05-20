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
 * BK Id: SCCS/s.fnmadd.c 1.6 05/17/01 18:14:22 cort
 */
#include <linux/types.h>
#include <linux/errno.h>
#include <asm/uaccess.h>

#include "soft-fp.h"
#include "double.h"

int
fnmadd(void *frD, void *frA, void *frB, void *frC)
{
	FP_DECL_D(R);
	FP_DECL_D(A);
	FP_DECL_D(B);
	FP_DECL_D(C);
	FP_DECL_D(T);
	int ret = 0;

#ifdef DEBUG
	printk("%s: %p %p %p %p\n", __FUNCTION__, frD, frA, frB, frC);
#endif

	__FP_UNPACK_D(A, frA);
	__FP_UNPACK_D(B, frB);
	__FP_UNPACK_D(C, frC);

#ifdef DEBUG
	printk("A: %ld %lu %lu %ld (%ld)\n", A_s, A_f1, A_f0, A_e, A_c);
	printk("B: %ld %lu %lu %ld (%ld)\n", B_s, B_f1, B_f0, B_e, B_c);
	printk("C: %ld %lu %lu %ld (%ld)\n", C_s, C_f1, C_f0, C_e, C_c);
#endif

	if ((A_c == FP_CLS_INF && C_c == FP_CLS_ZERO) ||
	    (A_c == FP_CLS_ZERO && C_c == FP_CLS_INF))
                ret |= EFLAG_VXIMZ;

	FP_MUL_D(T, A, C);

	if (T_s != B_s && T_c == FP_CLS_INF && B_c == FP_CLS_INF)
		ret |= EFLAG_VXISI;

	FP_ADD_D(R, T, B);

	if (R_c != FP_CLS_NAN)
		R_s ^= 1;

#ifdef DEBUG
	printk("D: %ld %lu %lu %ld (%ld)\n", R_s, R_f1, R_f0, R_e, R_c);
#endif

	return (ret | __FP_PACK_D(frD, R));
}
