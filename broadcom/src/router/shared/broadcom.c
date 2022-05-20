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
 * Web Page Configuration Support Routines
 *
 * Copyright 2005, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 * $Id: broadcom.c,v 1.1.1.1 2007/01/25 12:52:22 jiahao_jhou Exp $
 */

#ifdef WEBS
#include <webs.h>
#include <uemf.h>
#include <ej.h>
#else /* !WEBS */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <httpd.h>
#endif /* WEBS */


/* Required for hash table headers*/
#if defined(linux)
/* Use SVID search */
#define __USE_GNU
#include <search.h>
#endif

#include <typedefs.h>
#include <proto/ethernet.h>
#include <bcmparams.h>
#include <bcmnvram.h>
#include <bcmutils.h>
#include <shutils.h>
#include <netconf.h>
#include <nvparse.h>
#include <wlutils.h>
#include <bcmcvar.h>
#include <ezc.h>
#include <bcmconfig.h>
#include <opencrypto.h>
#include <time.h>
#include <epivers.h>


int internal_init(void);

static char * encrypt_var(char *varname, char *ctext, int ctext_len, char *ptext, int *ptext_len,char *key, int keylen);
static char * decrypt_var(char *varname, char *ptext, int ptext_len, char *ctext, int *ctext_len,char *key, int keylen);
static char * make_wl_prefix(char *prefix,int prefix_size, int mode, char *ifname);
static char * remove_dups(char *inlist, int inlist_size);
static char * rfctime(const time_t *timep);
static char * reltime(unsigned int seconds);
static bool find_ethaddr_in_list (void *ethaddr, struct maclist *list);

#define wan_prefix(unit, prefix)	snprintf(prefix, sizeof(prefix), "wan%d_", unit)

/*
 * Country names and abbreviations from ISO 3166
 */
typedef struct {
	char *name;     /* Long name */
	char *abbrev;   /* Abbreviation */
} country_name_t;
country_name_t country_names[];     /* At end of this file */

struct variable variables[];
extern struct nvram_tuple router_defaults[];

enum {
	NOTHING,
	REBOOT,
	RESTART,
};

static const char * const apply_header =
"<head>"
"<title>Broadcom Home Gateway Reference Design: Apply</title>"
"<meta http-equiv=\"Content-Type\" content=\"application/html; charset=utf-8\">"
"<style type=\"text/css\">"
"body { background: white; color: black; font-family: arial, sans-serif; font-size: 9pt }"
".title	{ font-family: arial, sans-serif; font-size: 13pt; font-weight: bold }"
".subtitle { font-family: arial, sans-serif; font-size: 11pt }"
".label { color: #306498; font-family: arial, sans-serif; font-size: 7pt }"
"</style>"
"</head>"
"<body>"
"<p>"
"<span class=\"title\">APPLY</span><br>"
"<span class=\"subtitle\">This screen notifies you of any errors "
"that were detected while changing the router's settings.</span>"
"<form method=\"get\" action=\"apply.cgi\">"
"<p>"
;

static const char * const apply_footer =
"<p>"
"<input type=\"button\" name=\"action\" value=\"Continue\" OnClick=\"document.location.href='%s';\">"
"</form>"
"<p class=\"label\">&#169;2001-2005 ASUSTek Inc.. All rights reserved.</p>"
"</body>"
;

static int ret_code;
static char posterr_msg[255];
static int action = NOTHING;

#define ERR_MSG_SIZE sizeof(posterr_msg)
#if defined(linux)

#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/klog.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <net/if.h>

typedef u_int64_t u64;
typedef u_int32_t u32;
typedef u_int16_t u16;
typedef u_int8_t u8;
#include <linux/ethtool.h>
#include <linux/sockios.h>
#include <net/if_arp.h>

#define sys_restart() kill(1, SIGHUP)
#define sys_reboot() kill(1, SIGTERM)
#define sys_stats(url) eval("stats", (url))

#ifndef WEBS

#define MIN_BUF_SIZE	4096

/* Upgrade from remote server or socket stream */
static int
sys_upgrade(char *url, FILE *stream, int *total)
{
	char upload_fifo[] = "/tmp/uploadXXXXXX";
	FILE *fifo = NULL;
	char *write_argv[] = { "write", upload_fifo, "linux", NULL };
	pid_t pid;
	char *buf = NULL;
	int count, ret = 0;
	long flags = -1;
	int size = BUFSIZ;

	assert(stream);
	assert(total);
	
	if (url)
		return eval("write", url, "linux");

	/* Feed write from a temporary FIFO */
	if (!mktemp(upload_fifo) ||
	    mkfifo(upload_fifo, S_IRWXU) < 0||
	    (ret = _eval(write_argv, NULL, 0, &pid)) ||
	    !(fifo = fopen(upload_fifo, "w"))) {
		if (!ret)
			ret = errno;
		goto err;
	}

	/* Set nonblock on the socket so we can timeout */
	if ((flags = fcntl(fileno(stream), F_GETFL)) < 0 ||
	    fcntl(fileno(stream), F_SETFL, flags | O_NONBLOCK) < 0) {
		ret = errno;
		goto err;
	}

	/*
	* The buffer must be at least as big as what the stream file is
	* using so that it can read all the data that has been buffered 
	* in the stream file. Otherwise it would be out of sync with fn
	* select specially at the end of the data stream in which case
	* the select tells there is no more data available but there in 
	* fact is data buffered in the stream file's buffer. Since no
	* one has changed the default stream file's buffer size, let's
	* use the constant BUFSIZ until someone changes it.
	*/
	if (size < MIN_BUF_SIZE)
		size = MIN_BUF_SIZE;
	if ((buf = malloc(size)) == NULL) {
		ret = ENOMEM;
		goto err;
	}
	
	/* Pipe the rest to the FIFO */
	cprintf("Upgrading.\n");
	while (total && *total) {
		if (waitfor(fileno(stream), 5) <= 0)
			break;
		count = safe_fread(buf, 1, size, stream);
		if (!count && (ferror(stream) || feof(stream)))
			break;
		*total -= count;
		safe_fwrite(buf, 1, count, fifo);
		cprintf(".");
	}
	fclose(fifo);
	fifo = NULL;

	/* Wait for write to terminate */
	waitpid(pid, &ret, 0);
	cprintf("done\n");

	/* Reset nonblock on the socket */
	if (fcntl(fileno(stream), F_SETFL, flags) < 0) {
		ret = errno;
		goto err;
	}

 err:
 	if (buf)
		free(buf);
	if (fifo)
		fclose(fifo);
	unlink(upload_fifo);
	return ret;
}

#endif /* WEBS */

/* Dump firewall log */
static int
ej_dumplog(int eid, webs_t wp, int argc, char_t **argv)
{
	char buf[4096], *line, *next, *s;
	int len, ret = 0;

	time_t tm;
	char *verdict, *src, *dst, *proto, *spt, *dpt;

	if (klogctl(3, buf, 4096) < 0) {
		websError(wp, 400, "Insufficient memory\n");
		return -1;
	}

	for (next = buf; (line = strsep(&next, "\n"));) {
		if (!strncmp(line, "<4>DROP", 7))
			verdict = "denied";
		else if (!strncmp(line, "<4>ACCEPT", 9))
			verdict = "accepted";
		else
			continue;

		/* Parse into tokens */
		s = line;
		len = strlen(s);
		while (strsep(&s, " "));

		/* Initialize token values */
		time(&tm);
		src = dst = proto = spt = dpt = "n/a";

		/* Set token values */
		for (s = line; s < &line[len] && *s; s += strlen(s) + 1) {
			if (!strncmp(s, "TIME=", 5))
				tm = strtoul(&s[5], NULL, 10);
			else if (!strncmp(s, "SRC=", 4))
				src = &s[4];
			else if (!strncmp(s, "DST=", 4))
				dst = &s[4];
			else if (!strncmp(s, "PROTO=", 6))
				proto = &s[6];
			else if (!strncmp(s, "SPT=", 4))
				spt = &s[4];
			else if (!strncmp(s, "DPT=", 4))
				dpt = &s[4];
		}

		ret += websWrite(wp, "%s %s connection %s to %s:%s from %s:%s\n",
				 rfctime(&tm), proto, verdict, dst, dpt, src, spt);
		ret += websWrite(wp, "<br>");
	}

	return ret;
}

static int
ej_syslog(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fp;
	char buf[256] = "/sbin/logread > ";
	char tmp[] = "/tmp/log.XXXXXX";
	int ret;

	if (!nvram_match("log_ram_enable", "1")) {
		websError(wp, 400, "\"Syslog in RAM\" is not enabled.\n");
		return (-1);
	}
	
	mktemp(tmp);
	strcat(buf, tmp);
	system(buf);
	
	fp = fopen(tmp, "r");

	unlink(tmp);

	if (fp == NULL) {
		websError(wp, 400, "logread error\n");
		return (-1);
	}

	websWrite(wp, "<pre>");	

	ret = 0;
	while(fgets(buf, sizeof(buf), fp))
		ret += websWrite(wp, buf);

	ret += websWrite(wp, "</pre>");

	fclose(fp);
		
	return (ret);
}

struct lease_t {
	unsigned char chaddr[16];
	u_int32_t yiaddr;
	u_int32_t expires;
	char hostname[64];
};

/* Dump leases in <tr><td>hostname</td><td>MAC</td><td>IP</td><td>expires</td></tr> format */
static int
ej_lan_leases(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fp = NULL;
	struct lease_t lease;
	int i;
	int index,num_interfaces=0;
	char buf[128];
	struct in_addr addr;
	unsigned long expires = 0;
	char sigusr1[] = "-XX";
	int ret = 0;
	char word[32], *next = NULL;

	/* Write out leases file */
	sprintf(sigusr1, "-%d", SIGUSR1);
	eval("killall", sigusr1, "udhcpd");
	
	/* Count the number of lan and guest interfaces */
	
	if (nvram_get("lan_ifname")) 
		num_interfaces++;
	
	if (nvram_get("unbridged_ifnames"))
		foreach(word,nvram_get("unbridged_ifnames"),next)
			num_interfaces++;
	
	for (index =0; index < num_interfaces; index++){
		snprintf(buf,sizeof(buf),"/tmp/udhcpd%d.leases",index);
	
		if (!(fp = fopen(buf, "r")))
			continue;

		while (fread(&lease, sizeof(lease), 1, fp)) {
			/* Do not display reserved leases */
			if (ETHER_ISNULLADDR(lease.chaddr))
				continue;
			ret += websWrite(wp, "<tr><td>%s</td><td>", lease.hostname);
			for (i = 0; i < 6; i++) {
				ret += websWrite(wp, "%02X", lease.chaddr[i]);
				if (i != 5) ret += websWrite(wp, ":");
			}
			addr.s_addr = lease.yiaddr;
			ret += websWrite(wp, "</td><td>%s</td><td>", inet_ntoa(addr));
			expires = ntohl(lease.expires);
			if (!expires)
				ret += websWrite(wp, "Expired");
			else
				ret += websWrite(wp, "%s", reltime(expires));
			ret += websWrite(wp, "</td></tr>");
		}

		fclose(fp);
	}

	return ret;
}

/* Renew lease */
static int
sys_renew(void)
{
	int unit;
	char tmp[NVRAM_BUFSIZE];
	char *str = NULL;
	int pid;

	if ((unit = atoi(nvram_safe_get("wan_unit"))) < 0)
		unit = 0;
	
	snprintf(tmp, sizeof(tmp), "/var/run/udhcpc%d.pid", unit);
	if ((str = file2str(tmp))) {
		pid = atoi(str);
		free(str);
		return kill(pid, SIGUSR1);
	}
	
	return -1;
}

/* Release lease */
static int
sys_release(void)
{
	int unit;
	char tmp[NVRAM_BUFSIZE];
	char *str= NULL;
	int pid;

	if ((unit = atoi(nvram_safe_get("wan_unit"))) < 0)
		unit = 0;
	
	snprintf(tmp, sizeof(tmp), "/var/run/udhcpc%d.pid", unit);
	if ((str = file2str(tmp))) {
		pid = atoi(str);
		free(str);
		return kill(pid, SIGUSR2);
	}
	
	return -1;
}

#ifdef __CONFIG_NAT__
#define sin_addr(s) (((struct sockaddr_in *)(s))->sin_addr)

/* Return WAN link state */
static int
ej_wan_link(int eid, webs_t wp, int argc, char_t **argv)
{
	char *wan_ifname;
	int s;
	struct ifreq ifr;
	struct ethtool_cmd ecmd;
	FILE *fp= NULL;
	int unit;
	char tmp[NVRAM_BUFSIZE], prefix[] = "wanXXXXXXXXXX_";

	if ((unit = atoi(nvram_safe_get("wan_unit"))) < 0)
		unit = 0;
	wan_prefix(unit, prefix);

	/* non-exist and disabled */
	if (nvram_match(strcat_r(prefix, "proto", tmp), "") ||
	    nvram_match(strcat_r(prefix, "proto", tmp), "disabled")) {
		return websWrite(wp, "N/A");
	}
	/* PPPoE connection status */
	else if (nvram_match(strcat_r(prefix, "proto", tmp), "pppoe")) {
		wan_ifname = nvram_safe_get(strcat_r(prefix, "pppoe_ifname", tmp));
		if ((fp = fopen(strcat_r("/tmp/ppp/link.", wan_ifname, tmp), "r"))) {
			fclose(fp);
			return websWrite(wp, "Connected");
		} else
			return websWrite(wp, "Disconnected");
	}
	/* Get real interface name */
	else
		wan_ifname = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

	/* Open socket to kernel */
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		return websWrite(wp, "N/A");

	/* Check for hardware link */
	strncpy(ifr.ifr_name, wan_ifname, IFNAMSIZ);
	ifr.ifr_data = (void *) &ecmd;
	ecmd.cmd = ETHTOOL_GSET;
	if (ioctl(s, SIOCETHTOOL, &ifr) < 0) {
		close(s);
		return websWrite(wp, "Unknown");
	}
	if (!ecmd.speed) {
		close(s);
		return websWrite(wp, "Disconnected");
	}

	/* Check for valid IP address */
	strncpy(ifr.ifr_name, wan_ifname, IFNAMSIZ);
	if (ioctl(s, SIOCGIFADDR, &ifr) < 0) {
		close(s);
		return websWrite(wp, "Connecting");
	}

	/* Otherwise we are probably configured */
	close(s);
	return websWrite(wp, "Connected");
}

/* Display IP Address lease */
static int
ej_wan_lease(int eid, webs_t wp, int argc, char_t **argv)
{
	unsigned long expires = 0;
	int ret = 0;
	int unit;
	char tmp[NVRAM_BUFSIZE], prefix[] = "wanXXXXXXXXXX_";

	if ((unit = atoi(nvram_safe_get("wan_unit"))) < 0)
		unit = 0;
	wan_prefix(unit, prefix);
	
	if (nvram_match(strcat_r(prefix, "proto", tmp), "dhcp")) {
		char *str;
		time_t now;

		snprintf(tmp, sizeof(tmp), "/tmp/udhcpc%d.expires", unit); 
		if ((str = file2str(tmp))) {
			expires = atoi(str);
			free(str);
		}
		time(&now);
		if (expires <= now)
			ret += websWrite(wp, "Expired");
		else
			ret += websWrite(wp, "%s", reltime(expires - now));
	} else
		ret += websWrite(wp, "N/A");

	return ret;
}
#endif	/* __CONFIG_NAT__ */

/* Report sys up time */
static int
ej_sysuptime(int eid, webs_t wp, int argc, char_t **argv)
{
	char *str = file2str("/proc/uptime");
	if (str) {
		unsigned int up = atoi(str);
		free(str);
		return websWrite(wp, reltime(up));
	}
	return websWrite(wp, "N/A");
}

#ifdef __CONFIG_NAT__
/* Return a list of wan interfaces (eth0/eth1/eth2/eth3) */
static int
ej_wan_iflist(int eid, webs_t wp, int argc, char_t **argv)
{
	char name[IFNAMSIZ], *next;
	int ret = 0;
	int unit;
	char tmp[NVRAM_BUFSIZE], prefix[] = "wanXXXXXXXXXX_";
	char ea[64];
	int s;
	struct ifreq ifr;

	/* current unit # */
	if ((unit = atoi(nvram_safe_get("wan_unit"))) < 0)
		unit = 0;
	wan_prefix(unit, prefix);
	
	if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
		return errno;
	
	/* build wan interface name list */
	foreach(name, nvram_safe_get("wan_ifnames"), next) {
		strncpy(ifr.ifr_name, name, IFNAMSIZ);
		if (ioctl(s, SIOCGIFHWADDR, &ifr))
			continue;
		ret += websWrite(wp, "<option value=\"%s\" %s>%s (%s)</option>", name,
				 nvram_match(strcat_r(prefix, "ifname", tmp), name) ? "selected" : "",
				 name, ether_etoa(ifr.ifr_hwaddr.sa_data, ea));
	}

	close(s);

	return ret;
}
#endif	/* __CONFIG_NAT__ */


#endif /* vxworks */

static int
ej_lan_guest_iflist(int eid, webs_t wp, int argc, char_t **argv)
{
	int ret = 0;
	char ifnames[255],name[IFNAMSIZ],os_name[IFNAMSIZ],*next=NULL;
	char buf[32],*config_num, *value=NULL;

	if (ejArgs(argc, argv, "%s", &config_num) < 1) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
	
	/* Do some of the housekeeping here to remove any leftover invalid NVRAM vars 
	   prior to display */
	   	
	snprintf(ifnames, sizeof(ifnames), "%s", nvram_safe_get("unbridged_ifnames"));

	snprintf(buf, sizeof(buf), "lan%s_ifname", config_num);
	
	value = nvram_safe_get(buf);

	
	if (!*ifnames)
		{ 
			ret += websWrite(wp, "<option value=\"NONE\" selected >NONE</option>");
			return ret;
		}

	if (!remove_dups(ifnames,sizeof(ifnames))){
			websError(wp, 400, "Unable to remove duplicate interfaces from ifname list<br>");	
			return -1;
		}

	foreach(name, ifnames, next) {
	
		   
		if (nvifname_to_osifname( name, os_name, sizeof(os_name) ) < 0)
			continue;
		
		ret += websWrite(wp, "<option value=\"%s\"%s>%s%s</option>",
			name, strcmp(name,value) ? "":" selected ",
			name, !wl_probe(os_name) ? " (Wireless)" : "");
	}
	
	ret += websWrite(wp, "<option value=\"NONE\"%s>NONE</option>",(*value) ? "" : " selected ");
	
	return ret;
}
static int
ej_asp_list(int eid, webs_t wp, int argc, char_t **argv)
{
	websWrite(wp,
	  "<tr>\n"
	  "  <td><a href=\"index.asp\"><img border=\"0\" src=\"basic.gif\" alt=\"Basic\"></a></td>\n"
	  "  <td><a href=\"lan.asp\"><img border=\"0\" src=\"lan.gif\" alt=\"LAN\"></a></td>\n");
#ifdef __CONFIG_NAT__
	websWrite(wp,
	  "  <td><a href=\"wan.asp\"><img border=\"0\" src=\"wan.gif\" alt=\"WAN\"></a></td>\n");
#endif
	websWrite(wp,
	  "  <td><a href=\"status.asp\"><img border=\"0\" src=\"status.gif\" alt=\"Status\"></a></td>\n");
#ifdef __CONFIG_NAT__
	websWrite(wp,
	  "  <td><a href=\"filter.asp\"><img border=\"0\" src=\"filter.gif\" alt=\"Filters\"></a></td>\n"
	  "  <td><a href=\"forward.asp\"><img border=\"0\" src=\"forward.gif\" alt=\"Routing\"></a></td>\n");
#endif
	websWrite(wp,
	  "  <td><a href=\"wireless.asp\"><img border=\"0\" src=\"wireless.gif\" alt=\"Wireless\"></a></td>\n"
	  "  <td><a href=\"security.asp\"><img border=\"0\" src=\"security.gif\" alt=\"Security\"></a></td>\n"
	  "  <td><a href=\"firmware.asp\"><img border=\"0\" src=\"firmware.gif\" alt=\"Firmware\"></a></td>\n");
	websWrite(wp,
	  "  <td width=\"100%%\"></td>\n"
	  "</tr>\n");
	return 0;
}

static char *
rfctime(const time_t *timep)
{
	static char s[201];
	struct tm tm;

#if defined(linux)
	setenv("TZ", nvram_safe_get("time_zone"), 1);
#endif
	memcpy(&tm, localtime(timep), sizeof(struct tm));
	strftime(s, 200, "%a, %d %b %Y %H:%M:%S %z", &tm);
	return s;
}

static char *
reltime(unsigned int seconds)
{
	static char s[] = "XXXXX days, XX hours, XX minutes, XX seconds";
	char *c = s;

	if (seconds > 60*60*24) {
		c += sprintf(c, "%d days, ", seconds / (60*60*24));
		seconds %= 60*60*24;
	}
	if (seconds > 60*60) {
		c += sprintf(c, "%d hours, ", seconds / (60*60));
		seconds %= 60*60;
	}
	if (seconds > 60) {
		c += sprintf(c, "%d minutes, ", seconds / 60);
		seconds %= 60;
	}
	c += sprintf(c, "%d seconds", seconds);

	return s;
}

/* Report time in RFC-822 format */
static int
ej_localtime(int eid, webs_t wp, int argc, char_t **argv)
{
	time_t tm;

	time(&tm);
	return websWrite(wp, rfctime(&tm));
}

static int
ej_wl_mode_list(int eid, webs_t wp, int argc, char_t **argv) 
{
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	char wl_mode[]="wlXXXXXXXXXX_mode";
	char *name=NULL, *next=NULL;
	int ap = 0, sta = 0, wet = 0, wds = 0;
	char cap[WLC_IOCTL_SMLEN];
	char caps[WLC_IOCTL_SMLEN];

		
	if (!make_wl_prefix(prefix,sizeof(prefix),0,NULL)){
		websError(wp, 400, "unit number variable doesn't exist\n");
		return -1;		
	}
	
	snprintf(wl_mode,sizeof(wl_mode),"%smode",prefix);
	
	name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

	if (wl_iovar_get(name, "cap", (void *)caps, WLC_IOCTL_SMLEN))
		return -1;
	
	foreach(cap, caps, next) {
		if (!strcmp(cap, "ap"))
			ap = wds = 1;
		else if (!strcmp(cap, "sta"))
			sta = 1;
		else if (!strcmp(cap, "wet"))
			wet = 1;
	}

	if (ap)
		websWrite(wp, "<option value=\"ap\" %s>Access Point</option>\n", 
				nvram_match(wl_mode, "ap" ) ? "selected" : "");
	if (wds)
		websWrite(wp, "<option value=\"wds\" %s>Wireless Bridge</option>\n", 
				nvram_match(wl_mode, "wds" ) ? "selected" : "");
	if (wet)
		websWrite(wp, "<option value=\"wet\" %s>Wireless Ethernet</option>\n", 
			nvram_match(wl_mode, "wet" ) ? "selected" : "");
	return 0;
}

static int
ej_wl_guest_ssid(int eid, webs_t wp, int argc, char_t **argv)
{	

	char *bss_config_num=NULL,*ssid=NULL;
	char vif[64],*unit=NULL;
	
	if (ejArgs(argc, argv, "%s", &bss_config_num) < 1) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
	
	
	unit = nvram_get("wl_unit");
	if (!unit){
		websError(wp, 400, "unit number variable doesn't exist\n");
		return -1;
	}
	
	snprintf(vif,sizeof(vif),"wl%s.%s_ssid",unit,bss_config_num);
	
	ssid  = nvram_get(vif);

	return (ssid ? websWrite(wp, "%s", ssid): 0);
}
static int
ej_wl_inlist(int eid, webs_t wp, int argc, char_t **argv) 
{
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	char *name=NULL, *next=NULL;
	char cap[WLC_IOCTL_SMLEN];
	char caps[WLC_IOCTL_SMLEN];
	char *var=NULL, *item=NULL;

	if (ejArgs(argc, argv, "%s %s", &var, &item) < 2) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
	
	if (!make_wl_prefix(prefix,sizeof(prefix),0,NULL)){
		websError(wp, 400, "unit number variable doesn't exist\n");
		return -1;
	}

	name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

	if (wl_iovar_get(name, var, (void *)caps, WLC_IOCTL_SMLEN))
		return -1;

	foreach(cap, caps, next) {
		if (!strcmp(cap, item))
			return websWrite(wp, "1");
	}

	return websWrite(wp, "0");
}

static int
ej_wl_wds_status(int eid, webs_t wp, int argc, char_t **argv)
{
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	char *macs=NULL, *next=NULL, *name=NULL;
	char mac[100];
	int i=0, len=0;
	sta_info_t *sta=NULL;
	char buf[300];
	
	if (ejArgs(argc, argv, "%d", &i) < 1) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	if (!make_wl_prefix(prefix,sizeof(prefix),0,NULL)){
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));
	macs = nvram_safe_get(strcat_r(prefix, "wds", tmp));
	
	foreach(mac, macs, next) {
		if (i-- == 0) {
			len = sprintf(buf, "sta_info");
			ether_atoe(mac, (unsigned char *)&buf[len + 1]);
			if (atoi(nvram_safe_get(strcat_r(prefix, "wds_timeout", tmp))) &&
			    !wl_ioctl(name, WLC_GET_VAR, buf, sizeof(buf))) {
				sta = (sta_info_t *)buf;
				return websWrite(wp, "%s", (sta->flags & WL_WDS_LINKUP) ? "up" : "down");
			}
			else
				return websWrite(wp, "%s", "unknown");
		}
	}

	return 0;
}

static int
ej_ses_button_display(int eid, webs_t wp, int argc, char_t **argv)
{

	return 1;
}


static int
ej_ses_cl_button_display(int eid, webs_t wp, int argc, char_t **argv)
{

	return 1;
}
static int
ej_wl_radio_roam_option(int eid, webs_t wp, int argc, char_t **argv)
{
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	char *name=NULL;
	int radio_status = 0;		

	if (!make_wl_prefix(prefix,sizeof(prefix),0,NULL)) {
		websError(wp, 400, "unit number variable doesn't exist\n");
		return -1;
	}
	name = nvram_get(strcat_r(prefix, "ifname", tmp));
	
	if (!name){
		websError(wp, 400, "Could not find: %s\n",strcat_r(prefix, "ifname", tmp));
		return -1;
	}
	
	wl_ioctl(name, WLC_GET_RADIO, &radio_status, sizeof (radio_status));
	radio_status &= WL_RADIO_SW_DISABLE | WL_RADIO_HW_DISABLE;

	if (!radio_status) /* Radio on*/
		websWrite(wp, "<input type=\"submit\" name=\"action\" value=\"RadioOff\" >");
	else /* Radio Off */
		websWrite(wp, "<input type=\"submit\" name=\"action\" value=\"RadioOn\" >");

	return 1;


}
static int 
wl_radio_onoff(webs_t wp, int disable)
{
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	char *name=NULL;
	char *interface_status=NULL;		
	uint radiomaskval = 0;

	if (!make_wl_prefix(prefix,sizeof(prefix),0,NULL)) {
		websError(wp, 400, "unit number variable doesn't exist\n");
		return -1;
	}
	
	interface_status = nvram_get(strcat_r(prefix, "radio", tmp));

	if (interface_status != NULL) {
		if (!strcmp(interface_status, "1")) {
			name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));
			radiomaskval = WL_RADIO_SW_DISABLE << 16 | disable;
			wl_ioctl(name, WLC_SET_RADIO, &radiomaskval, sizeof (disable));
		}
		else {
			websWrite(wp, "Interface is not Enabled...");
			return 0;
		}		
	}
	else {
		websWrite(wp, "Interface  status UnKnown...");
		return 0;
	}


	return 0;

}

/*
 * Example: 
 * lan_ipaddr=192.168.1.1
 * <% nvram_get("lan_ipaddr"); %> produces "192.168.1.1"
 * <% nvram_get("undefined"); %> produces ""
 */
static int
ej_nvram_get(int eid, webs_t wp, int argc, char_t **argv)
{
	char *name=NULL, *c=NULL;
	int ret = 0;

	if (ejArgs(argc, argv, "%s", &name) < 1) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	assert(name);
	
	for (c = nvram_safe_get(name); *c; c++) {
		if (isprint((int) *c) &&
		    *c != '"' && *c != '&' && *c != '<' && *c != '>')
			ret += websWrite(wp, "%c", *c);
		else
			ret += websWrite(wp, "&#%d", *c);
	}

	return ret;
}

/*
 * Example: 
 * wan_proto=dhcp
 * <% nvram_match("wan_proto", "dhcp", "selected"); %> produces "selected"
 * <% nvram_match("wan_proto", "static", "selected"); %> does not produce
 */
static int
ej_nvram_match(int eid, webs_t wp, int argc, char_t **argv)
{
	char *name=NULL, *match=NULL, *output=NULL;

	if (ejArgs(argc, argv, "%s %s %s", &name, &match, &output) < 3) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	assert(name);
	assert(match);
	assert(output);
	
	if (nvram_match(name, match))
		return websWrite(wp, output);

	return 0;
}	

static int
ej_wme_match_op(int eid, webs_t wp, int argc, char_t **argv)
{
	char *name=NULL, *match=NULL, *output=NULL;
	char word[256], *next=NULL;

	if (ejArgs(argc, argv, "%s %s %s", &name, &match, &output) < 3) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	assert(name);
	assert(match);
	assert(output);
	
	foreach(word, nvram_safe_get(name), next) {
		if (!strcmp(word, match))
			return websWrite(wp, output);
	}

	return 0;
}

static int
wl_print_channel_list(webs_t wp, char *name, char *phytype, char *abbrev)
{
	int j, status = 0;
	wl_channels_in_country_t *cic = (wl_channels_in_country_t *)malloc(WLC_IOCTL_MAXLEN);
		
	assert(name);
	assert(phytype);
	assert(abbrev);
	
	if (!cic) {
		status = -1;
		goto exit;
	}

	cic->buflen = WLC_IOCTL_MAXLEN;
	strcpy(cic->country_abbrev, abbrev);
	if (!strcmp(phytype, "a"))
		cic->band = WLC_BAND_A;
	else if ((!strcmp(phytype, "b")) || (!strcmp(phytype, "g")))
		cic->band = WLC_BAND_B;
	else {
		status = -1;
		goto exit;
	}

	if (wl_ioctl(name, WLC_GET_CHANNELS_IN_COUNTRY, cic, cic->buflen) == 0) {
		if (cic->count == 0) {
			status = 0;
			goto exit;
		}
		websWrite(wp, "\t\tif (country == \"%s\")\n\t\t\tchannels = new Array(0",
			abbrev);
		for(j = 0; j < cic->count; j++)
			websWrite(wp, ", %d", cic->channel[j]);
		websWrite(wp,");\n");
	}

exit:
	if (cic)
		free((void *)cic);
	return status;
}

