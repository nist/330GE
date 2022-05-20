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
 * BK Id: SCCS/s.types.c 1.5 05/17/01 18:14:23 cort
 */

#include "soft-fp.h"
#include "double.h"
#include "single.h"

void
fp_unpack_d(long *_s, unsigned long *_f1, unsigned long *_f0,
	    long *_e, long *_c, void *val)
{
	FP_DECL_D(X);

	__FP_UNPACK_RAW_2(D, X, val);

	_FP_UNPACK_CANONICAL(D, 2, X);

	*_s = X_s;
	*_f1 = X_f1;
	*_f0 = X_f0;
	*_e = X_e;
	*_c = X_c;
}

int
fp_pack_d(void *val, long X_s, unsigned long X_f1,
	  unsigned long X_f0, long X_e, long X_c)
{
	int exc;

	exc = _FP_PACK_CANONICAL(D, 2, X);
	if (!exc || !__FPU_TRAP_P(exc))
		__FP_PACK_RAW_2(D, val, X);
	return exc;
}

int
fp_pack_ds(void *val, long X_s, unsigned long X_f1,
	   unsigned long X_f0, long X_e, long X_c)
{
	FP_DECL_S(__X);
	int exc;

	FP_CONV(S, D, 1, 2, __X, X);
	exc = _FP_PACK_CANONICAL(S, 1, __X);
	if (!exc || !__FPU_TRAP_P(exc)) {
		_FP_UNPACK_CANONICAL(S, 1, __X);
		FP_CONV(D, S, 2, 1, X, __X);
		exc |= _FP_PACK_CANONICAL(D, 2, X);
		if (!exc || !__FPU_TRAP_P(exc))
			__FP_PACK_RAW_2(D, val, X);
	}
	return exc;
}
