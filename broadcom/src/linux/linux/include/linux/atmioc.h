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
/* atmioc.h - ranges for ATM-related ioctl numbers */
 
/* Written 1995-1999 by Werner Almesberger, EPFL LRC/ICA */


/*
 * See http://icawww1.epfl.ch/linux-atm/magic.html for the complete list of
 * "magic" ioctl numbers.
 */


#ifndef _LINUX_ATMIOC_H
#define _LINUX_ATMIOC_H

#include <asm/ioctl.h>
		/* everybody including atmioc.h will also need _IO{,R,W,WR} */

#define ATMIOC_PHYCOM	  0x00 /* PHY device common ioctls, globally unique */
#define ATMIOC_PHYCOM_END 0x0f
#define ATMIOC_PHYTYP	  0x10 /* PHY dev type ioctls, unique per PHY type */
#define ATMIOC_PHYTYP_END 0x2f
#define ATMIOC_PHYPRV	  0x30 /* PHY dev private ioctls, unique per driver */
#define ATMIOC_PHYPRV_END 0x4f
#define ATMIOC_SARCOM	  0x50 /* SAR device common ioctls, globally unique */
#define ATMIOC_SARCOM_END 0x50
#define ATMIOC_SARPRV	  0x60 /* SAR dev private ioctls, unique per driver */
#define ATMIOC_SARPRV_END 0x7f
#define ATMIOC_ITF	  0x80 /* Interface ioctls, globally unique */
#define ATMIOC_ITF_END	  0x8f
#define ATMIOC_BACKEND	  0x90 /* ATM generic backend ioctls, u. per backend */
#define ATMIOC_BACKEND_END 0xaf
/* 0xb0-0xbf: Reserved for future use */
#define ATMIOC_AREQUIPA	  0xc0 /* Application requested IP over ATM, glob. u. */
#define ATMIOC_LANE	  0xd0 /* LAN Emulation, globally unique */
#define ATMIOC_MPOA       0xd8 /* MPOA, globally unique */
#define	ATMIOC_CLIP	  0xe0 /* Classical IP over ATM control, globally u. */
#define	ATMIOC_CLIP_END	  0xef
#define	ATMIOC_SPECIAL	  0xf0 /* Special-purpose controls, globally unique */
#define	ATMIOC_SPECIAL_END 0xff

#endif
