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
 *	For declaring structures shared with assembly routines
 *
 *	$Id: dgrs_asstruct.h,v 1.1.1.1 2007/01/25 12:51:55 jiahao_jhou Exp $
 */

#if ASSEMBLER

#	define MO(t,a)		(a)
#	define VMO(t,a)		(a)

#	define	BEGIN_STRUCT(x)	_Off=0
#	define	S1A(t,x,n)	_Off=(_Off+0)&~0; x=_Off; _Off=_Off+(1*n)
#	define	S2A(t,x,n)	_Off=(_Off+1)&~1; x=_Off; _Off=_Off+(2*n)
#	define	S4A(t,x,n)	_Off=(_Off+3)&~3; x=_Off; _Off=_Off+(4*n)
#	define	WORD(x)		_Off=(_Off+3)&~3; x=_Off; _Off=_Off+4
#	define	WORDA(x,n)	_Off=(_Off+3)&~3; x=_Off; _Off=_Off+(4*n)
#	define	VWORD(x)	_Off=(_Off+3)&~3; x=_Off; _Off=_Off+4
#	define	S1(t,x)		_Off=(_Off+0)&~0; x=_Off; _Off=_Off+1
#	define	S2(t,x)		_Off=(_Off+1)&~1; x=_Off; _Off=_Off+2
#	define	S4(t,x)		_Off=(_Off+3)&~3; x=_Off; _Off=_Off+4
#	define	END_STRUCT(x)	_Off=(_Off+3)&~3; x=_Off 

#else	/* C */

#define VMO(t,a)        (*(volatile t *)(a))

#	define BEGIN_STRUCT(x) struct x {
#	define S1(t,x)         t x ;
#	define S1A(t,x,n)      t x[n] ;
#	define S2(t,x)         t x ;
#	define S2A(t,x,n)      t x[n] ;
#	define S4(t,x)         t x ;
#	define S4A(t,x,n)      t x[n] ;
#	define END_STRUCT(x)   } ;

#endif
