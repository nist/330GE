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
#ifndef	__OSDEP_H__
#define __OSDEP_H__

/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2, or (at
 * your option) any later version.
 */

#if	defined(__linux__) || defined(__FreeBSD__) || defined(__MIPSEB__)
#define	ETHERBOOT32
#define ntohl(x) (x)
#define htonl(x) (x)
#define ntohs(x) (x)
#define htons(x) (x)

#endif




/* ANSI prototyping macro */
#ifdef	__STDC__
#define	P(x)	x
#else
#define	P(x)	()
#endif

#endif

/*
 * Local variables:
 *  c-basic-offset: 8
 * End:
 */
