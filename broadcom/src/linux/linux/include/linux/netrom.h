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
 * These are the public elements of the Linux kernel NET/ROM implementation.
 * For kernel AX.25 see the file ax25.h. This file requires ax25.h for the
 * definition of the ax25_address structure.
 */

#ifndef	NETROM_KERNEL_H
#define	NETROM_KERNEL_H

#define NETROM_MTU	236

#define NETROM_T1	1
#define NETROM_T2	2
#define NETROM_N2	3
#define	NETROM_T4	6
#define	NETROM_IDLE	7

#define	SIOCNRDECOBS		(SIOCPROTOPRIVATE+2)

struct nr_route_struct {
#define	NETROM_NEIGH	0
#define	NETROM_NODE	1
	int		type;
	ax25_address	callsign;
	char		device[16];
	unsigned int	quality;
	char		mnemonic[7];
	ax25_address	neighbour;
	unsigned int	obs_count;
	unsigned int	ndigis;
	ax25_address	digipeaters[AX25_MAX_DIGIS];
};

#endif
