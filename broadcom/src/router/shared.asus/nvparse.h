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
 * Routines for managing persistent storage of port mappings, etc.
 *
 * Copyright 2004, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: nvparse.h,v 1.1.1.1 2007/01/25 12:53:02 jiahao_jhou Exp $
 */

#ifndef _nvparse_h_
#define _nvparse_h_

/* 256 entries per list */
#define MAX_NVPARSE 256

/* 
 * Automatic (application specific) port forwards are described by a
 * netconf_app_t structure. A specific outbound connection triggers
 * the expectation of one or more inbound connections which may be
 * optionally remapped to a different port range.
 */
extern bool valid_autofw_port(const netconf_app_t *app);
extern bool get_autofw_port(int which, netconf_app_t *app);
extern bool set_autofw_port(int which, const netconf_app_t *app);
extern bool del_autofw_port(int which);


/* 
 * Persistent (static) port forwards are described by a netconf_nat_t
 * structure. On Linux, a netconf_filter_t that matches the target
 * parameters of the netconf_nat_t should also be added to the INPUT
 * and FORWARD tables to ACCEPT the forwarded connection.
 */
extern bool valid_forward_port(const netconf_nat_t *nat);
extern bool get_forward_port(int which, netconf_nat_t *nat);
extern bool set_forward_port(int which, const netconf_nat_t *nat);
extern bool del_forward_port(int which);

/* 
 * Client filters are described by two netconf_filter_t structures that
 * differ in match.src.ipaddr alone (to support IP address ranges)
 */
extern bool valid_filter_client(const netconf_filter_t *start, const netconf_filter_t *end);
extern bool get_filter_client(int which, netconf_filter_t *start, netconf_filter_t *end);
extern bool set_filter_client(int which, const netconf_filter_t *start, const netconf_filter_t *end);
extern bool del_filter_client(int which);

/*
* WPA/WDS per link configuration. Parameters after 'auth' are
* authentication algorithm dependant:
*
* When auth is "psk", the parameter list is:
*
* 	bool get_wds_wsec(int unit, int which, char *mac, char *role,
*		char *crypto, char *auth, char *ssid, char *passphrase);
*/
extern bool get_wds_wsec(int unit, int which, char *mac, char *role,
			 char *crypto, char *auth, ...);
extern bool set_wds_wsec(int unit, int which, char *mac, char *role,
			 char *crypto, char *auth, ...);
extern bool del_wds_wsec(int unit, int which);

/* Conversion routine for deprecated variables */
extern void convert_deprecated(void);

#endif /* _nvparse_h_ */
