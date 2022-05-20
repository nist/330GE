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
 |  fpu_arith.c                                                              |
 |                                                                           |
 | Code to implement the FPU register/register arithmetic instructions       |
 |                                                                           |
 | Copyright (C) 1992,1993,1997                                              |
 |                  W. Metzenthen, 22 Parker St, Ormond, Vic 3163, Australia |
 |                  E-mail   billm@suburbia.net                              |
 |                                                                           |
 |                                                                           |
 +---------------------------------------------------------------------------*/

#include "fpu_system.h"
#include "fpu_emu.h"
#include "control_w.h"
#include "status_w.h"


void fadd__()
{
  /* fadd st,st(i) */
  int i = FPU_rm;
  clear_C1();
  FPU_add(&st(i), FPU_gettagi(i), 0, control_word);
}


void fmul__()
{
  /* fmul st,st(i) */
  int i = FPU_rm;
  clear_C1();
  FPU_mul(&st(i), FPU_gettagi(i), 0, control_word);
}



void fsub__()
{
  /* fsub st,st(i) */
  clear_C1();
  FPU_sub(0, FPU_rm, control_word);
}


void fsubr_()
{
  /* fsubr st,st(i) */
  clear_C1();
  FPU_sub(REV, FPU_rm, control_word);
}


void fdiv__()
{
  /* fdiv st,st(i) */
  clear_C1();
  FPU_div(0, FPU_rm, control_word);
}


void fdivr_()
{
  /* fdivr st,st(i) */
  clear_C1();
  FPU_div(REV, FPU_rm, control_word);
}



void fadd_i()
{
  /* fadd st(i),st */
  int i = FPU_rm;
  clear_C1();
  FPU_add(&st(i), FPU_gettagi(i), i, control_word);
}


void fmul_i()
{
  /* fmul st(i),st */
  clear_C1();
  FPU_mul(&st(0), FPU_gettag0(), FPU_rm, control_word);
}


void fsubri()
{
  /* fsubr st(i),st */
  clear_C1();
  FPU_sub(DEST_RM, FPU_rm, control_word);
}


void fsub_i()
{
  /* fsub st(i),st */
  clear_C1();
  FPU_sub(REV|DEST_RM, FPU_rm, control_word);
}


void fdivri()
{
  /* fdivr st(i),st */
  clear_C1();
  FPU_div(DEST_RM, FPU_rm, control_word);
}


void fdiv_i()
{
  /* fdiv st(i),st */
  clear_C1();
  FPU_div(REV|DEST_RM, FPU_rm, control_word);
}



void faddp_()
{
  /* faddp st(i),st */
  int i = FPU_rm;
  clear_C1();
  if ( FPU_add(&st(i), FPU_gettagi(i), i, control_word) >= 0 )
    FPU_pop();
}


void fmulp_()
{
  /* fmulp st(i),st */
  clear_C1();
  if ( FPU_mul(&st(0), FPU_gettag0(), FPU_rm, control_word) >= 0 )
    FPU_pop();
}



void fsubrp()
{
  /* fsubrp st(i),st */
  clear_C1();
  if ( FPU_sub(DEST_RM, FPU_rm, control_word) >= 0 )
    FPU_pop();
}


void fsubp_()
{
  /* fsubp st(i),st */
  clear_C1();
  if ( FPU_sub(REV|DEST_RM, FPU_rm, control_word) >= 0 )
    FPU_pop();
}


void fdivrp()
{
  /* fdivrp st(i),st */
  clear_C1();
  if ( FPU_div(DEST_RM, FPU_rm, control_word) >= 0 )
    FPU_pop();
}


void fdivp_()
{
  /* fdivp st(i),st */
  clear_C1();
  if ( FPU_div(REV|DEST_RM, FPU_rm, control_word) >= 0 )
    FPU_pop();
}