static int
ej_wl_country_list(int eid, webs_t wp, int argc, char_t **argv)
{
	int i =0, status = 0;
	char *name =NULL;
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	char *phytype = NULL;
	wl_country_list_t *cl = (wl_country_list_t *)malloc(WLC_IOCTL_MAXLEN);
	country_name_t *cntry=NULL;
	char *abbrev=NULL;

	if (!cl) {
		status = -1;
		goto exit;
	}

	if (ejArgs(argc, argv, "%s", &phytype) < 1) {
		websError(wp, 400, "Insufficient args\n");
		status = -1;
		goto exit;
	}

	assert(phytype);
	if (!make_wl_prefix(prefix,sizeof(prefix),0,NULL)) {
		websError(wp, 400, "unit number variable doesn't exist\n");
		status = -1;
		goto exit;
	}	

	name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

	cl->buflen = WLC_IOCTL_MAXLEN;
	cl->band_set = TRUE;

	if (!strcmp(phytype, "a"))
		cl->band = WLC_BAND_A;
	else if ((!strcmp(phytype, "b")) || (!strcmp(phytype, "g")))
		cl->band = WLC_BAND_B;
	else {
		status = -1;
		goto exit;
	}

	if (wl_ioctl(name, WLC_GET_COUNTRY_LIST, cl, cl->buflen) == 0) {
		websWrite(wp, "\t\tvar countries = new Array(");
		for(i = 0; i < cl->count; i++) {
			abbrev = &cl->country_abbrev[i*WLC_CNTRY_BUF_SZ];
			websWrite(wp, "\"%s\"", abbrev);
			if (i != (cl->count - 1))
				websWrite(wp, ", ");
		}
		websWrite(wp, ");\n");
		for(i = 0; i < cl->count; i++) {
			abbrev = &cl->country_abbrev[i*WLC_CNTRY_BUF_SZ];
			for(cntry = country_names;
				cntry->name && strcmp(abbrev, cntry->abbrev);
				cntry++); 
			websWrite(wp, "\t\tdocument.forms[0].wl_country_code[%d] = new Option(\"%s\", \"%s\");\n",
				i, cntry->name ? cntry->name : abbrev, abbrev);
		}
	}

exit:
	if (cl)
		free((void *)cl);
	return status;
}

static int
ej_wl_channel_list(int eid, webs_t wp, int argc, char_t **argv)
{
	int  i, status = 0;
	char *name=NULL;
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	char *phytype = NULL;
	wl_country_list_t *cl = (wl_country_list_t *)malloc(WLC_IOCTL_MAXLEN);
	char *abbrev=NULL;

	if (!cl) {
		status = -1;
		goto exit;
	}

	if (ejArgs(argc, argv, "%s", &phytype) < 1) {
		websError(wp, 400, "Insufficient args\n");
		status = -1;
		goto exit;
	}
	
	if (!make_wl_prefix(prefix,sizeof(prefix),0,NULL)) {
		websError(wp, 400, "unit number variable doesn't exist\n");
		status = -1;
		goto exit;
	}

	name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

	cl->buflen = WLC_IOCTL_MAXLEN;
	cl->band_set = TRUE;

	if (!strcmp(phytype, "a"))
		cl->band = WLC_BAND_A;
	else if ((!strcmp(phytype, "b")) || (!strcmp(phytype, "g")))
		cl->band = WLC_BAND_B;
	else {
		status = -1;
		goto exit;
	}

	if (wl_ioctl(name, WLC_GET_COUNTRY_LIST, cl, cl->buflen) == 0) {
		for(i = 0; i < cl->count; i++) {
			abbrev = &cl->country_abbrev[i*WLC_CNTRY_BUF_SZ];
			wl_print_channel_list(wp, name, phytype, abbrev);
		}
	}


exit:
	if (cl)
		free((void *)cl);
	return status;
}

static bool find_ethaddr_in_list (void *ethaddr, struct maclist *list)
{
	int i=0;
	
	assert(ethaddr);
	assert(list);
	
	for (i = 0; i < list->count; i ++) {
		if (!bcmp(ethaddr, (void *)&list->ea[i], ETHER_ADDR_LEN)) {
			return TRUE;
		}
	}

	return FALSE;
}

static int
ej_wl_auth_list(int eid, webs_t wp, int argc, char_t **argv)
{
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	char *name=NULL;
	struct maclist *auth=NULL, *assoc=NULL, *authorized=NULL, *wme=NULL;
	int max_sta_count, maclist_size;
	int i=0;
	
	if (!make_wl_prefix(prefix,sizeof(prefix),0,NULL)) {
		websError(wp, 400, "unit number variable doesn't exist\n");
		return -1;
	}

	name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

	/* buffers and length */
	max_sta_count = MAX_STA_COUNT;
	maclist_size = sizeof(auth->count) + max_sta_count * sizeof(struct ether_addr);

	auth = malloc(maclist_size);
	assoc = malloc(maclist_size);
	authorized = malloc(maclist_size);
	wme = malloc(maclist_size);

	if (!auth || !assoc || !authorized || !wme)
		goto exit;

	/* query wl for authenticated sta list */
	strcpy((char*)auth, "authe_sta_list");
	if (wl_ioctl(name, WLC_GET_VAR, auth, maclist_size))
		goto exit;

	/* query wl for associated sta list */
	assoc->count = max_sta_count;
	if (wl_ioctl(name, WLC_GET_ASSOCLIST, assoc, maclist_size))
		goto exit;

	/* query wl for authorized sta list */
	strcpy((char*)authorized, "autho_sta_list");
	if (wl_ioctl(name, WLC_GET_VAR, authorized, maclist_size))
		goto exit;

	/* query wl for WME sta list */
	strcpy((char*)wme, "wme_sta_list");
	if (wl_ioctl(name, WLC_GET_VAR, wme, maclist_size))
		goto exit;

	/* build authenticated/associated/authorized sta list */
	for (i = 0; i < auth->count; i ++) {
		char ea[ETHER_ADDR_STR_LEN];
		char *value;
		websWrite(wp, "<tr><td>%s</td>", ether_etoa((void *)&auth->ea[i], ea));

		value = (find_ethaddr_in_list ((void *)&auth->ea[i], assoc))? "Yes" : "No";
		websWrite(wp, "<td>%s</td>", value);

		value = (find_ethaddr_in_list ((void *)&auth->ea[i], authorized))? "Yes" : "No";
		websWrite(wp, "<td>%s</td>", value);

		value = (find_ethaddr_in_list ((void *)&auth->ea[i], wme))? "Yes" : "No";
		websWrite(wp, "<td>%s</td>", value);

		websWrite(wp, "</tr>");
	}

	/* error/exit */
exit:
	if (auth) free(auth);
	if (assoc) free(assoc);
	if (authorized) free(authorized);
	if (wme) free(wme);
	
	return 0;
}

/*
 * Example: 
 * wan_proto=dhcp
 * <% nvram_invmatch("wan_proto", "dhcp", "disabled"); %> does not produce
 * <% nvram_invmatch("wan_proto", "static", "disabled"); %> produces "disabled"
 */
static int
ej_nvram_invmatch(int eid, webs_t wp, int argc, char_t **argv)
{
	char *name=NULL, *invmatch=NULL, *output=NULL;

	if (ejArgs(argc, argv, "%s %s %s", &name, &invmatch, &output) < 3) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	assert(name);
	assert(invmatch);
	assert(output);
	
	if (nvram_invmatch(name, invmatch))
		return websWrite(wp, output);

	return 0;
}	

/*
 * Example: 
 * filter_maclist=00:12:34:56:78:00 00:87:65:43:21:00
 * <% nvram_list("filter_maclist", 1); %> produces "00:87:65:43:21:00"
 * <% nvram_list("filter_maclist", 100); %> produces ""
 */
static int
ej_nvram_list(int eid, webs_t wp, int argc, char_t **argv)
{
	char *name=NULL;
	int which=0;
	char word[256], *next=NULL;

	if (ejArgs(argc, argv, "%s %d", &name, &which) < 2) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	foreach(word, nvram_safe_get(name), next) {
		if (which-- == 0)
			return websWrite(wp, word);
	}

	return 0;
}

static int
ej_nvram_inlist(int eid, webs_t wp, int argc, char_t **argv)
{
	char *name, *item, *output;
	char word[256], *next;

	if (ejArgs(argc, argv, "%s %s %s", &name, &item, &output) < 3) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	foreach(word, nvram_safe_get(name), next) {
		if (!strcmp(word, item))
			return websWrite(wp, output);
	}

	return 0;
}

static int
ej_nvram_invinlist(int eid, webs_t wp, int argc, char_t **argv)
{
	char *name, *item, *output;
	char word[256], *next;

	if (ejArgs(argc, argv, "%s %s %s", &name, &item, &output) < 3) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	foreach(word, nvram_safe_get(name), next) {
		if (!strcmp(word, item))
			return 0;
	}

	return websWrite(wp, output);
}

#ifdef __CONFIG_NAT__
/*
 * Example: 
 * <% filter_client(1, 10); %> produces a table of the first 10 client filter entries
 */
