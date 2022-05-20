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
 * Miscellaneous services
 *
 * Copyright 2004, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: services.c,v 1.1.1.1 2007/01/25 12:52:21 jiahao_jhou Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#include <bcmnvram.h>
#include <netconf.h>
#include <shutils.h>
#include <rc.h>

#ifndef ASUS_EXT
int
start_dhcpd(void)
{
	FILE *fp;
	char name[100];

	if (nvram_match("router_disable", "1") || nvram_invmatch("lan_proto", "dhcp"))
		return 0;

	dprintf("%s %s %s %s\n",
		nvram_safe_get("lan_ifname"),
		nvram_safe_get("dhcp_start"),
		nvram_safe_get("dhcp_end"),
		nvram_safe_get("lan_lease"));

	/* Touch leases file */
	if (!(fp = fopen("/tmp/udhcpd.leases", "a"))) {
		perror("/tmp/udhcpd.leases");
		return errno;
	}
	fclose(fp);

	/* Write configuration file based on current information */
	if (!(fp = fopen("/tmp/udhcpd.conf", "w"))) {
		perror("/tmp/udhcpd.conf");
		return errno;
	}
	fprintf(fp, "pidfile /var/run/udhcpd.pid\n");
	fprintf(fp, "start %s\n", nvram_safe_get("dhcp_start"));
	fprintf(fp, "end %s\n", nvram_safe_get("dhcp_end"));
	fprintf(fp, "interface %s\n", nvram_safe_get("lan_ifname"));
	fprintf(fp, "remaining yes\n");
	fprintf(fp, "lease_file /tmp/udhcpd.leases\n");
	fprintf(fp, "option subnet %s\n", nvram_safe_get("lan_netmask"));
	fprintf(fp, "option router %s\n", nvram_safe_get("lan_ipaddr"));
	fprintf(fp, "option dns %s\n", nvram_safe_get("lan_ipaddr"));
	fprintf(fp, "option lease %s\n", nvram_safe_get("lan_lease"));
	snprintf(name, sizeof(name), "%s_wins", nvram_safe_get("dhcp_wins"));
	if (nvram_invmatch(name, ""))
		fprintf(fp, "option wins %s\n", nvram_get(name));
	snprintf(name, sizeof(name), "%s_domain", nvram_safe_get("dhcp_domain"));
	if (nvram_invmatch(name, ""))
		fprintf(fp, "option domain %s\n", nvram_get(name));
	fclose(fp);

	eval("udhcpd", "-i", "br0", "/tmp/udhcpd.conf");

	dprintf("done\n");
	return 0;
}

int
stop_dhcpd(void)
{
	char sigusr1[] = "-XX";
	int ret;

/*
* Process udhcpd handles two signals - SIGTERM and SIGUSR1
*
*  - SIGUSR1 saves all leases in /tmp/udhcpd.leases
*  - SIGTERM causes the process to be killed
*
* The SIGUSR1+SIGTERM behavior is what we like so that all current client
* leases will be honorred when the dhcpd restarts and all clients can extend
* their leases and continue their current IP addresses. Otherwise clients
* would get NAK'd when they try to extend/rebind their leases and they 
* would have to release current IP and to request a new one which causes 
* a no-IP gap in between.
*/
	sprintf(sigusr1, "-%d", SIGUSR1);
	eval("killall", sigusr1, "udhcpd");
	ret = eval("killall", "udhcpd");

	dprintf("done\n");
	return ret;
}

int
start_dns(void)
{
	int ret;
	FILE *fp;
	
	if (nvram_match("router_disable", "1"))
		return 0;

	/* Create resolv.conf with empty nameserver list */
	if (!(fp = fopen("/tmp/resolv.conf", "w"))) {
		perror("/tmp/resolv.conf");
		return errno;
	}
	fclose(fp);

	/* launch dns relay */
	ret = eval("dnsmasq", "-h",
		   "-i", nvram_safe_get("lan_ifname"),
		   "-r", "/tmp/resolv.conf");

	dprintf("done\n");
	return ret;
}	

int
stop_dns(void)
{
	int ret = eval("killall", "dnsmasq");
	
	/* Remove resolv.conf */
	unlink("/tmp/resolv.conf");

	dprintf("done\n");
	return ret;
}	
#endif

int
start_httpd(void)
{
	int ret;

	chdir("/www");
#ifdef ASUS_EXT
	if (nvram_invmatch("router_disable", "1"))
		ret = eval("httpd", nvram_safe_get("wan0_ifname"));
	else
#endif
	ret = eval("httpd");

	chdir("/");

	dprintf("done\n");
	return ret;
}

int
stop_httpd(void)
{
	int ret = eval("killall", "httpd");

	dprintf("done\n");
	return ret;
}

int
start_upnp(void)
{
	char *wan_ifname;
	int ret;
	char var[100], prefix[] = "wanXXXXXXXXXX_";

	if (!nvram_invmatch("upnp_ENABLE", "0") || nvram_match("router_disable", "1"))
		return 0;
	
	ret = eval("killall", "-SIGUSR1", "upnp");
	if (ret != 0) {
	    snprintf(prefix, sizeof(prefix), "wan%d_", wan_primary_ifunit());
	    wan_ifname = nvram_match(strcat_r(prefix, "proto", var), "pppoe") ? 
					nvram_safe_get(strcat_r(prefix, "pppoe_ifname", var)) :
					nvram_safe_get(strcat_r(prefix, "ifname", var));
	    ret = eval("upnp", "-D",
		       "-L", nvram_safe_get("lan_ifname"),
		       "-W", wan_ifname);

	}
	dprintf("done\n");
	return ret;
}

