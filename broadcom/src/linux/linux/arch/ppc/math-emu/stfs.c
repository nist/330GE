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
 * BK Id: SCCS/s.stfs.c 1.6 05/17/01 18:14:23 cort
 */
#include <linux/types.h>
#include <linux/errno.h>
#include <asm/uaccess.h>

#include "soft-fp.h"
#include "double.h"
#include "single.h"

int
stfs(void *frS, void *ea)
{
	FP_DECL_D(A);
	FP_DECL_S(R);
	float f;
	int err;

#ifdef DEBUG
	printk("%s: S %p, ea %p\n", __FUNCTION__, frS, ea);
#endif

	__FP_UNPACK_D(A, frS);

#ifdef DEBUG
	printk("A: %ld %lu %lu %ld (%ld)\n", A_s, A_f1, A_f0, A_e, A_c);
#endif

	FP_CONV(S, D, 1, 2, R, A);

#ifdef DEBUG
	printk("R: %ld %lu %ld (%ld)\n", R_s, R_f, R_e, R_c);
#endif

	err = _FP_PACK_CANONICAL(S, 1, R);
	if (!err || !__FPU_TRAP_P(err)) {
		__FP_PACK_RAW_1(S, &f, R);
		if (copy_to_user(ea, &f, sizeof(float)))
			return -EFAULT;
	}

	return err;
}
