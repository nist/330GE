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
/*******************************************************************************
*
*   (c) 1998 by Computone Corporation
*
********************************************************************************
*
*
*   PACKAGE:     Linux tty Device Driver for IntelliPort II family of multiport
*                serial I/O controllers.
*
*   DESCRIPTION: Driver constants and type definitions.
*
*   NOTES:
*
*******************************************************************************/
#ifndef IP2TYPES_H
#define IP2TYPES_H

//*************
//* Constants *
//*************

// Define some limits for this driver. Ports per board is a hardware limitation
// that will not change. Current hardware limits this to 64 ports per board.
// Boards per driver is a self-imposed limit.
//
#define IP2_MAX_BOARDS        4
#define IP2_PORTS_PER_BOARD   ABS_MOST_PORTS
#define IP2_MAX_PORTS         (IP2_MAX_BOARDS*IP2_PORTS_PER_BOARD)

#define ISA    0
#define PCI    1
#define EISA   2

//********************
//* Type Definitions *
//********************

typedef struct tty_struct *   PTTY;
typedef wait_queue_head_t   PWAITQ;

typedef unsigned char         UCHAR;
typedef unsigned int          UINT;
typedef unsigned short        USHORT;
typedef unsigned long         ULONG;

typedef struct 
{
	short irq[IP2_MAX_BOARDS]; 
	unsigned short addr[IP2_MAX_BOARDS];
	int type[IP2_MAX_BOARDS];
} ip2config_t;

#endif
