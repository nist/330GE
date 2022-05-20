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

  fp_trig.c: floating-point math routines for the Linux-m68k
  floating point emulator.

  Copyright (c) 1998-1999 David Huggins-Daines / Roman Zippel.

  I hereby give permission, free of charge, to copy, modify, and
  redistribute this software, in source or binary form, provided that
  the above copyright notice and the following disclaimer are included
  in all such copies.

  THIS SOFTWARE IS PROVIDED "AS IS", WITH ABSOLUTELY NO WARRANTY, REAL
  OR IMPLIED.

*/

#include "fp_emu.h"

struct fp_ext *
fp_fsqrt(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("fsqrt\n");

	fp_monadic_check(dest, src);

	if (IS_ZERO(dest))
		return dest;

	if (dest->sign) {
		fp_set_nan(dest);
		return dest;
	}
	if (IS_INF(dest))
		return dest;

	return dest;
}

struct fp_ext *
fp_fetoxm1(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("fetoxm1\n");

	fp_monadic_check(dest, src);

	if (IS_ZERO(dest))
		return dest;

	return dest;
}

struct fp_ext *
fp_fetox(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("fetox\n");

	fp_monadic_check(dest, src);

	return dest;
}

struct fp_ext *
fp_ftwotox(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("ftwotox\n");

	fp_monadic_check(dest, src);

	return dest;
}

struct fp_ext *
fp_ftentox(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("ftentox\n");

	fp_monadic_check(dest, src);

	return dest;
}

struct fp_ext *
fp_flogn(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("flogn\n");

	fp_monadic_check(dest, src);

	return dest;
}

struct fp_ext *
fp_flognp1(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("flognp1\n");

	fp_monadic_check(dest, src);

	return dest;
}

struct fp_ext *
fp_flog10(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("flog10\n");

	fp_monadic_check(dest, src);

	return dest;
}

struct fp_ext *
fp_flog2(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("flog2\n");

	fp_monadic_check(dest, src);

	return dest;
}

struct fp_ext *
fp_fgetexp(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("fgetexp\n");

	fp_monadic_check(dest, src);

	return dest;
}

struct fp_ext *
fp_fgetman(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("fgetman\n");

	fp_monadic_check(dest, src);

	return dest;
}

