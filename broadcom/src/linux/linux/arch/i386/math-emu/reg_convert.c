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
/*---------------------------------------------------------------------------+
 |  reg_convert.c                                                            |
 |                                                                           |
 |  Convert register representation.                                         |
 |                                                                           |
 | Copyright (C) 1992,1993,1994,1996,1997                                    |
 |                  W. Metzenthen, 22 Parker St, Ormond, Vic 3163, Australia |
 |                  E-mail   billm@suburbia.net                              |
 |                                                                           |
 |                                                                           |
 +---------------------------------------------------------------------------*/

#include "exception.h"
#include "fpu_emu.h"


int FPU_to_exp16(FPU_REG const *a, FPU_REG *x)
{
  int sign = getsign(a);

  *(long long *)&(x->sigl) = *(const long long *)&(a->sigl);

  /* Set up the exponent as a 16 bit quantity. */
  setexponent16(x, exponent(a));

  if ( exponent16(x) == EXP_UNDER )
    {
      /* The number is a de-normal or pseudodenormal. */
      /* We only deal with the significand and exponent. */

      if (x->sigh & 0x80000000)
	{
	  /* Is a pseudodenormal. */
	  /* This is non-80486 behaviour because the number
	     loses its 'denormal' identity. */
	  addexponent(x, 1);
	}
      else
	{
	  /* Is a denormal. */
	  addexponent(x, 1);
	  FPU_normalize_nuo(x);
	}
    }

  if ( !(x->sigh & 0x80000000) )
    {
      EXCEPTION(EX_INTERNAL | 0x180);
    }

  return sign;
}

