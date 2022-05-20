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
 * Ports Web Page Configuration Support Routines
 *
 * Copyright 2004, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 * $Id: nvports.c,v 1.1.1.1 2007/01/25 12:53:02 jiahao_jhou Exp $
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <typedefs.h>
#include <bcmnvram.h>
#include <bcmutils.h>
#include <../shared/shutils.h>
#include <nvports.h>
/* return TRUE if attrib exists and FALSE otherwise */
uint
nvExistsPortAttrib(char *attrib, uint portno)
{
	char port[] = "XXXXXXXXX";
	char *next, port_attrib[80];
	int hasAttrib;

    snprintf(port,sizeof(port),"port%d",portno);
    hasAttrib = 0;
    foreach(port_attrib, nvram_safe_get(port), next)
    {
        if (strncmp(port_attrib, attrib, strlen(attrib)) == 0)
            hasAttrib = 1;
    }
    return hasAttrib;
}

/* set attribute in nvram variable for portn */
void
nvSetPortAttrib(char *attrib, uint portno)
{
	char *next, port_attrib[80], new_attribs[128];
	char port[] = "XXXXXXXXX";
	int hasAttrib, buf_index;
	char *port_attribs;

    hasAttrib = 0;
    buf_index = 0;
    strcpy(new_attribs,"");
    snprintf(port,sizeof(port),"port%d",portno);
    port_attribs = nvram_safe_get(port);
    foreach(port_attrib, port_attribs, next)
    {
        if (strncmp(port_attrib, attrib, strlen(attrib)) == 0) {
            hasAttrib = 1;
        	/* yes, copy to new attrib string */
            strcpy(&new_attribs[buf_index],port_attrib);
            strcat(new_attribs," ");
            buf_index = strlen(new_attribs);
        }
        else {
        	/* this is not attribute searching for, just copy to new string */
            strcpy(&new_attribs[buf_index],port_attrib);
            strcat(new_attribs," ");
            buf_index = strlen(new_attribs);
        }
    }
    /* check to see if attrib not found, (if so, add) */
    if (!hasAttrib)
        strcpy(&new_attribs[buf_index], attrib);
    /* now set new attributes */
    nvram_set(port,new_attribs);
}

/* unset attribute in nvram variable for portn */
void
nvUnsetPortAttrib(char *attrib, uint portno)
{
	char *next, port_attrib[80], new_attribs[128];
	char port[] = "XXXXXXXXX";
	int hasAttrib, buf_index;

    hasAttrib = 0;
    buf_index = 0;
    strcpy(new_attribs,"");
    snprintf(port,sizeof(port),"port%d",portno);
    foreach(port_attrib, nvram_safe_get(port), next)
    {
        if (strncmp(port_attrib, attrib, strlen(attrib)))
        {
        	/* this is not attribute searching for, just copy to new string */
            strcpy(&new_attribs[buf_index],port_attrib);
            strcat(new_attribs," ");
            buf_index = strlen(new_attribs);
        }
    }
    /* now set new attributes */
    nvram_set(port,new_attribs);
}

/* unset all port force attributes in nvram variable for portn */
void
nvUnsetAllForcePortAttrib(uint portno)
{
	char force_attribs[] = "10h 10f 100h 100f down poweroff";
	char *next, port_attrib[80];

	foreach(port_attrib, force_attribs, next)
    {
		nvUnsetPortAttrib(port_attrib, portno);
	}
}


/* TRUE if any port force attribute set, FALSE otherwise */
int
nvExistsAnyForcePortAttrib(uint portno)
{
	char force_attribs[] = "10h 10f 100h 100f down poweroff";
	char *next, port_attrib[80];
	int hasAttrib;

	hasAttrib = 0;
	foreach(port_attrib, force_attribs, next)
    {
		if (nvExistsPortAttrib(port_attrib, portno))
			hasAttrib = 1;
	}
	return hasAttrib;
}

/* gets port attributes from portn nvram variable */
PORT_ATTRIBS
nvGetSwitchPortAttribs(uint portno)
{
	PORT_ATTRIBS retval;

    memset(&retval,0,sizeof(retval));
	if (nvExistsPortAttrib("native", portno))
		retval.native = 1;
	if (!nvExistsPortAttrib("noan", portno))
		retval.autoneg = 1;
	if (nvExistsPortAttrib("10h", portno))
		retval.force = FORCE_10H;
	if (nvExistsPortAttrib("10f", portno))
		retval.force = FORCE_10F;
	if (nvExistsPortAttrib("100h", portno))
		retval.force = FORCE_100H;
	if (nvExistsPortAttrib("100f", portno))
		retval.force = FORCE_100F;
	if (nvExistsPortAttrib("down", portno))
		retval.force = FORCE_DOWN;
	return retval;
}

	
