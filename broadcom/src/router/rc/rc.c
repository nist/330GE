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
 * Copyright 2004, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND ASUS GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: rc.c,v 1.1.1.1 2007/01/25 12:52:21 jiahao_jhou Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <signal.h>
#include <string.h>
#include <sys/klog.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <sys/reboot.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <net/if_arp.h>
#include <dirent.h>

#include <epivers.h>
#include <bcmnvram.h>
#include <mtd.h>
#include <shutils.h>
#include <rc.h>
#include <netconf.h>
#include <nvparse.h>
#include <bcmdevs.h>
#include <wlutils.h>
#include <bcmparams.h>
#include <etioctl.h>
#include <linux/sockios.h>
//#include <linux/ethtool.h>
#include "ethtool-util.h"

#ifdef WSC /* Cherry Cho added in 2007/2/7. */
extern int start_wsc(void);
#endif

static void restore_defaults(void);
static void sysinit(void);
static void rc_signal(int sig);

extern struct nvram_tuple router_defaults[];

static int restore_defaults_g=0;

#define PROFILE_HEADER 	"HDR1"

static int
build_ifnames(char *type, char *names, int *size)
{
	char name[32], *next;
	int len = 0;
	int s;

	/* open a raw scoket for ioctl */
	if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
       		return -1;

	/*
	 * go thru all device names (wl<N> il<N> et<N> vlan<N>) and interfaces to 
	 * build an interface name list in which each i/f name coresponds to a device
	 * name in device name list. Interface/device name matching rule is device
	 * type dependant:
	 *
	 *	wl:	by unit # provided by the driver, for example, if eth1 is wireless
	 *		i/f and its unit # is 0, then it will be in the i/f name list if
	 *		wl0 is in the device name list.
	 *	il/et:	by mac address, for example, if et0's mac address is identical to
	 *		that of eth2's, then eth2 will be in the i/f name list if et0 is 
	 *		in the device name list.
	 *	vlan:	by name, for example, vlan0 will be in the i/f name list if vlan0
	 *		is in the device name list.
	 */
	foreach (name, type, next) {
		struct ifreq ifr;
		int i, unit;
		char var[32], *mac, ea[ETHER_ADDR_LEN];

		//printf("ifname: %s \n", name);

		/* vlan: add it to interface name list */
		if (!strncmp(name, "vlan", 4)) {
			/* append interface name to list */
			len += snprintf(&names[len], *size - len, "%s ", name);
			continue;
		}

		/* others: proceed only when rules are met */
		for (i = 1; i <= DEV_NUMIFS; i ++) {
			/* ignore i/f that is not ethernet */
			ifr.ifr_ifindex = i;
			if (ioctl(s, SIOCGIFNAME, &ifr))
				continue;
			if (ioctl(s, SIOCGIFHWADDR, &ifr))
				continue;
			if (ifr.ifr_hwaddr.sa_family != ARPHRD_ETHER)
				continue;
			if (!strncmp(ifr.ifr_name, "vlan", 4))
				continue;
			/* wl: use unit # to identify wl */
			if (!strncmp(name, "wl", 2)) {
				if (wl_probe(ifr.ifr_name) ||
				    wl_ioctl(ifr.ifr_name, WLC_GET_INSTANCE, &unit, sizeof(unit)) ||
				    unit != atoi(&name[2]))					
					continue;
			}
			/* et/il: use mac addr to identify et/il */
			else if (!strncmp(name, "et", 2) || !strncmp(name, "il", 2)) {
				snprintf(var, sizeof(var), "%smacaddr", name);
				if (!(mac = nvram_get(var)) || !ether_atoe(mac, ea) ||
				    bcmp(ea, ifr.ifr_hwaddr.sa_data, ETHER_ADDR_LEN))
					continue;

				// add by Chen-I to filter out wl interface here
				if (!wl_probe(ifr.ifr_name))
					continue;

			}
			/* mac address: compare value */
			else if (ether_atoe(name, ea) && !bcmp(ea, ifr.ifr_hwaddr.sa_data, ETHER_ADDR_LEN))
				;
			/* others: ignore */
			else
				continue;

			/* append interface name to list */
			len += snprintf(&names[len], *size - len, "%s ", ifr.ifr_name);
		}
	}
	
	close(s);

	*size = len;
	return 0;
}

