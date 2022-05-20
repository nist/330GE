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
 * Robo switch configuration
 *
 * Copyright 2004, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: switch.c,v 1.1.1.1 2007/01/25 12:52:21 jiahao_jhou Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <syslog.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if_arp.h>
#include <bcmnvram.h>
#include <netconf.h>
#include <shutils.h>
#include <rc.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>
#include <sys/ioctl.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define uint32 unsigned long
#define uint unsigned int
#define uint8 unsigned char
#define uint64 unsigned long long
#include <swmod.h>
#include <swapi.h>
#include <nvports.h>
#include <bcmdevs.h>


void setup_switch(void)
{
	int portid, max_port = BCM_NUM_PORTS;
	PORT_ATTRIBS port_attribs;
	return;
	
	if ((bcm_api_init())<0) {
        if (bcm_is_robo())
        {
	        fprintf(stderr,"No Robo device found\n");
        }
        return;
	}
	printf("Initializing Ethernet switch "
	       "controller (%d ports)\n", max_port);
	for(portid=1; portid <= max_port;portid++) {
	    /* get nvram attributes associated with port and set switch regs */
	    port_attribs = nvGetSwitchPortAttribs(portid);
	    bcm_set_port_attributes(&port_attribs, portid);
	}
	bcm_api_deinit();

}

