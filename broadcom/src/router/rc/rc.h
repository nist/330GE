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
 * Router rc control script
 *
 * Copyright 2004, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: rc.h,v 1.1.1.1 2007/01/25 12:52:21 jiahao_jhou Exp $
 */

#ifndef _rc_h_
#define _rc_h_

#include <bcmconfig.h>
#include <netinet/in.h>

#define IFUP (IFF_UP | IFF_RUNNING | IFF_BROADCAST | IFF_MULTICAST)

#define sin_addr(s) (((struct sockaddr_in *)(s))->sin_addr)

/* udhcpc scripts */
extern int udhcpc_main(int argc, char **argv);

/* ppp scripts */
extern int ipup_main(int argc, char **argv);
extern int ipdown_main(int argc, char **argv);
extern int ppp_ifunit(char *ifname);

/* http functions */
extern int http_get(const char *server, char *buf, size_t count, off_t offset);
extern int http_post(const char *server, char *buf, size_t count);
extern int http_stats(const char *url);

/* init */
extern int console_init(void);
extern pid_t run_shell(int timeout, int nowait);
extern void signal_init(void);
extern void fatal_signal(int sig);

/* interface */
extern int ifconfig(char *ifname, int flags, char *addr, char *netmask);
extern int route_add(char *name, int metric, char *dst, char *gateway, char *genmask);
extern int route_del(char *name, int metric, char *dst, char *gateway, char *genmask);
extern void config_loopback(void);
extern int config_vlan(void);

/* network */
extern void start_lan(void);
extern void stop_lan(void);
extern void start_wan(void);
extern void stop_wan(void);
extern void wan_up(char *ifname);
extern void wan_down(char *ifname);
extern int hotplug_net(void);
extern int wan_ifunit(char *ifname);
extern int wan_primary_ifunit(void);

/* services */
extern int start_dhcpd(void);
extern int stop_dhcpd(void);
extern int start_dns(void);
extern int stop_dns(void);
extern int start_ntpc(void);
extern int stop_ntpc(void);
extern int start_nas(char *type);
extern int stop_nas(void);
extern int start_services(void);
extern int stop_services(void);

/* firewall */
#ifdef __CONFIG_NETCONF__
extern int start_firewall(void);
extern int stop_firewall(void);
extern int start_firewall2(char *ifname);
extern int stop_firewall2(char *ifname);
#else
#define start_firewall() do {} while (0)
#define stop_firewall() do {} while (0)
#define start_firewall2(ifname) do {} while (0)
#define stop_firewall2(ifname) do {} while (0)
#endif

/* routes */
extern int preset_wan_routes(char *ifname);

#ifdef BTN_SETUP
enum BTNSETUP_STATE
{
	BTNSETUP_NONE=0,
	BTNSETUP_DETECT,
	BTNSETUP_START,
	BTNSETUP_DATAEXCHANGE,
	BTNSETUP_DATAEXCHANGE_FINISH,
	BTNSETUP_DATAEXCHANGE_EXTEND,
	BTNSETUP_FINISH
};
#endif
#endif /* _rc_h_ */
