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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <bcmnvram.h>
#include <shutils.h>
#include <wlutils.h>
#include <syslog.h>
#include "iboxcom.h"

#define MAXARRAY 64
#define MAXARRSIZE  64

#ifdef NOPARSER
int
parse_array(char *file, char *func, char *strarr)
{
	FILE *fp;
	char line[1024], *s, *l;
	int flag=0, len;
	int i;

	if(!(fp=fopen(file, "r+"))) return 0;
	
	while(fgets(line, sizeof(line), fp))
	{
		if(strstr(line, func)) flag=1;
		if(flag && strstr(line, "return"))
		{
			flag=2;
			break;
		}	
	}
	fclose(fp);

	if (flag!=2) return 0;

	l = strchr(line, ']');
	if(l) *l=0;
	l = strchr(line, '[');
	l++;
	s=l;
	len = strlen(s);
	while (strsep(&s, ","));

	i=0;
	/* Set token values */
	for (s = l; s < &l[len] && *s; s+=strlen(s)+1) 
	{
		strcpy(&strarr[i*MAXARRSIZE], s);
		i++;

		if(i==MAXARRAY) break;
	}
	return i;	
}

int isClaimedIDE(char *model)
{
	char tmparray[MAXARRAY][MAXARRSIZE];
	int tmpidx;
	unsigned long size=0;
	int i;

	tmpidx=parse_array("/tmp/diskstatus", "claimed_disk_interface_names", tmparray);

	for(i=0;i<tmpidx;i++)
	{
		printf("%d:%s\n", i, &tmparray[i][0]);
		if (strstr(&tmparray[i][0], "IDE")) break;
	}

	if (i==tmpidx) return 0;

	tmpidx=parse_array("/tmp/diskstatus", "claimed_disk_total_size", tmparray);
	if (i>=tmpidx) return 0;
	size = atoi(&tmparray[i][0]);
	
	tmpidx=parse_array("/tmp/diskstatus", "claimed_disk_model_info", tmparray);
	if (i>=tmpidx) return 0;
	strcpy(model, &tmparray[i][0]);
	return size;
}

int isBlankIDE(char *model)
{
	char tmparray[MAXARRAY][MAXARRSIZE];
	int tmpidx;
	unsigned long size=0;
	int i;

	tmpidx=parse_array("/tmp/diskstatus", "blank_disk_interface_names", tmparray);

	for(i=0;i<tmpidx;i++)
	{
		printf("%d:%s\n", i, &tmparray[i][0]);
		if (strstr(&tmparray[i][0], "IDE")) break;
	}

	if (i==tmpidx) return 0;

	tmpidx=parse_array("/tmp/diskstatus", "blank_disk_total_size", tmparray);
	if (i>=tmpidx) return 0;
	size = atoi(&tmparray[i][0]);
	
	tmpidx=parse_array("/tmp/diskstatus", "blank_disk_model_info", tmparray);
	if (i>=tmpidx) return 0;
	strcpy(model, &tmparray[i][0]);
	return size;
}


int isPrinter(char *model1, char *model2)
{
	char tmparray[MAXARRAY][MAXARRSIZE];
	int tmpidx;
	unsigned long size=0;
	int i;

	tmpidx=parse_array("/tmp/diskstatus", "printer_models", tmparray);
	if (tmpidx<=0) return 0;

	if (tmpidx>=1)
		strcpy(model1, &tmparray[0][0]);
	if (tmpidx>=2)
		strcpy(model2, &tmparray[1][0]);
	
	return tmpidx;
}
#endif

int
getStorageStatus(STORAGE_INFO_T *st)
{
	char tmparray[MAXARRAY][MAXARRSIZE];

	st->Capability=0;

	if(nvram_invmatch("apps_pool", "") && nvram_invmatch("apps_share", ""))
	{
		st->Capability = atoi(nvram_safe_get("apps_caps"));
		strcpy(st->AppsPool, nvram_safe_get("apps_pool"));
		strcpy(st->AppsShare, nvram_safe_get("apps_share"));
	}
	else
	{
		st->Capability=0;
		st->AppsPool[0]=0;
		st->AppsShare[0]=0;
	}

	st->DiskSize=0;
	st->DiskStatus=DISK_STATUS_NONE;
	st->DiskModel[0]=0;
	st->PrinterModel1[0]=0;
	st->PrinterModel2[0]=0;

#ifdef NOPARSER
	// get from page
	eval("wget", "http://127.0.0.1/diskstatus.asp", "-O", "/tmp/diskstatus");
	// find claimed ide
	if ((st->DiskSize=isClaimedIDE(st->DiskModel)))
	{
		st->DiskStatus=DISK_STATUS_CLAIMED;
	}
	else if ((st->DiskSize=isBlankIDE(st->DiskModel)))
	{	
		st->DiskStatus=DISK_STATUS_BLANK;
	}
	isPrinter(st->PrinterModel1, st->PrinterModel2);
#endif

	//printf("Storage: %d %s\n", st->DiskStatus, st->AppsShare);
	return 0;
}

