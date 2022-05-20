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
 * Broadcom Home Gateway Reference Design
 * BCM53xx RoboSwitch Broadcom tag driver header
 *
 * Copyright 2004, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 * $Id: brcmtag.h,v 1.1.1.1 2007/01/25 12:52:22 jiahao_jhou Exp $
 */

#ifndef _brcm_h_
#define _brcm_h_

#include <linux/netdevice.h>

struct brcm_dev_info {
	struct net_device *real_dev;    /* the underlying device/interface */
	struct net_device_stats dev_stats; /* Device stats (rx-bytes, tx-pkts, etc...) */
	uint port;
};
/* opcode defs */
#define BRCMTAG_UNICAST			0
#define BRCMTAG_MULTICAST		1
#define BRCMTAG_EGRESS_DIRECT	2
#define BRCMTAG_INGRESS_DIRECT	3
#define BRCMTAG_MULT_EGRESS		5
#pragma pack(1)
typedef struct _brcm_tag {
	unsigned char 	port:4;
	unsigned char 	cid_src:2;
	unsigned char 	rsvd1:2;
	unsigned char 	cid_dest:2;
	unsigned char 	rsvd:6;
	unsigned int 	count:11;
	unsigned char 	mirror_only:1;
	unsigned char 	mirror:1;
	unsigned char 	opcode:3;
} brcm_tag;
#pragma pack()

typedef struct _brcm_hdr {
   unsigned short       proto;  /* BRCM_TYPE */
   brcm_tag       		tag; 	/* brcm tag */
} brcm_hdr;

#define BRCM_DEV_INFO(x) ((struct brcm_dev_info *)(x->priv))

#define BRCM_TYPE 0x8874
#define BRCM_ETH_HLEN 20        /* total octets in header */
#define BRCM_VLAN_ETH_HLEN 24   /* total octets in header w/vlan tag also */
#define BRCM_HLEN	6		/* The additional bytes (on top of the Ethernet header) */
							/* required by brcm header */

/* min packet len is ETH_ZLEN + sizeof(FCS).  must pad to this length after VLAN and */
/* BRCM headers removed */
#define GET_PAD_BYTES(pkt_len) ((pkt_len) - (ETH_ZLEN + sizeof(uint32_t)) - VLAN_HLEN - sizeof(brcm_hdr))

void brcm_module_init(void);
void brcm_module_deinit(void);
struct net_device *register_brcmtag_device(const char *eth_IF_name,
				const char *suffix, uint port);
int unregister_brcm_device(const char *brcm_IF_name);


#endif /* _brcm_h_ */