#ifdef GUEST_ACCOUNT
static void
virtual_radio_restore_defaults(void)
{
	char tmp[100], prefix[] = "wlXXXXXXXXXX_mssid_";
	int i,j;
	
	nvram_unset("unbridged_ifnames");
	nvram_unset("ure_disable");
	
	/* Delete dynamically generated variables */
	for (i = 0; i < MAX_NVPARSE; i++) {
		sprintf(prefix, "wl%d_", i);
		nvram_unset(strcat_r(prefix, "vifs", tmp));
		nvram_unset(strcat_r(prefix, "ssid", tmp));
		nvram_unset(strcat_r(prefix, "guest", tmp));
		nvram_unset(strcat_r(prefix, "ure", tmp));
		nvram_unset(strcat_r(prefix, "ipconfig_index", tmp));
		sprintf(prefix, "lan%d_", i);
		nvram_unset(strcat_r(prefix, "ifname", tmp));
		nvram_unset(strcat_r(prefix, "ifnames", tmp));
		nvram_unset(strcat_r(prefix, "gateway", tmp));
		nvram_unset(strcat_r(prefix, "proto", tmp));
		nvram_unset(strcat_r(prefix, "ipaddr", tmp));
		nvram_unset(strcat_r(prefix, "netmask", tmp));
		nvram_unset(strcat_r(prefix, "lease", tmp));
		sprintf(prefix, "dhcp%d_", i);
		nvram_unset(strcat_r(prefix, "start", tmp));
		nvram_unset(strcat_r(prefix, "end", tmp));
		
		/* clear virtual versions */
		for (j=0; j< 16;j++){
			sprintf(prefix, "wl%d.%d_", i,j);
			nvram_unset(strcat_r(prefix, "ssid", tmp));
			nvram_unset(strcat_r(prefix, "ipconfig_index", tmp));
			nvram_unset(strcat_r(prefix, "guest", tmp));		
			nvram_unset(strcat_r(prefix, "closed", tmp));
			nvram_unset(strcat_r(prefix, "wpa_psk", tmp));
			nvram_unset(strcat_r(prefix, "auth", tmp));
			nvram_unset(strcat_r(prefix, "wep", tmp));
			nvram_unset(strcat_r(prefix, "auth_mode", tmp));
			nvram_unset(strcat_r(prefix, "crypto", tmp));
			nvram_unset(strcat_r(prefix, "akm", tmp));
		}
	}
}

#endif

int issyspara(char *p)
{
	struct nvram_tuple *t, *u;

	for (t = router_defaults; t->name; t++)
	{
		if (strstr(p, t->name))
			break;
	}

	if (t->name) return 1;
	else return 0;
}

void nvram_restore_odm()
{
   	FILE *fp;
   	char header[8], *p, *v;
  	unsigned long count, *filelen;
  	char buf[NVRAM_SPACE];

   	if ((fp=fopen("/tmp/odm_setting", "r+"))==NULL) return -1;

   	count = fread(header, 1, 8, fp);
   	if (count>=8 && strncmp(header, PROFILE_HEADER, 4)==0)
   	{
		filelen = (unsigned long *)(header + 4);
		count = fread(buf, 1, *filelen, fp);
   	}
   	fclose(fp);

   	p = buf;       	           
   
   	while(*p)
   	{       
       		v = strchr(p, '=');

		if(v!=NULL)
		{
			*v++ = NULL;

			if (issyspara(p))
			{
				nvram_set(p, v);
			}

       			p = v + strlen(v) + 1;
		}
		else
		{
			nvram_unset(p);
			p = p + 1;
		}
   	}
}