static int
ej_filter_client(int eid, webs_t wp, int argc, char_t **argv)
{
	int i, n, j, ret = 0;
	netconf_filter_t start, end;
	bool valid;
	char port[] = "XXXXX";
	char *days[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
	char *hours[] = {
		"12:00 AM", "1:00 AM", "2:00 AM", "3:00 AM", "4:00 AM", "5:00 AM",
		"6:00 AM", "7:00 AM", "8:00 AM", "9:00 AM", "10:00 AM", "11:00 AM",
		"12:00 PM", "1:00 PM", "2:00 PM", "3:00 PM", "4:00 PM", "5:00 PM",
		"6:00 PM", "7:00 PM", "8:00 PM", "9:00 PM", "10:00 PM", "11:00 PM"
	};

	if (ejArgs(argc, argv, "%d %d", &i, &n) < 2) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	for (; i <= n; i++) {
		valid = get_filter_client(i, &start, &end);

		ret += websWrite(wp, "<tr>");
		ret += websWrite(wp, "<td></td>");

		/* Print address range */
		ret += websWrite(wp, "<td><input name=\"filter_client_from_start%d\" value=\"%s\" size=\"15\" maxlength=\"15\"></td>",
				 i, valid ? inet_ntoa(start.match.src.ipaddr) : "");
		ret += websWrite(wp, "<td>-</td>");
		ret += websWrite(wp, "<td><input name=\"filter_client_from_end%d\" value=\"%s\" size=\"15\" maxlength=\"15\"></td>",
				 i, valid ? inet_ntoa(end.match.src.ipaddr) : "");
		ret += websWrite(wp, "<td></td>");

		/* Print protocol */
		ret += websWrite(wp, "<td>");
		ret += websWrite(wp, "<select name=\"filter_client_proto%d\">", i);
		ret += websWrite(wp, "<option value=\"tcp\" %s>TCP</option>",
				 valid && start.match.ipproto == IPPROTO_TCP ? "selected" : "");
		ret += websWrite(wp, "<option value=\"udp\" %s>UDP</option>",
				 valid && start.match.ipproto == IPPROTO_UDP ? "selected" : "");
		ret += websWrite(wp, "</select>");
		ret += websWrite(wp, "</td>");
		ret += websWrite(wp, "<td></td>");

		/* Print port range */
		if (valid)
			snprintf(port, sizeof(port), "%d", ntohs(start.match.dst.ports[0]));
		else
			*port = '\0';
		ret += websWrite(wp, "<td><input name=\"filter_client_to_start%d\" value=\"%s\" size=\"5\" maxlength=\"5\"></td>",
				 i, port);
		ret += websWrite(wp, "<td>-</td>");
		if (valid)
			snprintf(port, sizeof(port), "%d", ntohs(start.match.dst.ports[1]));
		else
			*port = '\0';
		ret += websWrite(wp, "<td><input name=\"filter_client_to_end%d\" value=\"%s\" size=\"5\" maxlength=\"5\"></td>",
				 i, port);
		ret += websWrite(wp, "<td></td>");

		/* Print day range */
		ret += websWrite(wp, "<td>");
		ret += websWrite(wp, "<select name=\"filter_client_from_day%d\">", i);
		for (j = 0; j < ARRAYSIZE(days); j++)
			ret += websWrite(wp, "<option value=\"%d\" %s>%s</option>",
					 j, valid && start.match.days[0] == j ? "selected" : "", days[j]);
		ret += websWrite(wp, "</select>");
		ret += websWrite(wp, "</td>");
		ret += websWrite(wp, "<td>-</td>");
		ret += websWrite(wp, "<td>");
		ret += websWrite(wp, "<select name=\"filter_client_to_day%d\">", i);
		for (j = 0; j < ARRAYSIZE(days); j++)
			ret += websWrite(wp, "<option value=\"%d\" %s>%s</option>",
					 j, valid && start.match.days[1] == j ? "selected" : "", days[j]);
		ret += websWrite(wp, "</select>");
		ret += websWrite(wp, "</td>");
		ret += websWrite(wp, "<td></td>");

		/* Print time range */
		ret += websWrite(wp, "<td>");
		ret += websWrite(wp, "<select name=\"filter_client_from_sec%d\">", i);
		for (j = 0; j < ARRAYSIZE(hours); j++)
			ret += websWrite(wp, "<option value=\"%d\" %s>%s</option>",
					 j * 3600, valid && start.match.secs[0] == (j * 3600) ? "selected" : "", hours[j]);
		ret += websWrite(wp, "</select>");
		ret += websWrite(wp, "</td>");
		ret += websWrite(wp, "<td>-</td>");

		ret += websWrite(wp, "<td>");
		ret += websWrite(wp, "<select name=\"filter_client_to_sec%d\">", i);
		for (j = 0; j < ARRAYSIZE(hours); j++)
			ret += websWrite(wp, "<option value=\"%d\" %s>%s</option>",
					 j * 3600, valid && start.match.secs[1] == (j * 3600) ? "selected" : "", hours[j]);
		/* Special case for 11:59:59 PM */
		ret += websWrite(wp, "<option value=\"%d\" %s>12:00 AM</option>",
				 24 * 3600 - 1, valid && start.match.secs[1] == (24 * 3600 - 1) ? "selected" : "");
		ret += websWrite(wp, "</select>");
		ret += websWrite(wp, "</td>");
		ret += websWrite(wp, "<td></td>");

		/* Print enable */
		ret += websWrite(wp, "<td><input type=\"checkbox\" name=\"filter_client_enable%d\" %s></td>",
				 i, valid && !(start.match.flags & NETCONF_DISABLED) ? "checked" : "");

		ret += websWrite(wp, "</tr>");
	}

	return ret;
}

/*
 * Example: 
 * <% forward_port(1, 10); %> produces a table of the first 10 port forward entries
 */
static int
ej_forward_port(int eid, webs_t wp, int argc, char_t **argv)
{
	int i, n, ret = 0;
	netconf_nat_t nat;
	bool valid;
	char port[] = "XXXXX";

	if (ejArgs(argc, argv, "%d %d", &i, &n) < 2) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	for (; i <= n; i++) {
		valid = get_forward_port(i, &nat);

		ret += websWrite(wp, "<tr>");
		ret += websWrite(wp, "<td></td>");

		/* Print protocol */
		ret += websWrite(wp, "<td>");
		ret += websWrite(wp, "<select name=\"forward_port_proto%d\">", i);
		ret += websWrite(wp, "<option value=\"tcp\" %s>TCP</option>",
				 valid && nat.match.ipproto == IPPROTO_TCP ? "selected" : "");
		ret += websWrite(wp, "<option value=\"udp\" %s>UDP</option>",
				 valid && nat.match.ipproto == IPPROTO_UDP ? "selected" : "");
		ret += websWrite(wp, "</select>");
		ret += websWrite(wp, "</td>");
		ret += websWrite(wp, "<td></td>");

		/* Print WAN destination port range */
		if (valid)
			snprintf(port, sizeof(port), "%d", ntohs(nat.match.dst.ports[0]));
		else
			*port = '\0';
		ret += websWrite(wp, "<td><input name=\"forward_port_from_start%d\" value=\"%s\" size=\"5\" maxlength=\"5\"></td>",
				 i, port);
		ret += websWrite(wp, "<td>-</td>");
		if (valid)
			snprintf(port, sizeof(port), "%d", ntohs(nat.match.dst.ports[1]));
		else
			*port = '\0';
		ret += websWrite(wp, "<td><input name=\"forward_port_from_end%d\" value=\"%s\" size=\"5\" maxlength=\"5\"></td>",
				 i, port);
		ret += websWrite(wp, "<td>></td>");

		/* Print address range */
		ret += websWrite(wp, "<td><input name=\"forward_port_to_ip%d\" value=\"%s\" size=\"15\" maxlength=\"15\"></td>",
				 i, valid ? inet_ntoa(nat.ipaddr) : "");
		ret += websWrite(wp, "<td>:</td>");

		/* Print LAN destination port range */
		if (valid)
			snprintf(port, sizeof(port), "%d", ntohs(nat.ports[0]));
		else
			*port = '\0';
		ret += websWrite(wp, "<td><input name=\"forward_port_to_start%d\" value=\"%s\" size=\"5\" maxlength=\"5\"></td>",
				 i, port);
		ret += websWrite(wp, "<td>-</td>");
		if (valid)
			snprintf(port, sizeof(port), "%d", ntohs(nat.ports[1]));
		else
			*port = '\0';
		ret += websWrite(wp, "<td><input name=\"forward_port_to_end%d\" value=\"%s\" size=\"5\" maxlength=\"5\"></td>",
				 i, port);
		ret += websWrite(wp, "<td></td>");

		/* Print enable */
		ret += websWrite(wp, "<td><input type=\"checkbox\" name=\"forward_port_enable%d\" %s></td>",
				 i, valid && !(nat.match.flags & NETCONF_DISABLED) ? "checked" : "");

		ret += websWrite(wp, "</tr>");
	}

	return ret;
}

static int
ej_autofw_port(int eid, webs_t wp, int argc, char_t **argv)
{
	int i, n, ret = 0;
	netconf_app_t app;
	bool valid;
	char port[] = "XXXXX";

	if (ejArgs(argc, argv, "%d %d", &i, &n) < 2) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	for (; i <= n; i++) {
		valid = get_autofw_port(i, &app);

		/* Parse out_proto:out_port,in_proto:in_start-in_end>to_start-to_end,enable,desc */
		ret += websWrite(wp, "<tr>");
		ret += websWrite(wp, "<td></td>");

		/* Print outbound protocol */
		ret += websWrite(wp, "<td>");
		ret += websWrite(wp, "<select name=\"autofw_port_out_proto%d\">", i);
		ret += websWrite(wp, "<option value=\"tcp\" %s>TCP</option>",
				 valid && app.match.ipproto == IPPROTO_TCP ? "selected" : "");
		ret += websWrite(wp, "<option value=\"udp\" %s>UDP</option>",
				 valid && app.match.ipproto == IPPROTO_UDP ? "selected" : "");
		ret += websWrite(wp, "</select>");
		ret += websWrite(wp, "</td>");
		ret += websWrite(wp, "<td></td>");

		/* Print outbound port */
		if (valid)
			snprintf(port, sizeof(port), "%d", ntohs(app.match.dst.ports[0]));
		else
			*port = '\0';
		ret += websWrite(wp, "<td><input name=\"autofw_port_out_start%d\" value=\"%s\" size=\"5\" maxlength=\"5\"></td>",
				 i, port);
		ret += websWrite(wp, "<td>-</td>");
		if (valid)
			snprintf(port, sizeof(port), "%d", ntohs(app.match.dst.ports[1]));
		else
			*port = '\0';
		ret += websWrite(wp, "<td><input name=\"autofw_port_out_end%d\" value=\"%s\" size=\"5\" maxlength=\"5\"></td>",
				 i, port);
		ret += websWrite(wp, "<td></td>");

		/* Print related protocol */
		ret += websWrite(wp, "<td>");
		ret += websWrite(wp, "<select name=\"autofw_port_in_proto%d\">", i);
		ret += websWrite(wp, "<option value=\"tcp\" %s>TCP</option>",
				 valid && app.proto == IPPROTO_TCP ? "selected" : "");
		ret += websWrite(wp, "<option value=\"udp\" %s>UDP</option>",
				 valid && app.proto == IPPROTO_UDP ? "selected" : "");
		ret += websWrite(wp, "</select>");
		ret += websWrite(wp, "</td>");
		ret += websWrite(wp, "<td></td>");

		/* Print related destination port range */
		if (valid)
			snprintf(port, sizeof(port), "%d", ntohs(app.dport[0]));
		else
			*port = '\0';
		ret += websWrite(wp, "<td><input name=\"autofw_port_in_start%d\" value=\"%s\" size=\"5\" maxlength=\"5\"></td>",
				 i, port);
		ret += websWrite(wp, "<td>-</td>");
		if (valid)
			snprintf(port, sizeof(port), "%d", ntohs(app.dport[1]));
		else
			*port = '\0';
		ret += websWrite(wp, "<td><input name=\"autofw_port_in_end%d\" value=\"%s\" size=\"5\" maxlength=\"5\"></td>",
				 i, port);
		ret += websWrite(wp, "<td></td>");

		/* Print mapped destination port range */
		if (valid)
			snprintf(port, sizeof(port), "%d", ntohs(app.to[0]));
		else
			*port = '\0';
		ret += websWrite(wp, "<td><input name=\"autofw_port_to_start%d\" value=\"%s\" size=\"5\" maxlength=\"5\"></td>",
				 i, port);
		ret += websWrite(wp, "<td>-</td>");
		if (valid)
			snprintf(port, sizeof(port), "%d", ntohs(app.to[1]));
		else
			*port = '\0';
		ret += websWrite(wp, "<td><input name=\"autofw_port_to_end%d\" value=\"%s\" size=\"5\" maxlength=\"5\"></td>",
				 i, port);
		ret += websWrite(wp, "<td></td>");

		/* Print enable */
		ret += websWrite(wp, "<td><input type=\"checkbox\" name=\"autofw_port_enable%d\" %s></td>",
				 i, valid && !(app.match.flags & NETCONF_DISABLED) ? "checked" : "");

		ret += websWrite(wp, "</tr>");
	}

	return ret;
}
#endif	/* __CONFIG_NAT__ */

/*
 * Example: 
 * lan_route=192.168.2.0:255.255.255.0:192.168.2.1:1
 * <% lan_route("ipaddr", 0); %> produces "192.168.2.0"
 */
static int
ej_lan_route(int eid, webs_t wp, int argc, char_t **argv)
{
	char *arg;
	int which;
	char word[256], *next;
	char *ipaddr, *netmask, *gateway, *metric;

	if (ejArgs(argc, argv, "%s %d", &arg, &which) < 2) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	foreach(word, nvram_safe_get("lan_route"), next) {
		if (which-- == 0) {
			netmask = word;
			ipaddr = strsep(&netmask, ":");
			if (!ipaddr || !netmask)
				continue;
			gateway = netmask;
			netmask = strsep(&gateway, ":");
			if (!netmask || !gateway)
				continue;
			metric = gateway;
			gateway = strsep(&metric, ":");
			if (!gateway || !metric)
				continue;
			if (!strcmp(arg, "ipaddr"))
				return websWrite(wp, ipaddr);
			else if (!strcmp(arg, "netmask"))
				return websWrite(wp, netmask);
			else if (!strcmp(arg, "gateway"))
				return websWrite(wp, gateway);
			else if (!strcmp(arg, "metric"))
				return websWrite(wp, metric);
		}
	}

	return 0;
}

#ifdef __CONFIG_NAT__
/*
 * Example: 
 * wan_route=192.168.10.0:255.255.255.0:192.168.10.1:1
 * <% wan_route("ipaddr", 0); %> produces "192.168.10.0"
 */
static int
ej_wan_route(int eid, webs_t wp, int argc, char_t **argv)
{
	char *arg;
	int which;
	char word[256], *next;
	char *ipaddr, *netmask, *gateway, *metric;
	int unit;
	char tmp[NVRAM_BUFSIZE], prefix[] = "wanXXXXXXXXXX_";


	if (ejArgs(argc, argv, "%s %d", &arg, &which) < 2) {websError(wp, 400, "Insufficient args\n");
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	if ((unit = atoi(nvram_safe_get("wan_unit"))) < 0)
		unit = 0;
	wan_prefix(unit, prefix);
	
	foreach(word, nvram_safe_get(strcat_r(prefix, "route", tmp)), next) {
		if (which-- == 0) {
			netmask = word;
			ipaddr = strsep(&netmask, ":");
			if (!ipaddr || !netmask)
				continue;
			gateway = netmask;
			netmask = strsep(&gateway, ":");
			if (!netmask || !gateway)
				continue;
			metric = gateway;
			gateway = strsep(&metric, ":");
			if (!gateway || !metric)
				continue;
			if (!strcmp(arg, "ipaddr"))
				return websWrite(wp, ipaddr);
			else if (!strcmp(arg, "netmask"))
				return websWrite(wp, netmask);
			else if (!strcmp(arg, "gateway"))
				return websWrite(wp, gateway);
			else if (!strcmp(arg, "metric"))
				return websWrite(wp, metric);
		}
	}

	return 0;
}
#endif	/* __CONFIG_NAT__ */

/* Return a list of the currently present wireless interfaces */
static int
ej_wl_list(int eid, webs_t wp, int argc, char_t **argv)
{
	char name[IFNAMSIZ], *next=NULL;
	char wl_name[IFNAMSIZ],os_name[IFNAMSIZ];
	int unit=-1,subunit=-1, ret = 0;
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	char unit_str[]="000000";
	char *hwaddr=NULL, *ssid=NULL, *virtual=NULL;
	char ifnames[256];

	if (ejArgs(argc, argv, "%s", &virtual) > 0) {
		if (strcmp(virtual,"INCLUDE_VIFS")){
			websError(wp, 400, "Unknown argument %s\n",virtual);	
			return -1;
		}
	}
		
	
	snprintf(ifnames, sizeof(ifnames), "%s %s %s", 
		nvram_safe_get("wan_ifnames"),
		nvram_safe_get("lan_ifnames"),
		nvram_safe_get("unbridged_ifnames"));
		
	if (!remove_dups(ifnames,sizeof(ifnames))){
			websError(wp, 400, "Unable to remove duplicate interfaces from ifname list<br>");	
			return -1;
		}		
	
	foreach(name, ifnames, next) {
		   
		if ( nvifname_to_osifname( name, os_name, sizeof(os_name) ) < 0 )
			continue;

		if (wl_probe(os_name) ||
		    wl_ioctl(os_name, WLC_GET_INSTANCE, &unit, sizeof(unit)))
			continue;

		/* Convert eth name to wl name */	

		if( osifname_to_nvifname( name, wl_name, sizeof(wl_name) ) != 0 )
		{			
			websError(wp, 400, "wl name for interface:%s not found\n",name);	
			return -1;
		}
		
		/* Get configured ethernet MAC address */
		snprintf(prefix, sizeof(prefix), "wl%d_", unit);
		hwaddr = nvram_get(strcat_r(prefix, "hwaddr", tmp));
		
		/* Slave intefaces have a '.' in the name. assume the MAC address
		   is the same as the primary interface*/
		if (strchr(wl_name,'.')) {
			/* If Physical interfaces are specified do not
			   process the slave interfaces skip writing out the info
			  */
			if (virtual) 
				snprintf(prefix, sizeof(prefix),"%s_",wl_name);
			else
				continue;
		}
		
		if (get_ifname_unit(wl_name,&unit,&subunit) < 0) {
			websError(wp, 400, "Error extracting unit and subunit name from %s\n",wl_name);	
			return -1;
		}

		/* Should not need to test if pysical interfaces flag is set,
		   since that the code above will skip this portion. However
		   it guards against future problems if this gets reworked
		   in the future as the explicit checks here prevent any
		   ambiguity
		*/
		
		if ((subunit > 0 ) && (virtual) ) 
			snprintf(unit_str,sizeof(unit_str),"%d.%d",unit,subunit);
		else
			snprintf(unit_str,sizeof(unit_str),"%d",unit);			
	
		ssid = nvram_get(strcat_r(prefix, "ssid", tmp));

		if (!hwaddr || !*hwaddr || !ssid || !*ssid)
			continue;

		ret += websWrite(wp, "<option value=\"%s\" %s>%s(%s)</option>\n", unit_str,
				(!strncmp(unit_str,nvram_safe_get("wl_unit"),sizeof(unit_str))) ? "selected" : "",
				 ssid, hwaddr);
	}

	if (!ret)
		ret += websWrite(wp, "<option value=\"-1\" selected>None</option>");

	return ret;
}

/* Return a list of the supported bands on the currently selected wireless interface */
static int
ej_wl_phytypes(int eid, webs_t wp, int argc, char_t **argv)
{
	int  ret = 0;
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	char *phytype=NULL;
	char *phylist=NULL;
	int i=0;

	if (!make_wl_prefix(prefix,sizeof(prefix),0,NULL)) 
		return websWrite(wp, "None");

	/* Get available phy types of the currently selected wireless interface */
	phylist = nvram_safe_get(strcat_r(prefix, "phytypes", tmp));

	/* Get configured phy type */
	phytype = nvram_safe_get("wl_phytype");

	for (i = 0; i < strlen(phylist); i++) {
		ret += websWrite(wp, "<option value=\"%c\" %s>802.11%c (%s GHz)</option>",
				 phylist[i], phylist[i] == *phytype ? "selected" : "", phylist[i], 
				 phylist[i] == 'a' ? "5" : "2.4");
	}

	return ret;
}

/* Return a radio ID given a phy type */
static int
ej_wl_radioid(int eid, webs_t wp, int argc, char_t **argv)
{
	char *phytype=NULL, var[NVRAM_BUFSIZE], *next;
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	int which;

	if (ejArgs(argc, argv, "%s", &phytype) < 1) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
	
	assert(phytype);
	
	if (!make_wl_prefix(prefix,sizeof(prefix),0,NULL)) 
		return websWrite(wp, "None");

	which = strcspn(nvram_safe_get(strcat_r(prefix, "phytypes", tmp)), phytype);
	foreach(var, nvram_safe_get(strcat_r(prefix, "radioids", tmp)), next) {
		if (which == 0)
			return websWrite(wp, var);
		which--;
	}

	return websWrite(wp, "None");
}

/* Return current core revision */
static int
ej_wl_corerev(int eid, webs_t wp, int argc, char_t **argv)
{
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";

	if (!make_wl_prefix(prefix,sizeof(prefix),0,NULL)) 
		return websWrite(wp, "None");

	return websWrite(wp, nvram_safe_get(strcat_r(prefix, "corerev", tmp)));
}

/* Return current wireless channel */
static int
ej_wl_cur_channel(int eid, webs_t wp, int argc, char_t **argv)
{
	char *name=NULL;
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	channel_info_t ci;

	if (!make_wl_prefix(prefix,sizeof(prefix),0,NULL)){
		websError(wp, 400, "unit number variable doesn't exist\n");
		return -1;
	}

	name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

	wl_ioctl(name, WLC_GET_CHANNEL, &ci, sizeof(ci));
	return websWrite(wp, "Current: %d", ci.target_channel);
}

/* Return current country */
static int
ej_wl_cur_country(int eid, webs_t wp, int argc, char_t **argv)
{
	char *name=NULL;
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	char buf[WLC_CNTRY_BUF_SZ];

	if (!make_wl_prefix(prefix,sizeof(prefix),0,NULL)){
		websError(wp, 400, "unit number variable doesn't exist\n");
		return -1;
	}

	name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

	wl_ioctl(name, WLC_GET_COUNTRY, buf, sizeof(buf));
	return websWrite(wp, "%s", buf);
}

/* Return current phytype */
static int
ej_wl_cur_phytype(int eid, webs_t wp, int argc, char_t **argv)
{
	char *name;
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	int phytype;

	if (!make_wl_prefix(prefix,sizeof(prefix),0,NULL)){
		websError(wp, 400, "unit number variable doesn't exist\n");
		return -1;
	}
	
	name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

	/* Get configured phy type */
	wl_ioctl(name, WLC_GET_PHYTYPE, &phytype, sizeof(phytype));

	return websWrite(wp, "Current: 802.11%s", phytype == WLC_PHY_TYPE_A ? "a" :
		phytype == WLC_PHY_TYPE_B ? "b" : "g");
}

#ifdef __CONFIG_NAT__
static char * 
wan_name(int unit, char *prefix, char *name, int len)
{
	char tmp[NVRAM_BUFSIZE], *desc;
	desc = nvram_safe_get(strcat_r(prefix, "desc", tmp));
	snprintf(tmp, sizeof(tmp), "Connection %d", unit + 1);
	snprintf(name, len, "%s", !strcmp(desc, "") ? tmp : desc);
	return name;
}

/* Return a list of wan connections (Connection <N>/<Connection Name>) */
static int
ej_wan_list(int eid, webs_t wp, int argc, char_t **argv)
{
	char tmp[NVRAM_BUFSIZE], prefix[] = "wanXXXXXXXXXX_";
	int unit, ret = 0;

	/* build wan connection name list */
	for (unit = 0; unit < MAX_NVPARSE; unit ++) {
		wan_prefix(unit, prefix);
		if (!nvram_get(strcat_r(prefix, "unit", tmp)))
			continue;
		ret += websWrite(wp, "<option value=\"%d\" %s>%s</option>", unit,
				unit == atoi(nvram_safe_get("wan_unit")) ? "selected" : "",
				wan_name(unit, prefix, tmp, sizeof(tmp)));
	}

	return ret;
}
#endif	/* __CONFIG_NAT__ */

/* write "selected" to page if the argument matches the current
	 wl_ure setting */
static int
ej_ure_list(int eid, webs_t wp, int argc, char_t **argv)
{
	char *temp;
	int match_value;
	int nv_value;

	if (ejArgs(argc, argv, "%d", &match_value) < 1) {
		websError(wp, 400, "Insufficient args\n");
		return EINVAL;
	}

	temp = nvram_get("wl_ure");
	if(strlen( temp ) == 0) {
		if( match_value == 0 )
		{
			/* treat empty string as "0" */
			websWrite(wp, "selected" );
		}
		return 0;
	}

	nv_value = atoi( temp );
	if( nv_value == match_value) {
		websWrite(wp, "selected" );
	}

	return 0;
}

/* Return wlX_ure setting for the current wireless interface.  If
   the current wireless interface is a virtual interface, return 
   the value of the parent interface 
*/
static int
ej_ure_enabled(int eid, webs_t wp, int argc, char_t **argv)
{
	char *temp;
	int unit = -1;
	int sub_unit = -1;
	char nv_param[NVRAM_MAX_PARAM_LEN];

	temp = nvram_get("wl_unit");
	if(strlen( temp ) == 0) {
 		websError(wp, 400, "Error getting wl_unit\n");
		return EINVAL;
	}

	if( get_ifname_unit( temp, &unit, &sub_unit ) != 0 ) {
 		websError(wp, 400, "Error getting unit/subunit\n");
		return EINVAL;
	}

	sprintf( nv_param, "wl%d_ure", unit );
	temp = nvram_safe_get( nv_param );
	if( strncmp( temp, "1", 1 ) == 0 ) {
		websWrite(wp, "\"1\"");
	}
	else {
		websWrite(wp, "\"0\"");
	}

	return 0;
}


char *webs_buf=NULL;
int webs_buf_offset=0;

/* This routine extracts the index variable in the string
   format 
   eg get_string_index("lan_","lan0_ifname",buf,sizeof(buf))
   returns the string "0" in buf.
   
   In the case of where there is no index 
   eg get_string_index("lan_","lan_ifname",buf,sizeof(buf))
   returns NULL in buf[0].
   
   Inputs : 
   - prefix: prefix to start of index string
   - varname: value to process
   - outbuf: output buffer
   - bufsize: output buffer size
   
   Returns:
   -string null terminated string in output buffer
   -pointer to output buffer or NULL if error
*/
char *
get_index_string(char *prefix, char *varname, char *outbuf, int bufsize)
{
	int offset, len;
	
	if (!prefix) return NULL;
	if (!varname) return NULL;
	if (!outbuf) return NULL;
	if (!bufsize) return NULL;
	
	memset(outbuf,0,bufsize);
	
	/* offset is the start of the index number eg
	 *  offset of dhcp0 is 4. prefix contains the "dhcp" as
	 *  the prefix 
	 */
	
	offset = strlen(prefix);
		
	/* This calculates the string length of the index
	 * ie the index value represented as a string 
	 * strchr(varname,'_') is the end of the index string.
	 * strchr(varname,'_') - varname is the length of the var including 
	 * the index string but before the underscore "_"
	*/
	len = strchr(varname,'_') - varname - offset ;
	
	if (len > bufsize) return NULL;
	
	if (len) strncpy(outbuf,&varname[offset],len);
	outbuf[len]='\0';
	
	return outbuf;
}

static void
validate_list(webs_t wp, char *value, struct variable *v,
	      int (*valid)(webs_t, char *, struct variable *), char *varname )
{
	int n, i;
	char name[100];
	char buf[1000] = "", *cur = buf;
	
	assert(v);

	ret_code = EINVAL;
	n = atoi(value);

	for (i = 0; i < n; i++) {
		snprintf(name, sizeof(name), "%s%d", v->name, i);
		if (!(value = websGetVar(wp, name, NULL)))
				return;

		if (!*value && v->nullok)
			continue;
		if (!valid(wp, value, v ))
			continue;
		cur += snprintf(cur, buf + sizeof(buf) - cur, "%s%s",
				cur == buf ? "" : " ", value);
	}
	
	/* Use varname override if specified. Used to make the routine
	   multiinstance compatible */
	if (varname)
		nvram_set(varname, buf);
	else
		nvram_set(v->name, buf);
	
	ret_code = 0;
}	

static int
valid_ipaddr(webs_t wp, char *value, struct variable *v)
{
	unsigned int buf[4];
	struct in_addr ipaddr, netaddr, broadaddr, netmask;
	
	assert(v);

	if (sscanf(value, "%d.%d.%d.%d", &buf[0], &buf[1], &buf[2], &buf[3]) != 4) {
		websBufferWrite(wp, "Invalid <b>%s</b> %s: not an IP address<br>",
			  v->longname, value);
		return FALSE;
	}

	ipaddr.s_addr = htonl((buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3]);

	if (v->argv) {
		(void) inet_aton(nvram_safe_get(v->argv[0]), &netaddr);
		(void) inet_aton(nvram_safe_get(v->argv[1]), &netmask);
		netaddr.s_addr &= netmask.s_addr;
		broadaddr.s_addr = netaddr.s_addr | ~netmask.s_addr;
		if (netaddr.s_addr != (ipaddr.s_addr & netmask.s_addr)) {
			websBufferWrite(wp, "Invalid <b>%s</b> %s: not in the %s/",
				  v->longname, value, inet_ntoa(netaddr));
			websBufferWrite(wp, "%s network<br>", inet_ntoa(netmask));
			return FALSE;
		}
		if (ipaddr.s_addr == netaddr.s_addr) {
			websBufferWrite(wp, "Invalid <b>%s</b> %s: cannot be the network address<br>",
				  v->longname, value);
			return FALSE;
		}
		if (ipaddr.s_addr == broadaddr.s_addr) {
			websBufferWrite(wp, "Invalid <b>%s</b> %s: cannot be the broadcast address<br>",
				  v->longname, value);
			return FALSE;
		}
	}

	return TRUE;
}

static void
validate_ipaddr(webs_t wp, char *value, struct variable *v , char *varname)
{

	assert(v);	
	
	ret_code = EINVAL;
	
	if (!valid_ipaddr(wp, value, v) ) return;
	
	if (varname)
		nvram_set(varname,value) ;
	else
		nvram_set(v->name, value);
		
	ret_code = 0;

}

static void
validate_ipaddrs(webs_t wp, char *value, struct variable *v, char *varname)
{
	ret_code = EINVAL;
	validate_list(wp, value, v, valid_ipaddr, varname);
}

static int
valid_choice(webs_t wp, char *value, struct variable *v)
{
	char **choice=NULL;
	
	assert(v);	

	for (choice = v->argv; *choice; choice++) {
		if (!strcmp(value, *choice))
			return TRUE;
	}

	websBufferWrite(wp, "Invalid <b>%s</b> %s: not one of ", v->longname, value);
	for (choice = v->argv; *choice; choice++)
		websBufferWrite(wp, "%s%s", choice == v->argv ? "" : "/", *choice);
	websBufferWrite(wp, "<br>");
	
	return FALSE;
}

static void
validate_choice(webs_t wp, char *value, struct variable *v, char *varname)
{

	assert(v);
	
	ret_code = EINVAL;
	
	if (!valid_choice(wp, value, v)) return;
	
	if (varname ) 
		nvram_set(varname,value) ;
	else
		nvram_set(v->name, value);
	ret_code = 0;
		
}

static void
validate_router_disable(webs_t wp, char *value, struct variable *v, 
												char *varname)
{
	char *temp = NULL;

	assert(v);
	
	ret_code = EINVAL;
	
	if (!valid_choice(wp, value, v)) return;
	
	/* we need to find out if we're changing the router mode or not.  if 
		 we're really changing the setting, we need to reboot */
	temp = nvram_safe_get( v->name );

	cprintf( "\n\n temp = %s, value = %s\n\n", temp, value );

	if( strcmp( temp, value ) )
		action = REBOOT;

	if (varname ) 
		nvram_set(varname,value) ;
	else
		nvram_set(v->name, value);
	ret_code = 0;
		
}

static int
valid_range(webs_t wp, char *value, struct variable *v)
{
	int n, start, end;
	
	assert(v);

	n = atoi(value);
	start = atoi(v->argv[0]);
	end = atoi(v->argv[1]);

	if (n < start || n > end) {
		websBufferWrite(wp, "Invalid <b>%s</b> %s: out of range %d-%d<br>",
			  v->longname, value, start, end);
		return FALSE;
	}

	return TRUE;
}

static void
validate_range(webs_t wp, char *value, struct variable *v , char *varname)
{
	
	assert(v);
	
	ret_code = EINVAL;
	
	if (!valid_range(wp, value, v)) return ;

	if (varname ) 
		nvram_set(varname,value) ;
	else
		nvram_set(v->name, value);
	
	ret_code = 0;
}

static int
valid_name(webs_t wp, char *value, struct variable *v)
{
	int n, min, max;
	
	assert(v);
	
	n = strlen(value);
	min = atoi(v->argv[0]);
	max = atoi(v->argv[1]);

	if (n > max) {
		websBufferWrite(wp, "Invalid <b>%s</b> %s: longer than %d characters<br>",
			  v->longname, value, max);
		return FALSE;
	}
	else if (n < min) {
		websBufferWrite(wp, "Invalid <b>%s</b> %s: shorter than %d characters<br>",
			  v->longname, value, min);
		return FALSE;
	}

	return TRUE;
}

static void
validate_guest_lan_ifname(webs_t wp, char *value, struct variable *v, char *varname )
{
	int index,unit;
	char ifname[IFNAMSIZ],os_name[IFNAMSIZ];
	
	assert(v);
	assert(value);
	
	ret_code = EINVAL;
	
	if (!*value){
		websBufferWrite(wp, "Guest LAN interface must be specified.<br>");
		return;
	}
	
	if (!v->argv[0]){
		websBufferWrite(wp, "Guest LAN interface index must be specified.<br>");
		return;
	}
	
	index = atoi (v->argv[0]);
	
	if ((index < 1 ) || (index > 4))
		if (!v->argv[0]){
		websBufferWrite(wp, "Guest LAN interface index must be between 1 and 4.<br>");
		return;
	}
	
	if (nvifname_to_osifname( value, os_name, sizeof(os_name) ) < 0){
		websBufferWrite(wp, "Unable to translate Guest LAN interface name: %s.<br>",value);
			return;
	}
			
	if (wl_probe(os_name) ||
		    wl_ioctl(os_name, WLC_GET_INSTANCE, &unit, sizeof(unit))){
			websBufferWrite(wp, "Guest LAN interface %s is not a Wireless Interface.<br>",value);
			return;
	}
	
	/* Guest SSID are not part of a bridge, unset lanX_ifnames */
	
	snprintf(ifname,sizeof(ifname),"lan%d_ifname",index);
	nvram_set(ifname,value);
	
	snprintf(ifname,sizeof(ifname),"lan%d_ifnames",index);
	nvram_unset(ifname);
			    
	ret_code=0;
}
static void
validate_guest_ssid(webs_t wp, char *value, struct variable *v, char *varname )
{
/* Validation of the guest ssids does 3 things
 * 1)adds the wlX.Y_ssid field
 * 2)updates the wlX_vif list
 * 3)removes entry from wlX_vif if the interface is empty
*/
   	
	char *wl_unit=NULL;
	char wl_vif[]="wlXXXXXXXXX_vifs",*wl_vif_value=NULL;
	char wl_ssid[]="wlXXXXXXXXX_ssid";
	char wl_guest[]="wlXXXXXXXXX_guest";
	char wl_radio[]="wlXXXXXXXXX_radio";
	char wl_mode[]="wlXXXXXXXXX_mode";
	char buf[]="wlXXXXXXXXX_vifs";
	char prefix[]="wlXXXXX";
	int p=-1;
	char *subunit=NULL,unit[]="0000";
	char *argv[3];
	int wl_ure = -1;
	
	assert(v);
	assert(value);
	
	ret_code = EINVAL;


	if (varname) 
		wl_unit=varname;
	else

		wl_unit=websGetVar(wp, "wl_unit", NULL);
	
	if (get_ifname_unit(wl_unit,&p,NULL) < 0) return ;
	
	if (p < 0) return;
	
	snprintf(unit,sizeof(unit),"%d",p);
	subunit = v->argv[0];
	
	/* If SSID is not null try to validate it for correct range */
	if (*value){
		struct variable local;
		
		memcpy(&local,v,sizeof(local));
		argv[0]="1";
		argv[1]="32";
		argv[2]=NULL;
		local.argv=argv;
		if (!valid_name(wp, value, &local)) return;
	}
	
	snprintf(wl_ssid,sizeof(wl_ssid),"wl%s.%s_ssid",unit,subunit);
	snprintf(wl_vif,sizeof(wl_vif),"wl%s_vifs",unit);
	snprintf(wl_guest,sizeof(wl_guest),"wl%s.%s_guest",unit,subunit);
	
	memset(buf,0,sizeof(buf));

	/* This logic here decides if updates to virtual interface list on the
	   parent is required */
	   
	wl_vif_value = nvram_get(wl_vif);
	
	if (wl_vif_value ){
	 		char vif[]="wlXXXXXXXXX";
			char *ptr=NULL;
			
			snprintf(vif,sizeof(vif),"wl%s.%s",unit,subunit);
			
			/* Look to see if the virtual interface is present on
			   on the list already
			*/
			ptr=strstr(wl_vif_value,vif);
			
			if (*value){
				/* New interface , non-NULL SSID add to wl_vifs */
				if (!ptr)  
					snprintf(buf,sizeof(buf),"%s wl%s.%s",wl_vif_value,unit,subunit);
				else
				/* Interface present ,non-NULL SSID copy entire vifs string */
					snprintf(buf,sizeof(buf),"%s",wl_vif_value);
			}else{
			/* Purge interface from wl_vifs as the SSID is now NULL */
			
			/* vif present , delete from wl_vifs */
				if ((ptr)){
					char name[IFNAMSIZ], *next = NULL;
					
					memset(buf,0,sizeof(buf));
					
					foreach(name,wl_vif_value,next)
						/* Copy all the interfaces except the the one we want to remove*/
						if (strcmp(name,vif)){
							int len;
							
							len = strlen(buf); 
							if (*buf)
								strncat(buf," ",len-1);
							strncat(buf,name,len-strlen(name));
						}
				}else
					/* Interface absent from wl_vifs, just copy vifs string */
					snprintf(buf,sizeof(buf),"%s",wl_vif_value);		
			}
	}else
		if (*value) snprintf(buf,sizeof(buf),"wl%s.%s",unit,subunit);

	/* before allowing any "nvram_set()":  If URE is enabled, we want to 
		 error here if an SSID isn't present */
	wl_ure = atoi( websGetVar( wp, "wl_ure", NULL ) );
	if(( wl_ure == 1) && (strlen(value) == 0 ))
	{
		websError(wp, 400, "Guest SSID not defined for URE mode\n");
		ret_code = EINVAL;
		return;
	}

	/* Regenerate virtual interface list */
	if (*buf)	
		nvram_set(wl_vif,buf);
	else
		nvram_unset(wl_vif);
		

	/* Update/clean up wlX.Y_guest flag and wlX.Y_ssid */
	
	snprintf(prefix,sizeof(prefix),"wl%s.%s",unit,subunit);
	snprintf(wl_radio,sizeof(wl_radio),"wl%s.%s_radio",unit,subunit);
	snprintf(wl_mode,sizeof(wl_mode),"wl%s.%s_mode",unit,subunit);

	if (*value){
		if ( wl_ure == 0 )
			nvram_set(wl_guest,"1");
		else
			nvram_unset(wl_guest);
		nvram_set(wl_mode,"ap");
		nvram_set(wl_ssid,value);
		nvram_set(wl_radio,"1");
	}else{
		nvram_unset(wl_ssid);
		nvram_set(wl_guest,"0");
		nvram_unset(wl_radio);
		nvram_unset(wl_mode);
	}
	
	ret_code = 0;
}
static void
validate_name(webs_t wp, char *value, struct variable *v, char *varname )
{
	ret_code = EINVAL;
	
	assert(v);
	
	if (!valid_name(wp, value, v)) return;
	
	if (varname ) 
		nvram_set(varname,value) ;
	else	
		nvram_set(v->name, value);
	
	ret_code = 0;
}

static void
validate_ure(webs_t wp, char *value, struct variable *v, char *varname)
{
	char *temp=NULL;
	int wl_unit = -1;
	int wl_subunit = -1;
	int wl_ure = -1;
	int ii = 0;
	char nv_param[NVRAM_MAX_PARAM_LEN];
	char nv_interface[NVRAM_MAX_PARAM_LEN];
	char os_interface[NVRAM_MAX_PARAM_LEN];
	char interface_list[NVRAM_MAX_VALUE_LEN];
	int interface_list_size = sizeof(interface_list);
	char *wan0_ifname = "wan0_ifname";
	char *lan_ifnames = "lan_ifnames";
	char *wan_ifnames = "wan_ifnames";

	if ((temp = websGetVar(wp, "wl_unit", NULL))) 
	{
		if( *temp )
			get_ifname_unit( temp, &wl_unit, &wl_subunit );

		if( wl_subunit != -1 )
		{
			websError(wp, 400, "URE can't be enabled for a virtual I/F\n");
			ret_code = EINVAL;
			return;
		}
	}

	if((temp = websGetVar(wp, "wl_ure", NULL)))
	{
		if( *temp )
			wl_ure = atoi( temp );
	}

	if( wl_ure < 0 || wl_unit < 0 )
	{
		websError(wp, 400, "Insufficient args\n");
		ret_code = EINVAL;
		return;
	}

#if defined(linux)
	if ( nvram_match( "router_disable", "1" ) && wl_ure == 1 )
	{
		websError(wp, 400, "Basic Page: Router Mode != Router\n");
		ret_code = EINVAL;
		return;
	}
#endif /* defined( linux ) */

	if( wl_ure == 1 )
	{
		/* turning on URE*/

		/* If TR, we can only allow a single TR interface */
		for( ii = 0; ii < DEV_NUMIFS; ii++ ) {
			if( ii == wl_unit )
				continue;

			sprintf( nv_param, "wl%d_ure", ii );
			temp = nvram_safe_get( nv_param );
			if( temp && strlen( temp ) > 0 )
			{
				if( atoi( temp ) == 1 ) {
					websError(wp, 400, "Can not have more than one URE interface\n");
					ret_code = EINVAL;
					return;
				}
			}
		}


		/*  Make sure that an SSID has been set for the virtual I/F here!!!	*/
		temp = websGetVar(wp, "wl_guest_ssid_1", NULL);
		if ( !temp || strlen( temp ) == 0 ) 
		{
			websError(wp, 400, "No definition for Guest SSID\n");
			ret_code = EINVAL;
			return;
		}
		
		sprintf( nv_param, "wl_ure" );
		temp = nvram_safe_get( nv_param );
		if( strncmp( temp, "0", 1 ) == 0 || strlen( temp ) == 0 )
		{
			action = REBOOT;
		}
		else {
			/* nothing to do, bail out now */
			return;
		}
 		/* turn URE on for this primary interface */
		nvram_set( nv_param, "1" );

		nvram_set( "ure_disable", "0" );

		/* Set the wl modes for the primary wireless adapter and it's
			 virtual interface */
		sprintf( nv_param, "wl%d_mode", wl_unit );
		nvram_set( nv_param, "sta" );
		nvram_set( "wl_mode", "sta" );

		sprintf( nv_param, "wl%d.1_mode", wl_unit );
		nvram_set( nv_param, "ap" );
	
		/* For URE with routing(Travel Router)we're using the STA part of our URE 
			 enabled radio as our WAN connection.  So, we need to remove this 
			 interface from the list of bridged lan interfaces and set it up as the 
			 WAN device.  
		*/
		temp = nvram_safe_get(lan_ifnames);
		if( interface_list_size <= strlen( temp ) )
		{
			websError(wp, 400, "string too long\n");
			ret_code = 1;
			return;
		}
		strncpy( interface_list, temp, interface_list_size );
		/* this may be confusing, right now the interface name that is stored
			 in the nvram lists is OS specific.  For linux it's an "ethX" for
			 wireless interfaces and for VxWorks "wlX" is stored as "wlX" so
			 this is trying to make OS independant code */
		sprintf( nv_interface, "wl%d", wl_unit );
		nvifname_to_osifname( nv_interface, os_interface, sizeof(os_interface) );
		remove_from_list( os_interface, interface_list, interface_list_size );
		nvram_set( lan_ifnames, interface_list );

		/* Now remove the existing WAN interface from "wan_ifnames" */ 
		temp = nvram_safe_get( wan_ifnames );
		if( interface_list_size <= (strlen( temp ) + strlen( interface_list )) )
		{
			websError(wp, 400, "string too long\n");
			ret_code = 1;
			return;
		}
		strncpy( interface_list, temp, interface_list_size );
		temp = nvram_safe_get( wan0_ifname );
		if( strlen( temp ) != 0 )
		{
			remove_from_list( temp, interface_list, interface_list_size );
		}

		/* set the new WAN interface as the pimary WAN interface and add to
		   the list wan_ifnames */
		nvram_set( wan0_ifname, os_interface );
		add_to_list( os_interface, interface_list, interface_list_size );
		nvram_set( wan_ifnames, interface_list );

		/* now add the AP to the list of bridged lan interfaces */
		temp = nvram_safe_get(lan_ifnames);
		if( interface_list_size <= strlen( temp ) )
		{
			websError(wp, 400, "string too long\n");
			ret_code = 1;
			return;
		}
		strncpy( interface_list, temp, interface_list_size );
		sprintf( nv_interface, "wl%d.1", wl_unit );
		/* virtual interfaces that appear in NVRAM lists are ALWAYS stored
			 as the NVRAM_FORM so we can add to list without translating */
		if( add_to_list( nv_interface, interface_list, interface_list_size ) != 0 )
		{
			websError(wp, 400, "Failed to add AP interface to lan_ifnames\n");
			ret_code = 1;
			return;
		}
		nvram_set( lan_ifnames, interface_list );

		/* Security - We don't support any RADIUS-based authentication, so 
			 we must force these options to OFF */
		/* turn off wlX_auth_mode and wlX.1_auth_mode */
		sprintf( nv_param, "wl%d_auth_mode", wl_unit );
		nvram_set( nv_param, "none" );
		sprintf( nv_param, "wl%d.1_auth_mode", wl_unit );
		nvram_set( nv_param, "none" );
		/* remove wpa and wpa2 from wlX_akm and wlX.1_akm */
		/* wl_akm should be used here rather than wlX_akm, it's an 
			 indexed param, so wl_akm represents wlX_akm */
		sprintf( nv_param, "wl_akm" );
		temp = nvram_get( nv_param );
		if( temp && *temp )
		{
			memset( interface_list, 0, interface_list_size );
			/* NOTE: using "interface_list" to hold security nvram values */
			strncpy( interface_list, temp, interface_list_size - 1 );
			remove_from_list("wpa", interface_list, interface_list_size );
			remove_from_list("wpa2", interface_list, interface_list_size );
			nvram_set( nv_param, interface_list );
		}
		sprintf( nv_param, "wl%d.1_akm", wl_unit );
		temp = nvram_get( nv_param );
		if( temp && *temp )
		{
			memset( interface_list, 0, interface_list_size );
			/* NOTE: using "interface_list" to hold security nvram values */
			strncpy( interface_list, temp, interface_list_size - 1 );
			remove_from_list("wpa", interface_list, interface_list_size );
			remove_from_list("wpa2", interface_list, interface_list_size );
			nvram_set( nv_param, interface_list );
		}
	}
	else
	{
		sprintf( nv_param, "wl_ure");
		temp = nvram_get( nv_param );
		if( strncmp( temp, "1", 1 ) != 0 ) {
			/* nothing to do, bail out now */
			return;
		}
		nvram_set( nv_param, "0" );

		nvram_set( "ure_disable", "1" );

		/* Restore default WAN interface */

		/* Now remove the existing WAN interface from "wan_ifnames" */ 
		temp = nvram_safe_get( wan_ifnames );
		if( interface_list_size <= (strlen( temp ) + strlen( interface_list )) )
		{
			websError(wp, 400, "string too long\n");
			ret_code = 1;
			return;
		}
		strncpy( interface_list, temp, interface_list_size );
		temp = nvram_safe_get( wan0_ifname );
		if( strlen( temp ) != 0 )
		{
			remove_from_list( temp, interface_list, interface_list_size );
		}

		strcpy( os_interface, "eth1" );
		nvram_set( wan0_ifname, os_interface );
		add_to_list( os_interface, interface_list, interface_list_size );
		nvram_set( wan_ifnames, interface_list );

		/* Now we need to remove the virtual I/F from the bridged lan interfaces */
		temp = nvram_safe_get(lan_ifnames);
		if( interface_list_size <= strlen( temp ) )
		{
			websError(wp, 400, "string too long\n");
			ret_code = 1;
			return;
		}
		strncpy( interface_list, temp, interface_list_size );
		sprintf( nv_interface, "wl%d.1", wl_unit );
		/* virtual interfaces that appear in NVRAM lists are ALWAYS stored
			 as the NVRAM_FORM so we can add to list without translating */
		remove_from_list( nv_interface, interface_list, interface_list_size );
		/* Add our primary interface to lan_ifnames - default behavior */
		sprintf( nv_interface, "wl%d", wl_unit );
		nvifname_to_osifname( nv_interface, os_interface, sizeof(os_interface) );
		add_to_list( os_interface, interface_list, interface_list_size );
		nvram_set( lan_ifnames, interface_list );
	}
	action = REBOOT;

	/* We're unsetting the WAN hardware address so that we get the correct
		 address for the new WAN interface the next time we boot. */
	nvram_unset( "wan0_hwaddr" );

	ret_code = 0;
	return;
}

static int
valid_hwaddr(webs_t wp, char *value, struct variable *v)
{
	unsigned char hwaddr[6];

	assert(v);
	
	/* Make exception for "NOT IMPLELEMENTED" string */
	if (!strcmp(value,"NOT_IMPLEMENTED")) 
		return(TRUE);

	/* Check for bad, multicast, broadcast, or null address */
	if (!ether_atoe(value, hwaddr) ||
	    (hwaddr[0] & 1) ||
	    (hwaddr[0] & hwaddr[1] & hwaddr[2] & hwaddr[3] & hwaddr[4] & hwaddr[5]) == 0xff ||
	    (hwaddr[0] | hwaddr[1] | hwaddr[2] | hwaddr[3] | hwaddr[4] | hwaddr[5]) == 0x00) {
		websBufferWrite(wp, "Invalid <b>%s</b> %s: not a MAC address<br>",
			  v->longname, value);
		return FALSE;
	}

	return TRUE;
}

#ifdef __CONFIG_NAT__
static void
validate_hwaddr(webs_t wp, char *value, struct variable *v, char *varname )
{

	ret_code = EINVAL;
	
	if (!valid_hwaddr(wp, value, v)) return;
	
	if(varname ) 
		nvram_set(varname,value);
	else
		nvram_set(v->name, value);
	
	ret_code = 0;
		

}
#endif	/* __CONFIG_NAT__ */

static void
validate_hwaddrs(webs_t wp, char *value, struct variable *v, char *varname )
{

	assert(v);
	
	ret_code = EINVAL;
	validate_list(wp, value, v, valid_hwaddr, varname);
}

static void
validate_country(webs_t wp, char *value, struct variable *v, char *varname)
{
	country_name_t *country=NULL;

	assert(v);
	
	ret_code = EINVAL;	
	for(country = country_names; country->name; country++)
		if (!strcmp(value, country->abbrev)){
			if (varname) 
				nvram_set(varname,value);
			else	
				nvram_set(v->name, value);
			ret_code = 0;
			return ;
		}
	websBufferWrite(wp, "Country abbreviation <b>%s</b>: is invalid<br>",value);
}

static void
validate_dhcp(webs_t wp, char *value, struct variable *v, char *varname)
{
	struct variable dhcp_variables[] = {
		{ longname: "DHCP Server Starting LAN IP Address", argv: ARGV("lan_ipaddr", "lan_netmask") },
		{ longname: "DHCP Server Ending LAN IP Address", argv: ARGV("lan_ipaddr", "lan_netmask") },
	};
	char *start=NULL, *end=NULL;
	char dhcp_start[20]="dhcp_start";
	char dhcp_end[20]="dhcp_start";
	char lan_proto[20]="lan_proto";
	char lan_ipaddr[20]="lan_ipaddr";
	char lan_netmask[20]= "lan_netmask";
	char *dhcp0_argv[3],*dhcp1_argv[3];
	char index[5];
	
	assert(v);
	
	ret_code = EINVAL;
	
	memset(index,sizeof(index),0);
	/* fixup the nvram varnames if varname override is provided */
	if (varname){
		if (!get_index_string(v->prefix,
			varname,index,sizeof(index))) 
				return;

		snprintf(dhcp_start,sizeof(dhcp_start),"dhcp%s_start",index);
		snprintf(dhcp_end,sizeof(dhcp_end),"dhcp%s_end",index);
		snprintf(lan_proto,sizeof(lan_proto),"lan%s_proto",index);
		snprintf(lan_ipaddr,sizeof(lan_ipaddr),"lan%s_ipaddr",index);
		snprintf(lan_netmask,sizeof(lan_netmask),"lan%s_netmask",index);
		dhcp0_argv[0]=lan_ipaddr;
		dhcp0_argv[1]=lan_netmask;
		dhcp0_argv[2]=NULL;
		dhcp1_argv[0]=lan_ipaddr;
		dhcp1_argv[1]=lan_netmask;
		dhcp1_argv[2]=NULL;
		dhcp_variables[0].argv = dhcp0_argv;
		dhcp_variables[1].argv = dhcp1_argv;
	}
	
	if (!(start = websGetVar(wp, dhcp_start, NULL)) ||
	    !(end = websGetVar(wp, dhcp_end, NULL)))
		return;
	if (!*start) start = end;
	if (!*end) end = start;
	if (!*start && !*end && !strcmp(nvram_safe_get(lan_proto), "dhcp")) {
		websBufferWrite(wp, "Invalid <b>%s</b>: must specify a range<br>", v->longname);
		return;
	}
	if (!valid_ipaddr(wp, start, &dhcp_variables[0]) ||
	    !valid_ipaddr(wp, end, &dhcp_variables[1]))
		return;
	if (ntohl(inet_addr(start)) > ntohl(inet_addr(end))) {
		websBufferWrite(wp, "Invalid <b>%s</b> %s: greater than <b>%s</b> %s<br>",
			  dhcp_variables[0].longname, start, dhcp_variables[1].longname, end);
		return;
	}

	nvram_set(dhcp_start, start);
	nvram_set(dhcp_end, end);
	
	ret_code = 0;
}

static void
validate_lan_ipaddr(webs_t wp, char *value, struct variable *v, char *varname)
{
	struct variable fields[] = {
		{ name: "lan_ipaddr", longname: "LAN IP Address" },
		{ name: "lan_netmask", longname: "LAN Subnet Mask" },
	};
	char *lan_ipaddr=NULL, *lan_netmask=NULL;
	char tmp_ipaddr[20], tmp_netmask[20];
	struct in_addr ipaddr, netmask, netaddr, broadaddr;
	char lan_ipaddrs[][20] = { "dhcp_start", "dhcp_end", "dmz_ipaddr" };
#ifdef __CONFIG_NAT__
	netconf_filter_t start, end;
	netconf_nat_t nat;
	bool valid;
#endif	/* __CONFIG_NAT__ */
	int i;

	assert(v);
	
	ret_code = EINVAL;
	
	/* Insert name overrides */
	if (varname)
	{
		char index[5];
		
		if (!get_index_string(v->prefix,
			varname,index,sizeof(index))) 
				return ;
				
		snprintf(tmp_ipaddr,sizeof(tmp_ipaddr),"lan%s_ipaddr",index);
		snprintf(tmp_netmask,sizeof(tmp_netmask),"lan%s_netmask",index);
		fields[0].name = tmp_ipaddr;
		fields[1].name = tmp_netmask;
		snprintf(lan_ipaddrs[0],sizeof(lan_ipaddrs[0]),"dhcp%s_start",index);
		snprintf(lan_ipaddrs[1],sizeof(lan_ipaddrs[1]),"dhcp%s_end",index);
	}
	/* Basic validation */
	if (!(lan_ipaddr = websGetVar(wp, fields[0].name, NULL)) ||
	    !(lan_netmask = websGetVar(wp, fields[1].name, NULL)) ||
	    !valid_ipaddr(wp, lan_ipaddr, &fields[0]) ||
	    !valid_ipaddr(wp, lan_netmask, &fields[1]))
		return;

	/* Check for broadcast or network address */
	(void) inet_aton(lan_ipaddr, &ipaddr);
	(void) inet_aton(lan_netmask, &netmask);
	netaddr.s_addr = ipaddr.s_addr & netmask.s_addr;
	broadaddr.s_addr = netaddr.s_addr | ~netmask.s_addr;
	if (ipaddr.s_addr == netaddr.s_addr) {
		websBufferWrite(wp, "Invalid <b>%s</b> %s: cannot be the network address<br>",
			  fields[0].longname, lan_ipaddr);
		return;
	}
	if (ipaddr.s_addr == broadaddr.s_addr) {
		websBufferWrite(wp, "Invalid <b>%s</b> %s: cannot be the broadcast address<br>",
			  fields[0].longname, lan_ipaddr);
		return;
	}


	nvram_set(fields[0].name, lan_ipaddr);
	nvram_set(fields[1].name, lan_netmask);


	/* Fix up LAN IP addresses */
	for (i = 0; i < ARRAYSIZE(lan_ipaddrs); i++) {

		value = nvram_get(lan_ipaddrs[i]);
		if (value && *value) {
			(void) inet_aton(value, &ipaddr);
			ipaddr.s_addr &= ~netmask.s_addr;
			ipaddr.s_addr |= netaddr.s_addr;
			
			nvram_set(lan_ipaddrs[i], inet_ntoa(ipaddr));
		}
	}				

#ifdef __CONFIG_NAT__
	/* Fix up client filters and port forwards */
	for (i = 0; i < MAX_NVPARSE; i++) {
		if (get_filter_client(i, &start, &end)) {
			start.match.src.ipaddr.s_addr &= ~netmask.s_addr;
			start.match.src.ipaddr.s_addr |= netaddr.s_addr;
			end.match.src.ipaddr.s_addr &= ~netmask.s_addr;
			end.match.src.ipaddr.s_addr |= netaddr.s_addr;
			
			valid = set_filter_client(i, &start, &end);
			a_assert(valid);
		}
		if (get_forward_port(i, &nat)) {
			nat.ipaddr.s_addr &= ~netmask.s_addr;
			nat.ipaddr.s_addr |= netaddr.s_addr;
			valid = set_forward_port(i, &nat);
			a_assert(valid);
		}
	}
#endif	/* __CONFIG_NAT__ */

	ret_code = 0;
}	

#ifdef __CONFIG_NAT__
static void
validate_filter_client(webs_t wp, char *value, struct variable *v, char *varname)
{
	int n, i, j;
	bool valid;
	struct variable fields[] = {
		{ name: "filter_client_from_start%d", longname: "LAN Client Filter Starting IP Address", argv: ARGV("lan_ipaddr", "lan_netmask") },
		{ name: "filter_client_from_end%d", longname: "LAN Client Filter Ending IP Address", argv: ARGV("lan_ipaddr", "lan_netmask") },
		{ name: "filter_client_proto%d", longname: "LAN Client Filter Protocol", argv: ARGV("tcp", "udp") },
		{ name: "filter_client_to_start%d", longname: "LAN Client Filter Starting Destination Port", argv: ARGV("0", "65535") },
		{ name: "filter_client_to_end%d", longname: "LAN Client Filter Ending Destination Port", argv: ARGV("0", "65535") },
		{ name: "filter_client_from_day%d", longname: "LAN Client Filter Starting Day", argv: ARGV("0", "6") },
		{ name: "filter_client_to_day%d", longname: "LAN Client Filter Ending Day", argv: ARGV("0", "6") },
		{ name: "filter_client_from_sec%d", longname: "LAN Client Filter Starting Second", argv: ARGV("0", "86400") },
		{ name: "filter_client_to_sec%d", longname: "LAN Client Filter Ending Second", argv: ARGV("0", "86400") },
	};
	char *from_start=NULL, *from_end=NULL, *proto=NULL, *to_start=NULL;
	char *to_end=NULL, *from_day=NULL, *to_day=NULL, *from_sec=NULL, *to_sec=NULL, *enable=NULL;
	char **locals[] = { &from_start, &from_end, &proto, &to_start, &to_end, &from_day, &to_day, &from_sec, &to_sec };
	char name[1000];
	netconf_filter_t start, end;
	
	assert(v);
	
	ret_code = EINVAL;

	/* filter_client indicates how many to expect */
	if (!valid_range(wp, value, v)){
		websBufferWrite(wp, "Invalid filter client string <b>%s</b><br>",value);	
		return;
	}
	n = atoi(value);

	for (i = 0; i <= n; i++) {
		/* Set up field names */
		for (j = 0; j < ARRAYSIZE(fields); j++) {
			snprintf(name, sizeof(name), fields[j].name, i);
			if (!(*locals[j] = websGetVar(wp, name, NULL)))
				break;
		}
		/* Incomplete web page */
		if (j < ARRAYSIZE(fields))
			continue;
		/* Enable is a checkbox */
		snprintf(name, sizeof(name), "filter_client_enable%d", i);
		if (websGetVar(wp, name, NULL))
			enable = "on";
		else
			enable = "off";
		/* Delete entry if all fields are blank */
		if (!*from_start && !*from_end && !*to_start && !*to_end) {
			del_filter_client(i);
			continue;
		}
		/* Fill in empty fields with default values */
		if (!*from_start) from_start = from_end;
		if (!*from_end) from_end = from_start;
		if (!*to_start) to_start = to_end;
		if (!*to_end) to_end = to_start;
		if (!*from_start || !*from_end) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify a LAN IP Address Range<br>", v->longname);
			continue;
		}
		if (!*to_start || !*to_end) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify a Destination Port Range<br>", v->longname);
			continue;
		}
		/* Check individual fields */
		if (!valid_ipaddr(wp, from_start, &fields[0]) ||
		    !valid_ipaddr(wp, from_end, &fields[1]) ||
		    !valid_choice(wp, proto, &fields[2]) ||
		    !valid_range(wp, to_start, &fields[3]) ||
		    !valid_range(wp, to_end, &fields[4]) ||
		    !valid_range(wp, from_day, &fields[5]) ||
		    !valid_range(wp, to_day, &fields[6]) ||
		    !valid_range(wp, from_sec, &fields[7]) ||
		    !valid_range(wp, to_sec, &fields[8]))
			continue;
		/* Check dependencies between fields */
		if (ntohl(inet_addr(from_start)) > ntohl(inet_addr(from_end))) {
			websBufferWrite(wp, "Invalid <b>%s</b> %s: greater than <b>%s</b> %s<br>",
				  fields[0].longname, from_start, fields[1].longname, from_end);
			continue;
		}
		if (atoi(to_start) > atoi(to_end)) {
			websBufferWrite(wp, "Invalid <b>%s</b> %s: greater than <b>%s</b> %s<br>",
				  fields[3].longname, to_start, fields[4].longname, to_end);
			continue;
		}

		/* Set up parameters */
		memset(&start, 0, sizeof(netconf_filter_t));
		if (!strcmp(proto, "tcp"))
			start.match.ipproto = IPPROTO_TCP;
		else if (!strcmp(proto, "udp"))
			start.match.ipproto = IPPROTO_UDP;
		(void) inet_aton(from_start, &start.match.src.ipaddr);
		start.match.src.netmask.s_addr = htonl(0xffffffff);
		start.match.dst.ports[0] = htons(atoi(to_start));
		start.match.dst.ports[1] = htons(atoi(to_end));
		start.match.days[0] = atoi(from_day);
		start.match.days[1] = atoi(to_day);
		start.match.secs[0] = atoi(from_sec);
		start.match.secs[1] = atoi(to_sec);
		if (!strcmp(enable, "off"))
			start.match.flags |= NETCONF_DISABLED;
		memcpy(&end, &start, sizeof(netconf_filter_t));
		(void) inet_aton(from_end, &end.match.src.ipaddr);

		/* Do it */
		valid = set_filter_client(i, &start, &end);
		a_assert(valid);
	}
	
		ret_code = 0;
}

