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
 * 802.1Q VLAN protocol definitions
 *
 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: vlan.h,v 1.1.1.1 2007/01/25 12:52:12 jiahao_jhou Exp $
 */

#ifndef _vlan_h_
#define _vlan_h_

/* enable structure packing */
#if defined(__GNUC__)
#define	PACKED	__attribute__((packed))
#else
#pragma pack(1)
#define	PACKED
#endif

#define VLAN_VID_MASK		0xfff	/* low 12 bits are vlan id */
#define	VLAN_CFI_SHIFT		12	/* canonical format indicator bit */
#define VLAN_PRI_SHIFT		13	/* user priority */

#define VLAN_PRI_MASK		7	/* 3 bits of priority */

#define	VLAN_TAG_LEN		4
#define	VLAN_TAG_OFFSET		(2 * ETHER_ADDR_LEN)

struct ethervlan_header {
	uint8	ether_dhost[ETHER_ADDR_LEN];
	uint8	ether_shost[ETHER_ADDR_LEN];
	uint16	vlan_type;		/* 0x8100 */
	uint16	vlan_tag;		/* priority, cfi and vid */
	uint16	ether_type;
};

#define	ETHERVLAN_HDR_LEN	(ETHER_HDR_LEN + VLAN_TAG_LEN)

#undef PACKED
#if !defined(__GNUC__)
#pragma pack()
#endif

#endif /* _vlan_h_ */
