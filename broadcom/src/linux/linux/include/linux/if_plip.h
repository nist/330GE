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
 *	NET3	PLIP tuning facilities for the new Niibe PLIP.
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 *
 */
 
#ifndef _LINUX_IF_PLIP_H
#define _LINUX_IF_PLIP_H

#include <linux/sockios.h>

#define	SIOCDEVPLIP	SIOCDEVPRIVATE

struct plipconf
{
	unsigned short pcmd;
	unsigned long  nibble;
	unsigned long  trigger;
};

#define PLIP_GET_TIMEOUT	0x1
#define PLIP_SET_TIMEOUT	0x2

#endif