static void
validate_forward_port(webs_t wp, char *value, struct variable *v, char *varname)
{
	int n, i, j;
	bool valid;
	struct variable fields[] = {
		{ name: "forward_port_proto%d", longname: "Port Forward Protocol", argv: ARGV("tcp", "udp") },
		{ name: "forward_port_from_start%d", longname: "Port Forward Starting WAN Port", argv: ARGV("0", "65535") },
		{ name: "forward_port_from_end%d", longname: "Port Forward Ending WAN Port", argv: ARGV("0", "65535") },
		{ name: "forward_port_to_ip%d", longname: "Port Forward LAN IP Address", argv: ARGV("lan_ipaddr", "lan_netmask") },
		{ name: "forward_port_to_start%d", longname: "Port Forward Starting LAN Port", argv: ARGV("0", "65535") },
		{ name: "forward_port_to_end%d", longname: "Port Forward Ending LAN Port", argv: ARGV("0", "65535") },
	};
	char *proto=NULL, *from_start=NULL, *from_end=NULL;
	char *to_ip=NULL, *to_start=NULL, *to_end=NULL, *enable=NULL;
	char **locals[] = { &proto, &from_start, &from_end, &to_ip, &to_start, &to_end };
	char name[1000];
	netconf_nat_t nat;

	assert(v);
	
	ret_code = EINVAL;
	
	/* forward_port indicates how many to expect */
	if (!valid_range(wp, value, v)){
		websBufferWrite(wp, "Invalid forward port string <b>%s</b><br>",value);	
		return;
	}
	n = atoi(value);

	for (i = 0; i <= n; i++) {
		/* Set up field names */
		for (j = 0; j < ARRAYSIZE(fields); j++) {
			snprintf(name, sizeof(name), fields[j].name, i);
			if (!(*locals[j] = websGetVar(wp, name, NULL)))
				break;
		}
		/* Incomplete web page */
		if (j < ARRAYSIZE(fields))
			continue;
		/* Enable is a checkbox */
		snprintf(name, sizeof(name), "forward_port_enable%d", i);
		if (websGetVar(wp, name, NULL))
			enable = "on";
		else
			enable = "off";
		/* Delete entry if all fields are blank */
		if (!*from_start && !*from_end && !*to_ip && !*to_start && !*to_end) {
			del_forward_port(i);
			continue;
		}
		/* Fill in empty fields with default values */
		if (!*from_start) from_start = from_end;
		if (!*from_end) from_end = from_start;
		if (!*to_start && !*to_end)
			to_start = from_start;
		if (!*to_start) to_start = to_end;
		if (!*to_end) to_end = to_start;
		if (!*from_start || !*from_end) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify a LAN IP Address Range<br>", v->longname);
			continue;
		}
		if (!*to_ip) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify a LAN IP Address<br>", v->longname);
			continue;
		}
		if (!*to_start || !*to_end) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify a Destination Port Range<br>", v->longname);
			continue;
		}
		/* Check individual fields */
		if (!valid_choice(wp, proto, &fields[0]) ||
		    !valid_range(wp, from_start, &fields[1]) ||
		    !valid_range(wp, from_end, &fields[2]) ||
		    !valid_ipaddr(wp, to_ip, &fields[3]) ||
		    !valid_range(wp, to_start, &fields[4]) ||
		    !valid_range(wp, to_end, &fields[5]))
			continue;
		if (atoi(from_start) > atoi(from_end)) {
			websBufferWrite(wp, "Invalid <b>%s</b> %s: greater than <b>%s</b> %s<br>",
				  fields[1].longname, from_start, fields[2].longname, from_end);
			continue;
		}
		if (atoi(to_start) > atoi(to_end)) {
			websBufferWrite(wp, "Invalid <b>%s</b> %s: greater than <b>%s</b> %s<br>",
				  fields[4].longname, to_start, fields[5].longname, to_end);
			continue;
		}
		if ((atoi(from_end) - atoi(from_start)) != (atoi(to_end) - atoi(to_start))) {
			websBufferWrite(wp, "Invalid <b>%s</b>: WAN Port Range and LAN Port Range must be the same size<br>", v->longname);
			continue;
		}

		/* Set up parameters */
		memset(&nat, 0, sizeof(netconf_nat_t));
		if (!strcmp(proto, "tcp"))
			nat.match.ipproto = IPPROTO_TCP;
		else if (!strcmp(proto, "udp"))
			nat.match.ipproto = IPPROTO_UDP;
		nat.match.dst.ports[0] = htons(atoi(from_start));
		nat.match.dst.ports[1] = htons(atoi(from_end));
		(void) inet_aton(to_ip, &nat.ipaddr);
		nat.ports[0] = htons(atoi(to_start));
		nat.ports[1] = htons(atoi(to_end));
		if (!strcmp(enable, "off"))
			nat.match.flags |= NETCONF_DISABLED;

		/* Do it */
		valid = set_forward_port(i, &nat);
		a_assert(valid);
	}
	ret_code =0;
}

static void
validate_autofw_port(webs_t wp, char *value, struct variable *v, char *varname)
{
	int n, i, j;
	bool valid;
	struct variable fields[] = {
		{ name: "autofw_port_out_proto%d", longname: "Outbound Protocol", argv: ARGV("tcp", "udp") },
		{ name: "autofw_port_out_start%d", longname: "Outbound Port Start", argv: ARGV("0", "65535") },
		{ name: "autofw_port_out_end%d", longname: "Outbound Port End", argv: ARGV("0", "65535") },
		{ name: "autofw_port_in_proto%d", longname: "Inbound Protocol", argv: ARGV("tcp", "udp") },
		{ name: "autofw_port_in_start%d", longname: "Inbound Port Start", argv: ARGV("0", "65535") },
		{ name: "autofw_port_in_end%d", longname: "Inbound Port End", argv: ARGV("0", "65535") },
 		{ name: "autofw_port_to_start%d", longname: "To Port Start", argv: ARGV("0", "65535") },
 		{ name: "autofw_port_to_end%d", longname: "To Port End", argv: ARGV("0", "65535") },
	};
	char *out_proto=NULL, *out_start=NULL, *out_end=NULL, *in_proto=NULL;
	char *in_start=NULL, *in_end=NULL, *to_start=NULL, *to_end=NULL, *enable=NULL;
	char **locals[] = { &out_proto, &out_start, &out_end, &in_proto, &in_start, &in_end, &to_start, &to_end };
	char name[1000];
	netconf_app_t app;

	assert(v);
	
	ret_code = EINVAL;
	
	/* autofw_port indicates how many to expect */
	if (!valid_range(wp, value, v)){
		websBufferWrite(wp, "Invalid auto forward port string <b>%s</b><br>",value);
		return;
	}
	n = atoi(value);

	for (i = 0; i <= n; i++) {
		/* Set up field names */
		for (j = 0; j < ARRAYSIZE(fields); j++) {
			snprintf(name, sizeof(name), fields[j].name, i);
			if (!(*locals[j] = websGetVar(wp, name, NULL)))
				break;
		}
		/* Incomplete web page */
		if (j < ARRAYSIZE(fields))
			continue;
		/* Enable is a checkbox */
		snprintf(name, sizeof(name), "autofw_port_enable%d", i);
		if (websGetVar(wp, name, NULL))
			enable = "on";
		else
			enable = "off";
		/* Delete entry if all fields are blank */
		if (!*out_start && !*out_end && !*in_start && !*in_end && !*to_start && !*to_end) {
			del_autofw_port(i);
			continue;
		}
		/* Fill in empty fields with default values */
		if (!*out_start) out_start = out_end;
		if (!*out_end) out_end = out_start;
		if (!*in_start) in_start = in_end;
		if (!*in_end) in_end = in_start;
		if (!*to_start && !*to_end)
			to_start = in_start;
		if (!*to_start) to_start = to_end;
		if (!*to_end) to_end = to_start;
		if (!*out_start || !*out_end) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify an Outbound Port Range<br>", v->longname);
			continue;
		}
		if (!*in_start || !*in_end) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify an Inbound Port Range<br>", v->longname);
			continue;
		}
		if (!*to_start || !*to_end) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify a To Port Range<br>", v->longname);
			continue;
		}
		/* Check individual fields */
		if (!valid_choice(wp, out_proto, &fields[0]) ||
		    !valid_range(wp, out_start, &fields[1]) ||
		    !valid_range(wp, out_end, &fields[2]) ||
		    !valid_choice(wp, in_proto, &fields[3]) ||
		    !valid_range(wp, in_start, &fields[4]) ||
		    !valid_range(wp, in_end, &fields[5]) ||
		    !valid_range(wp, to_start, &fields[6]) ||
		    !valid_range(wp, to_end, &fields[7]))
			continue;
		/* Check dependencies between fields */
		if (atoi(out_start) > atoi(out_end)) {
			websBufferWrite(wp, "Invalid <b>%s</b> %s: greater than <b>%s</b> %s<br>",
				  fields[1].longname, out_start, fields[2].longname, out_end);
			continue;
		}
		if (atoi(in_start) > atoi(in_end)) {
			websBufferWrite(wp, "Invalid <b>%s</b> %s: greater than <b>%s</b> %s<br>",
				  fields[4].longname, in_start, fields[5].longname, in_end);
			continue;
		}
		if (atoi(to_start) > atoi(to_end)) {
			websBufferWrite(wp, "Invalid <b>%s</b> %s: greater than <b>%s</b> %s<br>",
				  fields[6].longname, in_start, fields[7].longname, in_end);
			continue;
		}
		if ((atoi(in_end) - atoi(in_start)) != (atoi(to_end) - atoi(to_start))) {
			websBufferWrite(wp, "Invalid <b>%s</b>: Inbound Port Range and To Port Range must be the same size<br>", v->longname);
			continue;
		}
#ifdef NEW_PORT_TRIG
		if ((atoi(in_end) - atoi(in_start)) > 100) {
			websBufferWrite(wp, "Invalid <b>%s</b>: Inbound Port Range must be less than 100<br>", v->longname);
			continue;
		}
#endif

		/* Set up parameters */
		memset(&app, 0, sizeof(netconf_app_t));
		if (!strcmp(out_proto, "tcp"))
			app.match.ipproto = IPPROTO_TCP;
		else if (!strcmp(out_proto, "udp"))
			app.match.ipproto = IPPROTO_UDP;
		app.match.dst.ports[0] = htons(atoi(out_start));
		app.match.dst.ports[1] = htons(atoi(out_end));
		if (!strcmp(in_proto, "tcp"))
			app.proto = IPPROTO_TCP;
		else if (!strcmp(in_proto, "udp"))
			app.proto = IPPROTO_UDP;
		app.dport[0] = htons(atoi(in_start));
		app.dport[1] = htons(atoi(in_end));
		app.to[0] = htons(atoi(to_start));
		app.to[1] = htons(atoi(to_end));
		if (!strcmp(enable, "off"))
			app.match.flags |= NETCONF_DISABLED;

		/* Do it */
		valid = set_autofw_port(i, &app);
		a_assert(valid);
	}
	ret_code = 0;
}
#endif	/* __CONFIG_NAT__ */

static void
validate_lan_route(webs_t wp, char *value, struct variable *v, char *varname)
{
	int n, i;
	char buf[1000] = "", *cur = buf;
	char lan_ipaddr[20] ="lan_ipaddr";
	char lan_netmask[20]="lan_netmask";
	char *lan_argv[3];
	
	struct variable lan_route_variables[] = {
		{ longname: "Route IP Address", argv: NULL },
		{ longname: "Route Subnet Mask", argv: NULL },
		{ longname: "Route Gateway", argv: NULL },
		{ longname: "Route Metric", argv: ARGV("0", "15") },
	};
	
	assert(v);
	
	ret_code = EINVAL;
	
	/* Insert name overrides */
	if (varname)
	{
		char index[5];
		
		if (!get_index_string(v->prefix,varname,
			index,sizeof(index)))
				return;

		snprintf(lan_ipaddr,sizeof(lan_ipaddr),"lan%s_ipaddr",index);
		snprintf(lan_netmask,sizeof(lan_netmask),"lan%s_netmask",index);
	}
	
	n = atoi(value);

	for (i = 0; i < n; i++) {
		char lan_route_ipaddr[] = "lan_route_ipaddrXXX";
		char lan_route_netmask[] = "lan_route_netmaskXXX";
		char lan_route_gateway[] = "lan_route_gatewayXXX";
		char lan_route_metric[] = "lan_route_metricXXX";
		char *ipaddr, *netmask, *gateway, *metric;

 		snprintf(lan_route_ipaddr, sizeof(lan_route_ipaddr), "%s_ipaddr%d", v->name, i);
		snprintf(lan_route_netmask, sizeof(lan_route_netmask), "%s_netmask%d", v->name, i);
 		snprintf(lan_route_gateway, sizeof(lan_route_gateway), "%s_gateway%d", v->name, i);
 		snprintf(lan_route_metric, sizeof(lan_route_metric), "%s_metric%d", v->name, i);
		if (!(ipaddr = websGetVar(wp, lan_route_ipaddr, NULL)) ||
		    !(netmask = websGetVar(wp, lan_route_netmask, NULL)) ||
		    !(gateway = websGetVar(wp, lan_route_gateway, NULL)) ||
		    !(metric = websGetVar(wp, lan_route_metric, NULL)))
			return;
		if (!*ipaddr && !*netmask && !*gateway && !*metric)
			continue;
		if (!*ipaddr && !*netmask && *gateway) {
			ipaddr = "0.0.0.0";
			netmask = "0.0.0.0";
		}
		if (!*gateway)
			gateway = "0.0.0.0";
		if (!*metric)
			metric = "0";
		if (!*ipaddr) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify an IP Address<br>", v->longname);
			continue;
		}
		if (!*netmask) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify a Subnet Mask<br>", v->longname);
			continue;
		}
		
		lan_argv[0]=lan_ipaddr;
		lan_argv[1]=lan_netmask;
		lan_argv[2]=NULL;
		
		lan_route_variables[2].argv = lan_argv;
		if (!valid_ipaddr(wp, ipaddr, &lan_route_variables[0]) ||
		    !valid_ipaddr(wp, netmask, &lan_route_variables[1]) ||
		    !valid_ipaddr(wp, gateway, &lan_route_variables[2]) ||
		    !valid_range(wp, metric, &lan_route_variables[3]))
			continue;
		cur += snprintf(cur, buf + sizeof(buf) - cur, "%s%s:%s:%s:%s",
				cur == buf ? "" : " ", ipaddr, netmask, gateway, metric);
	}

	if (varname)	
		nvram_set(varname,buf);
	else
		nvram_set(v->name, buf);
	ret_code = 0;
}

#ifdef __CONFIG_NAT__
static void
validate_wan_route(webs_t wp, char *value, struct variable *v, char *varname)
{
	int n, i;
	char buf[1000] = "", *cur = buf;
	struct variable wan_route_variables[] = {
		{ longname: "Route IP Address", argv: NULL },
		{ longname: "Route Subnet Mask", argv: NULL },
		{ longname: "Route Gateway", argv: NULL },
		{ longname: "Route Metric", argv: ARGV("0", "15") },
	};

	assert(v);
	
	ret_code = EINVAL;
	
	n = atoi(value);

	for (i = 0; i < n; i++) {
		char wan_route_ipaddr[] = "wan_route_ipaddrXXX";
		char wan_route_netmask[] = "wan_route_netmaskXXX";
		char wan_route_gateway[] = "wan_route_gatewayXXX";
		char wan_route_metric[] = "wan_route_metricXXX";
		char *ipaddr, *netmask, *gateway, *metric;

 		snprintf(wan_route_ipaddr, sizeof(wan_route_ipaddr), "%s_ipaddr%d", v->name, i);
		snprintf(wan_route_netmask, sizeof(wan_route_netmask), "%s_netmask%d", v->name, i);
 		snprintf(wan_route_gateway, sizeof(wan_route_gateway), "%s_gateway%d", v->name, i);
 		snprintf(wan_route_metric, sizeof(wan_route_metric), "%s_metric%d", v->name, i);
		if (!(ipaddr = websGetVar(wp, wan_route_ipaddr, NULL)) ||
		    !(netmask = websGetVar(wp, wan_route_netmask, NULL)) ||
		    !(gateway = websGetVar(wp, wan_route_gateway, NULL)) ||
		    !(metric = websGetVar(wp, wan_route_metric, NULL)))
			continue;
		if (!*ipaddr && !*netmask && !*gateway && !*metric)
			continue;
		if (!*ipaddr && !*netmask && *gateway) {
			ipaddr = "0.0.0.0";
			netmask = "0.0.0.0";
		}
		if (!*gateway)
			gateway = "0.0.0.0";
		if (!*metric)
			metric = "0";
		if (!*ipaddr) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify an IP Address<br>", v->longname);
			continue;
		}
		if (!*netmask) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must specify a Subnet Mask<br>", v->longname);
			continue;
		}
		if (!valid_ipaddr(wp, ipaddr, &wan_route_variables[0]) ||
		    !valid_ipaddr(wp, netmask, &wan_route_variables[1]) ||
		    !valid_ipaddr(wp, gateway, &wan_route_variables[2]) ||
		    !valid_range(wp, metric, &wan_route_variables[3]))
			continue;
		cur += snprintf(cur, buf + sizeof(buf) - cur, "%s%s:%s:%s:%s",
				cur == buf ? "" : " ", ipaddr, netmask, gateway, metric);
	}


	nvram_set(v->name, buf);
	
	ret_code = 0;
}
#endif	/* __CONFIG_NAT__ */

static void
validate_wl_auth(webs_t wp, char *value, struct variable *v, char *varname)
{

	assert(v);

	ret_code = EINVAL;
	
	if (!valid_choice(wp, value, v)){
		websBufferWrite(wp, "Invalid auth value string <b>%s</b><br>",value);
		return;
	}

	if (!strcmp(value, "1")) {
		char *wep = websGetVar(wp, "wl_wep", "");
		if (!wep || strcmp(wep, "enabled")) {
			websBufferWrite(wp, "Invalid <b>WEP Encryption</b>: must be <b>Enabled</b> when <b>802.11 Authentication</b> is <b>%s</b><br>", value);
			return;
		}
	}

	if (varname)
		nvram_set(varname, value);
	else
		nvram_set(v->name, value);
	ret_code = 0;
}

static void
validate_wl_preauth(webs_t wp, char *value, struct variable *v, char *varname)
{
	char *ptr=NULL;
	
	assert(v);
	
	ptr =( (varname) ? varname : v->name );
	
	if (!strcmp(value, "disabled"))
			nvram_set(ptr, "0");
	else
			nvram_set(ptr, "1");
			
	ret_code =0;
	
	return;
}
	
static void
validate_wl_auth_mode(webs_t wp, char *value, struct variable *v, char *varname)
{

	assert(v);
	
	ret_code = EINVAL;
	
	if (!valid_choice(wp, value, v)){
		websBufferWrite(wp, "Invalid auto mode string <b>%s</b>",value);
		return;
	}

	if (!strcmp(value, "radius")) {
		char *wep = websGetVar(wp, "wl_wep", "");
		char *ipaddr = websGetVar(wp, "wl_radius_ipaddr", "");
		if (!wep || strcmp(wep, "enabled")) {
			websBufferWrite(wp, "Invalid <b>WEP Encryption</b>: must be <b>Enabled</b> when <b>Network Authentication</b> is <b>%s</b><br>", value);
			return;
		}
		if (!ipaddr || !strcmp(ipaddr, "")) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must first specify a valid <b>RADIUS Server</b><br>", v->longname);
			return;
		}
	}
	
	if (varname)
		nvram_set(varname, value);
	else
		nvram_set(v->name, value);
	
	ret_code = 0;

}

static void 
validate_wl_akm(webs_t wp, char *value, struct variable *v, char *varname) 
{
	char akms[WLC_IOCTL_SMLEN] = "";
	char *wpa=NULL, *psk=NULL;
#ifdef BCMWPA2
	char *wpa2=NULL, *psk2=NULL;
#endif
	
	assert(v);
	
	ret_code = EINVAL;
	
	wpa = websGetVar(wp, "wl_akm_wpa", NULL);
	if( !wpa )
		wpa = "disabled";
	psk = websGetVar(wp, "wl_akm_psk", NULL);
#ifdef BCMWPA2
	wpa2 = websGetVar(wp, "wl_akm_wpa2", NULL);
	if( !wpa2 )
		wpa2 = "disabled";
	psk2 = websGetVar(wp, "wl_akm_psk2", NULL);
#endif
	if (!wpa || !psk
#ifdef BCMWPA2
	    || !wpa2 || !psk2
#endif
	    ) {
	    	ret_code = 0;
		return;
	}
	
	if (!strcmp(wpa, "enabled") 
#ifdef BCMWPA2
	    || !strcmp(wpa2, "enabled")
#endif /* BCMWPA2 */
	    ) {
		char *ipaddr = websGetVar(wp, "wl_radius_ipaddr", "");
		if (!ipaddr || !strcmp(ipaddr, "")) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must first specify a valid <b>RADIUS Server</b><br>", v->longname);
			return;
		}
	}

	if (!strcmp(psk, "enabled") 
#ifdef BCMWPA2
	    || !strcmp(psk2, "enabled")
#endif /* BCMWPA2 */
	    ) {
		char *key = websGetVar(wp, "wl_wpa_psk", "");
		if (!key || !strcmp(key, "")) {
			websBufferWrite(wp, "Invalid <b>%s</b>: must first specify a valid <b>WPA Pre-Shared Key</b><br>", v->longname);
			return;
		}
	} 

	if (!strcmp(wpa, "enabled") || !strcmp(psk, "enabled") 
#ifdef BCMWPA2
	    || !strcmp(wpa2, "enabled") || !strcmp(psk2, "enabled")
#endif /* BCMWPA2 */
	    ) {
		char *crypto = websGetVar(wp, "wl_crypto", "");
		if (!crypto || (strcmp(crypto, "tkip") && strcmp(crypto, "aes") &&
			strcmp(crypto, "tkip+aes"))) {
			websBufferWrite(wp, "Invalid <b>%s</b>: <b>Crypto Algorithm</b> mode must be TKIP or AES or TKIP+AES<br>", v->longname);
			return;
		}
	}

	if (!strcmp(wpa, "enabled"))
		strcat(akms, "wpa ");
	if (!strcmp(psk, "enabled"))
		strcat(akms, "psk ");
#ifdef BCMWPA2
	if (!strcmp(wpa2, "enabled"))
		strcat(akms, "wpa2 ");
	if (!strcmp(psk2, "enabled"))
		strcat(akms, "psk2 ");
#endif
	if (varname)
		nvram_set(varname, akms);
	else
		nvram_set("wl_akm", akms);
	
	ret_code = 0;
}

