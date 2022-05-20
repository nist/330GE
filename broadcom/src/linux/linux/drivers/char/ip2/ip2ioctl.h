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
*   DESCRIPTION: Driver constants for configuration and tuning
*
*   NOTES:
*
*******************************************************************************/

#ifndef IP2IOCTL_H
#define IP2IOCTL_H

//*************
//* Constants *
//*************

// High baud rates (if not defined elsewhere.
#ifndef B153600   
#	define B153600   0010005
#endif
#ifndef B307200   
#	define B307200   0010006
#endif
#ifndef B921600   
#	define B921600   0010007
#endif

#endif
