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

#define MAX_STRING 12

typedef struct {
	char productid[MAX_STRING];
	unsigned long piclen;
	unsigned long settinglen;
} PROFILE;

int
main(int argc, char **argv)
{
	FILE *fp;
	PROFILE profile;
	unsigned long flen;

	if (argc!=2)
	{
		printf(" addtail [fs file]\n");
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
		fseek(fp, 0, SEEK_END);
		flen = ftell(fp);
		printf("File system size: %d\n", flen);
	}

	printf("Append null Profile Information to file ...\n");

	memset(&profile, 0, sizeof(profile));
	fwrite(&profile, 1, sizeof(PROFILE), fp);
	fclose(fp);

	return 0;
}