static void
validate_wl_wpa_psk(webs_t wp, char *value, struct variable *v, char *varname)
{
	int len = strlen(value);
	char *c=NULL;

	assert(v);
	
	ret_code = EINVAL;
	
	if (len == 64) {
		for (c = value; *c; c++) {
			if (!isxdigit((int) *c)) {
				websBufferWrite(wp, "Invalid <b>%s</b>: character %c is not a hexadecimal digit<br>", v->longname, *c);
				return;
			}
		}
	} else if (len < 8 || len > 63) {
		websBufferWrite(wp, "Invalid <b>%s</b>: must be between 8 and 63 ASCII characters or 64 hexadecimal digits<br>", v->longname);
		return;
	}

	if (varname)
		nvram_set(varname, value);
	else
		nvram_set(v->name, value);
	
	ret_code = 0;
}

static void
validate_wl_key(webs_t wp, char *value, struct variable *v, char *varname)
{
	char *c=NULL;

	assert(v);
	
	ret_code = EINVAL;
	
	switch (strlen(value)) {
	case 5:
	case 13:			
		break;
	case 10:
	case 26:
		for (c = value; *c; c++) {
			if (!isxdigit((int) *c)) {
				websBufferWrite(wp, "Invalid <b>%s</b>: character %c is not a hexadecimal digit<br>", v->longname, *c);
				return;
			}
		}
		break;
	default:
		websBufferWrite(wp, "Invalid <b>%s</b>: must be 5 or 13 ASCII characters or 10 or 26 hexadecimal digits<br>", v->longname);
		return;
	}
	
	if (varname)
		nvram_set(varname, value);
	else	
		nvram_set(v->name, value);
		
	ret_code = 0;
}

static void
validate_wl_wep(webs_t wp, char *value, struct variable *v, char *varname)
{
	char *auth_mode=NULL,*name=NULL, *auth=NULL;
	
	assert(v);
	
	ret_code = EINVAL;
	
	if (!valid_choice(wp, value, v)){
		websBufferWrite(wp, "Invalid wep string <b>%s</b>",value);
		return;
	}
	if (varname)
		name = varname;
	else
		name = v->name;
	
	auth = websGetVar(wp, "wl_auth", NULL);
	auth_mode = websGetVar(wp, "wl_auth_mode", NULL);
	if (!strcmp(value, "enabled")) {
		if (!auth_mode || strcmp(auth_mode, "radius")) {
			char wl_key[] = "wl_keyXXX";
			char wl_key_index[] = "wl_keyXXX";
			
			if (varname){
				char index[5];
				if (!get_index_string(v->prefix,varname,index,sizeof(index)))
						return;
				snprintf(wl_key_index, sizeof(wl_key_index),"wl%s_key",index);
			}else
				snprintf(wl_key_index, sizeof(wl_key_index),"wl_key");
			
			snprintf(wl_key, sizeof(wl_key), "%s%s",wl_key_index, nvram_safe_get(wl_key_index));
			if (!strlen(nvram_safe_get(wl_key))) {
				websBufferWrite(wp, "Invalid <b>%s</b>: must first specify a valid <b>Network Key %s</b><br>", 
						v->longname, nvram_safe_get(wl_key_index));
				if (nvram_match(name, "enabled")) {
					websBufferWrite(wp, "<b>%s</b> is <b>Disabled</b><br>", v->longname);
					nvram_set(name, "disabled");
				}
				return;
			}
		}
	}
	else {
	    	if (!auth || !strcmp(auth, "shared") || ( auth_mode && !strcmp(auth_mode, "radius"))) {
			websBufferWrite(wp, "Invalid <b>WEP Encryption</b>: must be <b>Enabled</b> when <b>Network Authentication</b> is <b>%s</b><br>", auth_mode);
			return;
	    	}
	}
	
	nvram_set(name, value);

	ret_code = 0;
}

static void
validate_wl_crypto(webs_t wp, char *value, struct variable *v, char *varname)
{
	
	assert(v);
	
	ret_code = EINVAL;
	
	if (!valid_choice(wp, value, v)){
		websBufferWrite(wp, "Invalid crypto config string <b>%s</b>",value);
		return;
	}

	if (varname)
		nvram_set(varname, value);
	else
		nvram_set(v->name, value);
		
	ret_code = 0;
}

#ifdef __CONFIG_NAT__
static void
validate_wan_ifname(webs_t wp, char *value, struct variable *v, char *varname)
{
	char ifname[64], *next=NULL;
	
	assert(v);
	
	ret_code = EINVAL;
	foreach (ifname, nvram_safe_get("wan_ifnames"), next)
		if (!strcmp(ifname, value)) {	
			nvram_set(v->name, value);
			ret_code = 0;
			return;
		}
	websBufferWrite(wp, "Invalid <b>%s</b>: must be one of <b>%s</b><br>", v->longname, nvram_safe_get("wan_ifnames"));
}
#endif	/* __CONFIG_NAT__ */

static void
validate_wl_afterburner(webs_t wp, char *value, struct variable *v, char *varname)
{

	assert(v);
	
	ret_code = EINVAL;
	
	if (!valid_choice(wp, value, v)){
		websBufferWrite(wp, "Invalid  afterburner config string <b>%s</b>",value);
		return;
	}

	/* force certain wireless variables to fixed values */
	if (!strcmp(value, "auto")) {
		if ( nvram_invmatch("wl_mode", "ap"))  {
			/* notify the user */
			websBufferWrite(wp, "Invalid <b>%s</b>: AfterBurner mode requires:"
				"<br><b>Mode</b> set to <b>Access Point</b>"
				"<br><b>Fragmentation value</b> set to <b>2346(disable fragmentation)</b>"
				"<br><b>AfterBurner mode is disabled!</b>"
				"<br>", v->longname);
			return;
		}
	}

	if (varname)
		nvram_set(varname, value);
	else	
		nvram_set(v->name, value);
		
	ret_code = 0;
}

static void
validate_wl_lazywds(webs_t wp, char *value, struct variable *v, char *varname)
{
	assert(v);
	
	ret_code = EINVAL;
	validate_choice(wp, value, v, varname);
}
	
static void
validate_wl_wds_hwaddrs(webs_t wp, char *value, struct variable *v, char *varname)
{
	assert(v);
	
	ret_code = EINVAL;
	validate_list(wp, value, v, valid_hwaddr, varname);
}

static void
validate_wl_mode(webs_t wp, char *value, struct variable *v, char *varname)
{
	char *afterburner=NULL;
	
	assert(v);
	
	ret_code = EINVAL;

	if (strcmp(value, "ap") &&
	    (afterburner = websGetVar(wp, "wl_afterburner", NULL)) && (!strcmp(afterburner, "auto"))) {
		websBufferWrite(wp, "Invalid <b>%s</b>: must be set to <b>Access Point</b> when AfterBurner is enabled.<br>", v->longname);
		return;
	}
	validate_choice(wp, value, v, varname);
}

static void
validate_noack(webs_t wp, char *value, struct variable *v,char *varname)
{
	char *wme=NULL;

	assert(v);
	
	ret_code = EINVAL;
	
	/* return if wme is not enabled */
	if (!(wme = websGetVar(wp, "wl_wme", NULL))){
		ret_code = 0;
		return;
	}else if (strcmp(wme, "on")){
		ret_code = 0;
		return;
	}

	validate_choice(wp, value, v, varname);
}

static void
validate_wl_wme_params(webs_t wp, char *value, struct variable *v,char *varname)
{
	int n, i;
	int cwmin = 0, cwmax = 0;
	char *wme=NULL, *afterburner=NULL;
	char name[100];
	char buf[1000] = "", *cur = buf;
	struct {
		char *name;
		int range;
		char *arg1;
		char *arg2;
	} field_attrib[] = {
		{ "WME AC CWmin", 1, "0", "32767" },
		{ "WME AC CWmax", 1, "0", "32767" },
		{ "WME AC AIFSN", 1, "1", "15" },
		{ "WME AC TXOP(b)", 1, "0", "65504" },
		{ "WME AC TXOP(a/g)", 1, "0", "65504" },
		{ "WME AC Admin Forced", 0, "on", "off" }
	};

	assert(v);
	
	ret_code = EINVAL;
	
	/* return if wme is not enabled */
	if (!(wme = websGetVar(wp, "wl_wme", NULL))){
		ret_code =0;
		return;
	} else if (strcmp(wme, "on")){
		ret_code = 0;
		return;
	}

	/* return if afterburner enabled */
	if ((afterburner = websGetVar(wp, "wl_afterburner", NULL)) && (!strcmp(afterburner, "auto"))){
		ret_code =0;
		return;
	}

	n = atoi(value) + 1;

	for (i = 0; i < n; i++) {
		snprintf(name, sizeof(name), "%s%d", v->name, i);
		if (!(value = websGetVar(wp, name, NULL))){
			ret_code = 0;
			return;
		}
		
		if (!*value && v->nullok)
			continue;

		if (i == 0)
			cwmin = atoi(value);
		else if (i == 1) {
			cwmax = atoi(value);
			if (cwmax < cwmin) {
				websBufferWrite(wp, "Invalid <b>%s</b> %d: greater than <b>%s</b> %d<br>",
					field_attrib[0].name, cwmin, field_attrib[i].name, cwmax);
				return;
			}
		}
		if (field_attrib[i].range) {
			if (atoi(value) < atoi(field_attrib[i].arg1) || atoi(value) > atoi(field_attrib[i].arg2)) {
				websBufferWrite(wp, "Invalid <b>%s</b> %d: should be in range %s to %s<br>",
					field_attrib[i].name, atoi(value), field_attrib[i].arg1, field_attrib[i].arg2);
				return;
			}
		} else {
			if (strcmp(value, field_attrib[i].arg1) && strcmp(value, field_attrib[i].arg2)) {
				websBufferWrite(wp, "Invalid <b>%s</b> %s: should be %s or %s<br>",
					field_attrib[i].name, value, field_attrib[i].arg1, field_attrib[i].arg2);
			}
		}

		cur += snprintf(cur, buf + sizeof(buf) - cur, "%s%s",
				cur == buf ? "" : " ", value);
	}
	
	if (varname)
		nvram_set(varname, buf);
	else
		nvram_set(v->name, buf);
		
	ret_code = 0;
}
	
/* Hook to write wl_* default set through to wl%d_* variable set */
static void
wl_unit(webs_t wp, char *value, struct variable *v, char *varname)
{
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";

	assert(v);
	
	ret_code = 0;
	
	if (!value) return;
	
	/* The unit numbers are built dynamically so what is 
	   present is assumed to be running */ 
	   
	snprintf(prefix,sizeof(prefix),"wl%s_",value);

	/* Write through to selected variable set */
	for (; v >= variables && !strncmp(v->name, "wl_", 3); v--){
		if ( v->ezc_flags & WEB_IGNORE)
			continue;
		nvram_set(strcat_r(prefix, &v->name[3], tmp), nvram_safe_get(v->name));
	}
}

#ifdef __CONFIG_NAT__
static void
wan_primary(webs_t wp)
{
	char tmp[NVRAM_BUFSIZE], prefix[] = "wanXXXXXXXXXX_";
	int i;
	
	for (i = 0; i < MAX_NVPARSE; i ++) {
		/* skip non-exist and disabled connection */
		wan_prefix(i, prefix);
		if (!nvram_get(strcat_r(prefix, "unit", tmp))||
		    nvram_match(strcat_r(prefix, "proto", tmp), "disabled"))
			continue;
		/* make connection <i> primary */
		nvram_set(strcat_r(prefix, "primary", tmp), "1");
		/* notify the user */
		websBufferWrite(wp, "<br><b>%s</b> is set to primary.",
			wan_name(i, prefix, tmp, sizeof(tmp)));
		break;
	}
}

/* Hook to write wan_* default set through to wan%d_* variable set */
static void
wan_unit(webs_t wp, char *value, struct variable *v, char *varname)
{
	char tmp[NVRAM_BUFSIZE], prefix[] = "wanXXXXXXXXXX_";
	char pppx[] = "pppXXXXXXXXXXX";
	int unit, i;
	char *wan_ifname=NULL;
	int wan_disabled = 0;
	int wan_prim = 0;
	int wan_wildcard = 0;
	char *wan_pppoe_service=NULL;
	char *wan_pppoe_ac=NULL;
	char wan_tmp[NVRAM_BUFSIZE];
	int wildcard;
	char *pppoe_service=NULL;
	char *pppoe_ac=NULL;
	char ea[ETHER_ADDR_LEN], wan_ea[ETHER_ADDR_LEN];

	assert(v);
	ret_code = 0;
	
	/* Do not write through if no connections are present */
	if ((unit = atoi(value)) < 0)
		return;

	/* override wan_pppoe_ifname */
	if (nvram_match("wan_proto", "pppoe")) {
		snprintf(pppx, sizeof(pppx), "ppp%d", unit);
		nvram_set("wan_pppoe_ifname", pppx);
	}

	/*
	* Need to make sure this connection can co-exist with others.
	* Disable others if it can't (assuming this is the wanted one).
	* Disabled connection is for sure no problem to co-exist with
	* other connections.
	*/
	if (nvram_match("wan_proto", "disabled")) {
		/* Non primary always go with disabled connection. */
		nvram_set("wan_primary", "0");
		wan_disabled = 1;
	}
	/*
	* PPPoE connection is for sure no problem to co-exist with
	* other PPPoE connections even when they share the same 
	* ethernet interface, but we need to make sure certain 
	* PPPoE parameters are reasonablely different from eatch other 
	* if they share the same ethernet interface.
	*/
	else if (nvram_match("wan_proto", "pppoe")) {
		/* must disable others if this connection is wildcard (any service any ac) */
		wan_pppoe_service = nvram_get("wan_pppoe_service");
		wan_pppoe_ac = nvram_get("wan_pppoe_ac");
		wan_wildcard = (wan_pppoe_service == NULL || *wan_pppoe_service == 0) &&
			(wan_pppoe_ac == NULL || *wan_pppoe_ac == 0);
		wan_ifname = nvram_safe_get("wan_ifname");
		wan_name(unit, "wan_", wan_tmp, sizeof(wan_tmp));
		/* check all PPPoE connections that share the same interface */
		for (i = 0; i < MAX_NVPARSE; i ++) {
			/* skip the current connection */
			if (i == unit)
				continue;
			/* skip non-exist and connection that does not share the same i/f */
			wan_prefix(i, prefix);
			if (!nvram_get(strcat_r(prefix, "unit", tmp)) ||
			    nvram_match(strcat_r(prefix, "proto", tmp), "disabled") ||
			    nvram_invmatch(strcat_r(prefix, "ifname", tmp), wan_ifname))
				continue;
			/* PPPoE can share the same i/f, but none can be wildcard */
			if (nvram_match(strcat_r(prefix, "proto", tmp), "pppoe")) {
				if (wan_wildcard) {
					/* disable connection <i> */
					nvram_set(strcat_r(prefix, "proto", tmp), "disabled");
					nvram_set(strcat_r(prefix, "primary", tmp), "0");
					/* notify the user */
					websBufferWrite(wp, "<br><b>%s</b> is <b>disabled</b> because both "
						"<b>PPPoE Service Name</b> and <b>PPPoE Access Concentrator</b> "
						"in <b>%s</b> are empty.",
						wan_name(i, prefix, tmp, sizeof(tmp)), wan_tmp);
				}
				else {
					pppoe_service = nvram_get(strcat_r(prefix, "pppoe_service", tmp));
					pppoe_ac = nvram_get(strcat_r(prefix, "pppoe_ac", tmp));
					wildcard = (pppoe_service == NULL || *pppoe_service == 0) &&
						(pppoe_ac == NULL || *pppoe_ac == 0);
					/* allow connection <i> if certain pppoe parameters are not all same */
					if (!wildcard &&
					    (nvram_invmatch(strcat_r(prefix, "pppoe_service", tmp), nvram_safe_get("wan_pppoe_service")) ||
					         nvram_invmatch(strcat_r(prefix, "pppoe_ac", tmp), nvram_safe_get("wan_pppoe_ac"))))
						continue;
					/* disable connection <i> */
					nvram_set(strcat_r(prefix, "proto", tmp), "disabled");
					nvram_set(strcat_r(prefix, "primary", tmp), "0");
					/* notify the user */
					websBufferWrite(wp, "<br><b>%s</b> is <b>disabled</b> because both its "
						"<b>PPPoE Service Name</b> and <b>PPPoE Access Concentrator</b> "
						"are empty.",
						wan_name(i, prefix, tmp, sizeof(tmp)));
				}
			}
			/* other types can't (?) share the same i/f with PPPoE */
			else {
				/* disable connection <i> */
				nvram_set(strcat_r(prefix, "proto", tmp), "disabled");
				nvram_set(strcat_r(prefix, "primary", tmp), "0");
				/* notify the user */
				websBufferWrite(wp, "<br><b>%s</b> is <b>disabled</b> because it can't  "
					"share the same interface with <b>%s</b>.",
					wan_name(i, prefix, tmp, sizeof(tmp)), wan_tmp);
			}
		}
	}
	/* 
	* All other types (now DHCP, Static) can't co-exist with 
	* other connections if they use the same ethernet i/f.
	*/
	else {
		wan_ifname = nvram_safe_get("wan_ifname");
		wan_name(unit, "wan_", wan_tmp, sizeof(wan_tmp));
		/* check all connections that share the same interface */
		for (i = 0; i < MAX_NVPARSE; i ++) {
			/* skip the current connection */
			if (i == unit)
				continue;
			/* check if connection <i> exists and share the same i/f*/
			wan_prefix(i, prefix);
			if (!nvram_get(strcat_r(prefix, "unit", tmp)) ||
			    nvram_match(strcat_r(prefix, "proto", tmp), "disabled") ||
			    nvram_invmatch(strcat_r(prefix, "ifname", tmp), wan_ifname))
				continue;
			/* disable connection <i> */
			nvram_set(strcat_r(prefix, "proto", tmp), "disabled");
			nvram_set(strcat_r(prefix, "primary", tmp), "0");
			/* notify the user */
			websBufferWrite(wp, "<br><b>%s</b> is disabled because it can't share "
				"the ethernet interface with <b>%s</b>.",
				wan_name(i, prefix, tmp, sizeof(tmp)), wan_tmp);
		}
	}

	/*
	* Check if MAC address has been changed. Need to sync it to all connections
	* that share the same i/f if it is changed.
	*/
	wan_prefix(unit, prefix);
	ether_atoe(nvram_safe_get("wan_hwaddr"), wan_ea);
	ether_atoe(nvram_safe_get(strcat_r(prefix, "hwaddr", tmp)), ea);
	if (memcmp(ea, wan_ea, ETHER_ADDR_LEN)) {
		wan_ifname = nvram_safe_get("wan_ifname");
		wan_name(unit, "wan_", wan_tmp, sizeof(wan_tmp));
		/* sync all connections that share the same interface */
		for (i = 0; i < MAX_NVPARSE; i ++) {
			/* skip the current connection */
			if (i == unit)
				continue;
			/* check if connection <i> exists and share the same i/f*/
			wan_prefix(i, prefix);
			if (!nvram_get(strcat_r(prefix, "unit", tmp)) ||
			    nvram_invmatch(strcat_r(prefix, "ifname", tmp), wan_ifname))
				continue;
			/* check if connection <i>'s hardware address is different */
			if (ether_atoe(nvram_safe_get(strcat_r(prefix, "hwaddr", tmp)), ea) &&
			    !memcmp(ea, wan_ea, ETHER_ADDR_LEN))
			    continue;
			/* change connection <i>'s hardware address */
			nvram_set(strcat_r(prefix, "hwaddr", tmp), nvram_safe_get("wan_hwaddr"));
			/* notify the user */
			websBufferWrite(wp, "<br><b>MAC Address</b> in <b>%s</b> is changed to "
				"<b>%s</b> because it shares the ethernet interface with <b>%s</b>.",
				wan_name(i, prefix, tmp, sizeof(tmp)), nvram_safe_get("wan_hwaddr"),
				wan_tmp);
		}
	}

	/* Set prefix */
	wan_prefix(unit, prefix);

	/* Write through to selected variable set */
	for (; v >= variables && !strncmp(v->name, "wan_", 4); v--){
		if (v->ezc_flags & WEB_IGNORE)
			continue;
		nvram_set(strcat_r(prefix, &v->name[4], tmp), nvram_safe_get(v->name));
	}

	/*
	* There must be one and only one primary connection among all 
	* enabled connections so that traffic can be routed by default 
	* through the primary connection unless they are targetted to
	* a specific connection by means of static routes. (Primary ~=
	* Default Gateway).
	*/
	/* the current connection is primary, set others to non-primary */
	if (!wan_disabled && nvram_match(strcat_r(prefix, "primary", tmp), "1")) {
		/* set other connections to non-primary */
		for (i = 0; i < MAX_NVPARSE; i ++) {
			/* skip the current connection */
			if (i == unit)
				continue;
			/* skip non-exist and disabled connection */
			wan_prefix(i, prefix);
			if (!nvram_get(strcat_r(prefix, "unit", tmp)) ||
			    nvram_match(strcat_r(prefix, "proto", tmp), "disabled"))
				continue;
			/* skip non-primary connection */
			if (nvram_invmatch(strcat_r(prefix, "primary", tmp), "1"))
				continue;
			/* force primary to non-primary */
			nvram_set(strcat_r(prefix, "primary", tmp), "0");
			/* notify the user */
			websBufferWrite(wp, "<br><b>%s</b> is set to non-primary.",
				wan_name(i, prefix, tmp, sizeof(tmp)));
		}
		wan_prim = 1;
	}
	/* the current connection is not parimary, check if there is any primary */
	else {
		/* check other connections to see if there is any primary */
		for (i = 0; i < MAX_NVPARSE; i ++) {
			/* skip the current connection */
			if (i == unit)
				continue;
			/* primary connection exists, honor it */
			wan_prefix(i, prefix);
			if (nvram_match(strcat_r(prefix, "primary", tmp), "1")) {
				wan_prim = 1;
				break;
			}
		}
	}
	/* no one is primary, pick the first enabled one as primary */
	if (!wan_prim)
		wan_primary(wp);
}
#endif	/* __CONFIG_NAT__ */

/* This is the monster V-block
 * 
 * It controls the following functions
 *
 * Configuration variables are validated and saved in NVRAM
 * Variables saved by NVRAM save/restore routine
 * Method in which the variables are to be validates
 * SES handling
 *
 * The control flags:
 * EZC_FLAGS_READ,EZC_FLAGS_WRITE :Ses read/write flags
 * NVRAM_ENCRYPT: Encrypt variable prior to downloading NVRAM variable to file
 * NVRAM_MI: Multi instance NVRAM variable eg wlXX,wanXX
 * NVRAM_VLAN_MULTI: Special flag to handle oddball vlanXXname and its cousins
 * NVRAM_MP: Variable can be single & multi instance eg lan_ifname, lanX_ifname
 * NVRAM_IGNORE: Dont save/restore this var.
 * WEB_IGNORE: Don't validate or process this var during web validation.
 * 
 * Below is the definition of the structure
 *struct variable {
 *	char *name; <- name of variable
 * 	char *longname; <- display name
 * 	char *prefix; <- prefix for processing the multi-instance versions
 *	void (*validate)(); <- Validation routine
 * 	char **argv; <- Optional argument vector for validation routine
 *	int nullok; <- value can be NULL
 *	int ezc_flags; <- control flags
 * };
 *
 * IMPORTANT:
 * =========
 *
 * The variables in tne table below determine if they will be saved/restored 
 * by the UI NVRAM save/restore feature
 *
 * If an NVRAM variable is not present in this list it will
 * not be processed and thus will not be saved or restored.
 *
*/

/* 
 * Variables are set in order (put dependent variables later). Set
 * nullok to TRUE to ignore zero-length values of the variable itself.
 * For more complicated validation that cannot be done in one pass or
 * depends on additional form components or can throw an error in a
 * unique painful way, write your own validation routine and assign it
 * to a hidden variable (e.g. filter_ip).
 *
 * EZC_FLAGS_READ : implies the variable will be returned for ezconfig read request
 * EZC_FLAGS_WRITE : allows the variable to be modified by the ezconfig tool
 *
 * The variables marked with EZConfig have to maintain backward compatibility.
 * If they cannot then the ezc_version has to be bumped up
 *
 */
static char wl_prefix[]="wl";
static char wan_prefix[]="wan";
static char lan_prefix[]="lan";
static char dhcp_prefix[]="dhcp";
static char vlan_prefix[]="vlan";

static struct hsearch_data vtab;

