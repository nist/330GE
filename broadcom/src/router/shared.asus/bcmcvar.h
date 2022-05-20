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
 * Broadcom Web Page Configuration Variables
 *
 * Copyright 2004, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 * $Id: bcmcvar.h,v 1.1.1.1 2007/01/25 12:53:02 jiahao_jhou Exp $
 */

#ifndef _bcmcvar_h
#define _bcmcvar_h

struct variable {
	char *name;
	char *longname;
	void (*validate)(webs_t wp, char *value, struct variable *v);
	char **argv;
	int nullok;
	int ezc_flags;
};

#define POST_BUF_SIZE	10000
#define WEBS_BUF_SIZE	5000
#define MAX_STA_COUNT	256
#define NVRAM_BUFSIZE	100

#define websBufferInit(wp) {webs_buf = malloc(WEBS_BUF_SIZE); webs_buf_offset = 0;}
#define websBufferWrite(wp, fmt, args...) {webs_buf_offset += sprintf(webs_buf+webs_buf_offset, fmt, ## args);} 
#define websBufferFlush(wp) {webs_buf[webs_buf_offset] = '\0'; fprintf(wp, webs_buf); fflush(wp); free(webs_buf); webs_buf = NULL;}

#define ARGV(args...) ((char *[]) { args, NULL })
#define XSTR(s) STR(s)
#define STR(s) #s

int variables_arraysize(void);

#endif /* _bcmcvar_h */
