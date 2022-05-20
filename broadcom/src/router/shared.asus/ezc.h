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
 *
 * Copyright 2004, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 * $Id: ezc.h,v 1.1.1.1 2007/01/25 12:53:02 jiahao_jhou Exp $
 */

#ifndef _ezc_h_
#define _ezc_h_

#define EZC_VERSION_STR		"2"

#define EZC_FLAGS_READ		0x0001
#define EZC_FLAGS_WRITE		0x0002

#define EZC_SUCCESS	 	0
#define EZC_ERR_NOT_ENABLED 	1
#define EZC_ERR_INVALID_STATE 	2
#define EZC_ERR_INVALID_DATA 	3

/* ezc function callback with two args: name and value */
typedef void (*ezc_cb_fn_t)(char *, char *);

int ezc_register_cb(ezc_cb_fn_t fn);

void do_apply_ezconfig_post(char *url, FILE *stream, int len, char *boundary);
void do_ezconfig_asp(char *url, FILE *stream);

#endif /* _ezc_h_ */
