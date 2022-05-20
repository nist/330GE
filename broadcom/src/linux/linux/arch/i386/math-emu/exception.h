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
 |  exception.h                                                              |
 |                                                                           |
 | Copyright (C) 1992    W. Metzenthen, 22 Parker St, Ormond, Vic 3163,      |
 |                       Australia.  E-mail   billm@vaxc.cc.monash.edu.au    |
 |                                                                           |
 +---------------------------------------------------------------------------*/

#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_


#ifdef __ASSEMBLY__
#define	Const_(x)	$##x
#else
#define	Const_(x)	x
#endif

#ifndef SW_C1
#include "fpu_emu.h"
#endif /* SW_C1 */

#define FPU_BUSY        Const_(0x8000)   /* FPU busy bit (8087 compatibility) */
#define EX_ErrorSummary Const_(0x0080)   /* Error summary status */
/* Special exceptions: */
#define	EX_INTERNAL	Const_(0x8000)	/* Internal error in wm-FPU-emu */
#define EX_StackOver	Const_(0x0041|SW_C1)	/* stack overflow */
#define EX_StackUnder	Const_(0x0041)	/* stack underflow */
/* Exception flags: */
#define EX_Precision	Const_(0x0020)	/* loss of precision */
#define EX_Underflow	Const_(0x0010)	/* underflow */
#define EX_Overflow	Const_(0x0008)	/* overflow */
#define EX_ZeroDiv	Const_(0x0004)	/* divide by zero */
#define EX_Denormal	Const_(0x0002)	/* denormalized operand */
#define EX_Invalid	Const_(0x0001)	/* invalid operation */


#define PRECISION_LOST_UP    Const_((EX_Precision | SW_C1))
#define PRECISION_LOST_DOWN  Const_(EX_Precision)


#ifndef __ASSEMBLY__

#ifdef DEBUG
#define	EXCEPTION(x)	{ printk("exception in %s at line %d\n", \
	__FILE__, __LINE__); FPU_exception(x); }
#else
#define	EXCEPTION(x)	FPU_exception(x)
#endif

#endif /* __ASSEMBLY__ */ 

#endif /* _EXCEPTION_H_ */
