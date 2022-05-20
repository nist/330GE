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
/* $Id: ipac.h,v 1.1.1.1 2007/01/25 12:51:54 jiahao_jhou Exp $
 *
 * IPAC specific defines
 *
 * Author       Karsten Keil
 * Copyright    by Karsten Keil      <keil@isdn4linux.de>
 * 
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

/* All Registers original Siemens Spec  */

#define IPAC_CONF	0xC0
#define IPAC_MASK	0xC1
#define IPAC_ISTA	0xC1
#define IPAC_ID		0xC2
#define IPAC_ACFG	0xC3
#define IPAC_AOE	0xC4
#define IPAC_ARX	0xC5
#define IPAC_ATX	0xC5
#define IPAC_PITA1	0xC6
#define IPAC_PITA2	0xC7
#define IPAC_POTA1	0xC8
#define IPAC_POTA2	0xC9
#define IPAC_PCFG	0xCA
#define IPAC_SCFG	0xCB
#define IPAC_TIMR2	0xCC