int
stop_upnp(void)
{
	int ret = 0;

	if (nvram_invmatch("upnp_ENABLE", "0"))
	    ret = eval("killall", "upnp");

	dprintf("done\n");
	return ret;
}

int
start_nas(char *type)
{
	char cfgfile[64];
	char pidfile[64];
	if (!type || !*type)
		type = "lan";
	snprintf(cfgfile, sizeof(cfgfile), "/tmp/nas.%s.conf", type);
	snprintf(pidfile, sizeof(pidfile), "/tmp/nas.%s.pid", type);
	{
		char *argv[] = {"nas", cfgfile, pidfile, type, NULL};
		pid_t pid;

		_eval(argv, NULL, 0, &pid);
		dprintf("done\n");
	}
	return 0;
}

int
stop_nas(void)
{
	int ret = eval("killall", "nas");

	dprintf("done\n");
	return ret;
}

int
start_ntpc(void)
{
#ifdef ASUS_EXT
	char *ntp_argv[] = {"ntp", NULL};
	pid_t pid;

	_eval(ntp_argv, NULL, 0, &pid);
#else
	char *servers = nvram_safe_get("ntp_server");
	
	if (strlen(servers)) {
		char *nas_argv[] = {"ntpclient", "-h", servers, "-i", "3", "-l", "-s", NULL};
		_eval(nas_argv, NULL, 0, NULL);
	}
	
	dprintf("done\n");
#endif
	return 0;
}

int
stop_ntpc(void)
{
#ifdef ASUS_EXT
	eval("killall", "ntp");
	int ret = eval("killall", "ntpclient");
#else
	int ret = eval("killall", "ntpclient");
#endif

	dprintf("done\n");
	return ret;
}

/* +++ Cherry Cho added in 2006/12/14. +++ */
/*
int start_lltd(void)
{
	chdir("/usr/sbin");
	eval("lld2d", "br0");
	chdir("/");
	
	return 0;
}
*/

/* Cherry Cho added in 2006/12/14. */
/*
int stop_lltd(void)
{
	int ret;
	
	ret = eval("killall", "lld2d");

	return ret;
}
*/

int
start_services(void)
{
	start_httpd();
	start_dns();
	start_dhcpd();
#ifdef ASUS_EXT
	start_logger();
#endif
	start_upnp();

#ifdef WL330GE
	if (nvram_match("wl_mode_EX", "re") && nvram_match("wan_nat_X", "1"))		// for nas to run under Travel Router mode
		nvram_set("lan_ifnames", "wl0.1");
	else if (nvram_match("wl_mode_EX", "re") && nvram_match("wan_nat_X", "0"))	// for nas to run under Range Extender mode
		nvram_set("lan_ifnames", "eth1 wl0.1");
	else if (nvram_match("wl_mode_EX", "ap") && nvram_match("mssid_ENABLE", "1") && !(nvram_match("vlans_ENABLE", "1")) && nvram_match("wan_nat_X", "0"))	// for nas to nun under Multi-SSID mode
		nvram_set("lan_ifnames", "eth1 wl0.1 wl0.2 wl0.3");
	else if (nvram_match("wl_mode_EX", "ap") && nvram_match("wan_nat_X", "1"))	// for nas to nun under Home Gateway mode
		nvram_set("lan_ifnames", "eth1");
	else	// for nas to nun under AP or STA mode
		nvram_set("lan_ifnames", "eth0 eth1");

	if (!(nvram_match("wl_mode_EX", "ap") &&
	      nvram_match("wan_nat_X", "0") &&
	      nvram_match("mssid_ENABLE", "1") &&
	      nvram_match("vlans_ENABLE", "1")
	     )
	   )
#endif
	start_nas("lan");

#ifdef ASUS_EXT
	start_usb();
#endif

#ifdef GUEST_ACCOUNT
#ifdef RANGE_EXTENDER
	if (nvram_match("wl_mode_EX", "re"))
	{
	}
	else
#endif
	if (nvram_match("wl_guest_ENABLE", "1") && nvram_match("wl_mode_EX", "ap") && nvram_match("wan_nat_X", "1"))
	{
		sleep(5);
		start_dhcpd_guest();
		start_guest_nas(); 
	}
#endif

//	start_lltd(); /* Cherry Cho added in 2006/12/14. */
	dprintf("done\n");
	return 0;
}

int
stop_services(void)
{
#ifdef ASUS_EXT
	stop_usb();
#endif
	stop_nas();
	stop_upnp();
#ifdef ASUS_EXT
	stop_logger();
#endif

#ifdef GUEST_ACCOUNT
#ifdef RANGE_EXTENDER
	if(nvram_match("wl_mode_EX", "re"))
	{
		//do nothing
	}
	else
#endif
	stop_dhcpd_guest();
#endif
	stop_dhcpd();
	stop_dns();
	stop_httpd();

//	stop_lltd();/* Cherry Cho added in 2006/12/14. */	
	dprintf("done\n");
	return 0;
}

#ifdef GUEST_ACCOUNT
/* Start NAS for the guest interfaces */
int
start_guest_nas(void)
{
	char *unbridged_interfaces;
	char *next;
	char name[IFNAMSIZ],lan[IFNAMSIZ];
	int index;
		
	unbridged_interfaces = nvram_get("unbridged_ifnames");
	
	if (unbridged_interfaces)
		foreach(name,unbridged_interfaces,next){
			index = get_ipconfig_index(name);
			if (index < 0) 
				continue;
			snprintf(lan,sizeof(lan),"lan%d",index);
			start_nas(lan);
		}

	return 0;
}
#endif
