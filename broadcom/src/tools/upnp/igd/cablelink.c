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

#include "upnp_osl.h"
#include "upnp_dbg.h"
#include "upnp.h"


char *CableLinkConfigState_allowedValueList[] = { "notReady",
						"dsSyncComplete", "usParamAcquired", "rangingComplete",
						"ipComplete", "todEstablished", "paramTransferComplete",
						"registrationComplete", "operational", "accessDenied", 0 };
char *LinkType_allowedValueList[] = { "Ethernet", 0 };

char *Modulation_allowedValueList[] = { "64QAM", "256QAM", 0 };



static VarTemplate StateVariables[] = { 
    { "CableLinkConfigState", "operational", VAR_STRING|VAR_LIST, (allowedValue) { CableLinkConfigState_allowedValueList } }, 
    { "LinkType", "Ethernet", VAR_STRING|VAR_LIST, (allowedValue) { LinkType_allowedValueList } }, 
    { "DownstreamFrequency", "", VAR_LONG }, 
    { "DownstreamModulation", "64QAM", VAR_STRING|VAR_LIST, (allowedValue) {  Modulation_allowedValueList }}, 
    { "UpstreamFrequency", "", VAR_LONG }, 
    { "UpstreamModulation", "64QAM", VAR_STRING|VAR_LIST, (allowedValue) { Modulation_allowedValueList } }, 
    { "UpstreamChannelID", "", VAR_LONG }, 
    { "UpstreamPowerLevel", "", VAR_LONG }, 
    { "ConfigFile", "", VAR_STRING }, 
    { "TFTPServer", "", VAR_STRING }, 
    { "BPIEncryptionEnabled", "", VAR_BOOL }, 
    { 0 } 
};

#define VAR_CableLinkConfigState	0
#define VAR_LinkType	1
#define VAR_DownstreamFrequency	2
#define VAR_DownstreamModulation	3
#define VAR_UpstreamFrequency	4
#define VAR_UpstreamModulation	5
#define VAR_UpstreamChannelID	6
#define VAR_UpstreamPowerLevel	7
#define VAR_ConfigFile	8
#define VAR_TFTPServer	9
#define VAR_BPIEncryptionEnabled	10

#define GetConfigFile			DefaultAction
#define GetUpstreamModulation		DefaultAction
#define GetUpstreamFrequency		DefaultAction
#define GetUpstreamChannelID		DefaultAction
#define GetTFTPServer			DefaultAction
#define GetDownstreamFrequency		DefaultAction
#define GetBPIEncryptionEnabled		DefaultAction
#define GetUpstreamPowerLevel		DefaultAction
#define GetDownstreamModulation		DefaultAction
#define GetCableLinkConfigInfo		DefaultAction


static Action _GetConfigFile = { 
    "GetConfigFile", GetConfigFile,
        {
            {"NewConfigFile", VAR_ConfigFile, VAR_OUT},
            { 0 }
        }
};

static Action _GetUpstreamModulation = { 
    "GetUpstreamModulation", GetUpstreamModulation,
        {
            {"NewUpstreamModulation", VAR_UpstreamModulation, VAR_OUT},
            { 0 }
        }
};

static Action _GetUpstreamFrequency = { 
    "GetUpstreamFrequency", GetUpstreamFrequency,
        {
            {"NewUpstreamFrequency", VAR_UpstreamFrequency, VAR_OUT},
            { 0 }
        }
};

static Action _GetUpstreamChannelID = { 
    "GetUpstreamChannelID", GetUpstreamChannelID,
        {
            {"NewUpstreamChannelID", VAR_UpstreamChannelID, VAR_OUT},
            { 0 }
        }
};

static Action _GetTFTPServer = { 
    "GetTFTPServer", GetTFTPServer,
        {
            {"NewTFTPServer", VAR_TFTPServer, VAR_OUT},
            { 0 }
        }
};

static Action _GetDownstreamFrequency = { 
    "GetDownstreamFrequency", GetDownstreamFrequency,
        {
            {"NewDownstreamFrequency", VAR_DownstreamFrequency, VAR_OUT},
            { 0 }
        }
};

static Action _GetBPIEncryptionEnabled = { 
    "GetBPIEncryptionEnabled", GetBPIEncryptionEnabled,
        {
            {"NewBPIEncryptionEnabled", VAR_BPIEncryptionEnabled, VAR_OUT},
            { 0 }
        }
};

static Action _GetUpstreamPowerLevel = { 
    "GetUpstreamPowerLevel", GetUpstreamPowerLevel,
        {
            {"NewUpstreamPowerLevel", VAR_UpstreamPowerLevel, VAR_OUT},
            { 0 }
        }
};

static Action _GetDownstreamModulation = { 
    "GetDownstreamModulation", GetDownstreamModulation,
        {
            {"NewDownstreamModulation", VAR_DownstreamModulation, VAR_OUT},
            { 0 }
        }
};

static Action _GetCableLinkConfigInfo = { 
    "GetCableLinkConfigInfo", GetCableLinkConfigInfo,
        {
            {"NewCableLinkConfigState", VAR_CableLinkConfigState, VAR_OUT},
            {"NewLinkType", VAR_LinkType, VAR_OUT},
            { 0 }
        }
};

static PAction Actions[] = {
    &_GetConfigFile,
    &_GetUpstreamModulation,
    &_GetUpstreamFrequency,
    &_GetUpstreamChannelID,
    &_GetTFTPServer,
    &_GetDownstreamFrequency,
    &_GetBPIEncryptionEnabled,
    &_GetUpstreamPowerLevel,
    &_GetDownstreamModulation,
    &_GetCableLinkConfigInfo,
    NULL
};



ServiceTemplate Template_WANCableLinkConfig = {
    "WANCableLinkConfig:1",
    NULL,	/* service initialization */
    NULL,	/* state variable handler */
    NULL,	/* xml generator */
    ARRAYSIZE(StateVariables)-1, StateVariables,
    Actions
};





