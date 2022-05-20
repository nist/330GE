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
 |  control_w.h                                                              |
 |                                                                           |
 | Copyright (C) 1992,1993                                                   |
 |                       W. Metzenthen, 22 Parker St, Ormond, Vic 3163,      |
 |                       Australia.  E-mail   billm@vaxc.cc.monash.edu.au    |
 |                                                                           |
 +---------------------------------------------------------------------------*/

#ifndef _CONTROLW_H_
#define _CONTROLW_H_

#ifdef __ASSEMBLY__
#define	_Const_(x)	$##x
#else
#define	_Const_(x)	x
#endif

#define CW_RC		_Const_(0x0C00)	/* rounding control */
#define CW_PC		_Const_(0x0300)	/* precision control */

#define CW_Precision	Const_(0x0020)	/* loss of precision mask */
#define CW_Underflow	Const_(0x0010)	/* underflow mask */
#define CW_Overflow	Const_(0x0008)	/* overflow mask */
#define CW_ZeroDiv	Const_(0x0004)	/* divide by zero mask */
#define CW_Denormal	Const_(0x0002)	/* denormalized operand mask */
#define CW_Invalid	Const_(0x0001)	/* invalid operation mask */

#define CW_Exceptions  	_Const_(0x003f)	/* all masks */

#define RC_RND		_Const_(0x0000)
#define RC_DOWN		_Const_(0x0400)
#define RC_UP		_Const_(0x0800)
#define RC_CHOP		_Const_(0x0C00)

/* p 15-5: Precision control bits affect only the following:
   ADD, SUB(R), MUL, DIV(R), and SQRT */
#define PR_24_BITS        _Const_(0x000)
#define PR_53_BITS        _Const_(0x200)
#define PR_64_BITS        _Const_(0x300)
#define PR_RESERVED_BITS  _Const_(0x100)
/* FULL_PRECISION simulates all exceptions masked */
#define FULL_PRECISION  (PR_64_BITS | RC_RND | 0x3f)

#endif /* _CONTROLW_H_ */