static void
restore_defaults(void)
{
	struct nvram_tuple generic[] = {
		{ "lan_ifname", "br0", 0 },
		{ "lan_ifnames", "eth0 eth2 eth3 eth4", 0 },
		{ "wan_ifname", "eth1", 0 },
		{ "wan_ifnames", "eth1", 0 },
		{ 0, 0, 0 }
	};
	struct nvram_tuple vlan[] = {
		{ "lan_ifname", "br0", 0 },
		{ "lan_ifnames", "vlan0 eth1 eth2 eth3", 0 },
		{ "wan_ifname", "vlan1", 0 },
		{ "wan_ifnames", "vlan1", 0 },
		{ 0, 0, 0 }
	};
	struct nvram_tuple dyna[] = {
		{ "lan_ifname", "br0", 0 },
		{ "lan_ifnames", "", 0 },
		{ "wan_ifname", "", 0 },
		{ "wan_ifnames", "", 0 },
		{ 0, 0, 0 }
	};


#ifdef CONFIG_WL300G
	struct nvram_tuple wl300g[] = {
		{ "lan_ifname", "br0", 0 },
		{ "lan_ifnames", "eth2", 0 },
		{ "wan_ifname", "eth0", 0 },
		{ "wan_ifnames", "eth0", 0 },
		{ "wan_nat_x", "0", 0},
		{ "wan_route_x", "IP_Bridged", 0},
		{ 0, 0, 0 }
	};
#endif

#ifdef CONFIG_WL300G2
	struct nvram_tuple wl300g2[] = {
		{ "lan_ifname", "br0", 0 },
		{ "lan_ifnames", "eth0 eth2", 0 },
		{ "wan_ifname", "eth1", 0 },
		{ "wan_ifnames", "eth1", 0 },
		{ "wan_nat_x", "1", 0},
		{ "wan_route_x", "IP_Routed", 0},
		{ 0, 0, 0 }
	};
#endif

#ifdef CONFIG_WL331G
	struct nvram_tuple wl331g[] = {
		{ "lan_ifname", "br0", 0 },
		{ "lan_ifnames", "eth1", 0 },
		{ "wan_ifname", "eth0", 0 },
		{ "wan_ifnames", "eth0", 0 },
		{ "wan_nat_x", "1", 0},
		{ "wan_route_x", "IP_Routed", 0},
		{ 0, 0, 0 }
	};
#endif


#ifdef CONFIG_SENTRY5
#include "rcs5.h"
#else
#define RC1_START() 
#define RC1_STOP()  
#define RC7_START()
#define RC7_STOP()
#define LINUX_OVERRIDES() 
#define EXTRA_RESTORE_DEFAULTS() 
#endif

	struct nvram_tuple *linux_overrides;
	struct nvram_tuple *t, *u;
	int restore_defaults, i;
	uint boardflags;
	char *landevs, *wandevs;
	char lan_ifnames[128], wan_ifnames[128];
	char wan_ifname[32], *next;
	int len;
	int ap = 0;

	/* Restore defaults if told to or OS has changed */
	restore_defaults = !nvram_match("restore_defaults", "0") || nvram_invmatch("os_name", "linux");

	if (restore_defaults)
		cprintf("Restoring defaults...");

	/* Delete dynamically generated variables */
	if (restore_defaults) {
		char tmp[100], prefix[] = "wlXXXXXXXXXX_";
		for (i = 0; i < MAX_NVPARSE; i++) {
			del_filter_client(i);
			del_forward_port(i);
			del_autofw_port(i);
			snprintf(prefix, sizeof(prefix), "wl%d_", i);
			for (t = router_defaults; t->name; t ++) {
				if (!strncmp(t->name, "wl_", 3))
					nvram_unset(strcat_r(prefix, &t->name[3], tmp));
			}
			snprintf(prefix, sizeof(prefix), "wan%d_", i);
			for (t = router_defaults; t->name; t ++) {
				if (!strncmp(t->name, "wan_", 4))
					nvram_unset(strcat_r(prefix, &t->name[4], tmp));
			}
		}
#ifdef GUEST_ACCOUNT
		virtual_radio_restore_defaults();				
#endif
	}

	/* 
	 * Build bridged i/f name list and wan i/f name list from lan device name list
	 * and wan device name list. Both lan device list "landevs" and wan device list
	 * "wandevs" must exist in order to preceed.
	 */
	if ((landevs = nvram_get("landevs")) && (wandevs = nvram_get("wandevs"))) {
		/* build bridged i/f list based on nvram variable "landevs" */
		len = sizeof(lan_ifnames);
		if (!build_ifnames(landevs, lan_ifnames, &len) && len)
			dyna[1].value = lan_ifnames;
		else
			goto canned_config;
		/* build wan i/f list based on nvram variable "wandevs" */
		len = sizeof(wan_ifnames);
		if (!build_ifnames(wandevs, wan_ifnames, &len) && len) {
			dyna[3].value = wan_ifnames;
			foreach (wan_ifname, wan_ifnames, next) {
				dyna[2].value = wan_ifname;
				break;
			}
		}
		else ap = 1;
		linux_overrides = dyna;
	}
	/* override lan i/f name list and wan i/f name list with default values */
	else {
canned_config:
		boardflags = strtoul(nvram_safe_get("boardflags"), NULL, 0);
		if (boardflags & BFL_ENETVLAN)
			linux_overrides = vlan;
		else
			linux_overrides = generic;

		/* override the above linux_overrides with a different table */
		LINUX_OVERRIDES();
	}

#ifdef CONFIG_WL300G
	linux_overrides = wl300g;
#endif

#ifdef CONFIG_WL300G2
	linux_overrides = wl300g2;
#endif

#ifdef CONFIG_WL331G
	linux_overrides = wl331g;
#endif

#ifdef WL500GX
	// Thanks for Oleg
	linux_overrides = vlan;
	
	if (nvram_match("wan_ifname", "eth0"))
	{
		for (u = linux_overrides; u && u->name; u++) 
		{
			nvram_set(u->name, u->value);
		}
	}

	if (!nvram_get("vlan0ports"))
	{
		nvram_set("vlan0hwname", "et0");
		nvram_set("vlan0ports", "1 2 3 4 5*");
		nvram_set("vlan1hwname", "et0");
		nvram_set("vlan1ports", "0 5");
	}
#endif
	if (nvram_invmatch("productid_odm", "") && restore_defaults)
		nvram_restore_odm();

	/* Restore defaults */
	for (t = router_defaults; t->name; t++) {
		if (restore_defaults || !nvram_get(t->name)) {
			for (u = linux_overrides; u && u->name; u++) {
				if (!strcmp(t->name, u->name)) {
					nvram_set(u->name, u->value);
					break;
				}
			}
			if (nvram_match("productid_odm", "") && (!u || !u->name))
				nvram_set(t->name, t->value);
		}
	}

#ifdef R100
	if (restore_defaults)
		nvram_set("wl_ssid", "R100 Wireless Gateway");
#endif

#ifdef WIRELESS_WAN //thansk for Oleg
	if (nvram_invmatch("wl_mode_ex", "ap")) 
	{
#ifdef RANGE_EXTENDER
		nvram_set("wan0_ifname", nvram_safe_get("wan_ifname"));
		nvram_set("wan0_ifnames", nvram_safe_get("wan_ifname"));
#else
		nvram_set("wan0_ifname", nvram_safe_get("wl0_ifname"));
		nvram_set("wan0_ifnames", nvram_safe_get("wl0_ifname"));
#endif
	}
	else {
		nvram_set("wan0_ifname", nvram_safe_get("wan_ifname"));
		nvram_set("wan0_ifnames", nvram_safe_get("wan_ifname"));
	}
#endif

	/* Force to AP */
	if (ap)
		nvram_set("router_disable", "1");

	/* Always set OS defaults */
	nvram_set("os_name", "linux");
	nvram_set("os_version", EPI_ROUTER_VERSION_STR);
	nvram_set("os_date", __DATE__);

	nvram_set("is_modified", "0");

	/* Commit values */
	if (restore_defaults) {
		EXTRA_RESTORE_DEFAULTS();
		/* default value of vlan */
		nvram_commit();		
		cprintf("done\n");
	}
}

