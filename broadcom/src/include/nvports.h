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
 * BCM53xx RoboSwitch utility functions
 *
 * Copyright (C) 2002 ASUSTek Inc.
 * $Id: nvports.h,v 1.1.1.1 2007/01/25 12:52:12 jiahao_jhou Exp $
 */

#ifndef _nvports_h_
#define _nvports_h_

#define uint32 unsigned long
#define uint16 unsigned short
#define uint unsigned int
#define uint8 unsigned char
#define uint64 unsigned long long

enum FORCE_PORT {
	FORCE_OFF,
	FORCE_10H,
	FORCE_10F,
	FORCE_100H,
	FORCE_100F,
	FORCE_DOWN,
	POWER_OFF
};

typedef struct _PORT_ATTRIBS
{
	uint 	autoneg;
	uint	force;
	uint	native;	
} PORT_ATTRIBS;

extern uint
nvExistsPortAttrib(char *attrib, uint portno);

extern int
nvExistsAnyForcePortAttrib(uint portno);

extern void
nvSetPortAttrib(char *attrib, uint portno);

extern void
nvUnsetPortAttrib(char *attrib, uint portno);

extern void
nvUnsetAllForcePortAttrib(uint portno);

extern PORT_ATTRIBS
nvGetSwitchPortAttribs(uint portno);

#endif /* _nvports_h_ */