struct variable variables[] = {
	/* basic settings */
	{ "http_username", "Router Username",NULL, validate_name, ARGV("0", "63"), TRUE, EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "http_passwd", "Router Password",NULL, validate_name, ARGV("0", "63"), TRUE, NVRAM_ENCRYPT | EZC_FLAGS_WRITE },
	{ "http_wanport", "Router WAN Port",NULL, validate_range, ARGV("0", "65535"), TRUE, 0 },
	{ "http_lanport", "HTTP daemon lanport",NULL, NULL, NULL, TRUE, 0 },
	{ "router_disable", "Router Mode",NULL, validate_router_disable, ARGV("0", "1"), FALSE, 0 },
	{ "fw_disable", "Firewall",NULL, validate_choice, ARGV("0", "1"), FALSE, EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "time_zone", "Time Zone",NULL, validate_choice, ARGV("PST8PDT", "MST7MDT", "CST6CDT", "EST5EDT"), FALSE },
	{ "upnp_enable", "UPnP",NULL, validate_choice, ARGV("0", "1"), FALSE, 0 },
	{ "ezc_enable", "EZConfig",NULL, validate_choice, ARGV("0", "1"), FALSE, EZC_FLAGS_READ | EZC_FLAGS_WRITE},
	{ "ntp_server", "NTP Servers",NULL, validate_ipaddrs, NULL, TRUE, 0 },
	{ "log_level", "Connection Logging",NULL, validate_range, ARGV("0", "3"), FALSE, 0 },
	{ "log_ipaddr", "Log LAN IP Address",NULL, validate_ipaddr, ARGV("lan_ipaddr", "lan_netmask"), TRUE, 0 },
	{ "log_ram_enable", "Syslog in RAM",NULL, validate_choice, ARGV("0", "1"), FALSE, 0 },
	/* LAN settings */
	{ "lan_ifname" "LAN Interface Name", lan_prefix, NULL, NULL, FALSE, NVRAM_MI|NVRAM_IGNORE },
	{ "lan_dhcp", "DHCP Client", lan_prefix, validate_choice, ARGV("1", "0"), FALSE, NVRAM_MI | NVRAM_MP },
	{ "lan_ipaddr", "IP Address", lan_prefix, validate_lan_ipaddr, NULL, FALSE, NVRAM_MI| NVRAM_MP | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "lan_netmask", "Subnet Mask", lan_prefix, validate_ipaddr, NULL, TRUE, NVRAM_MI| NVRAM_MP },
	{ "lan_gateway", "Gateway Address", lan_prefix, validate_ipaddr, NULL, TRUE, NVRAM_MI| NVRAM_MP },
	{ "lan_proto", "DHCP Server", lan_prefix, validate_choice, ARGV("dhcp", "static"), FALSE, NVRAM_MI| NVRAM_MP | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "dhcp_start", "DHCP Server LAN IP Address Range", dhcp_prefix, validate_dhcp, NULL, FALSE, NVRAM_MI| NVRAM_MP | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "dhcp_end", "DHCP Server LAN IP End Address", dhcp_prefix, validate_ipaddr, NULL, TRUE, NVRAM_MI| NVRAM_MP },
	{ "dhcp_wins", "DHCP WINS domain", dhcp_prefix, NULL, NULL, TRUE, NVRAM_MI | NVRAM_MP},
	{ "dhcp_domain", "DHCP domain", dhcp_prefix, NULL, NULL, TRUE, NVRAM_MI | NVRAM_MP },
	{ "lan_lease", "DHCP Server Lease Time", lan_prefix, validate_range, ARGV("1", "604800"), FALSE,  NVRAM_MI | NVRAM_MP  |EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "lan_stp", "Spanning Tree Protocol", lan_prefix, validate_choice, ARGV("0", "1"), FALSE, NVRAM_MI | NVRAM_MP| EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "lan_route", "Static Routes", lan_prefix, validate_lan_route, NULL, FALSE,  NVRAM_MI | NVRAM_MP },
	{ "lan_wins", "Lan WINS", lan_prefix, NULL, NULL, TRUE, NVRAM_MI| NVRAM_MP},
	{ "lan_domain", "Lan Domain", lan_prefix,NULL, NULL, TRUE,  NVRAM_MI| NVRAM_MP },
	{ "lan_route","LAN route", lan_prefix, NULL, NULL, TRUE,  NVRAM_MI| NVRAM_MP },
	{ "lan_guest_ifname","Guest LAN 1", lan_prefix, validate_guest_lan_ifname, ARGV("1"), FALSE,  NVRAM_MI| NVRAM_IGNORE},
	/*VLAN config vars. These are multi-instance Used by NVRAM save/restore for the moment */
	{ "vlanhwname", "VLAN HW name", vlan_prefix, NULL, NULL, TRUE, NVRAM_VLAN_MULTI},
	{ "vlanports", "VLAN Ports", vlan_prefix, NULL, NULL, TRUE, NVRAM_VLAN_MULTI},
#ifdef __CONFIG_NAT__
	/* ALL wan_XXXX variables below till wan_unit variable are per-interface */
	{ "wan_desc", "Description", wan_prefix, validate_name, ARGV("0", "255"), TRUE, NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_proto", "Protocol", wan_prefix, validate_choice, ARGV("dhcp", "static", "pppoe", "disabled"), FALSE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_hostname", "Host Name", wan_prefix, validate_name, ARGV("0", "255"), TRUE, NVRAM_MI |EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_domain", "Domain Name", wan_prefix, validate_name, ARGV("0", "255"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_ifname", "Interface Name", wan_prefix, validate_wan_ifname, NULL, TRUE, NVRAM_MI  },
	{ "wan_hwaddr", "MAC Address", wan_prefix, validate_hwaddr, NULL, TRUE, NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_ipaddr", "IP Address", wan_prefix, validate_ipaddr, NULL, FALSE, NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_netmask", "Subnet Mask", wan_prefix, validate_ipaddr, NULL, FALSE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_gateway", "Default Gateway", wan_prefix, validate_ipaddr, NULL, TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_dns", "DNS Servers", wan_prefix, validate_ipaddrs, NULL, TRUE, NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_wins", "WINS Servers", wan_prefix, validate_ipaddrs, NULL, TRUE, NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pppoe_ifname", "PPPoE Interface Name", wan_prefix, NULL, NULL, TRUE, NVRAM_MI  },
	{ "wan_pppoe_username", "PPPoE Username", wan_prefix, validate_name, ARGV("0", "255"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pppoe_passwd", "PPPoE Password", wan_prefix, validate_name, ARGV("0", "255"), TRUE,NVRAM_MI | EZC_FLAGS_WRITE },
	{ "wan_pppoe_service", "PPPoE Service Name", wan_prefix, validate_name, ARGV("0", "255"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pppoe_ac", "PPPoE Access Concentrator", wan_prefix, validate_name, ARGV("0", "255"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pppoe_keepalive", "PPPoE Keep Alive", wan_prefix, validate_choice, ARGV("0", "1"), FALSE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pppoe_demand", "PPPoE Connect on Demand", wan_prefix, validate_choice, ARGV("0", "1"), FALSE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pppoe_idletime", "PPPoE Max Idle Time", wan_prefix, validate_range, ARGV("1", "3600"), TRUE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pppoe_mru", "PPPoE MRU", wan_prefix, validate_range, ARGV("128", "16384"), FALSE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_pppoe_mtu", "PPPoE MTU", wan_prefix, validate_range, ARGV("128", "16384"), FALSE,NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wan_primary", "Primary Interface", wan_prefix, validate_choice, ARGV("0", "1"), FALSE,NVRAM_MI },
	{ "wan_route", "Static Routes", wan_prefix, validate_wan_route, NULL, FALSE, NVRAM_MI  },
	/* MUST leave this entry here after all wl_XXXX per-interface variables */
	{ "wan_unit", "WAN Instance", wan_prefix, wan_unit, NULL, TRUE, NVRAM_MI|NVRAM_MP },
	/* filter settings */
	{ "filter_macmode", "MAC Filter Mode", NULL, validate_choice, ARGV("disabled", "allow", "deny"), FALSE, 0 },
	{ "filter_maclist", "MAC Filter", NULL, validate_hwaddrs, NULL, TRUE, 0 },
	{ "filter_client", "LAN Client Filter", NULL, validate_filter_client, ARGV("0", XSTR(MAX_NVPARSE - 1)), FALSE, NVRAM_GENERIC_MULTI },
	/* routing settings */
	{ "forward_port", "Port Forward", NULL, validate_forward_port, ARGV("0", XSTR(MAX_NVPARSE - 1)), FALSE, NVRAM_GENERIC_MULTI },
	{ "autofw_port", "Application Specific Port Forward", NULL, validate_autofw_port, ARGV("0", XSTR(MAX_NVPARSE - 1)), FALSE, NVRAM_GENERIC_MULTI },
	{ "dmz_ipaddr", "DMZ LAN IP Address", NULL, validate_ipaddr, ARGV("lan_ipaddr", "lan_netmask"), TRUE, 0 },
#endif	/* __CONFIG_NAT__ */
	/* ALL wl_XXXX variables are per-interface */
	{ "wl_ssid", "Network Name (ESSID)", wl_prefix, validate_name, ARGV("1", "32"), FALSE, NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_guest_ssid_1", "Guest Network Name 1(SSID)", wl_prefix, validate_guest_ssid, ARGV("1"), FALSE, NVRAM_MI|NVRAM_IGNORE },
	{ "wl_guest_ssid_2", "Guest Network Name 2(SSID)", wl_prefix, validate_guest_ssid, ARGV("2"), FALSE, NVRAM_MI|NVRAM_IGNORE },
	{ "wl_guest_ssid_3", "Guest Network Name 3(SSID)", wl_prefix, validate_guest_ssid, ARGV("3"), FALSE, NVRAM_MI|NVRAM_IGNORE },
	{ "wl_guest_ssid_4", "Guest Network Name 4(SSID)", wl_prefix, validate_guest_ssid, ARGV("4"), FALSE, NVRAM_MI|NVRAM_IGNORE },
	{ "wl_ure", "URE Mode",NULL, validate_ure, ARGV("0"), FALSE, NVRAM_MI|NVRAM_IGNORE },
	{ "wl_closed", "Network Type", wl_prefix, validate_choice, ARGV("0", "1"), FALSE,NVRAM_MI |  EZC_FLAGS_READ | EZC_FLAGS_WRITE },
        { "wl_ap_isolate", "AP Isolate", wl_prefix, validate_choice, ARGV("0", "1"), FALSE,NVRAM_MI |  0 },
	{ "wl_country_code", "Country Code", wl_prefix, validate_country, NULL, FALSE, NVRAM_MI |  EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_mode", "Mode", wl_prefix, validate_wl_mode, ARGV("ap", "wds", "sta", "wet", "apsta"), FALSE,NVRAM_MI },
	{ "wl_infra", "Network", wl_prefix, validate_choice, ARGV("0", "1"), FALSE, NVRAM_MI },
	{ "wl_lazywds", "Bridge Restrict", wl_prefix, validate_wl_lazywds, ARGV("0", "1"), FALSE,NVRAM_MI },
	{ "wl_wds", "Bridges", wl_prefix, validate_wl_wds_hwaddrs, NULL, TRUE, NVRAM_MI },
	{ "wl_wds_timeout", "Link Timeout Interval", wl_prefix, NULL, NULL, TRUE, NVRAM_MI },
	{ "wl_macmode", "MAC Restrict Mode", wl_prefix, validate_choice, ARGV("disabled", "allow", "deny"), FALSE,NVRAM_MI },
	{ "wl_maclist", "Allowed MAC Address", wl_prefix, validate_hwaddrs, NULL, TRUE, NVRAM_MI },
	{ "wl_radio", "Radio Enable", wl_prefix, validate_choice, ARGV("0", "1"), FALSE, NVRAM_MI },
	{ "wl_phytype", "Radio Band", wl_prefix, validate_choice, ARGV("a", "b", "g"), TRUE, NVRAM_MI },
	{ "wl_antdiv", "Antenna Diversity", wl_prefix, validate_choice, ARGV("-1", "0", "1", "3"), FALSE, NVRAM_MI },
	/* Channel and rate are fixed in wlconf() if incorrect */
	{ "wl_channel", "Channel", wl_prefix, validate_range, ARGV("0", "216"), FALSE, NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_reg_mode", "Regulatory Mode", wl_prefix, validate_choice, ARGV("off", "h", "d"), FALSE, NVRAM_MI },
	{ "wl_dfs_preism", "Pre-Network Radar Check", wl_prefix, validate_range, ARGV("0", "99"), FALSE,NVRAM_MI },
	{ "wl_dfs_postism", "In Network Radar Check", wl_prefix, validate_range, ARGV("10", "99"), FALSE,NVRAM_MI },
	{ "wl_tpc_db", "TPC Mitigation (db)",  wl_prefix, validate_range, ARGV("0", "99"), FALSE,NVRAM_MI },
	{ "wl_rate", "Rate", wl_prefix, validate_range, ARGV("0", "54000000"), FALSE,NVRAM_MI },
	{ "wl_rateset", "Supported Rates", wl_prefix, validate_choice, ARGV("all", "default", "12"), FALSE,NVRAM_MI },
	{ "wl_mrate", "Multicast Rate", wl_prefix, validate_range, ARGV("0", "54000000"), FALSE,NVRAM_MI },
	{ "wl_frag", "Fragmentation Threshold", wl_prefix, validate_range, ARGV("256", "2346"), FALSE,NVRAM_MI },
	{ "wl_rts", "RTS Threshold", wl_prefix, validate_range, ARGV("0", "2347"), FALSE, NVRAM_MI },
	{ "wl_dtim", "DTIM Period", wl_prefix, validate_range, ARGV("1", "255"), FALSE, NVRAM_MI },
	{ "wl_bcn", "Beacon Interval", wl_prefix, validate_range, ARGV("1", "65535"), FALSE, NVRAM_MI },
	{ "wl_plcphdr", "Preamble Type", wl_prefix, validate_choice, ARGV("long", "short"), FALSE, NVRAM_MI },
	{ "wl_maxassoc", "Max Assocation Limit", wl_prefix, validate_range, ARGV("1", "256"), FALSE,NVRAM_MI },
	{ "wl_gmode", "54g Mode", wl_prefix, validate_choice, ARGV(XSTR(GMODE_AUTO), XSTR(GMODE_ONLY), XSTR(GMODE_PERFORMANCE), XSTR(GMODE_LRS), XSTR(GMODE_LEGACY_B)), FALSE,NVRAM_MI |  0 },
	{ "wl_gmode_protection", "54g Protection", wl_prefix, validate_choice, ARGV("off", "auto"), FALSE,NVRAM_MI },
	{ "wl_frameburst", "XPress Technology", wl_prefix, validate_choice, ARGV("off", "on"), FALSE,NVRAM_MI |  EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_afterburner", "AfterBurner Technology", wl_prefix, validate_wl_afterburner, ARGV("off", "auto"), FALSE,NVRAM_MI |  EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_wme", "WME Support", wl_prefix, validate_choice, ARGV("off", "on"), FALSE, NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_wme_no_ack", "No-Acknowledgement", wl_prefix, validate_noack, ARGV("off", "on"), FALSE,NVRAM_MI |  EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_wme_ap_bk", "WME AP BK", wl_prefix, validate_wl_wme_params, NULL, TRUE, NVRAM_MI },
	{ "wl_wme_ap_be", "WME AP BE", wl_prefix, validate_wl_wme_params, NULL, TRUE, NVRAM_MI },
	{ "wl_wme_ap_vi", "WME AP VI", wl_prefix, validate_wl_wme_params, NULL, TRUE, NVRAM_MI },
	{ "wl_wme_ap_vo", "WME AP VO", wl_prefix, validate_wl_wme_params, NULL, TRUE, NVRAM_MI },
	{ "wl_wme_sta_bk", "WME STA BK", wl_prefix, validate_wl_wme_params, NULL, TRUE,NVRAM_MI },
	{ "wl_wme_sta_be", "WME STA BE", wl_prefix, validate_wl_wme_params, NULL, TRUE, NVRAM_MI },
	{ "wl_wme_sta_vi", "WME STA VI", wl_prefix, validate_wl_wme_params, NULL, TRUE, NVRAM_MI },
	{ "wl_wme_sta_vo", "WME STA VO", wl_prefix, validate_wl_wme_params, NULL, TRUE, NVRAM_MI },
	/* security parameters */
	{ "wl_key", "Network Key Index", wl_prefix, validate_range, ARGV("1", "4"), FALSE,NVRAM_MI | EZC_FLAGS_WRITE },
	{ "wl_key1", "Network Key 1", wl_prefix, validate_wl_key, NULL, TRUE, NVRAM_MI | NVRAM_ENCRYPT | EZC_FLAGS_WRITE},
	{ "wl_key2", "Network Key 2", wl_prefix, validate_wl_key, NULL, TRUE, NVRAM_MI | NVRAM_ENCRYPT | EZC_FLAGS_WRITE},
	{ "wl_key3", "Network Key 3", wl_prefix, validate_wl_key, NULL, TRUE,NVRAM_MI |  NVRAM_ENCRYPT | EZC_FLAGS_WRITE},
	{ "wl_key4", "Network Key 4", wl_prefix, validate_wl_key, NULL, TRUE,NVRAM_MI | NVRAM_ENCRYPT |  EZC_FLAGS_WRITE},
	{ "wl_auth", "802.11 Authentication", wl_prefix, validate_wl_auth, ARGV("0", "1"), FALSE, NVRAM_MI | 0},
	{ "wl_auth_mode", "Network Authentication", wl_prefix, validate_wl_auth_mode, ARGV("radius", "none"), FALSE, NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_akm", "Authenticated Key Management", wl_prefix, validate_wl_akm, NULL, FALSE, NVRAM_MI |  EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_wep", "WEP Encryption", wl_prefix, validate_wl_wep, ARGV("disabled", "enabled"), FALSE, NVRAM_MI |  EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_crypto", "WPA Encryption", wl_prefix, validate_wl_crypto, ARGV("tkip", "aes", "tkip+aes"), FALSE, NVRAM_MI |  EZC_FLAGS_READ | EZC_FLAGS_WRITE },
#ifdef BCMWPA2
	{ "wl_net_reauth", "Network Re-auth Interval", wl_prefix, NULL, NULL, TRUE, NVRAM_MI  },
	{ "wl_preauth", "Network Preauthentication Support", wl_prefix, validate_wl_preauth, ARGV("disabled", "enabled"), FALSE,NVRAM_MI |  0 },
#endif
	{ "wl_radius_ipaddr", "RADIUS Server", wl_prefix, validate_ipaddr, NULL, TRUE, NVRAM_MI | EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_radius_port", "RADIUS Port", wl_prefix, validate_range, ARGV("0", "65535"), FALSE,NVRAM_MI |  EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	{ "wl_radius_key", "RADIUS Shared Secret", wl_prefix, validate_name, ARGV("0", "255"), TRUE, NVRAM_MI |  NVRAM_ENCRYPT | EZC_FLAGS_WRITE},
	{ "wl_wpa_psk", "WPA Pre-Shared Key", wl_prefix, validate_wl_wpa_psk, ARGV("64"), TRUE, NVRAM_ENCRYPT | NVRAM_MI |  EZC_FLAGS_WRITE},
	{ "wl_wpa_gtk_rekey", "Network Key Rotation Interval", wl_prefix, NULL, NULL, TRUE, NVRAM_MI |  EZC_FLAGS_READ | EZC_FLAGS_WRITE },
	/* Multi SSID Guest interface flag */
	{ "wl_guest", "Guest SSID Interface", wl_prefix, NULL, NULL, TRUE, WEB_IGNORE | NVRAM_MI  },
	/* MUST leave this entry here after all wl_XXXX variables */
	{ "wl_unit", "802.11 Instance", wl_prefix, wl_unit, NULL, TRUE, NVRAM_IGNORE | NVRAM_MI | NVRAM_MP },
	/* Internal variables */
	{ "os_server", "OS Server", NULL, NULL, NULL, TRUE, 0 },
	{ "stats_server", "Stats Server", NULL, NULL, NULL, TRUE, 0 },
	{ "timer_interval", "Timer Interval", NULL, NULL, NULL, TRUE, 0 },
	{ "lan_ifname", "LAN Interface Name", lan_prefix, NULL, NULL, TRUE, NVRAM_MI | NVRAM_MP },
	{ "lan_ifnames", "LAN Interface Names", NULL, NULL, NULL, TRUE, 0 },
	{ "wan_ifnames", "WAN Interface Names", NULL, NULL, NULL, TRUE, 0 },
};

/* build hashtable of the monster v-block

   Inputs:
   -tab: 	hash table  structure
   -vblock:	the monster V-block
   -num_items:	estimated size of hash table
   
   Returns: 0 on success -1 on error
*/

int 
hash_vtab(struct hsearch_data *tab,struct variable *vblock,int num_items)
{
	ENTRY e, *ep=NULL;
	int count;
	
	assert(tab);
	assert(vblock);

	if (!num_items) return -1;
	 
	if (!hcreate_r(num_items,tab))  
	{
			return -1;
	}
	
	
	for (count=0; count < num_items; count++)
	{
		e.key = vblock[count].name;
		e.data = &vblock[count];
		if (!hsearch_r(e, ENTER, &ep, tab))
			return -1;		
	}
	
	return 0;
	
}
/* The routine gets the pointer into the giant "V" block above give the cgi var name..
   Inputs:
   -varname: Pointer to cgi var name
   
   Returns: entry within "V" block or NULL if not found.
   
   It will try the following forms
   varXX_type
   var_typeXX
   vlanXXtype
   
   where XX is the instance number

*/

static struct variable*
get_var_handle(char *varname)
{
	ENTRY e, *ep=NULL;
	struct variable *variable=NULL;
	char *ptr=NULL,*ptr2=NULL;
	int offset;
	char prefix[8],tmp[64]; 
	
	if (!varname ) return NULL;
	if (!*varname) return NULL;
	
	ep=NULL;
	e.key = varname;
	hsearch_r(e, FIND, &ep, &vtab);
	
	
	/* found something */
	if (ep)
	{
		variable = (struct variable *)ep->data;
		if ( variable->ezc_flags & NVRAM_MP) return variable;
		
		/* Drop the variable is multi instance but not
		   Multi personality
		*/
		
		if ( variable->ezc_flags & NVRAM_MI) return NULL;
		if ( variable->ezc_flags & NVRAM_VLAN_MULTI) return NULL;
		if ( variable->ezc_flags & NVRAM_GENERIC_MULTI) return NULL;

		return variable;
		
	}
	
	/* variable not found could be the form of vlanXXtype, varXX_type or var_typeXX*/
	
	ptr=strchr(varname,'_');
	
	/* Is it  vlanXXtype ? */
	if (!ptr)
	{
		if (!strstr(varname,"vlan")) return NULL;
		strcpy(tmp,"vlan");
		offset=4;
		ptr=&varname[offset];
		while (*ptr){
				if (!isdigit((int)*ptr)) tmp[offset++] = *ptr;
				ptr++;
		}

		tmp[offset] = '\0';
		ep = NULL;
		e.key = tmp;
		hsearch_r(e, FIND, &ep, &vtab);
		
		/* check to see if this has the 
		   NVRAM_VLAN MULTI flag set in the v-block
		*/
		if (ep){
			variable = (struct variable *)ep->data;
			if (variable->ezc_flags & NVRAM_VLAN_MULTI) 
					return variable;
		}

		return  NULL;
	}
	
	/* Is it varXX_type ? */
	ptr2=varname;
	offset=0;
	memset(prefix,0,sizeof(prefix));
	while (ptr2 < ptr){
		if (isdigit((int)*ptr2)) break;
		prefix[offset++] =*ptr2;
		ptr2++;
	}
	snprintf(tmp,sizeof(tmp),"%s%s",prefix,ptr);

	ep = NULL;
	e.key = tmp;
	hsearch_r(e, FIND, &ep, &vtab);
	
	if (ep){
		variable = (struct variable *)ep->data;
		if (variable->ezc_flags & NVRAM_MI)
	 			return  variable;
	}
	
	/* Is is a var_typeXX */
	strncpy(tmp,varname,sizeof(tmp));
	offset = strlen(varname) - 1 ;
	while(isdigit((int)tmp[offset]))
			tmp[offset--] = '\0';
	ep = NULL;
	e.key = tmp;
	hsearch_r(e, FIND, &ep, &vtab);
		
	if (ep){
		variable = (struct variable *)ep->data;
		if (variable->ezc_flags & NVRAM_GENERIC_MULTI)
	 			return  variable;
	}

	return  NULL;
}
int 
variables_arraysize(void)
{
	return ARRAYSIZE(variables);
}

/* Need to do special handling for the lan cgi stuff as the DHCP ranges need
 overlap checking . In addition multi index variables are also present 
  on the same page. This breaks the conventional validation flow as implemented
*/
static void
validate_lan_cgi(webs_t wp)
{
	char cgi_vars[][32]= {
		       "lan_ifname",
		       "lan_ifnames",
		       "lan_dhcp", 	
		       "lan_ipaddr",	
		       "lan_netmask",
		       "lan_gateway",
		       "lan_proto",
		       "dhcp_start",
		       "dhcp_end",
		       "lan_lease",
		       "lan_stp",
		       "lan_route"
		       }; 

	int count,num_ifaces;
	char *varname=NULL,*value=NULL;
	struct variable *v=NULL;
	int num_items = sizeof(cgi_vars)/sizeof(cgi_vars[0]);
	struct in_addr i_addr,g_addr,i_mask,g_mask;
	char err_msg[255] ;
	char vector[16];
	int router_enable=0;
	
	ret_code = EINVAL;
	
	memset(err_msg,0,sizeof(err_msg));
	memset(vector,0,sizeof(vector));
	
	websBufferInit(wp);
	if (!webs_buf) {
		snprintf(err_msg,sizeof(err_msg),"out of memory<br>");
		goto validate_lan_cgi_error;
	}
	
	value =  websGetVar(wp, "num_lan_ifaces" , NULL);
	if (!value){
		snprintf(err_msg,sizeof(err_msg),
			"unable to get number of lan interfaces<br>");
		goto validate_lan_cgi_error;
	}
	
	num_ifaces=atoi(value);
	
	router_enable = nvram_match("router_disable","0");
	
	
	/* Build a SSID valid enabled vector, skip processing if the
	   vector is not set. Set vector on virtual i/f only if wl_vifs is set 
	*/
	for (count=0; count < num_ifaces ; count++){	
		char buf[64];
		
		if (count)
			snprintf(buf,sizeof(buf),"lan%d_ifname",count);
		else
			snprintf(buf,sizeof(buf),"lan_ifname");

		value =  websGetVar(wp, buf , NULL);
		if (value) 		
		{
			if (!*value) 
				continue;
			
			/* Clear lan mac address if the ip configuration is unset */
			if (!strcmp(value,"NONE")){
				/* Clear lanX_ifname if the inteface name has been set to NONE in the web page */
				nvram_unset(buf);
				/* Clear up lanX_hwaddr */
				snprintf(buf,sizeof(buf),"lan%d_hwaddr",count);
				nvram_unset(buf);
				continue;
			}
				
			if (strstr(value,".")){
				int unit=-1,subunit=-1;
				char *wl_vifs;
			
				if (get_ifname_unit(value,&unit,&subunit) < 0) 
					continue;
				if (subunit < 0 )
					continue;
				
				snprintf(buf,sizeof(buf),"wl%d_vifs",unit);
			
				wl_vifs = nvram_get(buf);
			
				if (!wl_vifs)
					continue;
			
				if (!strstr(wl_vifs,value) )
					continue;
			}
				
			vector[count] = 1;
		}
		
	}
	
	/* check to see if the ip addresses overlap */
	
	for (count=0; count < num_ifaces ; count++){
		int entry;
		char *valueA,*valueB;
		char lanA_ipaddr[32],lanA_netmask[32];
		char lanB_ipaddr[32],lanB_netmask[32];
		char dhcpA_start[32],dhcpB_start[32];
		char dhcpA_end[32],dhcpB_end[32];
		char lanA_ifname[32],lanB_ifname[32];
		char lanA_proto[32],lanB_proto[32];
		
		/*Skip entry if the SSID is not turned on */
		if (!vector[count]) 
			continue;
			
		if (count){
			snprintf(lanA_ifname,sizeof(lanA_ifname),"lan%d_ifname",count);		
			snprintf(lanA_ipaddr,sizeof(lanA_ipaddr),"lan%d_ipaddr",count);
			snprintf(lanA_netmask,sizeof(lanA_netmask),"lan%d_netmask",count);
			snprintf(lanA_proto,sizeof(lanA_proto),"lan%d_proto",count);
			snprintf(dhcpA_start,sizeof(dhcpA_start),"dhcp%d_start",count);	
			snprintf(dhcpA_end,sizeof(dhcpA_end),"dhcp%d_end",count);		
		}else{
			snprintf(lanA_ifname,sizeof(lanA_ifname),"lan_ifname");
			snprintf(lanA_ipaddr,sizeof(lanA_ipaddr),"lan_ipaddr");
			snprintf(lanA_netmask,sizeof(lanA_netmask),"lan_netmask");
			snprintf(lanA_proto,sizeof(lanA_proto),"lan_proto");
			snprintf(dhcpA_start,sizeof(dhcpA_start),"dhcp_start");
			snprintf(dhcpA_end,sizeof(dhcpA_end),"dhcp_end");
		}
			
		for (entry=0; entry < num_ifaces; entry++){
			
			if (count == entry) 
				continue;
			
			/*Skip entry if the SSID is not turned on */
			if (!vector[entry]) 
				continue;
			
			if (entry){
				snprintf(lanB_ifname,sizeof(lanB_ifname),"lan%d_ifname",entry);
				snprintf(lanB_ipaddr,sizeof(lanB_ipaddr),"lan%d_ipaddr",entry);
				snprintf(lanB_netmask,sizeof(lanB_netmask),"lan%d_netmask",entry);
				snprintf(lanB_proto,sizeof(lanB_proto),"lan%d_proto",count);
				snprintf(dhcpB_start,sizeof(dhcpB_start),"dhcp%d_start",entry);
				snprintf(dhcpB_end,sizeof(dhcpB_end),"dhcp%d_end",entry);	
			}else{
				snprintf(lanB_ifname,sizeof(lanB_ifname),"lan_ifname");
				snprintf(lanB_ipaddr,sizeof(lanB_ipaddr),"lan_ipaddr");
				snprintf(lanB_netmask,sizeof(lanB_netmask),"lan_netmask");
				snprintf(lanB_proto,sizeof(lanB_proto),"lan_proto");
				snprintf(dhcpB_start,sizeof(dhcpB_start),"dhcp_start");
				snprintf(dhcpB_end,sizeof(dhcpB_end),"dhcp_end");
			}
			
			/* Check for the same interface unit numbers */
			valueA = websGetVar(wp , lanA_ifname , NULL);
			valueB = websGetVar(wp , lanB_ifname , NULL);
			
			if ((!valueA) ||(!valueB)) {
				snprintf(err_msg,sizeof(err_msg),
					"<br>%s is NULL<br>",(valueA) ? lanB_ifname : lanA_ifname);
	      			goto validate_lan_cgi_error;	
			}

			if (!strcmp(valueA,valueB)){
				snprintf(err_msg,sizeof(err_msg),
					"<br>Interface %s selected more than once<br>",valueA);
	      			goto validate_lan_cgi_error;	
			}
			
			value =  websGetVar(wp, lanA_ipaddr , NULL);
			if (! value ) goto validate_lan_cgi_error;
			(void)inet_aton(value,&i_addr);
	
			value = websGetVar(wp, lanB_ipaddr , NULL);
			if (! value ) goto validate_lan_cgi_error;
			(void)inet_aton(value,&g_addr);
	
			value =  websGetVar(wp, lanA_netmask , NULL);
			if (! value ) goto validate_lan_cgi_error;
			(void)inet_aton(value,&i_mask);
		
			value = websGetVar(wp, lanB_netmask , NULL);
			if (! value ) goto validate_lan_cgi_error;
			(void)inet_aton(value,&g_mask);
			
			if ((i_addr.s_addr & i_mask.s_addr)==(g_addr.s_addr & g_mask.s_addr) ){
				snprintf(err_msg,sizeof(err_msg),
					"<br>Overlapping IP address ranges:<br>(RangeA=%s/%s) (RangeB=%s/%s)<br>",
						lanA_ipaddr,lanA_netmask,lanB_ipaddr,lanB_netmask);
	      			goto validate_lan_cgi_error;	
			}
			
			valueA = websGetVar(wp , lanA_proto , NULL);
			valueB = websGetVar(wp , lanB_proto , NULL);
			
			/* If any of the proto vars are null skip the check */ 
			if ((!valueA) ||(!valueB)) 
						continue;
			
			/* Overlapping DHCP range check only if DHCP is the lan proto on both*/
			if (!strcmp(valueA,"dhcp") && !strcmp(valueB,"dhcp"))
			{
				value =  websGetVar(wp, dhcpA_start , NULL);
				if (! value ) goto validate_lan_cgi_error;
				(void) inet_aton(value,&i_addr);
		
				value = websGetVar(wp, dhcpB_start , NULL);
				if (! value ) goto validate_lan_cgi_error;
				(void) inet_aton(value,&g_addr);
		
				/* Are they in the same subnetwork ? */
		
				if ((i_addr.s_addr & i_mask.s_addr)==(g_addr.s_addr & g_mask.s_addr) ){
					snprintf(err_msg,sizeof(err_msg),
						"<br>Overlapping DHCP start ranges<br>(RangeA=%s/%s) (RangeB=%s/%s)<br>",
							dhcpA_start,lanA_netmask,dhcpB_start,lanB_netmask);
	      					goto validate_lan_cgi_error;
				}
				
				value =  websGetVar(wp, dhcpA_end , NULL);
				if (! value ) goto validate_lan_cgi_error;
				(void) inet_aton(value,&i_addr);
		
				value = websGetVar(wp, dhcpB_end , NULL);
				if (! value ) goto validate_lan_cgi_error;
				(void) inet_aton(value,&g_addr);
		
				/* Are they in the same subnetwork ? */
		
				if ((i_addr.s_addr & i_mask.s_addr)==(g_addr.s_addr & g_mask.s_addr) ){
					snprintf(err_msg,sizeof(err_msg),
						 "<br>Overlapping DHCP end ranges<br>(RangeA=%s/%s) (RangeB=%s/%s)<br>",
							dhcpA_end,lanA_netmask,dhcpB_end,lanB_netmask);
	      					goto validate_lan_cgi_error;
				}				
			}
		}	
	}
	
	ret_code = 0; 
	
	/* The individual validation functions will set ret_code to EINVAL
	   if an error is encountered. On success zero is returned.
	   Validation stops on the first error encountered
	*/ 
	for  ( count = 0; ( count < num_items && !(ret_code) ); count++){
		
		char var[64];
		int entry;

		/* Lookup template */
		varname = cgi_vars[count];
		v = get_var_handle(varname);
		
		/* Enumerate thru list of interfaces */
		if (v) 
			for (entry=0; entry < num_ifaces; entry++){
			
				/* Skip entry if the SSID is not turned on */
				if (!vector[entry])
					continue;
				
				if (v->ezc_flags & WEB_IGNORE)
					continue;
					
				if (entry && v->prefix )
					snprintf(var,sizeof(var),"%s%d_%s",
						v->prefix,entry,&varname[strlen(v->prefix) + 1]);
				else
					snprintf(var,sizeof(var),"%s",varname);
					
				value = websGetVar(wp, var, NULL);
					
				if (value){
					if ((!*value && v->nullok) || !v->validate)
						nvram_set(var, value);
					else
						v->validate(wp, value, v, var);
				}
			}

	}

	/* Set lanX_dhcp to static for all interfaces in router mode */
	if (router_enable)
		for (count = 0; count < num_ifaces; count++){
			char lan_dhcp[]="lanXXXXX_dhcp";
			/* Skip entry if the SSID is not turned on */
			if (!vector[count])
				continue;
			if (count)
				snprintf(lan_dhcp,sizeof(lan_dhcp),"lan%d_dhcp",count);
			else
				snprintf(lan_dhcp,sizeof(lan_dhcp),"lan_dhcp");
			nvram_set(lan_dhcp,"0");
				
		}
		
	/* Handlers already print error messages. No need for further explanation */
	if (ret_code)
		snprintf(err_msg,sizeof(err_msg),"Error during variable validation.<br>");
	
validate_lan_cgi_error:
	if (*err_msg)
		websWrite(wp, err_msg);
	
	websBufferFlush(wp);
}
	
static void
validate_cgi(webs_t wp)
{
	struct variable *v=NULL;
	char *value=NULL;

	websBufferInit(wp);
	if (!webs_buf) {
		websWrite(wp, "out of memory<br>");
		websDone(wp, 0);
		return;
	}


	/* Validate and set variables in table order */
	for (v = variables; v < &variables[ARRAYSIZE(variables)]; v++) {
		if (!(value = websGetVar(wp, v->name, NULL)))
			continue;

		if (v->ezc_flags & WEB_IGNORE)
					continue;
					
		if ((!*value && v->nullok) || !v->validate)
				nvram_set(v->name, value);
		else
				v->validate(wp, value, v, NULL);

	}

	websBufferFlush(wp);
}

static int
apply_cgi(webs_t wp, char_t *urlPrefix, char_t *webDir, int arg, 
	  char_t *url, char_t *path, char_t *query)
{
	char *value=NULL;
	char *page=NULL;
	action = NOTHING;

	websHeader(wp);
	websWrite(wp, (char_t *) apply_header);
	
	page = websGetVar(wp, "page", "");

	value = websGetVar(wp, "action", "");

	/* Apply values */
	if (!strcmp(value, "Apply")) {
		action = RESTART;
		websWrite(wp, "Validating values...");
		
		if (strcmp("lan.asp",page))
			validate_cgi(wp);
		else
			validate_lan_cgi(wp);
		if(ret_code)
		{
			websWrite(wp, "<br>");
			action = NOTHING;
		}else{
			websWrite(wp, "done<br>");
			websWrite(wp, "Committing values...");
			nvram_set("is_modified", "1");
			nvram_set("is_default", "0");
			nvram_commit();
			websWrite(wp, "done<br>");
		}
	}

	/* Restore defaults */
	else if (!strncmp(value, "Restore", 7)) {
		websWrite(wp, "Restoring defaults...");
		nvram_set("sdram_ncdl", "0");
		nvram_set("restore_defaults", "1");
		nvram_commit();
		websWrite(wp, "done<br>");
		action = REBOOT;
	}
	
	/* Release lease */
	else if (!strcmp(value, "Release")) {
		websWrite(wp, "Releasing lease...");
		if (sys_release())
			websWrite(wp, "error<br>");
		else
			websWrite(wp, "done<br>");
		action = NOTHING;
	}

	/* Renew lease */
	else if (!strcmp(value, "Renew")) {
		websWrite(wp, "Renewing lease...");
		if (sys_renew())
			websWrite(wp, "error<br>");
		else
			websWrite(wp, "done<br>");
		action = NOTHING;
	}

	/* Reboot router */
	else if (!strcmp(value, "Reboot")) {
		websWrite(wp, "Rebooting...");
		action = REBOOT;
	}

	/* Upgrade image */
	else if (!strcmp(value, "Upgrade")) {
		char *os_name = nvram_safe_get("os_name");
		char *os_server = websGetVar(wp, "os_server", nvram_safe_get("os_server"));
		char *os_version = websGetVar(wp, "os_version", "current");
		char url[PATH_MAX];
		if (!*os_version)
			os_version = "current";
		snprintf(url, sizeof(url), "%s/%s/%s/%s.trx",
			 os_server, os_name, os_version, os_name);
		websWrite(wp, "Retrieving %s...", url);
		if (sys_upgrade(url, NULL, NULL)) {
			websWrite(wp, "error<br>");
			goto footer;
		} else {
			websWrite(wp, "done<br>");
			action = REBOOT;
		}
	}

	/* Report stats */
	else if (!strcmp(value, "Stats")) {
		char *server = websGetVar(wp, "stats_server", nvram_safe_get("stats_server"));
		websWrite(wp, "Contacting %s...", server);
		if (sys_stats(server)) {
			websWrite(wp, "error<br>");
			goto footer;
		} else {
			websWrite(wp, "done<br>");
			nvram_set("stats_server", server);
		}
	}
	/* Radio On Off  */
	else if (!strcmp(value, "RadioOff")) {
		websWrite(wp, "Turing Off Radio...");
		wl_radio_onoff(wp, 1);
		action = NOTHING;
	}
	else if (!strcmp(value, "RadioOn")) {
		websWrite(wp, "Radio on...");
		wl_radio_onoff(wp, 0);
		action = NOTHING;
	}
#ifdef __CONFIG_NAT__
	/* Delete connection */
	else if (!strcmp(value, "Delete")) {
		int unit;
		if (!(value = websGetVar(wp, "wan_unit", NULL)) || 
		    (unit = atoi(value)) < 0 || unit >= MAX_NVPARSE) {
			websWrite(wp, "Unable to delete connection, index error.");
			action = NOTHING;
		}
		else {
			struct nvram_tuple *t;
			char tmp[NVRAM_BUFSIZE], prefix[] = "wanXXXXXXXXXX_";
			int unit2, units = 0;
			/*
			* We can't delete the last connection since we can't differentiate
			* the cases where user does not want any connection (user deletes 
			* the last one) vs. the router is booted the first time when there
			* is no connection at all (where a default one is created anyway).
			*/
			for (unit2 = 0; unit2 < MAX_NVPARSE; unit2 ++) {
				wan_prefix(unit2, prefix);
				if (nvram_get(strcat_r(prefix, "unit", tmp)) && units++ > 0)
					break;
			}
			if (units < 2) {
				websWrite(wp, "Can not delete the last connection.");
				action = NOTHING;
			}
			else {
				/* set prefix */
				wan_prefix(unit, prefix);
				/* remove selected wan%d_ set */
				websWrite(wp, "Deleting connection...");
				for (t = router_defaults; t->name; t ++) {
					if (!strncmp(t->name, "wan_", 4))
						nvram_unset(strcat_r(prefix, &t->name[4], tmp));
				}
				/* fix unit number */
				unit2 = unit;
				for (; unit < MAX_NVPARSE; unit ++) {
					wan_prefix(unit, prefix);
					if (nvram_get(strcat_r(prefix, "unit", tmp)))
						break;
				}
				if (unit >= MAX_NVPARSE) {
					unit = unit2 - 1;
					for (; unit >= 0; unit --) {
						wan_prefix(unit, prefix);
						if (nvram_get(strcat_r(prefix, "unit", tmp)))
							break;
					}
				}
				snprintf(tmp, sizeof(tmp), "%d", unit);
				nvram_set("wan_unit", tmp);
				/* check if there is any primary connection - see comment in wan_unit() */
				for (unit = 0; unit < MAX_NVPARSE; unit ++) {
					wan_prefix(unit, prefix);
					if (!nvram_get(strcat_r(prefix, "unit", tmp)))
						continue;
					if (nvram_invmatch(strcat_r(prefix, "proto", tmp), "disabled") &&
					    nvram_match(strcat_r(prefix, "primary", tmp), "1"))
						break;
				}
				/* no one is primary, pick the first enabled one as primary */
				if (unit >= MAX_NVPARSE)
					wan_primary(wp);
				/* save the change */
				nvram_set("is_modified", "1");
				nvram_set("is_default", "0");
				nvram_commit();
				websWrite(wp, "done<br>");
				action = RESTART;
			}
		}
	}
#endif	/* __CONFIG_NAT__ */
	
	/* Invalid action */
	else
		websWrite(wp, "Invalid action %s<br>", value);

 footer:
	websWrite(wp, (char_t *) apply_footer, websGetVar(wp, "page", ""));
	websFooter(wp);
	websDone(wp, 200);

	if (action == RESTART)
		sys_restart();
	else if (action == REBOOT)
		sys_reboot();

	return 1;
}

/* Copy all wl%d_XXXXX to wl_XXXXX */
static int
copy_wl_index_to_unindex(webs_t wp, char_t *urlPrefix, char_t *webDir, 
													int arg, char_t *url, char_t *path, char_t *query)
{
	struct variable *v=NULL;
	char *value=NULL;
	char name[IFNAMSIZ], os_name[IFNAMSIZ], *next=NULL;
	int unit = 0;
	char tmp[NVRAM_BUFSIZE], prefix[] = "wlXXXXXXXXXX_";
	char unit_str[]={'\0','\0','\0','\0','\0'};
	int applying = 0;

	/* Can enter this function through GET or POST */
	if ((value = websGetVar(wp, "action", NULL))) {
		if (strcmp(value, "Select"))
		{
			/* we need to make sure wl_unit on the web page matches the
				 wl_unit in NVRAM.  If they match, bail out now, otherwise 
				 proceed with the rest of the function */

			if ((value = websGetVar(wp, "wl_unit", NULL))) {
				if (!strcmp( nvram_safe_get("wl_unit" ), value))
					return apply_cgi(wp, urlPrefix, webDir, arg, url, path, query);
			}
			applying = 1;
		}
	}

	/* copy wl%d_XXXXXXXX to wl_XXXXXXXX */
	if ((value = websGetVar(wp, "wl_unit", NULL))) {
		strncpy(unit_str,value,sizeof(unit_str));
	} else {
		char ifnames[256];
		snprintf(ifnames, sizeof(ifnames), "%s %s %s", 
			nvram_safe_get("lan_ifnames"),
			nvram_safe_get("wan_ifnames"),
			nvram_safe_get("unbridged_ifnames"));
			
		if (!remove_dups(ifnames,sizeof(ifnames))){
			websError(wp, 400, "Unable to remove duplicate interfaces from ifname list<br>");	
			return -1;
		}
		
		/* Probe for first wl interface */
		foreach(name, ifnames, next) {
			if (nvifname_to_osifname( name, os_name, sizeof(os_name) ) < 0)
				continue;
			if (wl_probe(os_name) == 0 &&
			    wl_ioctl(os_name, WLC_GET_INSTANCE, &unit, sizeof(unit)) == 0){
			    	snprintf(unit_str,sizeof(unit_str),"%d",unit);
				break;
			}
		}

	}
	if (*unit_str) {
		snprintf(prefix, sizeof(prefix), "wl%s_", unit_str);
		for (v = variables; v < &variables[ARRAYSIZE(variables)]; v++) {
			if (v->ezc_flags & WEB_IGNORE)
				continue;
			if (!strncmp(v->name, "wl_", 3))
				nvram_set(v->name, nvram_safe_get(strcat_r(prefix, &v->name[3], tmp)));
			if (!strncmp(v->name, "wl_unit", 7))
				break;
		}
	}
	
	/* Set currently selected unit */
	nvram_set("wl_unit", unit_str);

	if( applying )
		return apply_cgi(wp, urlPrefix, webDir, arg, url, path, query);

	/* Display the page */
	return websDefaultHandler(wp, urlPrefix, webDir, arg, url, path, query);
}


#ifdef __CONFIG_NAT__
static int
wan_asp(webs_t wp, char_t *urlPrefix, char_t *webDir, int arg, 
	     char_t *url, char_t *path, char_t *query)
{
	struct variable *v=NULL;
	char *value=NULL;
	int unit = 0;
	char tmp[NVRAM_BUFSIZE], prefix[] = "wanXXXXXXXXXX_";
	char ustr[16];
	struct nvram_tuple *t=NULL;

	/* Can enter this function through GET or POST */
	if ((value = websGetVar(wp, "action", NULL))) {
		if (!strcmp(value, "New")) {
			/* pick one that 'does not' exist */
			for (unit = 0; unit < MAX_NVPARSE; unit ++) {
				wan_prefix(unit, prefix);
				if (!nvram_get(strcat_r(prefix, "unit", tmp)))
					break;
			}
			if (unit >= MAX_NVPARSE) {
				websHeader(wp);
				websWrite(wp, (char_t *) apply_header);
				websWrite(wp, "Unable to create new connection. Maximum %d.", MAX_NVPARSE);
				websWrite(wp, (char_t *) apply_footer, websGetVar(wp, "page", ""));
				websFooter(wp);
				websDone(wp, 200);
				return 1;
			}
			/* copy default to newly created wan%d_ set */
			for (t = router_defaults; t->name; t ++) {
				if (!strncmp(t->name, "wan_", 4))
					nvram_set(strcat_r(prefix, &t->name[4], tmp), t->value);
			}
			/* the following variables must be overwritten */
			snprintf(ustr, sizeof(ustr), "%d", unit);
			nvram_set(strcat_r(prefix, "unit", tmp), ustr);
			nvram_set(strcat_r(prefix, "proto", tmp), "disabled");
			nvram_set(strcat_r(prefix, "ifname", tmp), nvram_safe_get("wan_ifname"));
			nvram_set(strcat_r(prefix, "hwaddr", tmp), nvram_safe_get("wan_hwaddr"));
			nvram_set(strcat_r(prefix, "ifnames", tmp), nvram_safe_get("wan_ifnames"));
			/* commit change */
			nvram_set("is_modified", "1");
			nvram_set("is_default", "0");
			nvram_commit();
		}
		else if (!strcmp(value, "Select")) {
			if ((value = websGetVar(wp, "wan_unit", NULL)))
				unit = atoi(value);
			else
				unit = -1;
		}
		else
			return apply_cgi(wp, urlPrefix, webDir, arg, url, path, query);
	}
	else if ((value = websGetVar(wp, "wan_unit", NULL)))
		unit = atoi(value);
	else
		unit = atoi(nvram_safe_get("wan_unit"));
	if (unit < 0 || unit >= MAX_NVPARSE)
		unit = 0;

	/* Set prefix */
	wan_prefix(unit, prefix);

	/* copy wan%d_ set to wan_ set */
	for (v = variables; v < &variables[ARRAYSIZE(variables)]; v++) {
		if (v->ezc_flags & WEB_IGNORE)
			continue;	
		if (strncmp(v->name, "wan_", 4))
			continue;
		value = nvram_get(strcat_r(prefix, &v->name[4], tmp));
		if (value)
			nvram_set(v->name, value);
		if (!strncmp(v->name, "wan_unit", 8))
			break;
	}

	/* Set currently selected unit */
	snprintf(tmp, sizeof(tmp), "%d", unit);
	nvram_set("wan_unit", tmp);

	/* Display the page */
	return websDefaultHandler(wp, urlPrefix, webDir, arg, url, path, query);
}
#endif	/* __CONFIG_NAT__ */

#ifdef WEBS

void
initHandlers(void)
{
	websAspDefine("nvram_get", ej_nvram_get);
	websAspDefine("nvram_match", ej_nvram_match);
	websAspDefine("nvram_invmatch", ej_nvram_invmatch);
	websAspDefine("nvram_list", ej_nvram_list);
	websAspDefine("filter_client", ej_filter_client);
	websAspDefine("forward_port", ej_forward_port);
	websAspDefine("static_route", ej_static_route);
	websAspDefine("localtime", ej_localtime);
	websAspDefine("dumplog", ej_dumplog);
	websAspDefine("syslog", ej_syslog);
	websAspDefine("dumpleases", ej_dumpleases);
	websAspDefine("link", ej_link);
	websAspDefine("wme_match_op", ej_wme_match_op);

	websUrlHandlerDefine("/apply.cgi", NULL, 0, apply_cgi, 0);
	websUrlHandlerDefine("/wireless.asp", NULL, 0, copy_wl_index_to_unindex, 0);

	websSetPassword(nvram_safe_get("http_passwd"));

	websSetRealm("Broadcom Home Gateway Reference Design");
	
	/* Initialize hash table */
	hash_vtab(&vtab,v,variables_arraysize());
}

#else /* !WEBS */

int internal_init(void)
{
	/* Initialize hash table */
	if (hash_vtab(&vtab,variables,variables_arraysize()))
		return -1;
	return 0;
}
static void
do_auth(char *userid, char *passwd, char *realm)
{
	assert(userid);
	assert(passwd);
	assert(realm);
	
	strncpy(userid, nvram_safe_get("http_username"), AUTH_MAX);
	strncpy(passwd, nvram_safe_get("http_passwd"), AUTH_MAX);
	strncpy(realm, "Broadcom Home Gateway Reference Design", AUTH_MAX);
}

char post_buf[POST_BUF_SIZE];
char ezc_version[128];
char no_cache[] =
"Cache-Control: no-cache\r\n"
"Pragma: no-cache\r\n"
"Expires: 0"
;

char download_hdr[] =
"Cache-Control: no-cache\r\n"
"Pragma: no-cache\r\n"
"Expires: 0\r\n"
"Content-Type: application/download\r\n"
"Content-Disposition: attachment ; filename=nvram.txt"
;

static void
do_apply_post(char *url, FILE *stream, int len, char *boundary)
{
	assert(url);
	assert(stream);
			
	/* Get query */
	if (!fgets(post_buf, MIN(len + 1, sizeof(post_buf)), stream))
		return;
	len -= strlen(post_buf);

	/* Initialize CGI */
	init_cgi(post_buf);

	/* Slurp anything remaining in the request */
	while (len--)
		(void) fgetc(stream);
}

static void
do_apply_cgi(char *url, FILE *stream)
{
	char *path=NULL, *query=NULL;

	assert(url);
	assert(stream);

	/* Parse path */
	query = url;
	path = strsep(&query, "?") ? : url;

	apply_cgi(stream, NULL, NULL, 0, url, path, query);

	/* Reset CGI */
	init_cgi(NULL);
}


static void
do_upgrade_post(char *url, FILE *stream, int len, char *boundary)
{
	char buf[1024];

	assert(url);
	assert(stream);
	
	ret_code = EINVAL;

	/* Look for our part */
	while (len > 0) {
		if (!fgets(buf, MIN(len + 1, sizeof(buf)), stream))
			return;
		len -= strlen(buf);
		if (!strncasecmp(buf, "Content-Disposition:", 20) &&
		    strstr(buf, "name=\"file\""))
			break;
	}

	/* Skip boundary and headers */
	while (len > 0) {
		if (!fgets(buf, MIN(len + 1, sizeof(buf)), stream))
			return;
		len -= strlen(buf);
		if (!strcmp(buf, "\n") || !strcmp(buf, "\r\n"))
			break;
	}

	ret_code = sys_upgrade(NULL, stream, &len);

	/* Slurp anything remaining in the request */
	while (len--)
		(void) fgetc(stream);
}

static void
do_upgrade_cgi(char *url, FILE *stream)
{
	assert(url);
	assert(stream);
	
	websHeader(stream);
	websWrite(stream, (char_t *) apply_header);

	/* We could probably be more informative here... */
	if (ret_code)
		websWrite(stream, "Error during upgrade<br>");
	else
		websWrite(stream, "Upgrade complete. Rebooting...<br>");

	websWrite(stream, (char_t *) apply_footer, "firmware.asp");
	websFooter(stream);
	websDone(stream, 200);

	/* Reboot if successful */
	if (ret_code == 0)
		sys_reboot();
}
/* Utility function to remove duplicate entries in a space separated list
 */
 
static char *
remove_dups(char *inlist, int inlist_size)
{	
	char name[IFNAMSIZ], *next=NULL;
	char *outlist;
		
	if (!inlist_size)
		return NULL;
	
	if (!inlist)
		return NULL;
	
	outlist =(char *) malloc(inlist_size);
	
	if (!outlist ) return NULL;
	
	memset(outlist,0,inlist_size);
	
	foreach(name,inlist,next)
	{
		if (strstr(outlist,name) == NULL) 
		{
			if( strlen(outlist) == 0 )
			{
				snprintf(outlist,inlist_size,"%s", name);
			}
			else
			{
				strncat( outlist, " ", inlist_size - strlen(outlist) );
				strncat( outlist, name, inlist_size - strlen(outlist) );
			}
		}
	}
	
	strncpy(inlist,outlist,inlist_size);
	
	free(outlist);
	return inlist;
	
}
/* This utility routine builds the wl prefixes from wl_unit.
 * Input is expected to be a null terminated string
 *
 * Inputs -prefix: Pointer to prefix buffer
 *	  -prefix_size: Size of buffer
 *        -Mode flag: If set generates unit.subunit output
 *                    if not set generates unit only
 *	  -ifname: Optional interface name string
 *
 * 
 * Returns - pointer to prefix, NULL if error.
 *
 *
*/
static char *
make_wl_prefix(char *prefix,int prefix_size, int mode, char *ifname)
{
	int unit=-1,subunit=-1;
	char *wl_unit=NULL;

	assert(prefix);
	assert(prefix_size);

	if (ifname){
		assert(*ifname);
		wl_unit=ifname;
	}else{
		wl_unit=nvram_get("wl_unit");

		if (!wl_unit) 
			return NULL;
	}
	
	if (get_ifname_unit(wl_unit,&unit,&subunit) < 0 )
		return NULL;
		
	if (unit < 0) return NULL;
	
	if  ((mode) && (subunit > 0 ))
		snprintf(prefix, prefix_size, "wl%d.%d_", unit,subunit);
	else
		snprintf(prefix, prefix_size, "wl%d_", unit);

	return prefix;
}

/* Format of the NVRAM text file is as follows
 * the following major sections
 * Header Block
 * Constraint Block
 * NVRAM variables
 *  
 * Header consists of :
 *	LineCount
 *	Checksum
 *
 * Constraint block consists of :
 * Defined by NVRAM_CONSTRAINT_VARS 
 * 
 *
*/  				

/* Get major os version. The nvram variable gives the whole thing.
 * this function avoids the messy parsing.
 * Used by NVRAM constraint validation routines.
 * The input argument is not used. This is used to maintain 
 * argument list compatability with nvram_get().
 *
*/ 
static  char*
osversion_get(const char *name)
{

	static char ret_string[32];
	
	assert(name);
  
 	snprintf(ret_string,sizeof(ret_string),"%d.%d",
  			EPI_MAJOR_VERSION,EPI_MINOR_VERSION);
		
		
	return ret_string;
	
}

/* This routine strips the CRLF from the HTTP stream */
/* Returns new length or -1 if there is an error*/
static int 
remove_crlf(char *buf, int len)
{

	int slen;
	if (!buf) return -1;
	if (!len) return 0;

	slen = strlen(buf);
	len = len-slen;
	if ((buf[slen-1] == '\n' ) && (buf[slen-2] == '\r' )){
		buf[slen-1]='\0';
		buf[slen-2]='\0';

	} 
	else if ( buf[slen-1] == '\n' ) buf[slen-1]='\0';
	
	return len;
}
/* Utility routine to add a string from a string buffer.
 * 
 * Routine expects that the buffer is composed of text strings with 
 * a null as the delimiter.
 *
 * Input is expected to be a null terminated string
 *
 * Inputs -Pointer to char buffer
 *        -NULL input terminated string
 *	  -Offset to end of used area. Including the NULL terminator
 *	  -Size of the buffer (to avoid running of the end of the buffer)
 *
 * 
 * Returns - Number of bytes occupied including NULL terminator. -1 if error
 *
 *
*/
static int
add_string(char *buf, char *string, int start, int bufsize)
{
	int len;
	
	assert(buf);
	
	if (!string) return -1;
	if (!*string) return -1;
	if (!bufsize) return -1;
	if (start >=bufsize) return -1;
	
	len = strlen(string);
	
	if ( (start + len + 1) >= bufsize ) return -1;
	
	strncpy(buf,string,len);
		
	return start + len + 1;
}
static void
do_nvramul_post(char *url, FILE *stream, int len, char *boundary)
{
	char buf[1024];
	int  index,cur_entry;
	char *ptr=NULL;
	char *name=NULL;
	char tmp[NVRAM_MAX_STRINGSIZE];
	char file_checksum[NVRAM_SHA1BUFSIZE],checksum[NVRAM_SHA1BUFSIZE];
	char key[NVRAM_FILEKEYSIZE];
	int  checksum_linenum = NVRAM_CHECKSUM_LINENUM;
	char salt[NVRAM_SALTSIZE];
	int  numlines=0;
	int  entries,offset,slen;
	char passphrase[]=NVRAM_PASSPHRASE;
	char nvram_file_header[][NVRAM_MAX_STRINGSIZE/2] = NVRAM_FILEHEADER;
	upload_constraints constraint_vars [] = NVRAM_CONSTRAINT_VARS;
	struct pb { 
			char header[NVRAM_HEADER_LINECOUNT(nvram_file_header)][NVRAM_MAX_STRINGSIZE];
			char buf[NVRAM_SPACE];
		 }*tmpbuf=NULL;

	assert(stream);
	
	entries = NVRAM_HEADER_LINECOUNT(nvram_file_header);
	ret_code = EINVAL;

	/* Look for our part */
	while (len > 0) {
		if (!fgets(buf, MIN(len + 1, sizeof(buf)), stream))
			return;

		/* Remove LF that fgets()drags in */
		len=remove_crlf(buf,len);
		/* look for start of attached file header */

		if (*buf){
		  if (strstr(buf,NVRAM_BEGIN_WEBFILE)) break;
		}
		
	}
	
	if (!len) return;
	
	/* loop thru the header lines until we get the blank line
	   that signifies the start of file contents */
	while (len > 0){
	 	if (!fgets(buf, MIN(len + 1, sizeof(buf)), stream))
			return;
		/* Remove LF that fgets()drags in */
		len=remove_crlf(buf,len);
		
		/* look for the blank line */
		if (*buf == NVRAM_END_WEBFILE)  break;
	}
	
	if (!len) return;
	
	/* Found start of upload file. Proceed with the upload */
	/* Look for start of upload data */
	if (!fgets(buf, MIN(len + 1, sizeof(buf)), stream))
			return;
					
	len = len - strlen(buf);
	
	/* Get the number of NVRAM variables */ 
	sprintf(tmp,"%s=%s",nvram_file_header[NVRAM_LINECOUNT_LINENUM],"%d");
	
	if ( (sscanf(buf,tmp,&numlines) != 1) || !len ){
		strncpy(posterr_msg,"Invalid NVRAM header<br>",ERR_MSG_SIZE);
	 	return ;
	}
	
	if ((numlines > NVRAM_MAX_LINES ) || !numlines ){
		strncpy(posterr_msg,"Invalid NVRAM header<br>",ERR_MSG_SIZE);
	 	return;
	}
	
	tmpbuf = (struct pb *) malloc( sizeof(struct pb));
	if (!tmpbuf){
		strncpy(posterr_msg,"Memory allocation error<br>",ERR_MSG_SIZE);
	 	return;
	} 
	
	memset(tmpbuf,0,sizeof(struct pb));
	
	/* Copy over the first line of the file. Needed for 
	   proper checksum calculations */
	   
	strcpy(tmpbuf->header[0],buf);
	
	/* read in checksum */
	if (!fgets(tmpbuf->header[1], MIN(len + 1,NVRAM_MAX_STRINGSIZE), stream))
			goto do_nvramul_post_cleanup0;
		
	len = len - strlen(tmpbuf->header[1]);
	
	/* start reading in the rest of the NVRAM contents into memory buffer*/
	offset = 0;
	for (index = 2 ; (len > 0) &&  (index < numlines); index++){
		char filebuf[NVRAM_MAX_STRINGSIZE];
		
	 	if (!fgets(filebuf, MIN(len + 1,NVRAM_MAX_STRINGSIZE), stream))
			goto do_nvramul_post_cleanup0;
		
		offset = add_string(&tmpbuf->buf[offset],filebuf,offset,
						sizeof(((struct pb *)0)->buf));
		if (offset < 0){
			snprintf(posterr_msg,ERR_MSG_SIZE,
				"Error processing NVRAM variable:%s<br>",filebuf);
	 		goto do_nvramul_post_cleanup0;
		}
		len = len - strlen(filebuf);

		/* don't remove the LFs since the we are using  multipart
		   MIME encoding that does not touch the contents of the
		   uploaded file. LFs are actually part of the NVRAM var lines.
		*/
	}
	
	/*  Bail if the number of actual lines is less than that
	    in the header 
	*/
	
	if ( !len && (index < numlines) ){
		strncpy(posterr_msg,"NVRAM file incomplete<br>",ERR_MSG_SIZE);
	 	goto do_nvramul_post_cleanup0;
	}
	
	/* Save and decode checksum from file */
	ptr = tmpbuf->header[checksum_linenum];
	
	sprintf(tmp,"%s=",nvram_file_header[checksum_linenum]);
	
	if ( !strstr(ptr,tmp) ) {
		snprintf(posterr_msg,ERR_MSG_SIZE,
			"No checksum present at line %d<br>",checksum_linenum+1);
		goto do_nvramul_post_cleanup0 ;
	}
	ptr = &ptr[strlen(tmp)];
	
	if ( b64_decode(ptr,tmp,NVRAM_MAX_STRINGSIZE) != NVRAM_FILECHKSUM_SIZE){
		strncpy(posterr_msg,"Invalid checksum.<br>",ERR_MSG_SIZE);
		goto do_nvramul_post_cleanup0 ;
	};
	
	memset(file_checksum,0,sizeof(file_checksum));
	memcpy(file_checksum,tmp,NVRAM_HASHSIZE);
	
	/* Extract salt value from stored checksum*/
	memcpy(salt,&tmp[NVRAM_HASHSIZE],NVRAM_SALTSIZE);

	/* Regenerate encryption key */
	memset(key,0,NVRAM_FILEKEYSIZE);
	fPRF(passphrase,strlen(passphrase),NULL,0,
			(unsigned char*)salt,sizeof(salt),key,sizeof(key));
	
	/* Plug in filler for checksum into read buffer*/
	memset(tmpbuf->header[checksum_linenum],0,NVRAM_MAX_STRINGSIZE);
	snprintf(tmpbuf->header[checksum_linenum],
			NVRAM_MAX_STRINGSIZE,"%s\n",NVRAM_CHECKSUM_FILLER);
	
	/* Calculate checksum and compare with stored value*/ 
	
	memset(checksum,0,sizeof(checksum));
	hmac_sha1((unsigned char*)tmpbuf,sizeof(struct pb) ,
							key,sizeof(key),checksum);
	
	if (memcmp(checksum,file_checksum,NVRAM_HASHSIZE)){
		memcpy(posterr_msg,"File checksum error<br>",ERR_MSG_SIZE);
		goto do_nvramul_post_cleanup0;	
	}
	
	/* Check constraints on the data */
	
	cur_entry = NVRAM_HEADER_LINECOUNT(nvram_file_header);
	slen=0;
	name = tmpbuf->buf;
	for (index = 0  ; *constraint_vars[index].name && *name; index++,cur_entry++,name += slen + 1) {
		char *var=NULL,*ptr=NULL;
		int comparesize ;
		
		slen = strlen(name);
		
		if (cur_entry > numlines){		
			memcpy(posterr_msg,
				"Constraints mismatch between file and running image<br>",
										ERR_MSG_SIZE);
			goto do_nvramul_post_cleanup0;	
		}
	
	
		var = constraint_vars[index].get(constraint_vars[index].name);

		if (!var){
			snprintf(posterr_msg,ERR_MSG_SIZE,
				"NVRAM variable:%s not found in running image<br>",
								constraint_vars[index].name);
			goto do_nvramul_post_cleanup0;	
		}
		
		if (!strstr(name,constraint_vars[index].name)){
			snprintf(posterr_msg,ERR_MSG_SIZE,
				"NVRAM variable:%s not found in uploaded file<br>",
								constraint_vars[index].name);
			goto do_nvramul_post_cleanup0;	
		}
		
		/*Move past separator*/
		ptr = &name[strlen(constraint_vars[index].name) + 1] ;
		
		/* Ignore last character which is a \n */
		comparesize = strlen(ptr) -1;
		
		/* If the primary match fails try for the secomdary matches */
		if (strncmp(ptr,var,comparesize)){
		int sec_fail=0;
		
			/* If partial march is defined match altval pattern against 
			   file and image values*/
			if (constraint_vars[index].flags & NVRAM_CONS_PARTIAL_MATCH){
				sec_fail=( !strstr(ptr,constraint_vars[index].altval) && 
							!strstr(var,constraint_vars[index].altval));		
			}
			else if (constraint_vars[index].flags & NVRAM_CONS_ALT_MATCH){
				sec_fail=(strncmp(ptr,constraint_vars[index].altval,comparesize));
			}
			else sec_fail =1;
			
		if (sec_fail){
			snprintf(posterr_msg,ERR_MSG_SIZE,
				"NVRAM constraint mismatch FILE:<b>%s=%s</b>  IMAGE:<b>%s=%s</b><br>",
					constraint_vars[index].name,ptr,constraint_vars[index].name,var);
			goto do_nvramul_post_cleanup0;	
			}
		}
	
	}

	/* Process the NVRAM payload 
	 *
	 * Remove the carriage returns at the end of the NVRAM variables 
	 *
	*/
	
	cur_entry = NVRAM_HEADER_LINECOUNT(nvram_file_header) + NVRAM_HEADER_LINECOUNT(constraint_vars);
	
	slen=0;
	for (index = cur_entry ;(index < numlines) && *name; index++,name += slen + 1){
		int offset;
		char *ptr=NULL;		
		char *varname=NULL;
		char *myptr=NULL;
		struct variable *v=NULL;
	
		offset = index * NVRAM_MAX_STRINGSIZE;
		
		/* The length of the actual var must be saved here as the subsequent
		 * manipulation using strsep() changes the string buffer.
		 * This to ensure that the buffer (tmpbuf->buf) is correctly procesed
		 * This buffer separates the individual AVPs using a null character
		 */
		  
		slen =strlen(name);
		ptr = name;
		
		/* Remove the CR at the end of the string */
		ptr[slen-1] = '\0';
		
		/* Look for tag that indicates that the value is encrypted */
		if  (*ptr==NVRAM_ENCTAG){
			char buf[NVRAM_MAX_STRINGSIZE];
			int bufsize = sizeof(buf);
			char *varname=NULL;
		
			varname=strsep(&ptr, "=");
		
			/* Increment pointer to move past tag */
			varname++;

			if (!decrypt_var(varname,ptr,strlen(ptr),buf,&bufsize,key,sizeof(key))){
				snprintf(posterr_msg,ERR_MSG_SIZE,
					"Error decrypting value %s at line %d<br>",ptr,index);
				goto do_nvramul_post_cleanup0;	
			}
			
		snprintf(name,NVRAM_MAX_STRINGSIZE,"%s=%s",varname,buf);

		}
				
		/* 
		   Write out NVRAM variables.
		*/
		
		myptr = name;
		varname=strsep(&myptr, "=");
		
		v=get_var_handle(varname);
		
		/* Restore only those NVRAM vars in the validation table 
		   Ignore those with the obvious flag set
		*/
		
		if (!v) 
			continue;
		
		if (v->ezc_flags & NVRAM_IGNORE)
			continue;
			
		if (nvram_set(varname,myptr)) 
			goto do_nvramul_post_cleanup0;
			
	}
	
	nvram_commit();

	/* We are done */
	ret_code = 0;
	
do_nvramul_post_cleanup0:
	/* Clear up any outstanding stuff */
	/* Slurp anything remaining in the request */
	while (len--)
		(void) fgetc(stream);
	
	if (tmpbuf) free(tmpbuf);

	return;
}

static void
do_nvramul_cgi(char *url, FILE *stream)
{
	assert(stream);
	assert(url);
	
	websHeader(stream);
	websWrite(stream, (char_t *) apply_header);
	
	if (ret_code){
		websWrite(stream, "Error during NVRAM upload<br>");
		if (*posterr_msg){
			websWrite(stream, posterr_msg);
			memset(posterr_msg,0,ERR_MSG_SIZE);
		}
		
	} else websWrite(stream, "NVRAM upload complete.<br>Rebooting....<br>");

	websWrite(stream, (char_t *) apply_footer, "firmware.asp");
	websFooter(stream);
	websDone(stream, 200);
	
	/* Reboot if successful */
	if (ret_code == 0)
		sys_reboot();

}

/* 
   This routine validates and formats the NVRAM variable into the file output format.
   If the variable is not in the monster V-block or its allowed multi instance variants
   it is dropped 
   
   Returns number of characters printed or -1 on error
*/
static int
save_nvram_var(char *name, char *var_val,char *buf, int buflen,char *key, int keylen)
{
	char tmp[NVRAM_MAX_STRINGSIZE];
	int len=sizeof(tmp);
	int retval=0;
	struct variable *v = NULL; 
	
	assert(name);
	assert(buf);
	assert(key);
	assert(buflen);
	assert(keylen);
	

	/* If var_val is null, this forces the variable to be unset when the file is uploaded 
	  If the variable is supposed to be encrypted but is null, skip it and do not 
	  mark the string as encrypted.
				  
	  If var_val is null the variable does not exist. Skip and do not save in that case.
	*/
	
	v = get_var_handle(name);
	
	if (!v) 
		return 0;
	
	if (v->ezc_flags & NVRAM_IGNORE)
		return 0;
	
	if (var_val) {	
			if (strlen(var_val) > NVRAM_MAX_STRINGSIZE){
				cprintf("get_nvram_var():String too long Len=%d String=%s\n",	strlen(var_val) ,var_val);	
				return -1;
			}
			
			if ( (v->ezc_flags & NVRAM_ENCRYPT) && (*var_val) ){
				var_val = encrypt_var(name,var_val,strlen(var_val),tmp,&len,key,keylen);
					
				if (!var_val){
					cprintf("get_nvram_var():Error encrypting %s\n",name);
					return -1;
				}
					
				retval=snprintf(buf,buflen,"%c%s=%s\n",
						NVRAM_ENCTAG,
						(char_t *)name,
						(char_t *)var_val);
		    } else retval=snprintf(buf,buflen,"%s=%s\n",
						(char_t *)name,
						(char_t *)var_val);			
	}
	return retval;
}

/* This is the cgi handler for the NVRAM download function
 * Inputs: -url of the calling file (not used but HTTPD expects this form)
 *         -Pointer to the post buffer
 *	
 *	   
 *  Returns: None	  
*/
static void
do_nvramdl_cgi(char *url, FILE *stream)
{
	char checksum[NVRAM_SHA1BUFSIZE];
	char passphrase[]=NVRAM_PASSPHRASE;
	char salt[NVRAM_SALTSIZE];
	char key[NVRAM_FILEKEYSIZE];
	int entries;
	char tmp[NVRAM_MAX_STRINGSIZE],tmp1[NVRAM_MAX_STRINGSIZE];
	char tmp_buf[NVRAM_MAX_STRINGSIZE];
	char *buf=NULL;
	char *var_val=NULL;
	char *var_name=NULL;
	char *name=NULL;
	char *ptr=NULL;
	int index;
	int retval;
	upload_constraints constraint_vars [] = NVRAM_CONSTRAINT_VARS;
	char nvram_file_header[][NVRAM_MAX_STRINGSIZE/2] = NVRAM_FILEHEADER;
	struct pb { 
			char header[NVRAM_HEADER_LINECOUNT(nvram_file_header)][NVRAM_MAX_STRINGSIZE];
			char buf[NVRAM_SPACE];
		 } *post_buf=NULL;

	int offset = 0;
	
	assert(stream);
	
	post_buf = (struct pb *)malloc(sizeof (struct pb));
	
	if (!post_buf) {
		cprintf("do_nvramdl_cgi():Error allocating %d bytes for post_buf\n",
						sizeof (struct pb));
		goto do_nvramdl_cgi_error;
	}
	
	buf = (char *)malloc(NVRAM_SPACE);
	
	if (!buf) {
		cprintf("do_nvramdl_cgi():Error allocating %d bytes for buf\n",
						NVRAM_SPACE);
		goto do_nvramdl_cgi_error;
	}
	
	memset (post_buf,0,sizeof(struct pb));
	memset (buf,0,NVRAM_SPACE);

	assert(stream);
	
	entries = NVRAM_HEADER_LINECOUNT(nvram_file_header);
	memset(tmp_buf,0,sizeof(tmp_buf));
	
	memset(salt,0,sizeof(salt));

	srand(time((time_t *)NULL));
	for (index = 0 ; index < 30 ; index ++) rand();
	index =rand();
	memcpy(&salt[sizeof(index)],&index,sizeof(index));
	for (index = 0 ; index < 30 ; index ++) rand();
	index =rand();
	memcpy(&salt,&index,sizeof(index));

	/*
	   The first entry of the file is the number of variables
	   The second entry is the offset to the start of the NVRAM variables 
	   The third entry is the SHA1 checksum
	*/

	/* PopulateHeader info */
	offset = 0;
	for  (index =0 ;*constraint_vars[index].name;index++){
		entries++;
		var_val=constraint_vars[index].get(constraint_vars[index].name);
		snprintf(tmp_buf,NVRAM_MAX_STRINGSIZE,"%s=%s\n",
					constraint_vars[index].name,
					(var_val) ? var_val : "unknown");
		if (!offset) cprintf("Post_buf address = %p\n",&post_buf->buf[offset]);
		offset = add_string(&post_buf->buf[offset],tmp_buf,offset,sizeof(struct pb));
		if (offset < 0){
			cprintf("httpd: Error Adding NVRAM header info.\n");
			goto do_nvramdl_cgi_error;
		}
	};
	
	memset(tmp,0,sizeof(tmp));

	fPRF(passphrase,strlen(passphrase),NULL,0,
			(unsigned char*)salt,sizeof(salt),key,sizeof(key));	
	
	/* Plug in filler for checksum */
	snprintf(post_buf->header[NVRAM_CHECKSUM_LINENUM],NVRAM_MAX_STRINGSIZE,"%s\n",NVRAM_CHECKSUM_FILLER);
	/* Grab the NVRAM buffer */

	nvram_getall(buf, NVRAM_SPACE);
	
	for(name = buf; *name; name += strlen(name) + 1){
		var_val =tmp;
		strncpy(tmp,name,sizeof(tmp));
		var_name=strsep(&var_val,"=");
		retval = save_nvram_var(var_name,var_val,tmp_buf,
						sizeof(tmp_buf),key,sizeof(key));
						
		if (retval <0){
			cprintf("httpd: Error Adding NVRAM variable info.\n");
			goto do_nvramdl_cgi_error;
		};
		
		if  (retval > 0 ) {
			entries++;
			offset = add_string(&post_buf->buf[offset],tmp_buf,offset,sizeof(struct pb));
			if (offset < 0){
				cprintf("httpd: Error Adding NVRAM variable info.\n");
				goto do_nvramdl_cgi_error;
			}
		};
		
		if (offset > NVRAM_SPACE) {
			cprintf("httpd: NVRAM_SPACE of %d (%d) exceeded\n",NVRAM_SPACE,offset);
			goto do_nvramdl_cgi_error;
		};
	}
	

	/* Add the header info */
	snprintf(post_buf->header[NVRAM_LINECOUNT_LINENUM],NVRAM_MAX_STRINGSIZE,"%s=%d\n",
					nvram_file_header[NVRAM_LINECOUNT_LINENUM],entries);
	
	/*Generate the hash */

	memset(checksum,0,sizeof(checksum));
	hmac_sha1((unsigned char*)post_buf,sizeof(struct pb),key,sizeof(key),checksum);
		
	memcpy(tmp,checksum,NVRAM_HASHSIZE);
	memcpy(&tmp[NVRAM_HASHSIZE],salt,NVRAM_SALTSIZE);

	ptr = b64_encode(tmp,NVRAM_FILECHKSUM_SIZE, buf, NVRAM_SPACE );
	
	if (!ptr){
		cprintf("do_nvramdl_cgi():Error performing base-64 encode of NVRAM checksum.\n");
		goto do_nvramdl_cgi_error;
	}
	
	strncpy(tmp1,ptr,NVRAM_MAX_STRINGSIZE);

	snprintf(post_buf->header[NVRAM_CHECKSUM_LINENUM],NVRAM_MAX_STRINGSIZE,"%s=%s\n",
					nvram_file_header[NVRAM_CHECKSUM_LINENUM],tmp1);
	/* Write out header */
	for (index =0; index < NVRAM_HEADER_LINECOUNT(nvram_file_header) ; index ++)
		websWrite(stream, "%s", post_buf->header[index]);
		
	/* Write out rest of file */

	for(name = post_buf->buf; *name; name += strlen(name) + 1){
		/*cprintf("Val->%s\n",name);*/
		websWrite(stream, "%s", name);
		};
	
do_nvramdl_cgi_error:

	if (post_buf) 
		free(post_buf);
	if (buf)
		free(buf);
		
	websDone(stream, 200);
	
	return;
}


/* 
 * This routine takes the cipher text ctext and produces the plaintext ptext
 * using the provided key.
 *
 * This routine accepts the cipher text  in the MIME Base64 format.
 *
 * The plain text is 8 bytes less than the ciphertext
 */
static char*
decrypt_var(char *varname,char *ctext, int ctext_len, char *ptext, int *ptext_len,char *key, int keylen)
{
	unsigned char tmp[NVRAM_MAX_STRINGSIZE];
	int len;
	char *end=NULL;
	
	assert(ptext);
	assert(ctext);
	assert(ptext_len);
	assert(key);
	if (keylen < 1 ) return NULL;

	if (ctext_len > NVRAM_MAX_STRINGSIZE){
		cprintf("decrypt_var():Encrypted string is too long MAXSTRINGSIZE=%d Strlen=%d\n",
							NVRAM_MAX_STRINGSIZE,ctext_len);
	}

	/* Cipher text must be more than 8 chars for this aes_unwrap() routine to work*/
	if (ctext_len < 8) return NULL;
	
	len=b64_decode(ctext,tmp,NVRAM_MAX_STRINGSIZE);
	
	if (!len) return NULL;
	
	if (aes_unwrap(keylen,key,len,tmp,ptext)) return NULL;
	
	*ptext_len = len - 8;
	
	end = strstr(ptext,varname);
	
	if (end) 
		(*end)= '\0';
	else
		return NULL;
	
	return ptext;
}
/* 
 * This routine takes the plaintext text ptext and produces the ciphertext ctext
 * using the provided key.
 *
 * It accepts the plaintext in binary form and produces ciphertext in MIME Base64 format.
 *
 * 
 */
static char* 
encrypt_var(char *varname,char *ptext, int ptext_len, char *ctext, int *ctext_len,char *key, int keylen)
{
	unsigned char tmp[NVRAM_MAX_STRINGSIZE];
	char *buf=NULL;
	int newlen;
	int varname_len;
	
	assert(ptext);
	assert(ctext);
	assert(ctext_len);
	assert(key);
	if (keylen < 1 ) return NULL;
	if (ptext_len < 1) return NULL;
	
	varname_len = strlen (varname);
	 
	 /* Include the NULL at the end */
	newlen = ptext_len + varname_len + 1;
	/* Align the incoming buffer to AES block length boundaries */
	if (newlen % AES_BLOCK_LEN)
		newlen = (1 + newlen /AES_BLOCK_LEN ) * AES_BLOCK_LEN;

	/* Do a string length check. When the binary string is base-64 encoded
	   it becomes 30% larger as every 3 bytes are represented by 4 ascii
	   characters */
	  
	if ( (4*(1+(newlen+8)/3)) > NVRAM_MAX_STRINGSIZE ) 
	{
		cprintf("encrypt_var():The encrypted string is too long. MAXSTRINGSIZE=%d Strlen=%d\n",
				NVRAM_MAX_STRINGSIZE,(4*(1+(newlen+8)/3)));
		return NULL;
	}
		
	buf  = malloc (newlen);
	if (!buf) return NULL;
	memset(buf,0,newlen);
	memcpy(buf,ptext,ptext_len);
	memcpy(buf + ptext_len,varname,varname_len);
			
	if (aes_wrap(keylen,key,newlen,buf,ctext)){
		if (buf) free(buf);
	 	return NULL;
	};
	
	if (buf) free(buf);
	
	buf = b64_encode(ctext,newlen+8,tmp,NVRAM_MAX_STRINGSIZE-(ptext_len + 1));

	if (buf){ 
		strncpy(ctext,buf,NVRAM_MAX_STRINGSIZE);
		*ctext_len = strlen(ctext) ;
		return ctext;
	}else{
		cprintf("encrypt_var():base-64 encode error\n");
		*ctext_len = 0;
		return NULL;
	}
}


static void
do_wireless_asp(char *url, FILE *stream)
{
	char *path=NULL, *query=NULL;
	
	assert(stream);
	assert(url);
	
	/* Parse path */
	query = url;
	path = strsep(&query, "?") ? : url;

	copy_wl_index_to_unindex(stream, NULL, NULL, 0, url, path, query);

	/* Reset CGI */
	init_cgi(NULL);
}

static void
do_security_asp(char *url, FILE *stream)
{
	char *path=NULL, *query=NULL;

	assert(stream);
	assert(url);
	
	/* Parse path */
	query = url;
	path = strsep(&query, "?") ? : url;

	copy_wl_index_to_unindex(stream, NULL, NULL, 0, url, path, query);

	/* Reset CGI */
	init_cgi(NULL);
}

static void
do_internal_asp(char *url, FILE *stream)
{
	char *path=NULL, *query=NULL;

	assert(stream);
	assert(url);
	
	/* Parse path */
	query = url;
	path = strsep(&query, "?") ? : url;

	copy_wl_index_to_unindex(stream, NULL, NULL, 0, url, path, query);

	/* Reset CGI */
	init_cgi(NULL);
}

#ifdef __CONFIG_NAT__
static void
do_wan_asp(char *url, FILE *stream)
{
	char *path=NULL, *query=NULL;

	assert(stream);
	assert(url);
	
	/* Parse path */
	query = url;
	path = strsep(&query, "?") ? : url;

	wan_asp(stream, NULL, NULL, 0, url, path, query);

	/* Reset CGI */
	init_cgi(NULL);
}
#endif	/* __CONFIG_NAT__ */

struct mime_handler mime_handlers[] = {
#ifdef __CONFIG_NAT__
	{ "wan.asp", "text/html", no_cache, do_apply_post, do_wan_asp, do_auth },
#endif	/* __CONFIG_NAT__ */
	{ "wireless.asp", "text/html", no_cache, do_apply_post, do_wireless_asp, do_auth },
	{ "security.asp", "text/html", no_cache, do_apply_post, do_security_asp, do_auth },
	{ "internal.asp", "text/html", no_cache, do_apply_post, do_internal_asp, do_auth },
#ifdef __CONFIG_EZC__
	{ "ezconfig.asp", "text/html", ezc_version, do_apply_ezconfig_post, do_ezconfig_asp, do_auth },
#endif /* __CONFIG_EZC__ */
	{ "**.asp", "text/html", no_cache, NULL, do_ej, do_auth },
	{ "**.css", "text/css", NULL, NULL, do_file, do_auth },
	{ "**.gif", "image/gif", NULL, NULL, do_file, do_auth },
	{ "**.jpg", "image/jpeg", NULL, NULL, do_file, do_auth },
	{ "**.js", "text/javascript", NULL, NULL, do_file, do_auth },
	{ "apply.cgi*", "text/html", no_cache, do_apply_post, do_apply_cgi, do_auth },
	{ "upgrade.cgi*", "text/html", no_cache, do_upgrade_post, do_upgrade_cgi, do_auth },
	/* set MIME type to NULL to override the one built into the webserver. download_hdr
	   defines its content type
	*/
	{ "nvramdl.cgi*", NULL, download_hdr, NULL, do_nvramdl_cgi, do_auth },
	{ "nvramul.cgi*", NULL, "text/html", do_nvramul_post,do_nvramul_cgi , do_auth },
	{ NULL, NULL, NULL, NULL, NULL, NULL }
};

struct ej_handler ej_handlers[] = {
	{ "nvram_get", ej_nvram_get },
	{ "nvram_match", ej_nvram_match },
	{ "nvram_invmatch", ej_nvram_invmatch },
	{ "nvram_list", ej_nvram_list },
	{ "nvram_inlist", ej_nvram_inlist },
	{ "nvram_invinlist", ej_nvram_invinlist },
#ifdef __CONFIG_NAT__
	{ "wan_list", ej_wan_list },
	{ "wan_iflist", ej_wan_iflist },
	{ "wan_route", ej_wan_route },
	{ "wan_link", ej_wan_link },
	{ "wan_lease", ej_wan_lease },
	{ "filter_client", ej_filter_client },
	{ "forward_port", ej_forward_port },
	{ "autofw_port", ej_autofw_port },
#endif	/*  __CONFIG_NAT__ */
	{ "localtime", ej_localtime },
	{ "sysuptime", ej_sysuptime },
	{ "dumplog", ej_dumplog },
	{ "syslog", ej_syslog },
	{ "wl_list", ej_wl_list },
	{ "wl_guest_ssid", ej_wl_guest_ssid},
	{ "wl_phytypes", ej_wl_phytypes },
	{ "wl_radioid", ej_wl_radioid },
	{ "wl_corerev", ej_wl_corerev },
	{ "wl_cur_channel", ej_wl_cur_channel },
	{ "wl_cur_phytype", ej_wl_cur_phytype },
	{ "wl_cur_country", ej_wl_cur_country },
	{ "wl_country_list", ej_wl_country_list },
	{ "wl_channel_list", ej_wl_channel_list },
	{ "wl_auth_list", ej_wl_auth_list },
	{ "wl_mode_list", ej_wl_mode_list },
	{ "wl_inlist", ej_wl_inlist },
	{ "wl_wds_status", ej_wl_wds_status },
	{ "wl_radio_roam_option", ej_wl_radio_roam_option},
	{ "wl_radio_roam_option", ej_wl_radio_roam_option},
	{ "wl_ure_list", ej_ure_list },
	{ "wl_ure_enabled", ej_ure_enabled },
	{ "ses_button_display", ej_ses_button_display},
	{ "ses_cl_button_display", ej_ses_cl_button_display},
	{ "lan_route", ej_lan_route },
	{ "lan_guest_iflist", ej_lan_guest_iflist },
	{ "lan_leases", ej_lan_leases },
	{ "asp_list", ej_asp_list },
	{ "wme_match_op", ej_wme_match_op },
	{ NULL, NULL }
};

#endif /* !WEBS */

/*
 * Country names and abbreviations from ISO 3166
 */
country_name_t country_names[] = {

{"COUNTRY Z1",		 "Z1"},
{"COUNTRY Z2",		 "Z2"},
{"AFGHANISTAN",		 "AF"},
{"ALBANIA",		 "AL"},
{"ALGERIA",		 "DZ"},
{"AMERICAN SAMOA", 	 "AS"},
{"ANDORRA",		 "AD"},
{"ANGOLA",		 "AO"},
{"ANGUILLA",		 "AI"},
{"ANTARCTICA",		 "AQ"},
{"ANTIGUA AND BARBUDA",	 "AG"},
{"ARGENTINA",		 "AR"},
{"ARMENIA",		 "AM"},
{"ARUBA",		 "AW"},
{"ASCENSION ISLAND",		 "AC"},
{"ASHMORE AND BARBUDA",		 "AG"},
{"AUSTRALIA",		 "AU"},
{"AUSTRIA",		 "AT"},
{"AZERBAIJAN",		 "AZ"},
{"BAHAMAS",		 "BS"},
{"BAHRAIN",		 "BH"},
{"BAKER ISLAND",		 "Z2"},
{"BANGLADESH",		 "BD"},
{"BARBADOS",		 "BB"},
{"BELARUS",		 "BY"},
{"BELGIUM",		 "BE"},
{"BELIZE",		 "BZ"},
{"BENIN",		 "BJ"},
{"BERMUDA",		 "BM"},
{"BHUTAN",		 "BT"},
{"BOLIVIA",		 "BO"},
{"BOSNIA AND HERZEGOVINA","BA"},
{"BOTSWANA",		 "BW"},
{"BOUVET ISLAND",	 "BV"},
{"BRAZIL",		 "BR"},
{"BRITISH INDIAN OCEAN TERRITORY", 	"IO"},
{"BRUNEI DARUSSALAM",	 "BN"},
{"BULGARIA",		 "BG"},
{"BURKINA FASO",	 "BF"},
{"BURUNDI",		 "BI"},
{"CAMBODIA",		 "KH"},
{"CAMEROON",		 "CM"},
{"CANADA",		 "CA"},
{"CAPE VERDE",		 "CV"},
{"CAYMAN ISLANDS",	 "KY"},
{"CENTRAL AFRICAN REPUBLIC","CF"},
{"CHAD",		 "TD"},
{"CHANNEL ISLANDS",		 "Z1"},
{"CHILE",		 "CL"},
{"CHINA",		 "CN"},
{"CHRISTMAS ISLAND",	 "CX"},
{"CLIPPERTON ISLAND",	 "CP"},
{"COCOS (KEELING) ISLANDS","CC"},
{"COLOMBIA",		 "CO"},
{"COMOROS",		 "KM"},
{"CONGO",		 "CG"},
{"CONGO, THE DEMOCRATIC REPUBLIC OF THE", "CD"},
{"COOK ISLANDS",	 "CK"},
{"COSTA RICA",		 "CR"},
{"COTE D'IVOIRE",	 "CI"},
{"CROATIA",		 "HR"},
{"CUBA",		 "CU"},
{"CYPRUS",		 "CY"},
{"CZECH REPUBLIC",	 "CZ"},
{"DENMARK",		 "DK"},
{"DJIBOUTI",		 "DJ"},
{"DOMINICA",		 "DM"},
{"DOMINICAN REPUBLIC", 	 "DO"},
{"ECUADOR",		 "EC"},
{"EGYPT",		 "EG"},
{"EL SALVADOR",		 "SV"},
{"EQUATORIAL GUINEA",	 "GQ"},
{"ERITREA",		 "ER"},
{"ESTONIA",		 "EE"},
{"ETHIOPIA",		 "ET"},
{"FALKLAND ISLANDS (MALVINAS)",	"FK"},
{"FAROE ISLANDS",	 "FO"},
{"FIJI",		 "FJ"},
{"FINLAND",		 "FI"},
{"FRANCE",		 "FR"},
{"FRENCH GUIANA",	 "GF"},
{"FRENCH POLYNESIA",	 "PF"},
{"FRENCH SOUTHERN TERRITORIES",	 "TF"},
{"GABON",		 "GA"},
{"GAMBIA",		 "GM"},
{"GEORGIA",		 "GE"},
{"GERMANY",		 "DE"},
{"GHANA",		 "GH"},
{"GIBRALTAR",		 "GI"},
{"GREECE",		 "GR"},
{"GREENLAND",		 "GL"},
{"GRENADA",		 "GD"},
{"GUADELOUPE",		 "GP"},
{"GUAM",		 "GU"},
{"GUANTANAMO BAY",		 "Z1"},
{"GUATEMALA",		 "GT"},
{"GUERNSEY",		 "GG"},
{"GUINEA",		 "GN"},
{"GUINEA-BISSAU",	 "GW"},
{"GUYANA",		 "GY"},
{"HAITI",		 "HT"},
{"HEARD ISLAND AND MCDONALD ISLANDS",	"HM"},
{"HOLY SEE (VATICAN CITY STATE)", 	"VA"},
{"HONDURAS",		 "HN"},
{"HONG KONG",		 "HK"},
{"HOWLAND ISLAND",		 "Z2"},
{"HUNGARY",		 "HU"},
{"ICELAND",		 "IS"},
{"INDIA",		 "IN"},
{"INDONESIA",		 "ID"},
{"IRAN, ISLAMIC REPUBLIC OF",		"IR"},
{"IRAQ",		 "IQ"},
{"IRELAND",		 "IE"},
{"ISRAEL",		 "IL"},
{"ITALY",		 "IT"},
{"JAMAICA",		 "JM"},
{"JAN MAYEN AMAICA",		 "Z1"},
{"JAPAN",		 "JP"},
{"JAPAN_1",		 "J1"},
{"JAPAN_2",		 "J2"},
{"JAPAN_3",		 "J3"},
{"JAPAN_4",		 "J4"},
{"JAPAN_5",		 "J5"},
{"JAPAN_6",		 "J6"},
{"JAPAN_7",		 "J7"},
{"JAPAN_8",		 "J8"},
{"JARVIS ISLAND",		 "Z2"},
{"JERSEY",		 "JE"},
{"JOHNSTON ATOLL",		 "Z2"},
{"JORDON",		 "JO"},
{"KAZAKHSTAN",		 "KZ"},
{"KENYA",		 "KE"},
{"KINGMAN REEF",		 "Z2"},
{"KIRIBATI",		 "KI"},
{"KOREA, DEMOCRATIC PEOPLE'S REPUBLIC OF", "KP"},
{"KOREA, REPUBLIC OF",	 "KR"},
{"KUWAIT",		 "KW"},
{"KYRGYZSTAN",		 "KG"},
{"LAO PEOPLE'S DEMOCRATIC REPUBLIC", 	"LA"},
{"LATVIA",		 "LV"},
{"LEBANON",		 "LB"},
{"LESOTHO",		 "LS"},
{"LIBERIA",		 "LR"},
{"LIBYAN ARAB JAMAHIRIYA","LY"},
{"LIECHTENSTEIN",	 "LI"},
{"LITHUANIA",		 "LT"},
{"LUXEMBOURG",		 "LU"},
{"MACAO",		 "MO"},
{"MACEDONIA, THE FORMER YUGOSLAV REPUBLIC OF",	 "MK"},
{"MADAGASCAR",		 "MG"},
{"MALAWI",		 "MW"},
{"MALAYSIA",		 "MY"},
{"MALDIVES",		 "MV"},
{"MALI",		 "ML"},
{"MALTA",		 "MT"},
{"MAN, ISLE OF",		 "IM"},
{"MARSHALL ISLANDS",	 "MH"},
{"MARTINIQUE",		 "MQ"},
{"MAURITANIA",		 "MR"},
{"MAURITIUS",		 "MU"},
{"MAYOTTE",		 "YT"},
{"MEXICO",		 "MX"},
{"MICRONESIA, FEDERATED STATES OF", 	"FM"},
{"MIDWAY ISLANDS", 	"Z2"},
{"MOLDOVA, REPUBLIC OF", "MD"},
{"MONACO",		 "MC"},
{"MONGOLIA",		 "MN"},
{"MONTSERRAT",		 "MS"},
{"MOROCCO",		 "MA"},
{"MOZAMBIQUE",		 "MZ"},
{"MYANMAR",		 "MM"},
{"NAMIBIA",		 "NA"},
{"NAURU",		 "NR"},
{"NEPAL",		 "NP"},
{"NETHERLANDS",		 "NL"},
{"NETHERLANDS ANTILLES", "AN"},
{"NEW CALEDONIA",	 "NC"},
{"NEW ZEALAND",		 "NZ"},
{"NICARAGUA",		 "NI"},
{"NIGER",		 "NE"},
{"NIGERIA",		 "NG"},
{"NIUE",		 "NU"},
{"NORFOLK ISLAND",	 "NF"},
{"NORTHERN MARIANA ISLANDS","MP"},
{"NORWAY",		 "NO"},
{"OMAN",		 "OM"},
{"PAKISTAN",		 "PK"},
{"PALAU",		 "PW"},
{"PALESTINIAN TERRITORY, OCCUPIED", 	"PS"},
{"PALMYRA ATOLL", 	"Z2"},
{"PANAMA",		 "PA"},
{"PAPUA NEW GUINEA",	 "PG"},
{"PARAGUAY",		 "PY"},
{"PERU",		 "PE"},
{"PHILIPPINES",		 "PH"},
{"PITCAIRN",		 "PN"},
{"POLAND",		 "PL"},
{"PORTUGAL",		 "PT"},
{"PUERTO RICO",		 "PR"},
{"QATAR",		 "QA"},
{"REUNION",		 "RE"},
{"ROMANIA",		 "RO"},
{"ROTA ISLAND",		 "Z1"},
{"RUSSIAN FEDERATION",	 "RU"},
{"RWANDA",		 "RW"},
{"SAINT HELENA",	 "SH"},
{"SAINT KITTS AND NEVIS","KN"},
{"SAINT LUCIA",		 "LC"},
{"SAINT PIERRE AND MIQUELON",	 	"PM"},
{"SAINT VINCENT AND THE GRENADINES", 	"VC"},
{"SAIPAN", 	"Z1"},
{"SAMOA",		 "WS"},
{"SAN MARINO",		 "SM"},
{"SAO TOME AND PRINCIPE","ST"},
{"SAUDI ARABIA",	 "SA"},
{"SENEGAL",		 "SN"},
{"SEYCHELLES",		 "SC"},
{"SIERRA LEONE",	 "SL"},
{"SINGAPORE",		 "SG"},
{"SLOVAKIA",		 "SK"},
{"SLOVENIA",		 "SI"},
{"SOLOMON ISLANDS",	 "SB"},
{"SOMALIA",		 "SO"},
{"SOUTH AFRICA",	 "ZA"},
{"SOUTH GEORGIA AND THE SOUTH SANDWICH ISLANDS", "GS"},
{"SPAIN",		 "ES"},
{"SRI LANKA",		 "LK"},
{"SUDAN",		 "SD"},
{"SURINAME",		 "SR"},
{"SVALBARD AND JAN MAYEN","SJ"},
{"SWAZILAND",		 "SZ"},
{"SWEDEN",		 "SE"},
{"SWITZERLAND",		 "CH"},
{"SYRIAN ARAB REPUBLIC", "SY"},
{"TAIWAN, PROVINCE OF CHINA", 		"TW"},
{"TAJIKISTAN",		 "TJ"},
{"TANZANIA, UNITED REPUBLIC OF",	"TZ"},
{"THAILAND",		 "TH"},
{"TIMOR-LESTE",		 "TL"},
{"TOGO",		 "TG"},
{"TOKELAU",		 "TK"},
{"TONGA",		 "TO"},
{"TRINIDAD AND TOBAGO",	 "TT"},
{"TRISTAN DA CUNHA",	 "TA"},
{"TUNISIA",		 "TN"},
{"TURKEY",		 "TR"},
{"TURKMENISTAN",	 "TM"},
{"TURKS AND CAICOS ISLANDS",		"TC"},
{"TUVALU",		 "TV"},
{"UGANDA",		 "UG"},
{"UKRAINE",		 "UA"},
{"UNITED ARAB EMIRATES", "AE"},
{"UNITED KINGDOM",	 "GB"},
{"UNITED STATES",	 "US"},
{"UNITED STATES MINOR OUTLYING ISLANDS","UM"},
{"URUGUAY",		 "UY"},
{"UZBEKISTAN",		 "UZ"},
{"VANUATU",		 "VU"},
{"VENEZUELA",		 "VE"},
{"VIET NAM",		 "VN"},
{"VIRGIN ISLANDS, BRITISH", "VG"},
{"VIRGIN ISLANDS, U.S.", "VI"},
{"WALLIS AND FUTUNA",	 "WF"},
{"WESTERN SAHARA", 	 "EH"},
{"YEMEN",		 "YE"},
{"YUGOSLAVIA",		 "YU"},
{"ZAMBIA",		 "ZM"},
{"ZIMBABWE",		 "ZW"},
{"ALL",		 	 "ALL"},
{"RADAR CHANNELS",	 "RDR"},
{NULL, 			 NULL}
};