static void
set_wan0_vars(void)
{
	int unit;
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";
	
	/* check if there are any connections configured */
	for (unit = 0; unit < MAX_NVPARSE; unit ++) {
		snprintf(prefix, sizeof(prefix), "wan%d_", unit);
		if (nvram_get(strcat_r(prefix, "unit", tmp)))
			break;
	}
	/* automatically configure wan0_ if no connections found */
	if (unit >= MAX_NVPARSE) {
		struct nvram_tuple *t;
		char *v;

		/* Write through to wan0_ variable set */
		snprintf(prefix, sizeof(prefix), "wan%d_", 0);
		for (t = router_defaults; t->name; t ++) {
			if (!strncmp(t->name, "wan_", 4)) {
				if (nvram_get(strcat_r(prefix, &t->name[4], tmp)))
					continue;
				v = nvram_get(t->name);
				nvram_set(tmp, v ? v : t->value);
			}
		}
		nvram_set(strcat_r(prefix, "unit", tmp), "0");
		nvram_set(strcat_r(prefix, "desc", tmp), "Default Connection");
		nvram_set(strcat_r(prefix, "primary", tmp), "1");
	}
}

static int noconsole = 0;

static void
sysinit(void)
{
	time_t tm = 0;

	/* /proc */
	mount("proc", "/proc", "proc", MS_MGC_VAL, NULL);

	/* /tmp */
	mount("ramfs", "/tmp", "ramfs", MS_MGC_VAL, NULL);

	/* /var */
	mkdir("/tmp/var", 0777);
	mkdir("/var/lock", 0777);
	mkdir("/var/log", 0777);
	mkdir("/var/run", 0777);
	mkdir("/var/tmp", 0777);
/*
	mkdir("/tmp/samba", 0777);
	mkdir("/tmp/samba/private", 0777);
	mkdir("/tmp/samba/var", 0777);
	mkdir("/tmp/samba/var/locks", 0777);
	mkdir("/tmp/samba/lib", 0777);
*/
	/* Setup console */
	if (console_init())
		noconsole = 1;

	// for debug 
	//nvram_set("console_loglevel", "0");

	klogctl(8, NULL, atoi(nvram_safe_get("console_loglevel")));
	
	/* Set a sane date */
	stime(&tm);
}


static void
insmod(void)
{
	char buf[PATH_MAX];
	struct utsname name;
	struct stat tmp_stat;

	/* Modules */
	uname(&name);
	snprintf(buf, sizeof(buf), "/lib/modules/%s", name.release);
	if (stat("/proc/modules", &tmp_stat) == 0 &&
	    stat(buf, &tmp_stat) == 0) {
		char module[80], *modules, *next;
#ifdef WL500GX
		modules = nvram_get("kernel_mods") ? : "et wl";
#else
		modules = nvram_get("kernel_mods") ? : "et wl";
#endif
		foreach(module, modules, next)
			eval("insmod", module);
	}

#ifdef USB_SUPPORT
#ifdef LANGUAGE_TW
	eval("insmod", "nls_cp950.o");
	eval("insmod", "nls_big5.o");
	eval("insmod", "nls_cp936.o");
	eval("insmod", "nls_gb2312.o");
	eval("insmod", "nls_utf8.o");
#endif
#ifdef LANGUAGE_CN
	eval("insmod", "nls_cp936.o");
	eval("insmod", "nls_gb2312.o");
	eval("insmod", "nls_cp950.o");
	eval("insmod", "nls_big5.o");
	eval("insmod", "nls_utf8.o");
#endif
#ifdef LANGUAGE_KR
	eval("insmod", "nls_cp949.o");
	eval("insmod", "nls_euc-kr.o");
	eval("insmod", "nls_utf8.o");
#endif
#ifdef LANGUAGE_JP
	eval("insmod", "nls_cp932.o");
	eval("insmod", "nls_euc-jp.o");
	eval("insmod", "nls_sjis.o");
	eval("insmod", "nls_utf8.o");
#endif
#endif
	dprintf("done\n");
}

/* States */
enum {
	RESTART,
	STOP,
	START,
	TIMER,
	IDLE,
	SERVICE,
};
static int state = START;
static int signalled = -1;


/* Signal handling */
static void
rc_signal(int sig)
{
	if (state == IDLE) {	
		if (sig == SIGHUP) {
			dprintf("signalling RESTART\n");
			signalled = RESTART;
		}
		else if (sig == SIGUSR2) {
			dprintf("signalling START\n");
			signalled = START;
		}
		else if (sig == SIGINT) {
			dprintf("signalling STOP\n");
			signalled = STOP;
		}
		else if (sig == SIGALRM) {
			dprintf("signalling TIMER\n");
			signalled = TIMER;
		}
		else if (sig == SIGUSR1) {
			dprintf("signalling USR1\n");
			signalled = SERVICE;
		}
	}
}

