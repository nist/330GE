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
 * BK Id: SCCS/s.lfs.c 1.6 05/17/01 18:14:23 cort
 */
#include <linux/types.h>
#include <linux/errno.h>
#include <asm/uaccess.h>

#include "soft-fp.h"
#include "double.h"
#include "single.h"

int
lfs(void *frD, void *ea)
{
	FP_DECL_D(R);
	FP_DECL_S(A);
	float f;

#ifdef DEBUG
	printk("%s: D %p, ea %p\n", __FUNCTION__, frD, ea);
#endif

	if (copy_from_user(&f, ea, sizeof(float)))
		return -EFAULT;

	__FP_UNPACK_S(A, &f);

#ifdef DEBUG
	printk("A: %ld %lu %ld (%ld) [%08lx]\n", A_s, A_f, A_e, A_c,
	       *(unsigned long *)&f);
#endif

	FP_CONV(D, S, 2, 1, R, A);

#ifdef DEBUG
	printk("R: %ld %lu %lu %ld (%ld)\n", R_s, R_f1, R_f0, R_e, R_c);
#endif

	return __FP_PACK_D(frD, R);
}
