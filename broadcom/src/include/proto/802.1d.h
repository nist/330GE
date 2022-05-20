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
 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * Fundamental types and constants relating to 802.1D
 *
 * $Id: 802.1d.h,v 1.1.1.1 2007/01/25 12:52:12 jiahao_jhou Exp $
 */

#ifndef _802_1_D_
#define _802_1_D_

/* 802.1D priority defines */
#define	PRIO_8021D_NONE		2	/* None = - */
#define	PRIO_8021D_BK		1	/* BK - Background */
#define	PRIO_8021D_BE		0	/* BE - Best-effort */
#define	PRIO_8021D_EE		3	/* EE - Excellent-effort */
#define	PRIO_8021D_CL		4	/* CL - Controlled Load */
#define	PRIO_8021D_VI		5	/* Vi - Video */
#define	PRIO_8021D_VO		6	/* Vo - Voice */
#define	PRIO_8021D_NC		7	/* NC - Network Control */
#define	MAXPRIO			7	/* 0-7 */
#define NUMPRIO			(MAXPRIO + 1)

#endif /* _802_1_D__ */
