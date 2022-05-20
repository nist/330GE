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
 * BK Id: SCCS/s.unaligned.h 1.5 05/17/01 18:14:26 cort
 */
#ifdef __KERNEL__
#ifndef __PPC_UNALIGNED_H
#define __PPC_UNALIGNED_H

/*
 * The PowerPC can do unaligned accesses itself in big endian mode. 
 *
 * The strange macros are there to make sure these can't
 * be misused in a way that makes them not work on other
 * architectures where unaligned accesses aren't as simple.
 */

#define get_unaligned(ptr) (*(ptr))

#define put_unaligned(val, ptr) ((void)( *(ptr) = (val) ))

#endif
#endif /* __KERNEL__ */
