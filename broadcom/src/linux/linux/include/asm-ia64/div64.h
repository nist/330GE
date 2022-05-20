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
#ifndef _ASM_IA64_DIV64_H
#define _ASM_IA64_DIV64_H

/*
 * Copyright (C) 1999 Hewlett-Packard Co
 * Copyright (C) 1999 David Mosberger-Tang <davidm@hpl.hp.com>
 *
 * vsprintf uses this to divide a 64-bit integer N by a small integer BASE.
 * This is incredibly hard on IA-64...
 */

#define do_div(n,base)						\
({								\
	int _res;						\
	_res = ((unsigned long) (n)) % (unsigned) (base);	\
	(n) = ((unsigned long) (n)) / (unsigned) (base);	\
	_res;							\
})

#endif /* _ASM_IA64_DIV64_H */
