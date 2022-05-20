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
    Copyright 2001, ASUSTek Inc.
    All Rights Reserved.
    
    This is UNPUBLISHED PROPRIETARY SOURCE CODE of ASUSTek Inc.;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of ASUSTek Inc..
*/
/*
 * This module creates an array of name, value pairs
 * and supports updating the nvram space. 
 *
 * This module requires the following support routines
 *
 *      malloc, free, strcmp, strncmp, strcpy, strtol, strchr, printf and sprintf
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_STRING 12
#define MIN_BUF_SIZE 4096
#define IMAGE_HEADER "HDR0"
#define PROFILE_HEADER "HDR1"

typedef struct {
	char productid[MAX_STRING];
	unsigned long piclen;
	unsigned long settinglen;
} PROFILE;

int
main(int argc, char **argv)
{
	FILE *fp, *pfp, *sfp;
	PROFILE profile;
	char name[MAX_STRING+1];
	unsigned long plen, flen, slen;
	int i;
	char *buf=NULL;
	int size=BUFSIZ;
	int count=0;
	int remaind=0;
	char header[8];
	unsigned long *imagelen;

	if (argc<=4)
	{
		printf(" addver [fs file] [pic file] [setting file] [product id]\n");
		return 0;
	}

	fp = fopen(argv[1], "rb+");
	if (fp==NULL)
	{
		printf("Opening File System file fails!\n");
		return 0;
	}
	else
	{
   		count = fread(header, 1, 8, fp);
   		if (count>=8 && strncmp(header, IMAGE_HEADER, 4)==0)
   		{
   			imagelen = (unsigned long *)(header+4);
			fseek(fp, 0, SEEK_END);
			flen = ftell(fp);

			if (flen>(*imagelen+20))
			{
				fclose(fp);
				printf("Profile Information has been appended already!!\n");
				return 0;
			}

			fseek(fp, flen-20, SEEK_SET);
		}
		else
		{
			fclose(fp);
			printf("invalid File System format!!\n");
			return 0;
		}
	}

	plen=0;
	pfp = fopen(argv[2], "r");
	if (pfp==NULL)
	{
		fclose(fp);
		printf("Opening Logo file fail!\n");
		return 0;
	}
	else
	{
		fseek(pfp, 0, SEEK_END);
		plen = (unsigned long) ftell(pfp);
	}

	slen=0;
	sfp = fopen(argv[3], "r");
	if (sfp==NULL)
	{
		fclose(fp);
		fclose(pfp);
		printf("Opening Setting file fail!\n");
		return 0;
	}
	else
	{
   		count = fread(header, 1, 8, sfp);
   		if (count>=8 && strncmp(header, PROFILE_HEADER, 4)==0)
		{
			fseek(sfp, 0, SEEK_END);
			slen = (unsigned long) ftell(sfp);
		}
		else
		{
			fclose(sfp);
			printf("invalid Setting file format!!\n");
			return 0;
		}
	}

	printf("File system size: %d\n", flen);
	printf("Logo file size : %d\n", plen);
	printf("Setting file size : %d\n\n", slen);

	memset(&profile, 0, sizeof(profile));
	strncpy(profile.productid, "            ", MAX_STRING);
	strncpy(profile.productid, argv[4], (MAX_STRING>strlen(argv[4]))?strlen(argv[4]):MAX_STRING);
	strncpy(name, profile.productid, MAX_STRING);
	name[MAX_STRING] = 0;
	profile.piclen = plen;
	profile.settinglen = slen;

	printf("Append Profile Information to file ...\n");
	printf("   File System         : %s\n", argv[1]);
	printf("   Logo                : %s\n", argv[2]);
	printf("   Setting File        : %s\n", argv[3]);
	printf("   Product ID          : %s\n",    name);

	fwrite(&profile, 1, sizeof(PROFILE), fp);

	if (size < MIN_BUF_SIZE)
		size=MIN_BUF_SIZE;
	if ((buf=malloc(size))==NULL)
	{
		fclose(fp);
		fclose(pfp);
		return ENOMEM;
	}

	fseek(pfp, 0, SEEK_SET);
	while (plen) {
		count = fread(buf, 1, size, pfp);
		if (!count && (ferror(pfp) || feof(pfp)))
			break;
		plen -= count;
		fwrite(buf, 1, count, fp);
	}

	fseek(sfp, 0, SEEK_SET);
	while (slen) {
		count = fread(buf, 1, size, sfp);
		if (!count && (ferror(sfp) || feof(sfp)))
			break;
		slen -= count;
		fwrite(buf, 1, count, fp);
	}

	fclose(fp);
	fclose(pfp);
	fclose(sfp);
	return 0;
}
