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
 * NVRAM variable manipulation (Linux user mode half)
 *
 * Copyright 2004, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: nvram_linux.c,v 1.1.1.1 2007/01/25 12:52:21 jiahao_jhou Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <typedefs.h>
#include <bcmnvram.h>

#define PATH_DEV_NVRAM "/dev/nvram"

/* Globals */
static int nvram_fd = -1;
static char *nvram_buf = NULL;
//static char *nvram_xfr_buf = NULL;

int
nvram_init(void *unused)
{
	if ((nvram_fd = open(PATH_DEV_NVRAM, O_RDWR)) < 0)
		goto err;

	/* Map kernel string buffer into user space */
	if ((nvram_buf = mmap(NULL, NVRAM_SPACE, PROT_READ, MAP_SHARED, nvram_fd, 0)) == MAP_FAILED) {
		close(nvram_fd);
		nvram_fd = -1;
		goto err;
	}

	return 0;

 err:
	perror(PATH_DEV_NVRAM);
	return errno;
}

char *
nvram_get(const char *name)
{
	size_t count = strlen(name) + 1;
	char tmp[100], *value;
	unsigned long *off = (unsigned long *) tmp;

	if (nvram_fd < 0)
		if (nvram_init(NULL))
			return NULL;

	if (count > sizeof(tmp)) {
		if (!(off = malloc(count)))
			return NULL;
	}

	/* Get offset into mmap() space */
	strcpy((char *) off, name);

	count = read(nvram_fd, off, count);

	if (count == sizeof(unsigned long))
		value = &nvram_buf[*off];
	else
		value = NULL;

	if (count < 0)
		perror(PATH_DEV_NVRAM);

	if (off != (unsigned long *) tmp)
		free(off);

	return value;
}

int
nvram_getall(char *buf, int count)
{
	int ret;

	if (nvram_fd < 0)
		if ((ret = nvram_init(NULL)))
			return ret;

	if (count == 0)
		return 0;

	/* Get all variables */
	*buf = '\0';

	ret = read(nvram_fd, buf, count);

	if (ret < 0)
		perror(PATH_DEV_NVRAM);

	return (ret == count) ? 0 : ret;
}

static int
_nvram_set(const char *name, const char *value)
{
	size_t count = strlen(name) + 1;
	char tmp[100], *buf = tmp;
	int ret;
	
	/* +++ Cherry Cho added for fixing bug of WEP Key in 2007/1/17. +++ */
	#if 1
	char tmp1[130];
	unsigned char *p1 = tmp1;
	int j=0, wsc_set = 0;
	memset(tmp1, 0x0, 130);
		
	if(nvram_get("wsc_set")!=NULL){
		if(!strcmp(nvram_get("wsc_set"),"1"))
			wsc_set = 1;
	}	

	if(wsc_set && !strncmp(name,"wl_key1",7))
	{	
		for(j=0; j<13; j++){
			sprintf(p1, "%02X", (unsigned char)value[j]);
			p1 = p1 + 2;
		}
		
		//printf("nvram_linux.c : wsc set wl_key1: %s\n",tmp1);
		value= tmp1;
		
	}
	
	/* Cherry Cho added for fixing bug hat when the length of SSID is 
	   equal to 32, the value of SSID will be wrong in 2007/2/1. */
	if(wsc_set && !strcmp(name,"wl_ssid"))
	{	
		if(strlen(value)>32)
		{
			strncpy(tmp1,value,32);
			value = tmp1;
		}
	}
	#endif
	/* --- Cherry Cho added for for fixing bug of WEP Key in 2007/1/17. --- */
	
	if (nvram_fd < 0)
		if ((ret = nvram_init(NULL)))
			return ret;

	/* Unset if value is NULL */
	if (value)
		count += strlen(value) + 1;

	if (count > sizeof(tmp)) {
		if (!(buf = malloc(count)))
			return -ENOMEM;
	}

	if (value)
		sprintf(buf, "%s=%s", name, value);
	else
		strcpy(buf, name);

	ret = write(nvram_fd, buf, count);

	if (ret < 0)
		perror(PATH_DEV_NVRAM);

	if (buf != tmp)
		free(buf);

	return (ret == count) ? 0 : ret;
}

int
nvram_set(const char *name, const char *value)
{
	return _nvram_set(name, value);
}

int
nvram_unset(const char *name)
{
	return _nvram_set(name, NULL);
}

int
nvram_commit(void)
{
	int ret;

	if (nvram_fd < 0)
		if ((ret = nvram_init(NULL)))
			return ret;

	ret = ioctl(nvram_fd, NVRAM_MAGIC, NULL);

	if (ret < 0)
		perror(PATH_DEV_NVRAM);

	return ret;
}

/*
int
nvram_xfr(char *buf)
{
	int ret;
	char tmpbuf[1024];

	strcpy(tmpbuf, buf);

	if (nvram_fd < 0)
		if ((ret = nvram_init(NULL)))
			return ret;

	ret = ioctl(nvram_fd, NVRAM_MAGIC, tmpbuf);

	if (ret < 0)
		perror(PATH_DEV_NVRAM);

	printf("output: %s\n", tmpbuf);

	return ret;	
}
*/

/*
char *			// added by Jiahao for WL500gP
nvram_xfr(char *buf)
{
	size_t count = strlen(buf)*1.5 + 1;
	int ret;
	char tmpbuf[1024];

	if (nvram_fd < 0)
		if ((ret = nvram_init(NULL)))
			return NULL;

	if (count > sizeof(tmpbuf))
		return NULL;
	
	strcpy(tmpbuf, buf);
	
	if (!nvram_xfr_buf)
		nvram_xfr_buf=(char *)malloc(1024+1);
	if (!nvram_xfr_buf)
		return NULL;

	ret = ioctl(nvram_fd, NVRAM_MAGIC, tmpbuf);

	if (ret < 0)
	{
		perror(PATH_DEV_NVRAM);
		return NULL;
	}
	else
	{
		strcpy(nvram_xfr_buf, tmpbuf);
		return nvram_xfr_buf;
	}
}
*/

