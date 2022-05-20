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
 * BK Id: SCCS/s.setjmp.c 1.5 05/17/01 18:14:23 cort
 */
/*
 * Copyright (C) 1996 Paul Mackerras.
 *
 * NB this file must be compiled with -O2.
 */

int
xmon_setjmp(long *buf)
{
    asm ("mflr 0; stw 0,0(%0);"
	 "stw 1,4(%0); stw 2,8(%0);"
	 "mfcr 0; stw 0,12(%0);"
	 "stmw 13,16(%0)"
	 : : "r" (buf));
    return 0;
}

void
xmon_longjmp(long *buf, int val)
{
    if (val == 0)
	val = 1;
    asm ("lmw 13,16(%0);"
	 "lwz 0,12(%0); mtcrf 0x38,0;"
	 "lwz 0,0(%0); lwz 1,4(%0); lwz 2,8(%0);"
	 "mtlr 0; mr 3,%1"
	 : : "r" (buf), "r" (val));
}