/* Timer procedure */
int
do_timer(void)
{
	int interval = atoi(nvram_safe_get("timer_interval"));
	time_t now;
	struct tm gm, local;
	struct timezone tz;

	dprintf("%d\n", interval);

#ifdef ASUS_EXT
	/* Update kernel timezone */
	setenv("TZ", nvram_safe_get("time_zone_x"), 1);
	time(&now);
	gmtime_r(&now, &gm);
	localtime_r(&now, &local);
	tz.tz_minuteswest = (mktime(&gm) - mktime(&local)) / 60;
	settimeofday(NULL, &tz);
	return 0;
#endif
	if (interval == 0)
		return 0;

	/* Report stats */
	if (nvram_invmatch("stats_server", "")) {
		char *stats_argv[] = { "stats", nvram_get("stats_server"), NULL };
		_eval(stats_argv, NULL, 5, NULL);
	}

	/* Sync time */
	stop_ntpc();
	start_ntpc();

	/* Update kernel timezone */
	setenv("TZ", nvram_safe_get("time_zone_x"), 1);
	time(&now);
	gmtime_r(&now, &gm);
	localtime_r(&now, &local);
	tz.tz_minuteswest = (mktime(&gm) - mktime(&local)) / 60;
	settimeofday(NULL, &tz);
	alarm(interval);
	return 0;
}

/* Main loop */
static void
main_loop(void)
{
	sigset_t sigset;
	pid_t shell_pid = 0;
	uint boardflags;

	/* Basic initialization */
	sysinit();
	
//#ifdef ASUS_EXT	
//	convert_asus_values(0);		// move up by Jiahao for WL500gP
//#endif
	
//#ifndef WL500GX
//	insmod();
//#endif

	/* Setup signal handlers */
	signal_init();
	signal(SIGHUP, rc_signal);
	signal(SIGUSR1, rc_signal);
	signal(SIGUSR2, rc_signal);
	signal(SIGINT, rc_signal);
	signal(SIGALRM, rc_signal);
	sigemptyset(&sigset);

	/* Give user a chance to run a shell before bringing up the rest of the system */

	if (!noconsole)
		run_shell(1, 0);

#if defined(WL500GX) && defined(CONFIG_SENTRY5)
	if (!nvram_match("restore_defaults", "0"))
	{
		nvram_set("vlan_enable", "1");
		restore_defaults();
		set_wan0_vars();
		RC1_START();
	}
#endif

#ifdef WL500GX
	boardflags = BFL_ENETVLAN;
#else
	/* Add vlan */
	boardflags = strtoul(nvram_safe_get("boardflags"), NULL, 0);
#endif

	/* Add loopback */
	config_loopback();

	/* Convert deprecated variables */
	convert_deprecated();

	getsyspara();

	/* Restore defaults if necessary */
	restore_defaults();
#if 0	
	if((nvram_match("productid", "WL500g.Premium") || nvram_match("productid", "WL500W")) && nvram_invmatch("sdram_init", "0x0009"))
	{
		nvram_set("sdram_init", "0x0009");
		nvram_commit();
	}
#endif

#ifdef ASUS_EXT	
	convert_asus_values(0);

//	if (nvram_match("wl_mode_ex", "sta"))
//		nvram_set("wan_nat_x", nvram_safe_get("wl_nat"));

	nvram_set("wl_mode_EX", nvram_safe_get("wl_mode_ex"));
	nvram_set("wan_nat_X", nvram_safe_get("wan_nat_x"));
	nvram_set("wl_guest_ENABLE", nvram_safe_get("wl_guest_enable"));
	if (nvram_invmatch("wl_mode_EX", "ap"))
		nvram_set("wl_guest_enable", "0");
	if (nvram_match("wl_mode_EX", "re") && nvram_match("wan_nat_X", "1"))
		nvram_set("wl_hotspot", "1");
	else
		nvram_set("wl_hotspot", "0");
	nvram_set("mssid_ENABLE", nvram_safe_get("mssid_enable"));
	nvram_set("vlans_ENABLE", nvram_safe_get("vlans_enable"));
	nvram_set("upnp_ENABLE", nvram_safe_get("upnp_enable"));
	nvram_set("lan_proto_X", nvram_safe_get("lan_proto_x"));
#endif

	/* Setup wan0 variables if necessary */
	set_wan0_vars();

//#if !(defined(WL500GX) && defined(CONFIG_SENTRY5))
//#ifdef WL500GX
	insmod();
//#endif
//#endif

	/* Loop forever */
	for (;;) {
		switch (state) {
		case SERVICE:
			dprintf("SERVICE\n");
			service_handle();
			state = IDLE;
			break;
		case RESTART:
#ifdef WSC
			if(nvram_match("wsc_config_state","1")){/* Cherry Cho added for WSC in 2007/1/10. */
				break;
			}
#endif
			dprintf("RESTART\n");
			/* Fall through */
		case STOP:
			dprintf("STOP\n");

#ifdef ASUS_EXT
#ifdef WSC/* Cherry Cho added for WSC in 2007/1/10. */			
			stop_misc_no_watchdog();
#else			
			stop_misc();
#endif
#endif
			stop_services();
			stop_wan();
			stop_lan();

#if defined(WL500GX) && defined(CONFIG_SENTRY5)
			RC1_STOP();
#else
			if (boardflags & BFL_ENETVLAN)
			{
				stop_vlan();
			}
#endif

			if (state == STOP) {
				state = IDLE;
				break;
			}
			/* Fall through */
		case START:
			dprintf("START %d\n", boardflags);

#if defined(WL500GX) && defined(CONFIG_SENTRY5)
			RC1_START();
#else
#ifndef WL330GE
			if (boardflags & BFL_ENETVLAN)
			{
				start_vlan();
			}
#endif
#endif

			start_lan();

#ifdef WL330GE
//			if (nvram_invmatch("wl_mode_EX", "ap") && nvram_match("no_profile", "1"))
//				eval("wl", "bss", "down");

			if (nvram_match("wl_mode_EX", "re")// && nvram_match("wan_nat_X", "0")
			)
			{
				if (nvram_match("wl_individual_ure", "0") && nvram_match("no_profile", "1"))
					eval("wl", "bss", "-C", "1", "down");
				else
					eval("wl", "bss", "-C", "1", "up");
			}
			
			if (nvram_match("wl_mode_EX", "ap") && nvram_match("mssid_ENABLE", "1") && nvram_match("wan_nat_X", "0"))
			{
				if (nvram_match("vlans_ENABLE", "1"))
					vlan_config();
				else
					mssid_config();
			}
#endif
			//if (restore_defaults_g) 
			//{
			//	goto retry;
			//}
			start_services();
			start_wan();
			start_nas("wan");

#ifdef ASUS_EXT
			start_misc();
			sleep(5);
			diag_PaN();
#ifdef WSC/* Cherry Cho added in 2007/2/8. */
			if(nvram_match("wsc_config_state","0") && !nvram_match("x_Setting", "1"))		
			{		
				pid_t pid;
				char pid_buf[32];
					
				if(!file_to_buf("/var/run/wsc.pid", pid_buf, sizeof(pid_buf)))
					start_wsc();
				else if(!(pid=atoi(pid_buf)))
					start_wsc();	
			}	
#endif	
#endif
			/* Fall through */
		case TIMER:
			dprintf("TIMER\n");
			do_timer();			
			/* Fall through */
		case IDLE:
			dprintf("IDLE\n");
			state = IDLE;
			/* Wait for user input or state change */
			while (signalled == -1) {
				if (!noconsole && (!shell_pid || kill(shell_pid, 0) != 0))
					shell_pid = run_shell(0, 1);
				else
					sigsuspend(&sigset);
			}
			state = signalled;
			signalled = -1;
			break;
		default:
			dprintf("UNKNOWN\n");
			return;
		}
	}
}

