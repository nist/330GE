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
/* Symbol versioning nastiness.  */

#define __SYMBOL_VERSION(x)       __ver_ ## x
#define __VERSIONED_SYMBOL2(x,v)  x ## _R ## v
#define __VERSIONED_SYMBOL1(x,v)  __VERSIONED_SYMBOL2(x,v)
#define __VERSIONED_SYMBOL(x)     __VERSIONED_SYMBOL1(x,__SYMBOL_VERSION(x))

#ifndef _set_ver
#define _set_ver(x)		  __VERSIONED_SYMBOL(x)
#endif
