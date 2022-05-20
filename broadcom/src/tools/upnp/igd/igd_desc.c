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
 * Copyright 2004, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id$
 */

#include "upnp_dbg.h"
#include "upnp_osl.h"
#include "upnp.h"


#undef INCLUDE_ETHERLINK
#undef INCLUDE_CABLELINKCONFIG
#undef INCLUDE_LANDEVICE
#undef INCLUDE_LANHOSTCONFIG
#undef INCLUDE_PPPCONNECTION

#define INCLUDE_LAYER3
#define INCLUDE_IPCONNECTION
#define INCLUDE_PPPCONNECTION



extern int WANDevice_Init(PDevice pdev, device_state_t state, va_list ap);
extern int LANDevice_Init(PDevice pdev, device_state_t state, va_list ap);
extern int IGDevice_Init(PDevice igdev, device_state_t state);


/* Global structure for storing the state table for this device */

extern ServiceTemplate Template_Layer3Forwarding;
extern ServiceTemplate Template_WANCommonInterfaceConfig;
extern ServiceTemplate Template_WANIPConnection;
extern ServiceTemplate Template_WANCableLinkConfig;
extern ServiceTemplate Template_WANEthernetLinkConfig;
extern ServiceTemplate Template_WANPPPConnection;

extern ServiceTemplate Template_LANHostConfigManagement;

PServiceTemplate svcs_igd[]           = { 
#if defined(INCLUDE_OSINFO)
    &Template_OSInfo,
#endif
#ifdef INCLUDE_LAYER3
    &Template_Layer3Forwarding
#endif
};

PServiceTemplate svcs_wandevice[]     = { 
     &Template_WANCommonInterfaceConfig 
};

PServiceTemplate svcs_wanconnection[] = { 
#if defined(INCLUDE_ETHERLINK)
     &Template_WANEthernetLinkConfig, 
#endif
#if defined(INCLUDE_PPPCONNECTION)
     &Template_WANPPPConnection,
#endif
#if defined(INCLUDE_IPCONNECTION)
     &Template_WANIPConnection,
#endif
#if defined(INCLUDE_CABLELINKCONFIG)
     &Template_WANCableLinkConfig,
#endif
};

PServiceTemplate svcs_landevice[] = { 
#if defined(INCLUDE_LANHOSTCONFIG)
     &Template_LANHostConfigManagement,
#endif
};

DeviceTemplate subdevs_wandevice[] = { 
    {
	"urn:schemas-upnp-org:device:WANConnectionDevice:1",
	"WANCONNECTION",
	NULL, /* PFDEVINIT */
	NULL, /* PFDEVXML */
	ARRAYSIZE(svcs_wanconnection), svcs_wanconnection 
    }
};

DeviceTemplate LANDeviceTemplate = {
	"urn:schemas-upnp-org:device:LANDevice:1",
	"LANDEVICEUDN",
	LANDevice_Init, 	/* PFDEVINIT */
	NULL, /* PFDEVXML */
	ARRAYSIZE(svcs_landevice), svcs_landevice 
};

DeviceTemplate WANDeviceTemplate = {
    "urn:schemas-upnp-org:device:WANDevice:1",
    "WANDEVICEUDN",
    WANDevice_Init, 	/* PFDEVINIT */
    NULL, 		/* PFDEVXML */
    ARRAYSIZE(svcs_wandevice), svcs_wandevice, 
    ARRAYSIZE(subdevs_wandevice), subdevs_wandevice
};
    
extern void igd_xml(PDevice pdev, UFILE *up);

DeviceTemplate IGDeviceTemplate = {
    "urn:schemas-upnp-org:device:InternetGatewayDevice:1",
    "ROOTUDN",
    IGDevice_Init,  	/* PFDEVINIT */
    NULL,  		/* PFDEVXML */
    ARRAYSIZE(svcs_igd), svcs_igd
};