int
main(int argc, char **argv)
{
	char *base = strrchr(argv[0], '/');
	
	base = base ? base + 1 : argv[0];

	/* init */
	if (strstr(base, "init")) {
		main_loop();
		return 0;
	}

	/* Set TZ for all rc programs */
	setenv("TZ", nvram_safe_get("time_zone_x"), 1);

	/* ppp */
	if (strstr(base, "ip-up"))
		return ipup_main(argc, argv);
	else if (strstr(base, "ip-down"))
		return ipdown_main(argc, argv);
	/* udhcpc [ deconfig bound renew ] */
	else if (strstr(base, "udhcpc"))
		return udhcpc_main(argc, argv);
	/* restore default */
	else if (strstr(base, "restore"))
	{
		if (argc==2)
		{
			int step = atoi(argv[1]);
			if (step>=1)
			{
				nvram_set("vlan_enable", "1");
				restore_defaults();							}
			/* Setup wan0 variables if necessary */
			if (step>=2)
				set_wan0_vars();
			if (step>=3)
				RC1_START();
			if (step>=4)
				start_lan();
		}
		return 0;
	}
	/* stats [ url ] */
	else if (strstr(base, "stats"))
		return http_stats(argv[1] ? : nvram_safe_get("stats_server"));

	/* erase [device] */
	else if (strstr(base, "erase")) {
		if (argv[1])
			return mtd_erase(argv[1]);
		else {
			fprintf(stderr, "usage: erase [device]\n");
			return EINVAL;
		}
	}
#ifndef FLASH2M
	/* write [path] [device] */
	else if (strstr(base, "write")) {
		if (argc >= 3)
			return mtd_write(argv[1], argv[2]);
		else {
			fprintf(stderr, "usage: write [path] [device]\n");
			return EINVAL;
		}
	}
#endif
	/* hotplug [event] */
	else if (strstr(base, "hotplug")) {
		if (argc >= 2) {
			if (!strcmp(argv[1], "net"))
				return hotplug_net();
#ifdef ASUS_EXT
			else if(!strcmp(argv[1], "usb"))
				return hotplug_usb();
#endif
		} else {
			fprintf(stderr, "usage: hotplug [event]\n");
			return EINVAL;
		}
	}

#ifdef ASUS_EXT
	/* ddns update ok */
	else if (strstr(base, "stopservice")) {
		if (argc >= 2)
			return(stop_service_main(atoi(argv[1])));
		else return(stop_service_main(0));
	}
	/* ddns update ok */
	else if (strstr(base, "ddns_updated")) 
	{
		return ddns_updated_main();
	}
	/* ddns update ok */
	else if (strstr(base, "start_ddns")) 
	{
		return start_ddns();
	}
#ifdef USB_SUPPORT
	/* send alarm */
#if 0
	else if (strstr(base, "sendalarm")) {
		if (argc >= 1)
			return sendalarm_main(argc, argv);
		else {
			fprintf(stderr, "usage: sendalarm\n");
			return EINVAL;
		}
	}
#endif
#endif
	/* invoke watchdog */
	else if (strstr(base, "watchdog")) {
		return(watchdog_main());
	}
	else if (strstr(base, "gpio")) {
		return(gpio_main(/*atoi(argv[1])*/));
	}
	else if (strstr(base, "radioctrl")) {
		if (argc >= 1)
			return(radio_main(atoi(argv[1])));
		else return EINVAL;
	}
#ifdef BTN_SETUP
	/* invoke ots(one touch setup) */
/*
	else if (strstr(base, "ots"))
		return(ots_main());
*/

#ifdef WSC/* Cherry Cho added in 2007/1/4. */
        else if (strstr(base, "wsc")){
                return(wsc_main());
	}
#endif
#endif
#ifdef USB_SUPPORT
	/* remove webcam module */
	else if (strstr(base, "rmwebcam")) {
		if (argc >= 2)
			return (remove_webcam_main(atoi(argv[1])));
		else return EINVAL;
	}
	/* remove usbstorage module */
	else if (strstr(base, "rmstorage")) {
		return (remove_storage_main());
	}
#endif
	/* run ntp client */
	else if (strstr(base, "ntp")) {
		return (ntp_main());
	}
#ifdef USB_SUPPORT
#if 0
	/* run rcamd */
	else if (strstr(base, "rcamdmain")) {
		return (rcamd_main());
	}
	/* run waveserver */
	else if (strstr(base, "waveservermain")) {
		return (waveserver_main());
	}
#endif
#endif
	/* write srom */
	else if (strstr(base, "wsrom")) 
	{
		do_timer();
		if (argc >= 4) 
			return wsrom_main(argv[1], atoi(argv[2]), atoi(argv[3]));
		else {
			fprintf(stderr, "usage: wsrom [dev] [position] [value in 2 bytes]\n");
			return EINVAL;
		}
	}
	/* write srom */
	else if (strstr(base, "rsrom")) 
	{
		if (argc >= 3)
		{	 
			rsrom_main(argv[1], atoi(argv[2]), 1);
			return 0;
		}
		else {
			fprintf(stderr, "usage: rsrom [dev] [position]\n");
			return EINVAL;
		}
	}
	/* write mac */
	else if (strstr(base, "wmac")) 
	{
		if (argc >= 3) 
			return write_mac(argv[1], argv[2]);
		else {
			fprintf(stderr, "usage: wmac [dev] [mac]\n");
			return EINVAL;
		}
	}
	/* wlan update */
	else if (strstr(base, "wlan_update")) 
	{
		wlan_update();
		return 0;
	}
	else if (strstr(base, "early_convert"))
	{
		early_convert_asus_values();
		return 0;
	}
#ifndef FLASH2M
	/* udhcpc_ex [ deconfig bound renew ], for lan only */
	else if (strstr(base, "landhcpc"))
		return udhcpc_ex_main(argc, argv);
#endif
/*
	else if (strstr(base, "bpa_connect"))
                 return bpa_connect_main(argc, argv);
        else if (strstr(base, "bpa_disconnect"))
                 return bpa_disconnect_main(argc, argv);
*/                 
#endif

	/* rc [stop|start|restart ] */
	else if (strstr(base, "rc")) {
		if (argv[1]) {
			if (strncmp(argv[1], "start", 5) == 0)
				return kill(1, SIGUSR2);
			else if (strncmp(argv[1], "stop", 4) == 0)
				return kill(1, SIGINT);
			else if (strncmp(argv[1], "restart", 7) == 0)
				return kill(1, SIGHUP);
		} else {
			fprintf(stderr, "usage: rc [start|stop|restart]\n");
			return EINVAL;
		}
	}
#ifdef DLM
	else if (strstr(base, "run_apps"))
		return run_apps();

        else if (strstr(base, "run_ftpsamba"))		// added by Jiahao for WL500gP
        {
                run_ftpsamba();
                return 0;
        }
#endif
/*
	else if (strstr(base, "hotplug_usb_mass"))	// added by Jiahao for WL500gP
	{
		return hotplug_usb_mass("");
	}
*/
#ifdef DLM
	else if (strstr(base, "eject_usb1"))		// added by Jiahao for WL500gP
	{
		umount_disc_parts_rc(1);
		return 0;
	}
	else if (strstr(base, "eject_usb2"))		// added by Jiahao for WL500gP
	{
		umount_disc_parts_rc(2);
		return 0;
	}
#endif
	return EINVAL;
}

#ifdef WL330GE
void vlan_config()
{
	char tmpstr[32];
	char vlanstr[32];
	pid_t pid;

        if ( nvram_invmatch("wl_mode_EX", "ap") || nvram_invmatch("mssid_ENABLE", "1") || nvram_invmatch("vlans_ENABLE", "1") )
                return;

        memset(tmpstr, 0, sizeof(tmpstr));
        memset(vlanstr, 0, sizeof(vlanstr));

       	eval("vconfig", "add", "eth0", nvram_safe_get("mssid_vlanid_x0"));
       	sprintf(vlanstr, "VL%s", nvram_safe_get("mssid_vlanid_x0"));
      	eval("brctl", "addbr", vlanstr);
	eval("brctl", "setfd", vlanstr, "0");
	eval("brctl", "stp", vlanstr, "dis");	
//	eval("brctl", "sethello", vlanstr, "0");
       	sprintf(tmpstr, "eth0.%s", nvram_safe_get("mssid_vlanid_x0"));
	eval("brctl", "addif", vlanstr, tmpstr);
       	eval("brctl", "addif", vlanstr, "eth1");
	eval("ifconfig", tmpstr, "0.0.0.0", "up");
       	eval("ifconfig", "eth1", "0.0.0.0", "up");
       	eval("ifconfig", vlanstr, "up");
       	char *argv[] = {"nas", "/tmp/nas.lan.conf", "/tmp/nas.lan.pid", "lan", NULL};
       	nvram_set("lan_ifname", vlanstr);
       	nvram_set("lan_ifnames", "eth1");
       	_eval(argv, NULL, 0, &pid);
        if (nvram_match("mssid_enable_x1", "1"))
        {
        	eval("vconfig", "add", "eth0", nvram_safe_get("mssid_vlanid_x1"));
        	sprintf(vlanstr, "VL%s", nvram_safe_get("mssid_vlanid_x1"));
        	eval("brctl", "addbr", vlanstr);
		eval("brctl", "setfd", vlanstr, "0");
	      	eval("brctl", "stp", vlanstr, "dis");
//		eval("brctl", "sethello", vlanstr, "0");
        	sprintf(tmpstr, "eth0.%s", nvram_safe_get("mssid_vlanid_x1"));
        	eval("brctl", "addif", vlanstr, tmpstr);
        	eval("brctl", "addif", vlanstr, "wl0.1");
       		eval("ifconfig", tmpstr, "0.0.0.0", "up");
       		eval("ifconfig", "wl0.1", "0.0.0.0", "up");
        	eval("ifconfig", vlanstr, "up");
       		nvram_set("lan_ifname", vlanstr);
       		nvram_set("lan_ifnames", "wl0.1");
		char *argv1[] = {"nas", "/tmp/nas.lan1.conf", "/tmp/nas.lan1.pid", "lan", NULL};
		_eval(argv1, NULL, 0, &pid);
        }
        if (nvram_match("mssid_enable_x2", "1"))
        {
        	eval("vconfig", "add", "eth0", nvram_safe_get("mssid_vlanid_x2"));
        	sprintf(vlanstr, "VL%s", nvram_safe_get("mssid_vlanid_x2"));
        	eval("brctl", "addbr", vlanstr);
		eval("brctl", "setfd", vlanstr, "0");
	      	eval("brctl", "stp", vlanstr, "dis");
//		eval("brctl", "sethello", vlanstr, "0");
        	sprintf(tmpstr, "eth0.%s", nvram_safe_get("mssid_vlanid_x2"));
        	eval("brctl", "addif", vlanstr, tmpstr);
        	eval("brctl", "addif", vlanstr, "wl0.2");
       		eval("ifconfig", tmpstr, "0.0.0.0", "up");
       		eval("ifconfig", "wl0.2", "0.0.0.0", "up");
        	eval("ifconfig", vlanstr, "up");
       		nvram_set("lan_ifname", vlanstr);
       		nvram_set("lan_ifnames", "wl0.2");
        	char *argv2[] = {"nas", "/tmp/nas.lan2.conf", "/tmp/nas.lan2.pid", "lan", NULL};
        	_eval(argv2, NULL, 0, &pid);
        }
        if (nvram_match("mssid_enable_x3", "1"))
        {
        	eval("vconfig", "add", "eth0", nvram_safe_get("mssid_vlanid_x3"));
        	sprintf(vlanstr, "VL%s", nvram_safe_get("mssid_vlanid_x3"));
        	eval("brctl", "addbr", vlanstr);
		eval("brctl", "setfd", vlanstr, "0");
	      	eval("brctl", "stp", vlanstr, "dis");
//		eval("brctl", "sethello", vlanstr, "0");
        	sprintf(tmpstr, "eth0.%s", nvram_safe_get("mssid_vlanid_x3"));
        	eval("brctl", "addif", vlanstr, tmpstr);
        	eval("brctl", "addif", vlanstr, "wl0.3");
       		eval("ifconfig", tmpstr, "0.0.0.0", "up");
       		eval("ifconfig", "wl0.3", "0.0.0.0", "up");
        	eval("ifconfig", vlanstr, "up");
       		nvram_set("lan_ifname", vlanstr);
       		nvram_set("lan_ifnames", "wl0.3");
        	char *argv3[] = {"nas", "/tmp/nas.lan3.conf", "/tmp/nas.lan3.pid", "lan", NULL};
        	_eval(argv3, NULL, 0, &pid);
        }
}

void mssid_config()
{
	if ( nvram_invmatch("wl_mode_EX", "ap") || nvram_invmatch("mssid_ENABLE", "1") )
                return;
	
//	eval("brctl", "setfd", "br0", "0");
//	eval("brctl", "stp", "br0", "dis");

	eval("brctl", "addif", "br0", "eth1");
	eval("ifconfig", "eth1", "0.0.0.0", "up");
	
	if (nvram_match("mssid_enable_x1", "1"))
	{
		eval("brctl", "addif", "br0", "wl0.1");
		eval("ifconfig", "wl0.1", "0.0.0.0", "up");
	}
	if (nvram_match("mssid_enable_x2", "1"))
	{
		eval("brctl", "addif", "br0", "wl0.2");
		eval("ifconfig", "wl0.2", "0.0.0.0", "up");
	}
	if (nvram_match("mssid_enable_x3", "1"))
	{
		eval("brctl", "addif", "br0", "wl0.3");
		eval("ifconfig", "wl0.3", "0.0.0.0", "up");
	}
}
#endif
