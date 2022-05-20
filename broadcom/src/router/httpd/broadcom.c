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
 * Copyright 2004, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 * $Id: broadcom.c,v 1.1.1.1 2006/04/25 06:09:26 jiahao_jhou Exp $
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


#include <typedefs.h>
#include <proto/ethernet.h>
#include <bcmnvram.h>
#include <bcmutils.h>
#include <shutils.h>
#include <netconf.h>
#include <nvparse.h>
#include <wlutils.h>

static char * rfctime(const time_t *timep);
static char * reltime(unsigned int seconds);

#define wan_prefix(unit, prefix)	snprintf(prefix, sizeof(prefix), "wan%d_", unit)

/* For Backup/Restore settings */
#define BACKUP_SETTING_FILENAME	"s5config.dat"

/*
 * Country names and abbreviations from ISO 3166
 */
typedef struct {
	char *name;     /* Long name */
	char *abbrev;   /* Abbreviation */
} country_name_t;
country_name_t country_names[];     /* At end of this file */

static int ezc_error = 0;

struct variable {
	char *name;
	char *longname;
	void (*validate)(webs_t wp, char *value, struct variable *v);
	char **argv;
	int nullok;
	int ezc_flags;
};

#define WIF "eth1"
char buf[WLC_IOCTL_MAXLEN];

struct variable variables[];
extern struct nvram_tuple router_defaults[];

#define ARGV(args...) ((char *[]) { args, NULL })
#define XSTR(s) STR(s)
#define STR(s) #s

enum {
	NOTHING,
	REBOOT,
	RESTART,
};

#define EZC_FLAGS_READ		0x0001
#define EZC_FLAGS_WRITE		0x0002
#define EZC_FLAGS_CRYPT		0x0004

#define EZC_CRYPT_KEY		"620A83A6960E48d1B05D49B0288A2C1F"

#define EZC_SUCCESS	 	0
#define EZC_ERR_NOT_ENABLED 	1
#define EZC_ERR_INVALID_STATE 	2
#define EZC_ERR_INVALID_DATA 	3
#ifndef NOUSB
static const char * const apply_header =
"<head>"
"<title>Broadcom Home Gateway Reference Design: Apply</title>"
"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">"
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
"<p class=\"label\">&#169;2001-2004 ASUSTek Inc.. All rights reserved.</p>"
"</body>"
;
#endif

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
	cprintf("Upgrading");
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

#endif

int 
sys_send_signal(char *pidfile, int sig)
{

	FILE *fp;
	pid_t pid;	    
	fp=fopen(pidfile,"r");	    
	if (fp!=NULL)
	{
	    	fscanf(fp, "%d", &pid);
	    	kill(pid, sig);
	    	fclose(fp);	 
		return 0;
	}
	return 1;
}

void
sys_refresh_lease(void)
{
	char sigusr1[] = "-XX";

	/* Write out leases file */
	sprintf(sigusr1, "-%d", SIGUSR1);
	eval("killall", sigusr1, "udhcpd");
}

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

struct lease_t {
	unsigned char chaddr[16];
	u_int32_t yiaddr;
	u_int32_t expires;
	char hostname[64];
};

/* Dump leases in <tr><td>hostname</td><td>MAC</td><td>IP</td><td>expires</td></tr> format */
int
ej_lan_leases(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fp = NULL;
	struct lease_t lease;
	int i;
	struct in_addr addr;
	unsigned long expires = 0;
	int ret = 0;

        ret += websWrite(wp, "Host Name       Mac Address       IP Address      Lease\n");
			                                                  
	/* Write out leases file */
	if (!(fp = fopen("/tmp/udhcpd-br0.leases", "r")))
		return ret;

	while (fread(&lease, sizeof(lease), 1, fp)) {
		/* Do not display reserved leases */
		if (ETHER_ISNULLADDR(lease.chaddr))
			continue;

		//printf("lease: %s %d\n", lease.hostname, strlen(lease.hostname));
		ret += websWrite(wp, "%-16s", lease.hostname);
		for (i = 0; i < 6; i++) {
			ret += websWrite(wp, "%02X", lease.chaddr[i]);
			if (i != 5) ret += websWrite(wp, ":");
		}
		addr.s_addr = lease.yiaddr;
		ret += websWrite(wp, " %-15s ", inet_ntoa(addr));
		expires = ntohl(lease.expires);

		if (expires==0xffffffff) ret += websWrite(wp, "Manual\n");
		else if (!expires) ret += websWrite(wp, "Expired\n");
		else ret += websWrite(wp, "%s\n", reltime(expires));
	}
	fclose(fp);

#ifdef GUEST_ACCOUNT
	if(nvram_invmatch("wl_guest_ENABLE", "1") || nvram_match("mssid_ENABLE", "1")) return ret;

	/* Write out leases file */
	if (!(fp = fopen("/tmp/udhcpd-br1.leases", "r")))
		return ret;

	while (fread(&lease, sizeof(lease), 1, fp)) {
		/* Do not display reserved leases */
		if (ETHER_ISNULLADDR(lease.chaddr))
			continue;

		//printf("lease: %s %d\n", lease.hostname, strlen(lease.hostname));
		ret += websWrite(wp, "%-16s", lease.hostname);
		for (i = 0; i < 6; i++) {
			ret += websWrite(wp, "%02X", lease.chaddr[i]);
			if (i != 5) ret += websWrite(wp, ":");
		}
		addr.s_addr = lease.yiaddr;
		ret += websWrite(wp, " %-15s ", inet_ntoa(addr));
		expires = ntohl(lease.expires);

		if (expires==0xffffffff) ret += websWrite(wp, "Manual\n");
		else if (!expires) ret += websWrite(wp, "Expired\n");
		else ret += websWrite(wp, "%s\n", reltime(expires));
	}
	fclose(fp);
#endif

	return ret;
}

/* Renew lease */
int
sys_renew(void)
{
	int unit;
	char tmp[100];
	char *str;
	int pid;

	if ((unit = atoi(nvram_safe_get("wan_unit"))) < 0)
		unit = 0;

#ifdef REMOVE	
	snprintf(tmp, sizeof(tmp), "/var/run/udhcpc%d.pid", unit);
	if ((str = file2str(tmp))) {
		pid = atoi(str);
		free(str);
		return kill(pid, SIGUSR1);
	}	
	return -1;
#else
	snprintf(tmp, sizeof(tmp), "wan_connect,%d", unit);
	nvram_set("rc_service", tmp);
	kill(1, SIGUSR1);
#endif
}

/* Release lease */
int
sys_release(void)
{
	int unit;
	char tmp[100];
	char *str;
	int pid;

	if ((unit = atoi(nvram_safe_get("wan_unit"))) < 0)
		unit = 0;
	
#ifdef REMOVE
	snprintf(tmp, sizeof(tmp), "/var/run/udhcpc%d.pid", unit);
	if ((str = file2str(tmp))) {
		pid = atoi(str);
		free(str);
		return kill(pid, SIGUSR2);
	}	
	return -1;
#else	
	snprintf(tmp, sizeof(tmp), "wan_disconnect,%d", unit);
	nvram_set("rc_service", tmp);
	kill(1, SIGUSR1);
#endif
}

#ifndef NOUSB
static int
wan_restore_mac(webs_t wp)
{
	char tmp[50], tmp2[50], prefix[] = "wanXXXXXXXXXX_", *t2;
	int unit, errf = -1;
	char wan_ea[ETHER_ADDR_LEN];

	unit = atoi(websGetVar(wp, "wan_unit", NULL));
	if (unit >= 0)
	{
		strcpy(tmp2, nvram_safe_get("wan_ifname"));
		if (!strncmp(tmp2, "eth", 3))
		{
			sprintf(tmp, "et%dmacaddr", atoi(tmp2 + 3));
			t2 = nvram_safe_get(tmp);
			if (t2 && t2[0] != 0)
			{
				ether_atoe(t2, wan_ea);
				ether_etoa(wan_ea, tmp2);
				wan_prefix(unit, prefix);
				nvram_set("wan_hwaddr", tmp2);
				nvram_set(strcat_r(prefix, "hwaddr", tmp), tmp2);
				nvram_commit();
				errf = 0;
			}
		}
	}

	return errf;
}

#define sin_addr(s) (((struct sockaddr_in *)(s))->sin_addr)

/* Return WAN link state */
static int
ej_wan_link(int eid, webs_t wp, int argc, char_t **argv)
{
	char *wan_ifname;
	int s;
	struct ifreq ifr;
	struct ethtool_cmd ecmd;
	FILE *fp;
	int unit;
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";

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
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";

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

/* Return a list of wan interfaces (eth0/eth1/eth2/eth3) */
static int
ej_wan_iflist(int eid, webs_t wp, int argc, char_t **argv)
{
	char name[IFNAMSIZ], *next;
	int ret = 0;
	int unit;
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";
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

#endif
#endif

#ifdef REMOVE
static int
ej_wl_parse_str(int eid, webs_t wp, int argc, char_t **argv) 
{
	char *var, *match, *next;
	int unit, val = 0;
	char tmp[100], prefix[] = "wlXXXXXXXXXX_";
	char *name;
	char str[100];

	if (ejArgs(argc, argv, "%s %s", &var, &match) < 1) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	if ((unit = atoi(nvram_safe_get("wl_unit"))) < 0)
		return -1;

	snprintf(prefix, sizeof(prefix), "wl%d_", unit);
	name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));

	if (wl_get_val(name, var, (void *)tmp, 100))
		return -1;

	foreach(str, tmp, next) {
		if (strncmp(str, match, sizeof(str)) == 0) {
			val = 1;
			break;
		}
	}

	return websWrite(wp, "%u", val);
}
#endif
/*
int
ej_wl_sta_status(int eid, webs_t wp, char *ifname)
{
	int ret, i;
	char bssid[32];
	char bssinfobuf[2000];
	wl_bss_info_t *info;
	int val;

	// Get bssid
	ret=wl_ioctl(ifname, WLC_GET_BSSID, bssid, sizeof(bssid));
	//wl_scan_results();

	if (ret==0 && !(bssid[0]==0&&bssid[1]==0&&bssid[2]==0
		&&bssid[3]==0&&bssid[4]==0&&bssid[5]==0)) 	
	{
		return(websWrite(wp, "Status	: Connect to %s\n", nvram_safe_get("wl0_ssid")));
	}
	return(websWrite(wp, "Status	: Connecting to %s\n", nvram_safe_get("wl0_ssid")));
}
*/
int
ej_wl_sta_status(int eid, webs_t wp, char *ifname, char *MAC, int *cf)
{
	int ret, i;
	char bssid[32];
	char bssinfobuf[2000];
//	wl_bss_info_t *info;
	wl_bss_info_107_t *info;
	int val;

	// Get bssid
	ret=wl_ioctl(ifname, WLC_GET_BSSID, bssid, sizeof(bssid));
	//wl_scan_results();

	if (ret==0 && !(bssid[0]==0&&bssid[1]==0&&bssid[2]==0
		&&bssid[3]==0&&bssid[4]==0&&bssid[5]==0)) 	
	{
		*cf=1;
		sprintf(MAC, "%02X:%02X:%02X:%02X:%02X:%02X", (unsigned char)bssid[0], (unsigned char)bssid[1], (unsigned char)bssid[2], (unsigned char)bssid[3], (unsigned char)bssid[4], (unsigned char)bssid[5]);
		return(websWrite(wp, "Status	: Connect to %s\n", nvram_safe_get("wl0_ssid")));
	}
	return(websWrite(wp, "Status	: Connecting to %s\n", nvram_safe_get("wl0_ssid")));
}

int
ej_wl_status(int eid, webs_t wp, int argc, char_t **argv)
{
	int unit;
	char tmp[100], prefix[] = "wlXXXXXXXXXX_";
	char *name;
	struct maclist *auth, *assoc, *authorized;
	int max_sta_count, maclist_size;
	int i, j, ret, val;	
	channel_info_t ci;
	char uppermac[17];
	int connected_flag=0;

	if ((unit = atoi(nvram_safe_get("wl_unit"))) < 0)
		return -1;

	snprintf(prefix, sizeof(prefix), "wl%d_", unit);
	name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));		
	
	wl_ioctl(name, WLC_GET_RADIO, &val, sizeof(val));

	if (val==1) 
	{
		ret+=websWrite(wp, "Radio is disabled\n");
		return ret;
	}
	
	wl_ioctl(name, WLC_GET_CHANNEL, &ci, sizeof(ci));


	if (nvram_match(strcat_r(prefix, "mode", tmp), "ap"))
	{
//		if (nvram_match("wl_lazywds", "1") || nvram_match("wl_wdsapply_x", "1"))
		if (nvram_match("wl0_lazywds", "1") || nvram_match("wl0_wdsapply_x", "1"))
			ret+=websWrite(wp, "Mode	: Hybrid\n");
		else    ret+=websWrite(wp, "Mode	: AP Only\n");

		ret+=websWrite(wp, "Channel	: %d\n", ci.target_channel);

	}
	else if (nvram_match(strcat_r(prefix, "mode", tmp), "wds"))
	{
		ret+=websWrite(wp, "Mode	: WDS Only\n");
		ret+=websWrite(wp, "Channel	: %d\n", ci.target_channel);
	}
	else if (nvram_match(strcat_r(prefix, "mode", tmp), "sta"))
	{
		if (nvram_match("wl_ure", "1") && nvram_match("wl0_ure", "1") && nvram_match("ure_disable", "0"))
			ret+=websWrite(wp, "Mode	: Repeater\n");
		else
			ret+=websWrite(wp, "Mode	: Adapter\n");
		ret+=websWrite(wp, "Channel	: %d\n", ci.target_channel);
		ret+=ej_wl_sta_status(eid, wp, name, &uppermac, &connected_flag);
		if (!(nvram_match("wl_ure", "1") && nvram_match("wl0_ure", "1") && nvram_match("ure_disable", "0")))
			return ret;
	}
	else if (nvram_match(strcat_r(prefix, "mode", tmp), "wet"))
	{
//		ret+=websWrite(wp, "Mode	: Ethernet Bridge\n");
		if (nvram_match("wl_ure", "1") && nvram_match("wl0_ure", "1") && nvram_match("ure_disable", "0"))
			ret+=websWrite(wp, "Mode	: Repeater\n");
		else
			ret+=websWrite(wp, "Mode	: Adapter\n");
		ret+=websWrite(wp, "Channel	: %d\n", ci.target_channel);
		ret+=ej_wl_sta_status(eid, wp, name, &uppermac, &connected_flag);
		if (!(nvram_match("wl_ure", "1") && nvram_match("wl0_ure", "1") && nvram_match("ure_disable", "0")))
			return ret;
	}	

	/* buffers and length */
	max_sta_count = 256;
	maclist_size = sizeof(auth->count) + max_sta_count * sizeof(struct ether_addr);

	auth = malloc(maclist_size);
	assoc = malloc(maclist_size);
	authorized = malloc(maclist_size);

	if (!auth || !assoc || !authorized)
		goto exit;

	/* query wl for authenticated sta list */
	strcpy((char*)auth, "authe_sta_list");
	if (wl_ioctl(name, WLC_GET_VAR, auth, maclist_size))
		goto exit;

	/* query wl for associated sta list */
	assoc->count = max_sta_count;
	if (nvram_match("wl_mode_EX", "re"))
	{
		strcpy((char*)assoc, "authe_sta_list");
		if (wl_ioctl(name, WLC_GET_VAR, assoc, maclist_size))
			goto exit;
	}
	else
	{
		if (wl_ioctl(name, WLC_GET_ASSOCLIST, assoc, maclist_size))
			goto exit;
	}

	/* query wl for authorized sta list */
	strcpy((char*)authorized, "autho_sta_list");
	if (wl_ioctl(name, WLC_GET_VAR, authorized, maclist_size))
		goto exit;

	ret+=websWrite(wp, "\n");
	ret+=websWrite(wp, "Stations List                           \n");
	ret+=websWrite(wp, "----------------------------------------\n");
	//             00:00:00:00:00:00 associated authorized

	/* build authenticated/associated/authorized sta list */
	for (i = 0; i < auth->count; i ++) {
		char ea[ETHER_ADDR_STR_LEN];

		if (connected_flag==1 && !strncmp(uppermac, ether_etoa((void *)&auth->ea[i], ea), 17))
			continue;

		ret+=websWrite(wp, "%s ", ether_etoa((void *)&auth->ea[i], ea));

		for (j = 0; j < assoc->count; j ++) {
			if (!bcmp((void *)&auth->ea[i], (void *)&assoc->ea[j], ETHER_ADDR_LEN)) {
				ret+=websWrite(wp, " associated");
				break;
			}
		}

		for (j = 0; j < authorized->count; j ++) {
			if (!bcmp((void *)&auth->ea[i], (void *)&authorized->ea[j], ETHER_ADDR_LEN)) {
				ret+=websWrite(wp, " authorized");
				break;
			}
		}
		ret+=websWrite(wp, "\n");
	}

	/* error/exit */
exit:
	if (auth) free(auth);
	if (assoc) free(assoc);
	if (authorized) free(authorized);
	
	return ret;
}

int
ej_getclientlist(int eid, webs_t wp, int argc, char_t **argv)
{
	int unit;
	char tmp[100], prefix[] = "wlXXXXXXXXXX_";
	char *name;
	struct maclist *auth, *assoc, *authorized;
	int max_sta_count, maclist_size;
	int i, j, val;
	int ret=0;

	int retval=0;
	char bssid[6];

	char uppermac[17];
	int connected_flag=0;

	if ((unit = atoi(nvram_safe_get("wl_unit"))) < 0)
		return 0;

	snprintf(prefix, sizeof(prefix), "wl%d_", unit);
	name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));		
	
	wl_ioctl(name, WLC_GET_RADIO, &val, sizeof(val));

	if (val==1)	// Radio is disabled
		return 0;

	/* buffers and length */
	max_sta_count = 256;
	maclist_size = sizeof(auth->count) + max_sta_count * sizeof(struct ether_addr);

	auth = malloc(maclist_size);
	assoc = malloc(maclist_size);
	authorized = malloc(maclist_size);

	if (!auth || !assoc || !authorized)
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

	retval=wl_ioctl(WIF, WLC_GET_BSSID, bssid, sizeof(bssid));
	if (!retval && !(bssid[0]==0&&bssid[1]==0&&bssid[2]==0&&bssid[3]==0&&bssid[4]==0&&bssid[5]==0))
	{
		connected_flag=1;
		sprintf(uppermac, "%02X:%02X:%02X:%02X:%02X:%02X", (unsigned char)bssid[0], (unsigned char)bssid[1], (unsigned char)bssid[2], (unsigned char)bssid[3], (unsigned char)bssid[4], (unsigned char)bssid[5]);
	}

	for (i = 0; i < auth->count; i ++) {
		char ea[ETHER_ADDR_STR_LEN];
		if (connected_flag==1 && !strncmp(uppermac, ether_etoa((void *)&auth->ea[i], ea), 17))
			continue;
		ret+=websWrite(wp, "<option class=\"content_input_fd\" value=\"%s\">%s</option>", ether_etoa2((void *)&auth->ea[i], ea) , ether_etoa2((void *)&auth->ea[i], ea));
	}

	/* error/exit */
exit:
	if (auth) free(auth);
	if (assoc) free(assoc);
	if (authorized) free(authorized);
	
	return ret;
}

/* Dump NAT table <tr><td>destination</td><td>MAC</td><td>IP</td><td>expires</td></tr> format */
int
ej_nat_table(int eid, webs_t wp, int argc, char_t **argv)
{
    	int needlen = 0, listlen, i, ret;
    	netconf_nat_t *nat_list = 0;
	netconf_nat_t **plist, *cur;
	char line[256], tstr[32];

	ret += websWrite(wp, "Destination     Proto.  Port Range  Redirect to\n");

    	netconf_get_nat(NULL, &needlen);

    	if (needlen > 0) 
	{

		nat_list = (netconf_nat_t *) malloc(needlen);
		if (nat_list) {
	    		memset(nat_list, 0, needlen);
	    		listlen = needlen;
	    		if (netconf_get_nat(nat_list, &listlen) == 0 && needlen == listlen) {
				listlen = needlen/sizeof(netconf_nat_t);

				for(i=0;i<listlen;i++)
				{				
				//printf("%d %d %d\n", nat_list[i].target,
			        //		nat_list[i].match.ipproto,
				//		nat_list[i].match.dst.ipaddr.s_addr);	
				if (nat_list[i].target==NETCONF_DNAT)
				{
					if (nat_list[i].match.dst.ipaddr.s_addr==0)
					{
						sprintf(line, "%-15s", "all");
					}
					else
					{
						sprintf(line, "%-15s", inet_ntoa(nat_list[i].match.dst.ipaddr));
					}


					if (ntohs(nat_list[i].match.dst.ports[0])==0)	
						sprintf(line, "%s %-7s", line, "ALL");
					else if (nat_list[i].match.ipproto==IPPROTO_TCP)
						sprintf(line, "%s %-7s", line, "TCP");
					else sprintf(line, "%s %-7s", line, "UDP");

					if (nat_list[i].match.dst.ports[0] == nat_list[i].match.dst.ports[1])
					{
						if (ntohs(nat_list[i].match.dst.ports[0])==0)	
						sprintf(line, "%s %-11s", line, "ALL");
						else
						sprintf(line, "%s %-11d", line, ntohs(nat_list[i].match.dst.ports[0]));
					}
					else 
					{
						sprintf(tstr, "%d:%d", ntohs(nat_list[i].match.dst.ports[0]),
ntohs(nat_list[i].match.dst.ports[1]));
						sprintf(line, "%s %-11s", line, tstr);					
					}	
					sprintf(line, "%s %s\n", line, inet_ntoa(nat_list[i].ipaddr));
					ret += websWrite(wp, line);
				
				}
				}
	    		}
	    		free(nat_list);
		}
    	}
	return ret;
}

int
ej_route_table(int eid, webs_t wp, int argc, char_t **argv)
{
	char buff[256];
	int  nl = 0 ;
	struct in_addr dest;
	struct in_addr gw;
	struct in_addr mask;
	int flgs, ref, use, metric, ret;
	char flags[4];
	unsigned long int d,g,m;
	char sdest[16], sgw[16];
	FILE *fp;

        ret += websWrite(wp, "Destination     Gateway         Genmask         Flags Metric Ref    Use Iface\n");

	if (!(fp = fopen("/proc/net/route", "r"))) return 0;

	while(fgets(buff, sizeof(buff), fp) != NULL ) 
	{
		if(nl) 
		{
			int ifl = 0;
			while(buff[ifl]!=' ' && buff[ifl]!='\t' && buff[ifl]!='\0')
				ifl++;
			buff[ifl]=0;    /* interface */
			if(sscanf(buff+ifl+1, "%lx%lx%d%d%d%d%lx",
			   &d, &g, &flgs, &ref, &use, &metric, &m)!=7) {
				//error_msg_and_die( "Unsuported kernel route format\n");
				//continue;
			}

			ifl = 0;        /* parse flags */
			if(flgs&1)
				flags[ifl++]='U';
			if(flgs&2)
				flags[ifl++]='G';
			if(flgs&4)
				flags[ifl++]='H';
			flags[ifl]=0;
			dest.s_addr = d;
			gw.s_addr   = g;
			mask.s_addr = m;
			strcpy(sdest,  (dest.s_addr==0 ? "default" :
					inet_ntoa(dest)));
			strcpy(sgw,    (gw.s_addr==0   ? "*"       :
					inet_ntoa(gw)));

			if (strstr(buff, "br0") || strstr(buff, "wl0"))
			{
				ret += websWrite(wp, "%-16s%-16s%-16s%-6s%-6d %-2d %7d LAN\n",
				sdest, sgw,
				inet_ntoa(mask),
				flags, metric, ref, use);
			}
			else if(!strstr(buff, "lo"))
			{
				ret += websWrite(wp, "%-16s%-16s%-16s%-6s%-6d %-2d %7d WAN\n",
				sdest, sgw,
				inet_ntoa(mask),
				flags, metric, ref, use);
			}
		}
		nl++;
	}
	fclose(fp);
}

#define BIT(n) (1 << (n))
#define WPA_GET_LE16(a) ((u16) (((a)[1] << 8) | (a)[0]))

#define GENERIC_INFO_ELEM_ 0xdd
#define RSN_INFO_ELEM_ 0x30

#define WPA_CIPHER_NONE_ BIT(0)
#define WPA_CIPHER_WEP40_ BIT(1)
#define WPA_CIPHER_WEP104_ BIT(2)
#define WPA_CIPHER_TKIP_ BIT(3)
#define WPA_CIPHER_CCMP_ BIT(4)

#define WPA_KEY_MGMT_IEEE8021X_ BIT(0)
#define WPA_KEY_MGMT_IEEE8021X2_ BIT(6)
#define WPA_KEY_MGMT_PSK_ BIT(1)
#define WPA_KEY_MGMT_PSK2_ BIT(5)
#define WPA_KEY_MGMT_NONE_ BIT(2)
#define WPA_KEY_MGMT_IEEE8021X_NO_WPA_ BIT(3)
#define WPA_KEY_MGMT_WPA_NONE_ BIT(4)

#define WPA_PROTO_WPA_ BIT(0)
#define WPA_PROTO_RSN_ BIT(1)

#define PMKID_LEN 16

static const int WPA_SELECTOR_LEN = 4;
static const u8 WPA_OUI_TYPE[] = { 0x00, 0x50, 0xf2, 1 };
static const u16 WPA_VERSION_ = 1;
static const u8 WPA_AUTH_KEY_MGMT_NONE[] = { 0x00, 0x50, 0xf2, 0 };
static const u8 WPA_AUTH_KEY_MGMT_UNSPEC_802_1X[] = { 0x00, 0x50, 0xf2, 1 };
static const u8 WPA_AUTH_KEY_MGMT_PSK_OVER_802_1X[] = { 0x00, 0x50, 0xf2, 2 };
static const u8 WPA_CIPHER_SUITE_NONE[] = { 0x00, 0x50, 0xf2, 0 };
static const u8 WPA_CIPHER_SUITE_WEP40[] = { 0x00, 0x50, 0xf2, 1 };
static const u8 WPA_CIPHER_SUITE_TKIP[] = { 0x00, 0x50, 0xf2, 2 };
static const u8 WPA_CIPHER_SUITE_WRAP[] = { 0x00, 0x50, 0xf2, 3 };
static const u8 WPA_CIPHER_SUITE_CCMP[] = { 0x00, 0x50, 0xf2, 4 };
static const u8 WPA_CIPHER_SUITE_WEP104[] = { 0x00, 0x50, 0xf2, 5 };

static const int RSN_SELECTOR_LEN = 4;
static const u16 RSN_VERSION_ = 1;
static const u8 RSN_AUTH_KEY_MGMT_UNSPEC_802_1X[] = { 0x00, 0x0f, 0xac, 1 };
static const u8 RSN_AUTH_KEY_MGMT_PSK_OVER_802_1X[] = { 0x00, 0x0f, 0xac, 2 };
static const u8 RSN_CIPHER_SUITE_NONE[] = { 0x00, 0x0f, 0xac, 0 };
static const u8 RSN_CIPHER_SUITE_WEP40[] = { 0x00, 0x0f, 0xac, 1 };
static const u8 RSN_CIPHER_SUITE_TKIP[] = { 0x00, 0x0f, 0xac, 2 };
static const u8 RSN_CIPHER_SUITE_WRAP[] = { 0x00, 0x0f, 0xac, 3 };
static const u8 RSN_CIPHER_SUITE_CCMP[] = { 0x00, 0x0f, 0xac, 4 };
static const u8 RSN_CIPHER_SUITE_WEP104[] = { 0x00, 0x0f, 0xac, 5 };

struct wpa_ie_data {
	int proto;
	int pairwise_cipher;
	int group_cipher;
	int key_mgmt;
	int capabilities;
	int num_pmkid;
	const u8 *pmkid;
};

struct wpa_ie_hdr {
	u8 elem_id;
	u8 len;
	u8 oui[3];
	u8 oui_type;
	u8 version[2];
} __attribute__ ((packed));

struct rsn_ie_hdr {
	u8 elem_id; /* WLAN_EID_RSN */
	u8 len;
	u8 version[2];
} __attribute__ ((packed));

static int wpa_key_mgmt_to_bitfield(const u8 *s)
{
	if (memcmp(s, WPA_AUTH_KEY_MGMT_UNSPEC_802_1X, WPA_SELECTOR_LEN) == 0)
		return WPA_KEY_MGMT_IEEE8021X_;
	if (memcmp(s, WPA_AUTH_KEY_MGMT_PSK_OVER_802_1X, WPA_SELECTOR_LEN) ==
	    0)
		return WPA_KEY_MGMT_PSK_;
	if (memcmp(s, WPA_AUTH_KEY_MGMT_NONE, WPA_SELECTOR_LEN) == 0)
		return WPA_KEY_MGMT_WPA_NONE_;
	return 0;
}

static int rsn_key_mgmt_to_bitfield(const u8 *s)
{
	if (memcmp(s, RSN_AUTH_KEY_MGMT_UNSPEC_802_1X, RSN_SELECTOR_LEN) == 0)
		return WPA_KEY_MGMT_IEEE8021X2_;
	if (memcmp(s, RSN_AUTH_KEY_MGMT_PSK_OVER_802_1X, RSN_SELECTOR_LEN) ==
	    0)
		return WPA_KEY_MGMT_PSK2_;
	return 0;
}

static int wpa_selector_to_bitfield(const u8 *s)
{
	if (memcmp(s, WPA_CIPHER_SUITE_NONE, WPA_SELECTOR_LEN) == 0)
		return WPA_CIPHER_NONE_;
	if (memcmp(s, WPA_CIPHER_SUITE_WEP40, WPA_SELECTOR_LEN) == 0)
		return WPA_CIPHER_WEP40_;
	if (memcmp(s, WPA_CIPHER_SUITE_TKIP, WPA_SELECTOR_LEN) == 0)
		return WPA_CIPHER_TKIP_;
	if (memcmp(s, WPA_CIPHER_SUITE_CCMP, WPA_SELECTOR_LEN) == 0)
		return WPA_CIPHER_CCMP_;
	if (memcmp(s, WPA_CIPHER_SUITE_WEP104, WPA_SELECTOR_LEN) == 0)
		return WPA_CIPHER_WEP104_;
	return 0;
}

static int rsn_selector_to_bitfield(const u8 *s)
{
	if (memcmp(s, RSN_CIPHER_SUITE_NONE, RSN_SELECTOR_LEN) == 0)
		return WPA_CIPHER_NONE_;
	if (memcmp(s, RSN_CIPHER_SUITE_WEP40, RSN_SELECTOR_LEN) == 0)
		return WPA_CIPHER_WEP40_;
	if (memcmp(s, RSN_CIPHER_SUITE_TKIP, RSN_SELECTOR_LEN) == 0)
		return WPA_CIPHER_TKIP_;
	if (memcmp(s, RSN_CIPHER_SUITE_CCMP, RSN_SELECTOR_LEN) == 0)
		return WPA_CIPHER_CCMP_;
	if (memcmp(s, RSN_CIPHER_SUITE_WEP104, RSN_SELECTOR_LEN) == 0)
		return WPA_CIPHER_WEP104_;
	return 0;
}

static int wpa_parse_wpa_ie_wpa(const u8 *wpa_ie, size_t wpa_ie_len,
				struct wpa_ie_data *data)
{
	const struct wpa_ie_hdr *hdr;
	const u8 *pos;
	int left;
	int i, count;

	data->proto = WPA_PROTO_WPA_;
	data->pairwise_cipher = WPA_CIPHER_TKIP_;
	data->group_cipher = WPA_CIPHER_TKIP_;
	data->key_mgmt = WPA_KEY_MGMT_IEEE8021X_;
	data->capabilities = 0;
	data->pmkid = NULL;
	data->num_pmkid = 0;

	if (wpa_ie_len == 0) {
		/* No WPA IE - fail silently */
		return -1;
	}

	if (wpa_ie_len < sizeof(struct wpa_ie_hdr)) {
//		fprintf(stderr, "ie len too short %lu", (unsigned long) wpa_ie_len);
		return -1;
	}

	hdr = (const struct wpa_ie_hdr *) wpa_ie;

	if (hdr->elem_id != GENERIC_INFO_ELEM_ ||
	    hdr->len != wpa_ie_len - 2 ||
	    memcmp(&hdr->oui, WPA_OUI_TYPE, WPA_SELECTOR_LEN) != 0 ||
	    WPA_GET_LE16(hdr->version) != WPA_VERSION_) {
//		fprintf(stderr, "malformed ie or unknown version");
		return -1;
	}

	pos = (const u8 *) (hdr + 1);
	left = wpa_ie_len - sizeof(*hdr);

	if (left >= WPA_SELECTOR_LEN) {
		data->group_cipher = wpa_selector_to_bitfield(pos);
		pos += WPA_SELECTOR_LEN;
		left -= WPA_SELECTOR_LEN;
	} else if (left > 0) {
//		fprintf(stderr, "ie length mismatch, %u too much", left);
		return -1;
	}

	if (left >= 2) {
		data->pairwise_cipher = 0;
		count = WPA_GET_LE16(pos);
		pos += 2;
		left -= 2;
		if (count == 0 || left < count * WPA_SELECTOR_LEN) {
//			fprintf(stderr, "ie count botch (pairwise), "
//				   "count %u left %u", count, left);
			return -1;
		}
		for (i = 0; i < count; i++) {
			data->pairwise_cipher |= wpa_selector_to_bitfield(pos);
			pos += WPA_SELECTOR_LEN;
			left -= WPA_SELECTOR_LEN;
		}
	} else if (left == 1) {
//		fprintf(stderr, "ie too short (for key mgmt)");
		return -1;
	}

	if (left >= 2) {
		data->key_mgmt = 0;
		count = WPA_GET_LE16(pos);
		pos += 2;
		left -= 2;
		if (count == 0 || left < count * WPA_SELECTOR_LEN) {
//			fprintf(stderr, "ie count botch (key mgmt), "
//				   "count %u left %u", count, left);
			return -1;
		}
		for (i = 0; i < count; i++) {
			data->key_mgmt |= wpa_key_mgmt_to_bitfield(pos);
			pos += WPA_SELECTOR_LEN;
			left -= WPA_SELECTOR_LEN;
		}
	} else if (left == 1) {
//		fprintf(stderr, "ie too short (for capabilities)");
		return -1;
	}

	if (left >= 2) {
		data->capabilities = WPA_GET_LE16(pos);
		pos += 2;
		left -= 2;
	}

	if (left > 0) {
//		fprintf(stderr, "ie has %u trailing bytes", left);
		return -1;
	}

	return 0;
}

static int wpa_parse_wpa_ie_rsn(const u8 *rsn_ie, size_t rsn_ie_len,
				struct wpa_ie_data *data)
{
	const struct rsn_ie_hdr *hdr;
	const u8 *pos;
	int left;
	int i, count;

	data->proto = WPA_PROTO_RSN_;
	data->pairwise_cipher = WPA_CIPHER_CCMP_;
	data->group_cipher = WPA_CIPHER_CCMP_;
	data->key_mgmt = WPA_KEY_MGMT_IEEE8021X2_;
	data->capabilities = 0;
	data->pmkid = NULL;
	data->num_pmkid = 0;

	if (rsn_ie_len == 0) {
		/* No RSN IE - fail silently */
		return -1;
	}

	if (rsn_ie_len < sizeof(struct rsn_ie_hdr)) {
//		fprintf(stderr, "ie len too short %lu", (unsigned long) rsn_ie_len);
		return -1;
	}

	hdr = (const struct rsn_ie_hdr *) rsn_ie;

	if (hdr->elem_id != RSN_INFO_ELEM_ ||
	    hdr->len != rsn_ie_len - 2 ||
	    WPA_GET_LE16(hdr->version) != RSN_VERSION_) {
//		fprintf(stderr, "malformed ie or unknown version");
		return -1;
	}

	pos = (const u8 *) (hdr + 1);
	left = rsn_ie_len - sizeof(*hdr);

	if (left >= RSN_SELECTOR_LEN) {
		data->group_cipher = rsn_selector_to_bitfield(pos);
		pos += RSN_SELECTOR_LEN;
		left -= RSN_SELECTOR_LEN;
	} else if (left > 0) {
//		fprintf(stderr, "ie length mismatch, %u too much", left);
		return -1;
	}

	if (left >= 2) {
		data->pairwise_cipher = 0;
		count = WPA_GET_LE16(pos);
		pos += 2;
		left -= 2;
		if (count == 0 || left < count * RSN_SELECTOR_LEN) {
//			fprintf(stderr, "ie count botch (pairwise), "
//				   "count %u left %u", count, left);
			return -1;
		}
		for (i = 0; i < count; i++) {
			data->pairwise_cipher |= rsn_selector_to_bitfield(pos);
			pos += RSN_SELECTOR_LEN;
			left -= RSN_SELECTOR_LEN;
		}
	} else if (left == 1) {
//		fprintf(stderr, "ie too short (for key mgmt)");
		return -1;
	}

	if (left >= 2) {
		data->key_mgmt = 0;
		count = WPA_GET_LE16(pos);
		pos += 2;
		left -= 2;
		if (count == 0 || left < count * RSN_SELECTOR_LEN) {
//			fprintf(stderr, "ie count botch (key mgmt), "
//				   "count %u left %u", count, left);
			return -1;
		}
		for (i = 0; i < count; i++) {
			data->key_mgmt |= rsn_key_mgmt_to_bitfield(pos);
			pos += RSN_SELECTOR_LEN;
			left -= RSN_SELECTOR_LEN;
		}
	} else if (left == 1) {
//		fprintf(stderr, "ie too short (for capabilities)");
		return -1;
	}

	if (left >= 2) {
		data->capabilities = WPA_GET_LE16(pos);
		pos += 2;
		left -= 2;
	}

	if (left >= 2) {
		data->num_pmkid = WPA_GET_LE16(pos);
		pos += 2;
		left -= 2;
		if (left < data->num_pmkid * PMKID_LEN) {
//			fprintf(stderr, "PMKID underflow "
//				   "(num_pmkid=%d left=%d)", data->num_pmkid, left);
			data->num_pmkid = 0;
		} else {
			data->pmkid = pos;
			pos += data->num_pmkid * PMKID_LEN;
			left -= data->num_pmkid * PMKID_LEN;
		}
	}

	if (left > 0) {
//		fprintf(stderr, "ie has %u trailing bytes - ignored", left);
	}

	return 0;
}

int wpa_parse_wpa_ie(const u8 *wpa_ie, size_t wpa_ie_len,
		     struct wpa_ie_data *data)
{
	if (wpa_ie_len >= 1 && wpa_ie[0] == RSN_INFO_ELEM_)
		return wpa_parse_wpa_ie_rsn(wpa_ie, wpa_ie_len, data);
	else
		return wpa_parse_wpa_ie_wpa(wpa_ie, wpa_ie_len, data);
}

static const char * wpa_cipher_txt(int cipher)
{
	switch (cipher) {
	case WPA_CIPHER_NONE_:
		return "NONE";
	case WPA_CIPHER_WEP40_:
		return "WEP-40";
	case WPA_CIPHER_WEP104_:
		return "WEP-104";
	case WPA_CIPHER_TKIP_:
		return "TKIP";
	case WPA_CIPHER_CCMP_:
//		return "CCMP";
		return "AES";
	case (WPA_CIPHER_TKIP_|WPA_CIPHER_CCMP_):
		return "TKIP+AES";
	default:
		return "UNKNOWN";
	}
}

static const char * wpa_key_mgmt_txt(int key_mgmt, int proto)
{
	switch (key_mgmt) {
	case WPA_KEY_MGMT_IEEE8021X_:
/*
		return proto == WPA_PROTO_RSN_ ?
			"WPA2/IEEE 802.1X/EAP" : "WPA/IEEE 802.1X/EAP";
*/
		return "WPA-Enterprise/Radius with 802.1x";
	case WPA_KEY_MGMT_IEEE8021X2_:
		return "WPA2-Enterprise/Radius with 802.1x";
	case WPA_KEY_MGMT_PSK_:
/*
		return proto == WPA_PROTO_RSN_ ?
			"WPA2-PSK" : "WPA-PSK";
*/
		return "WPA-Personal";
	case WPA_KEY_MGMT_PSK2_:
		return "WPA2-Personal";
	case WPA_KEY_MGMT_NONE_:
		return "NONE";
	case WPA_KEY_MGMT_IEEE8021X_NO_WPA_:
//		return "IEEE 802.1X (no WPA)";
		return "Radius with 802.1x (no WPA-Enterprise/WPA2-Enterprise)";
	default:
		return "UNKNOWN";
	}
}

static const char * wpa_html_txt(int cipher, int key_mgmt, int proto)
{
	switch (cipher) {
	case WPA_CIPHER_TKIP_:
		switch (key_mgmt) {
		case WPA_KEY_MGMT_PSK_:
			return "2";
		case WPA_KEY_MGMT_PSK2_:
			return "4";
		case WPA_KEY_MGMT_IEEE8021X_:
		case WPA_KEY_MGMT_IEEE8021X2_:
		case WPA_KEY_MGMT_IEEE8021X_NO_WPA_:
			return "7";
		default:
			return "6";
		}
	case WPA_CIPHER_CCMP_:
	case (WPA_CIPHER_TKIP_|WPA_CIPHER_CCMP_):
		switch (key_mgmt) {
		case WPA_KEY_MGMT_PSK_:
			return "3";
		case WPA_KEY_MGMT_PSK2_:
			return "5";
		case WPA_KEY_MGMT_IEEE8021X_:
		case WPA_KEY_MGMT_IEEE8021X2_:
		case WPA_KEY_MGMT_IEEE8021X_NO_WPA_:
			return "7";
		default:
			return "6";
		}

	default:
		return "6";
	}
}

static const char * wpa_html_txt2(int cipher, int key_mgmt, int proto)	// for URE mode
{
	switch (cipher) {
	case WPA_CIPHER_TKIP_:
		switch (key_mgmt) {
		case WPA_KEY_MGMT_PSK_:
			return "2";
		case WPA_KEY_MGMT_PSK2_:
			return "4";
		case WPA_KEY_MGMT_IEEE8021X_:
		case WPA_KEY_MGMT_IEEE8021X2_:
		case WPA_KEY_MGMT_IEEE8021X_NO_WPA_:
			return "7";
		default:
			return "6";
		}
	case WPA_CIPHER_CCMP_:
		switch (key_mgmt) {
		case WPA_KEY_MGMT_PSK_:
			return "3";
		case WPA_KEY_MGMT_PSK2_:
			return "5";
		case WPA_KEY_MGMT_IEEE8021X_:
		case WPA_KEY_MGMT_IEEE8021X2_:
		case WPA_KEY_MGMT_IEEE8021X_NO_WPA_:
			return "7";
		default:
			return "6";
		}
	case (WPA_CIPHER_TKIP_|WPA_CIPHER_CCMP_): // select TKIP for TKIP+AES case
		switch (key_mgmt) {
		case WPA_KEY_MGMT_PSK_:
			return "8";
		case WPA_KEY_MGMT_PSK2_:
			return "9";
		case WPA_KEY_MGMT_IEEE8021X_:
		case WPA_KEY_MGMT_IEEE8021X2_:
		case WPA_KEY_MGMT_IEEE8021X_NO_WPA_:
			return "7";
		default:
			return "6";
		}

	default:
		return "6";
	}
}

static const char* char_to_ascii(char *output, char *input)/* Transfer Char to ASCII */
{                                                   /* Cherry_Cho added in 2006/9/29. */
	int i;
	char tmp[10];
	char *ptr;
	 
	ptr = output;
	 
	for( i=0; i<strlen(input); i++ )
    	{
         	if((input[i]>='0' && input[i] <='9')
         	   ||(input[i]>='A' && input[i]<='Z') 
         	   ||(input[i] >='a' && input[i]<='z')
         	   || input[i] == '!' || input[i] == '*'
         	   || input[i] == '(' || input[i] == ')'
         	   || input[i] == '_' || input[i] == '-'
         	   || input[i] == "'" || input[i] == '.')
         	{
         		*ptr = input[i];
         		ptr ++;
         	}
         	else
         	{
         		sprintf(tmp, "%%%.02X", input[i]);
         		strcpy(ptr, tmp);
         		ptr += 3;
         	}
     	}

     	*(ptr) = '\0';
}

struct bss_ie_hdr {
	u8 elem_id;
	u8 len;
	u8 oui[3];
} bss_ie;

typedef struct wl_scan_params2 {
	wlc_ssid_t ssid;	/* default is {0, ""} */
	struct ether_addr bssid;/* default is bcast */
	int8 bss_type;		/* default is any, DOT11_BSSTYPE_ANY/INFRASTRUCTURE/INDEPENDENT */
	int8 scan_type;		/* -1 use default, DOT11_SCANTYPE_ACTIVE/PASSIVE */
	int32 nprobes;		/* -1 use default, number of probes per channel */
	int32 active_time;	/* -1 use default, dwell time per channel for active scanning */
	int32 passive_time;	/* -1 use default, dwell time per channel for passive scanning */
	int32 home_time;	/* -1 use default, dwell time for the home channel between channel scans */
	int32 channel_num;	/* 0 use default (all available channels), count of channels in channel_list */
	uint16 channel_list[14];
}wl_scan_params_asus;

struct apinfo
{
	char BSSID[18];
	char SSID[32];
	int RSSI;
	uint8 channel;
	uint16 capability;
	int wep;
	int wpa;
	struct wpa_ie_data wid;
	int status;
}apinfos[32];

int
ej_ap_scan(int eid, webs_t wp, int argc, char_t **argv)
{
	nvram_set("ap_scanning", "1");
	int unit=0;
	int ret, i, j, k;
	wl_scan_results_t *result;
	wl_bss_info_t *info;
//	wl_bss_info_107_t *info;
	char *info_b;
	unsigned char *bssid;
	unsigned char bssid2[6];
	char uppermac[17];
	int rssi=-32768;
	int retval=0;
	int ssid_len=0;
	int apinfo_idx=0;
	int apinfo_idx_same=-1;
	int count=0;
	char ssid_str[256];
	FILE *fp;
	char sel_str[32];

	wlc_ssid_t wst = {0, ""};
	wl_scan_params_asus wsp;
	wsp.ssid=wst;
	wsp.bssid.octet[0]=0xff;
	wsp.bssid.octet[1]=0xff;
	wsp.bssid.octet[2]=0xff;
	wsp.bssid.octet[3]=0xff;
	wsp.bssid.octet[4]=0xff;
	wsp.bssid.octet[5]=0xff;
	wsp.bss_type=DOT11_BSSTYPE_ANY;
//	wsp.scan_type=DOT11_SCANTYPE_PASSIVE;
	wsp.scan_type=DOT11_SCANTYPE_ACTIVE;
	wsp.nprobes=wsp.active_time=wsp.passive_time=wsp.home_time=-1;
	wsp.channel_num=0;
	wsp.channel_list[0]=1;
	wsp.channel_list[1]=2;
	wsp.channel_list[2]=3;
	wsp.channel_list[3]=4;
	wsp.channel_list[4]=5;
	wsp.channel_list[5]=6;
	wsp.channel_list[6]=7;
	wsp.channel_list[7]=8;
	wsp.channel_list[8]=9;
	wsp.channel_list[9]=10;
	wsp.channel_list[10]=11;
	wsp.channel_list[11]=12;
	wsp.channel_list[12]=13;
	wsp.channel_list[13]=14;

	result = (wl_scan_results_t *)buf;
	result->buflen=WLC_IOCTL_MAXLEN - sizeof(result);
	result->version=WL_BSS_INFO_VERSION;
	result->count=0;

	int SSID_valid=1;

	while ((ret=wl_ioctl(WIF, WLC_SCAN, &wsp, sizeof(wsp))) < 0 && count<2)
	{
		count++;
		sleep(1);
	}
//	sleep(3);
	usleep(3500*1000);
	if (ret==0)
	{
		count=0;
		while ((ret=wl_ioctl(WIF, WLC_SCAN_RESULTS, result, WLC_IOCTL_MAXLEN)) < 0 && count<2)
		{
			count++;
			sleep(1);
		}
		if (ret==0)
		{
			info = &(result->bss_info[0]);
			info_b = (unsigned char *)info;
			
			for(i=0;i<result->count;i++)
			{
				if (info->SSID_len>32 || info->SSID_len==0)
					goto next_info;

				SSID_valid=1;
				for(j=0;j<info->SSID_len;j++)
				{
					if(info->SSID[j]<32 || info->SSID[j]>126)
					{
						SSID_valid=0;
						break;
					}
				}
				if(!SSID_valid)
					goto next_info;

				int left;
				struct bss_ie_hdr *ie;

				bssid = (unsigned char *)&info->BSSID;
				sprintf(uppermac, "%02X:%02X:%02X:%02X:%02X:%02X", (unsigned char)bssid[0], (unsigned char)bssid[1], (unsigned char)bssid[2], (unsigned char)bssid[3], (unsigned char)bssid[4], (unsigned char)bssid[5]);

				apinfo_idx_same=-1;
				for (k=0;k<apinfo_idx;k++)
				{
					if(strcmp(apinfos[k].BSSID, uppermac)==0 && strcmp(apinfos[k].SSID, info->SSID)==0)
					{
						apinfo_idx_same=k;
						break;
					}
				}

				if (apinfo_idx_same!=-1)
				{
					apinfos[apinfo_idx_same].RSSI=info->RSSI;
				} else
				{
					strcpy(apinfos[apinfo_idx].BSSID, uppermac);
					strcpy(apinfos[apinfo_idx].SSID, info->SSID);
					apinfos[apinfo_idx].RSSI=info->RSSI;
//					apinfos[apinfo_idx].channel=info->ctl_ch;
					apinfos[apinfo_idx].channel=((wl_bss_info_107_t *) info)->channel;

//					apinfos[apinfo_idx].channel=info->channel;
					if ((info->capability & 0x10) == 0x10)
						apinfos[apinfo_idx].wep=1;
					else
						apinfos[apinfo_idx].wep=0;
					apinfos[apinfo_idx].wpa=0;
					apinfo_idx++;
				}

				ie = (struct bss_ie_hdr *) ((u8 *) info + sizeof(*info));
				for (left = info->ie_length; left > 0; 
					left -= (ie->len + 2), ie = (struct bss_ie_hdr *) ((u8 *) ie + 2 + ie->len)) 
				{
					if (!(ie->elem_id == 0xdd) && !(ie->elem_id == 0x30))
						continue;

					if (wpa_parse_wpa_ie(&ie->elem_id, ie->len+2, &apinfos[apinfo_idx-1].wid)==0)
					{
						apinfos[apinfo_idx-1].wpa=1;
						break;
					}
				}
next_info:
				info = (wl_bss_info_t *) ((u8 *) info + info->length);
//				info = (wl_bss_info_107_t *) ((u8 *) info + info->length);
			}
		}
	}
end:
	retval += websWrite(wp, "<input type=\"hidden\" name=\"apinfo_n\" value=\"%d\" readonly=\"1\">", apinfo_idx);
	if (apinfo_idx==0)
	{
		retval += websWrite(wp, "<tr><td class='content_header_td_ap2'>&nbsp;</td>");
//		retval += websWrite(wp, "<td class='content_header_td_ap2'><font color='#FF0000'>Finds no AP!</font></td>");

		FILE *fp;
		char noap_str[32];
		if (nvram_match("preferred_lang", "TW"))
			fp=fopen("/www/noap_TW.asp", "r");
		else if (nvram_match("preferred_lang", "CN"))
			fp=fopen("/www/noap_CN.asp", "r");
		else if (nvram_match("preferred_lang", "JP"))
			fp=fopen("/www/noap_JP.asp", "r");
		if (fp!=NULL)
		{
			if (fgets(noap_str, 32, fp)==NULL)
				sprintf(noap_str, "No AP found!");
			fclose(fp);
		}

		if (nvram_match("preferred_lang", "EN"))
			retval += websWrite(wp, "<td class='content_header_td_ap2'><font color='#FF0000'>Finds no AP!</font></td>");
		else if (nvram_match("preferred_lang", "DE"))
			retval += websWrite(wp, "<td class='content_header_td_ap2'><font color='#FF0000'>Es wurde kein AP gefunden!</font></td>");
		else
			retval += websWrite(wp, "<td class='content_header_td_ap2'><font color='#FF0000'>%s</font></td>", noap_str);

		retval += websWrite(wp, "<td class='content_header_td_ap2'>&nbsp;</td>");
		retval += websWrite(wp, "<td class='content_header_td_ap2'>&nbsp;</td>");
		retval += websWrite(wp, "<td class='content_header_td_ap2'>&nbsp;</td></tr>");
	}
	else
	{

	if (nvram_match("preferred_lang", "TW"))
		fp=fopen("/www/select_TW.asp", "r");
	else if (nvram_match("preferred_lang", "CN"))
		fp=fopen("/www/select_CN.asp", "r");
	else if (nvram_match("preferred_lang", "KR"))
		fp=fopen("/www/select_KR.asp", "r");
	else if (nvram_match("preferred_lang", "DE"))
		fp=fopen("/www/select_DE.asp", "r");
	else if (nvram_match("preferred_lang", "JP"))
		fp=fopen("/www/select_JP.asp", "r");
        else if (nvram_match("preferred_lang", "TR"))
                fp=fopen("/www/select_TR.asp", "r");
	else
		fp=fopen("/www/select.asp", "r");
	if (fp!=NULL)
	{
		if (fgets(sel_str, 32, fp)==NULL)
			sprintf(sel_str, "Select");
		fclose(fp);
	}

	for (k=0;k<apinfo_idx;k++)
	{
		retval += websWrite(wp, "<tr><td class='content_header_td_ap2'>%s</td>", apinfos[k].BSSID);

		if (strlen(apinfos[k].SSID)==0)
		{
			retval += websWrite(wp, "<td nowarp=\"nowarp\" class='content_header_td_ap2'>&nbsp;</td><input type=\"hidden\" class=\"content_input_fd\" name=\"RSSID\" value=\"\" readonly=\"1\"><input type=\"hidden\" name=\"RSSIDE\" value=\"\" readonly=\"1\">");
		}
		else
		{
			retval += websWrite(wp, "<td nowarp=\"nowarp\" class='content_header_td_ap2'><input type=\"text\" class=\"content_input_fd\" name=\"RSSID\" value=\"\" readonly=\"1\">");
			memset(ssid_str, 0, sizeof(ssid_str));
			char_to_ascii(ssid_str, apinfos[k].SSID);

			if (apinfos[k].wpa==1)
				retval += websWrite(wp, "<input type=\"button\" style=\"cursor:pointer;\" class=\"content_input_fd_ro\" onClick=\"setSSID('%s', '%d', '%d');\" value=\"%s\"></td>", ssid_str, apinfos[k].wid.key_mgmt, apinfos[k].wid.pairwise_cipher, sel_str);
			else if (apinfos[k].wep==1)
				retval += websWrite(wp, "<input type=\"button\" style=\"cursor:pointer;\" class=\"content_input_fd_ro\" onClick=\"setSSID('%s', '128',  '0');\" value=\"%s\"></td>", ssid_str, sel_str);
			else
				retval += websWrite(wp, "<input type=\"button\" style=\"cursor:pointer;\" class=\"content_input_fd_ro\" onClick=\"setSSID('%s',  '0',  '0');\" value=\"%s\"></td>", ssid_str, sel_str);
			retval += websWrite(wp, "<input type=\"hidden\" name=\"RSSIDE\" value=\"%s\" readonly=\"1\">", ssid_str);
		}

		if (apinfos[k].wpa==1)
			retval += websWrite(wp, "<td class='content_header_td_ap2'>%s (%s)</td>", wpa_cipher_txt(apinfos[k].wid.pairwise_cipher), wpa_key_mgmt_txt(apinfos[k].wid.key_mgmt, apinfos[k].wid.proto));
		else if (apinfos[k].wep==1)
			retval += websWrite(wp, "<td class='content_header_td_ap2'>WEP</td>");
		else
			retval += websWrite(wp, "<td class='content_header_td_ap2'>Off</td>");
		retval += websWrite(wp, "<td class='content_header_td_ap2'>%d</td>", apinfos[k].channel);
		retval += websWrite(wp, "<td class='content_header_td_ap2'>%d dBm</td></tr>", apinfos[k].RSSI);
		
	}
	}
	nvram_set("ap_scanning", "0");
	return retval;
}

int
ej_ap_scan2(int eid, webs_t wp, int argc, char_t **argv)
{
	int RSSI_TH_A=atoi(nvram_safe_get("threshold_a"));
	int RSSI_TH_B=atoi(nvram_safe_get("threshold_b"));
	int RSSI_TH_C=atoi(nvram_safe_get("threshold_c"));
	int RSSI_TH_D=atoi(nvram_safe_get("threshold_d"));
	int RSSI_TH_E=atoi(nvram_safe_get("threshold_e"));
	int match_mac=0;
	char nvram_str[32];

	nvram_set("ap_scanning", "1");
	int unit=0;
	int ret, i, j, k;
	wl_scan_results_t *result;
	wl_bss_info_t *info;
	char *info_b;
	unsigned char *bssid;
	unsigned char bssid2[6];
	char uppermac[17];
	int rssi=-32768;
	int retval=0;
	int ssid_len=0;
	int apinfo_idx=0;
	int apinfo_idx_same=-1;
	int count=0;
	char ssid_str[256];
	FILE *fp;
	char sel_str[32];

	wlc_ssid_t wst = {0, ""};
	wl_scan_params_asus wsp;
	wsp.ssid=wst;
	wsp.bssid.octet[0]=0xff;
	wsp.bssid.octet[1]=0xff;
	wsp.bssid.octet[2]=0xff;
	wsp.bssid.octet[3]=0xff;
	wsp.bssid.octet[4]=0xff;
	wsp.bssid.octet[5]=0xff;
	wsp.bss_type=DOT11_BSSTYPE_ANY;
	wsp.scan_type=DOT11_SCANTYPE_ACTIVE;
	wsp.nprobes=wsp.active_time=wsp.passive_time=wsp.home_time=-1;
	wsp.channel_num=0;
	wsp.channel_list[0]=1;
	wsp.channel_list[1]=2;
	wsp.channel_list[2]=3;
	wsp.channel_list[3]=4;
	wsp.channel_list[4]=5;
	wsp.channel_list[5]=6;
	wsp.channel_list[6]=7;
	wsp.channel_list[7]=8;
	wsp.channel_list[8]=9;
	wsp.channel_list[9]=10;
	wsp.channel_list[10]=11;
	wsp.channel_list[11]=12;
	wsp.channel_list[12]=13;
	wsp.channel_list[13]=14;

	result = (wl_scan_results_t *)buf;
	result->buflen=WLC_IOCTL_MAXLEN - sizeof(result);
	result->version=WL_BSS_INFO_VERSION;
	result->count=0;

	int SSID_valid=1;

	while ((ret=wl_ioctl(WIF, WLC_SCAN, &wsp, sizeof(wsp))) < 0 && count<2)
	{
		count++;
		sleep(1);
	}
//	sleep(3);
	usleep(3500*1000);
	if (ret==0)
	{
		count=0;
		while ((ret=wl_ioctl(WIF, WLC_SCAN_RESULTS, result, WLC_IOCTL_MAXLEN)) < 0 && count<2)
		{
			count++;
			sleep(1);
		}
		if (ret==0)
		{
			info = &(result->bss_info[0]);
			info_b = (unsigned char *)info;
			
			for(i=0;i<result->count;i++)
			{
				if (info->SSID_len>32 || info->SSID_len==0)
					goto next_info;

				SSID_valid=1;
				for(j=0;j<info->SSID_len;j++)
				{
					if(info->SSID[j]<32 || info->SSID[j]>126)
					{
						SSID_valid=0;
						break;
					}
				}
				if(!SSID_valid)
					goto next_info;

				int left;
				struct bss_ie_hdr *ie;

				bssid = (unsigned char *)&info->BSSID;
				sprintf(uppermac, "%02X:%02X:%02X:%02X:%02X:%02X", (unsigned char)bssid[0], (unsigned char)bssid[1], (unsigned char)bssid[2], (unsigned char)bssid[3], (unsigned char)bssid[4], (unsigned char)bssid[5]);

				apinfo_idx_same=-1;
				for (k=0;k<apinfo_idx;k++)
				{
					if(strcmp(apinfos[k].BSSID, uppermac)==0 && strcmp(apinfos[k].SSID, info->SSID)==0)
					{
						apinfo_idx_same=k;
						break;
					}
				}

				if (apinfo_idx_same!=-1)
				{
					apinfos[apinfo_idx_same].RSSI=info->RSSI;
				} else
				{
					strcpy(apinfos[apinfo_idx].BSSID, uppermac);
					strcpy(apinfos[apinfo_idx].SSID, info->SSID);
					apinfos[apinfo_idx].RSSI=info->RSSI;
					apinfos[apinfo_idx].channel=((wl_bss_info_107_t *) info)->channel;

					if ((info->capability & 0x10) == 0x10)
						apinfos[apinfo_idx].wep=1;
					else
						apinfos[apinfo_idx].wep=0;
					apinfos[apinfo_idx].wpa=0;
					apinfos[apinfo_idx].status=0;
					apinfo_idx++;
				}

				ie = (struct bss_ie_hdr *) ((u8 *) info + sizeof(*info));
				for (left = info->ie_length; left > 0; 
					left -= (ie->len + 2), ie = (struct bss_ie_hdr *) ((u8 *) ie + 2 + ie->len)) 
				{
					if (!(ie->elem_id == 0xdd) && !(ie->elem_id == 0x30))
						continue;

					if (wpa_parse_wpa_ie(&ie->elem_id, ie->len+2, &apinfos[apinfo_idx-1].wid)==0)
					{
						apinfos[apinfo_idx-1].wpa=1;
						break;
					}
				}
next_info:
				info = (wl_bss_info_t *) ((u8 *) info + info->length);
			}
		}
	}

	if (nvram_invmatch("wl_sta_mac", ""))
	{
		for (k=0;k<apinfo_idx;k++)
		{
			if (!strcmp(apinfos[k].SSID, nvram_safe_get("wl0_ssid")) && strcmp(apinfos[k].BSSID, nvram_safe_get("wl_sta_mac"))==0)
			{
//				fprintf(stderr, "%s\n", apinfos[k].SSID);
				goto add_no_hidden_ap;
			}
		}

		wsp.ssid.SSID_len=strlen(nvram_safe_get("wl0_ssid"));
		memset(wsp.ssid.SSID, 0, sizeof(wsp.ssid.SSID));
		strcpy(wsp.ssid.SSID, nvram_safe_get("wl0_ssid"));
		result->count=0;

		count=0;
		while ((ret=wl_ioctl(WIF, WLC_SCAN, &wsp, sizeof(wsp))) < 0 && count<2)
		{
			count++;
			sleep(1);
		}
		usleep(2500*1000);

		if (ret==0)
		{
			count=0;
			while ((ret=wl_ioctl(WIF, WLC_SCAN_RESULTS, result, WLC_IOCTL_MAXLEN)) < 0 && count<2)
			{
				count++;
				sleep(1);
			}

			info = &(result->bss_info[0]);
			info_b = (unsigned char *)info;

			for(i=0;i<result->count;i++)
			{
//				fprintf(stderr, "%s\n", info->SSID);

				int left;
				struct bss_ie_hdr *ie;

				bssid = (unsigned char *)&info->BSSID;
				sprintf(uppermac, "%02X:%02X:%02X:%02X:%02X:%02X", (unsigned char)bssid[0], (unsigned char)bssid[1], (unsigned char)bssid[2], (unsigned char)bssid[3], (unsigned char)bssid[4], (unsigned char)bssid[5]);

				apinfo_idx_same=-1;
				for (k=0;k<apinfo_idx;k++)
				{
					if(strcmp(apinfos[k].BSSID, uppermac)==0 && strcmp(apinfos[k].SSID, info->SSID)==0)
					{
						apinfo_idx_same=k;
						break;
					}
				}

				if (apinfo_idx_same!=-1)
				{
					apinfos[apinfo_idx_same].RSSI=info->RSSI;
				} else
				{
					strcpy(apinfos[apinfo_idx].BSSID, uppermac);
					strcpy(apinfos[apinfo_idx].SSID, info->SSID);
					apinfos[apinfo_idx].RSSI=info->RSSI;
					apinfos[apinfo_idx].channel=((wl_bss_info_107_t *) info)->channel;

					if ((info->capability & 0x10) == 0x10)
						apinfos[apinfo_idx].wep=1;
					else
						apinfos[apinfo_idx].wep=0;
					apinfos[apinfo_idx].wpa=0;
					apinfos[apinfo_idx].status=0;
					apinfo_idx++;
				}

				ie = (struct bss_ie_hdr *) ((u8 *) info + sizeof(*info));
				for (left = info->ie_length; left > 0; 
					left -= (ie->len + 2), ie = (struct bss_ie_hdr *) ((u8 *) ie + 2 + ie->len)) 
				{
					if (!(ie->elem_id == 0xdd) && !(ie->elem_id == 0x30))
						continue;

					if (wpa_parse_wpa_ie(&ie->elem_id, ie->len+2, &apinfos[apinfo_idx-1].wid)==0)
					{
						apinfos[apinfo_idx-1].wpa=1;
						break;
					}
				}
				info = (wl_bss_info_t *) ((u8 *) info + info->length);
			}
		}
		
add_no_hidden_ap:
	}

end:
	nvram_set("wl_sta_state2", "");
	if (apinfo_idx>0 && nvram_invmatch("wl_mode_EX", "ap"))
	{
	for (k=0;k<apinfo_idx;k++)
	{
		if (strcmp(apinfos[k].BSSID, nvram_safe_get("wl_sta_mac"))==0 && strlen(nvram_safe_get("wl_sta_mac")))
		{
			match_mac=1;
			if (nvram_match("wl0_wep", "disabled"))
			{
				if (nvram_invmatch("wl0_akm", "psk") && nvram_invmatch("wl0_akm", "psk2"))
				{
					apinfos[k].status=2;
					nvram_set("wl_sta_state2", "authorized");
				}
				else
				{
					if (nvram_match("wl_sta_state", "authorized"))
					{
						apinfos[k].status=2;
						nvram_set("wl_sta_state2", "authorized");
					}
					else
					{
						apinfos[k].status=1;
					}
				}
			}
			else
			{
				apinfos[k].status=2;
				nvram_set("wl_sta_state2", "authorized");
			}
			break;
		}
	}
	}

/*
	if (apinfo_idx>0 && match_mac==0 && nvram_invmatch("wl_mode_EX", "ap"))
	{
		for (k=0;k<apinfo_idx;k++)
		{
			if (strcmp(apinfos[k].SSID, nvram_safe_get("wl0_ssid"))==0)
			{
				apinfos[k].status=1;
				break;
			}
		}
	}
*/

	for (i=0; i<4; i++)
	{
		memset(nvram_str, 0, sizeof(nvram_str));
		sprintf(nvram_str, "pf%d_ssid", i+1);

		if (nvram_invmatch(nvram_str, ""))
		{		
			for (k=0;k<apinfo_idx;k++)
			{
				if (strcmp(apinfos[k].SSID, nvram_safe_get(nvram_str))==0)
				{
					if (apinfos[k].status==0)
						apinfos[k].status=3;
				}
			}
		}
	}

	retval += websWrite(wp, "<script language=\"JavaScript\">\n");
	retval += websWrite(wp, "var ap_count=%d;\n", apinfo_idx);
	if (apinfo_idx>0)
	{
		retval += websWrite(wp, "var AP_status = new Array(ap_count);\n");	// var AP_status = new Array(a_count);
		for (k=0;k<apinfo_idx;k++)
		{
			memset(ssid_str, 0, sizeof(ssid_str));
			char_to_ascii(ssid_str, apinfos[k].SSID);
			retval += websWrite(wp, "AP_status[%d] = new Array('%s', '%d', ", k, ssid_str, apinfos[k].channel);	// AP_status[0] = new Array("AP_1", "1", 
			
			if (apinfos[k].wpa==1)
				retval += websWrite(wp, "'%s', ", wpa_html_txt2(apinfos[k].wid.pairwise_cipher, apinfos[k].wid.key_mgmt, apinfos[k].wid.proto));
			else if (apinfos[k].wep==1)
				retval += websWrite(wp, "'1', ");
			else
				retval += websWrite(wp, "'0', ");

			if(apinfos[k].RSSI<RSSI_TH_D)
				retval += websWrite(wp, "'1', ");
			else if ((apinfos[k].RSSI>=RSSI_TH_D) && (apinfos[k].RSSI<RSSI_TH_C))
				retval += websWrite(wp, "'2', ");
			else if ((apinfos[k].RSSI>=RSSI_TH_C) && (apinfos[k].RSSI<RSSI_TH_B))
				retval += websWrite(wp, "'3', ");
			else if ((apinfos[k].RSSI>=RSSI_TH_B) && (apinfos[k].RSSI<RSSI_TH_A))
				retval += websWrite(wp, "'4', ");
			else
				retval += websWrite(wp, "'5', ");

			if(apinfos[k].status==0)
				retval += websWrite(wp, "'none'");
			else if(apinfos[k].status==1)
				retval += websWrite(wp, "'connecting'");
			else if(apinfos[k].status==2)
				retval += websWrite(wp, "'authenticated'");
			else if(apinfos[k].status==3)
				retval += websWrite(wp, "'default'");
			else
				retval += websWrite(wp, "'fail'");
//			retval += websWrite(wp, "\"none\"");

			retval += websWrite(wp, ");\n");
		}
	}
	retval += websWrite(wp, "</script>");

	nvram_set("ap_scanning", "0");
	return retval;
}

int
ej_ap_scan3(int eid, webs_t wp, int argc, char_t **argv)
{
	int RSSI_TH_A=atoi(nvram_safe_get("threshold_a"));
	int RSSI_TH_B=atoi(nvram_safe_get("threshold_b"));
	int RSSI_TH_C=atoi(nvram_safe_get("threshold_c"));
	int RSSI_TH_D=atoi(nvram_safe_get("threshold_d"));
	int RSSI_TH_E=atoi(nvram_safe_get("threshold_e"));
	int match_mac=0;
	char nvram_str[32];

	nvram_set("ap_scanning", "1");
	int unit=0;
	int ret, i, j, k;
	wl_scan_results_t *result;
	wl_bss_info_t *info;
	char *info_b;
	unsigned char *bssid;
	unsigned char bssid2[6];
	char uppermac[17];
	int rssi=-32768;
	int retval=0;
	int ssid_len=0;
	int apinfo_idx=0;
	int apinfo_idx_same=-1;
	int count=0;
	char ssid_str[256];
	FILE *fp;
	char sel_str[32];

	wlc_ssid_t wst = {0, ""};
	wl_scan_params_asus wsp;
	wsp.ssid=wst;
	wsp.bssid.octet[0]=0xff;
	wsp.bssid.octet[1]=0xff;
	wsp.bssid.octet[2]=0xff;
	wsp.bssid.octet[3]=0xff;
	wsp.bssid.octet[4]=0xff;
	wsp.bssid.octet[5]=0xff;
	wsp.bss_type=DOT11_BSSTYPE_ANY;
	wsp.scan_type=DOT11_SCANTYPE_ACTIVE;
	wsp.nprobes=wsp.active_time=wsp.passive_time=wsp.home_time=-1;
	wsp.channel_num=0;
	wsp.channel_list[0]=1;
	wsp.channel_list[1]=2;
	wsp.channel_list[2]=3;
	wsp.channel_list[3]=4;
	wsp.channel_list[4]=5;
	wsp.channel_list[5]=6;
	wsp.channel_list[6]=7;
	wsp.channel_list[7]=8;
	wsp.channel_list[8]=9;
	wsp.channel_list[9]=10;
	wsp.channel_list[10]=11;
	wsp.channel_list[11]=12;
	wsp.channel_list[12]=13;
	wsp.channel_list[13]=14;

	result = (wl_scan_results_t *)buf;
	result->buflen=WLC_IOCTL_MAXLEN - sizeof(result);
	result->version=WL_BSS_INFO_VERSION;
	result->count=0;

	int SSID_valid=1;

	while ((ret=wl_ioctl(WIF, WLC_SCAN, &wsp, sizeof(wsp))) < 0 && count<2)
	{
		count++;
		sleep(1);
	}
//	sleep(3);
	usleep(3500*1000);
	if (ret==0)
	{
		count=0;
		while ((ret=wl_ioctl(WIF, WLC_SCAN_RESULTS, result, WLC_IOCTL_MAXLEN)) < 0 && count<2)
		{
			count++;
			sleep(1);
		}
		if (ret==0)
		{
			info = &(result->bss_info[0]);
			info_b = (unsigned char *)info;
			
			for(i=0;i<result->count;i++)
			{
				if (info->SSID_len>32 || info->SSID_len==0)
					goto next_info;

				SSID_valid=1;
				for(j=0;j<info->SSID_len;j++)
				{
					if(info->SSID[j]<32 || info->SSID[j]>126)
					{
						SSID_valid=0;
						break;
					}
				}
				if(!SSID_valid)
					goto next_info;

				int left;
				struct bss_ie_hdr *ie;

				bssid = (unsigned char *)&info->BSSID;
				sprintf(uppermac, "%02X:%02X:%02X:%02X:%02X:%02X", (unsigned char)bssid[0], (unsigned char)bssid[1], (unsigned char)bssid[2], (unsigned char)bssid[3], (unsigned char)bssid[4], (unsigned char)bssid[5]);

				apinfo_idx_same=-1;
				for (k=0;k<apinfo_idx;k++)
				{
					if(strcmp(apinfos[k].BSSID, uppermac)==0 && strcmp(apinfos[k].SSID, info->SSID)==0)
					{
						apinfo_idx_same=k;
						break;
					}
				}

				if (apinfo_idx_same!=-1)
				{
					apinfos[apinfo_idx_same].RSSI=info->RSSI;
				} else
				{
					strcpy(apinfos[apinfo_idx].BSSID, uppermac);
					strcpy(apinfos[apinfo_idx].SSID, info->SSID);
					apinfos[apinfo_idx].RSSI=info->RSSI;
					apinfos[apinfo_idx].channel=((wl_bss_info_107_t *) info)->channel;

					if ((info->capability & 0x10) == 0x10)
						apinfos[apinfo_idx].wep=1;
					else
						apinfos[apinfo_idx].wep=0;
					apinfos[apinfo_idx].wpa=0;
					apinfos[apinfo_idx].status=0;
					apinfo_idx++;
				}

				ie = (struct bss_ie_hdr *) ((u8 *) info + sizeof(*info));
				for (left = info->ie_length; left > 0; 
					left -= (ie->len + 2), ie = (struct bss_ie_hdr *) ((u8 *) ie + 2 + ie->len)) 
				{
					if (!(ie->elem_id == 0xdd) && !(ie->elem_id == 0x30))
						continue;

					if (wpa_parse_wpa_ie(&ie->elem_id, ie->len+2, &apinfos[apinfo_idx-1].wid)==0)
					{
						apinfos[apinfo_idx-1].wpa=1;
						break;
					}
				}
next_info:
				info = (wl_bss_info_t *) ((u8 *) info + info->length);
			}
		}
	}

	if (nvram_invmatch("wl_sta_mac", ""))
	{
		for (k=0;k<apinfo_idx;k++)
		{
			if (!strcmp(apinfos[k].SSID, nvram_safe_get("wl0_ssid")) && strcmp(apinfos[k].BSSID, nvram_safe_get("wl_sta_mac"))==0)
			{
//				fprintf(stderr, "%s\n", apinfos[k].SSID);
				goto add_no_hidden_ap;
			}
		}

		wsp.ssid.SSID_len=strlen(nvram_safe_get("wl0_ssid"));
		memset(wsp.ssid.SSID, 0, sizeof(wsp.ssid.SSID));
		strcpy(wsp.ssid.SSID, nvram_safe_get("wl0_ssid"));
		result->count=0;

		count=0;
		while ((ret=wl_ioctl(WIF, WLC_SCAN, &wsp, sizeof(wsp))) < 0 && count<2)
		{
			count++;
			sleep(1);
		}
		usleep(2500*1000);

		if (ret==0)
		{
			count=0;
			while ((ret=wl_ioctl(WIF, WLC_SCAN_RESULTS, result, WLC_IOCTL_MAXLEN)) < 0 && count<2)
			{
				count++;
				sleep(1);
			}

			info = &(result->bss_info[0]);
			info_b = (unsigned char *)info;

			for(i=0;i<result->count;i++)
			{
//				fprintf(stderr, "%s\n", info->SSID);

				int left;
				struct bss_ie_hdr *ie;

				bssid = (unsigned char *)&info->BSSID;
				sprintf(uppermac, "%02X:%02X:%02X:%02X:%02X:%02X", (unsigned char)bssid[0], (unsigned char)bssid[1], (unsigned char)bssid[2], (unsigned char)bssid[3], (unsigned char)bssid[4], (unsigned char)bssid[5]);

				apinfo_idx_same=-1;
				for (k=0;k<apinfo_idx;k++)
				{
					if(strcmp(apinfos[k].BSSID, uppermac)==0 && strcmp(apinfos[k].SSID, info->SSID)==0)
					{
						apinfo_idx_same=k;
						break;
					}
				}

				if (apinfo_idx_same!=-1)
				{
					apinfos[apinfo_idx_same].RSSI=info->RSSI;
				} else
				{
					strcpy(apinfos[apinfo_idx].BSSID, uppermac);
					strcpy(apinfos[apinfo_idx].SSID, info->SSID);
					apinfos[apinfo_idx].RSSI=info->RSSI;
					apinfos[apinfo_idx].channel=((wl_bss_info_107_t *) info)->channel;

					if ((info->capability & 0x10) == 0x10)
						apinfos[apinfo_idx].wep=1;
					else
						apinfos[apinfo_idx].wep=0;
					apinfos[apinfo_idx].wpa=0;
					apinfos[apinfo_idx].status=0;
					apinfo_idx++;
				}

				ie = (struct bss_ie_hdr *) ((u8 *) info + sizeof(*info));
				for (left = info->ie_length; left > 0; 
					left -= (ie->len + 2), ie = (struct bss_ie_hdr *) ((u8 *) ie + 2 + ie->len)) 
				{
					if (!(ie->elem_id == 0xdd) && !(ie->elem_id == 0x30))
						continue;

					if (wpa_parse_wpa_ie(&ie->elem_id, ie->len+2, &apinfos[apinfo_idx-1].wid)==0)
					{
						apinfos[apinfo_idx-1].wpa=1;
						break;
					}
				}
				info = (wl_bss_info_t *) ((u8 *) info + info->length);
			}
		}
		
add_no_hidden_ap:
	}

end:
	nvram_set("wl_sta_state2", "");
	if (apinfo_idx>0 && nvram_invmatch("wl_mode_EX", "ap"))
	{
	for (k=0;k<apinfo_idx;k++)
	{
		if (strcmp(apinfos[k].BSSID, nvram_safe_get("wl_sta_mac"))==0 && strlen(nvram_safe_get("wl_sta_mac")))
		{
			match_mac=1;
			if (nvram_match("wl0_wep", "disabled"))
			{
				if (nvram_invmatch("wl0_akm", "psk") && nvram_invmatch("wl0_akm", "psk2"))
				{
					apinfos[k].status=2;
					nvram_set("wl_sta_state2", "authorized");
				}
				else
				{
					if (nvram_match("wl_sta_state", "authorized"))
					{
						apinfos[k].status=2;
						nvram_set("wl_sta_state2", "authorized");
					}
					else
					{
						apinfos[k].status=1;
					}
				}
			}
			else
			{
				apinfos[k].status=2;
				nvram_set("wl_sta_state2", "authorized");
			}
			break;
		}
	}
	}

/*
	if (apinfo_idx>0 && match_mac==0 && nvram_invmatch("wl_mode_EX", "ap"))
	{
		for (k=0;k<apinfo_idx;k++)
		{
			if (strcmp(apinfos[k].SSID, nvram_safe_get("wl0_ssid"))==0)
			{
				apinfos[k].status=1;
				break;
			}
		}
	}
*/

	for (i=0; i<4; i++)
	{
		memset(nvram_str, 0, sizeof(nvram_str));
		sprintf(nvram_str, "pf%d_ssid", i+1);

		if (nvram_invmatch(nvram_str, ""))
		{		
			for (k=0;k<apinfo_idx;k++)
			{
				if (strcmp(apinfos[k].SSID, nvram_safe_get(nvram_str))==0)
				{
					if (apinfos[k].status==0)
						apinfos[k].status=3;
				}
			}
		}
	}

	retval += websWrite(wp, "<script language=\"JavaScript\">\n");
	retval += websWrite(wp, "var ap_count=%d;\n", apinfo_idx);
	if (apinfo_idx>0)
	{
		retval += websWrite(wp, "var AP_status = new Array(ap_count);\n");	// var AP_status = new Array(a_count);
		for (k=0;k<apinfo_idx;k++)
		{

			memset(ssid_str, 0, sizeof(ssid_str));
			char_to_ascii(ssid_str, apinfos[k].SSID);
			retval += websWrite(wp, "AP_status[%d] = new Array('%s', '%d', ", k, ssid_str, apinfos[k].channel);	// AP_status[0] = new Array("AP_1", "1", 
			
			if (apinfos[k].wpa==1)
				retval += websWrite(wp, "'%s', ", wpa_html_txt2(apinfos[k].wid.pairwise_cipher, apinfos[k].wid.key_mgmt, apinfos[k].wid.proto));
			else if (apinfos[k].wep==1)
				retval += websWrite(wp, "'1', ");
			else
				retval += websWrite(wp, "'0', ");

			if(apinfos[k].RSSI<RSSI_TH_D)
				retval += websWrite(wp, "'1', ");
			else if ((apinfos[k].RSSI>=RSSI_TH_D) && (apinfos[k].RSSI<RSSI_TH_C))
				retval += websWrite(wp, "'2', ");
			else if ((apinfos[k].RSSI>=RSSI_TH_C) && (apinfos[k].RSSI<RSSI_TH_B))
				retval += websWrite(wp, "'3', ");
			else if ((apinfos[k].RSSI>=RSSI_TH_B) && (apinfos[k].RSSI<RSSI_TH_A))
				retval += websWrite(wp, "'4', ");
			else
				retval += websWrite(wp, "'5', ");

			if(apinfos[k].status==0)
				retval += websWrite(wp, "'none'");
			else if(apinfos[k].status==1)
				retval += websWrite(wp, "'connecting'");
			else if(apinfos[k].status==2)
				retval += websWrite(wp, "'authenticated'");
			else if(apinfos[k].status==3)
				retval += websWrite(wp, "'default'");
			else
				retval += websWrite(wp, "'fail'");
//			retval += websWrite(wp, "\"none\"");

			retval += websWrite(wp, ");\n");
		}
	}
	retval += websWrite(wp, "</script>");

	nvram_set("ap_scanning", "0");
	return retval;
}

int
ej_ap_scan4(int eid, webs_t wp, int argc, char_t **argv)
{
	if (nvram_match("wl_hotspot", "1"))
		return ej_ap_scan3(eid, wp, argc, argv);
	else
		return 0;
}

int
ej_autoip_src(int eid, webs_t wp, int argc, char_t **argv)
{
	int retval=0;

	if (nvram_match("preferred_lang", "TW"))
		retval += websWrite(wp, "graph/autoIP_tw.gif");
	else if (nvram_match("preferred_lang", "CN"))
		retval += websWrite(wp, "graph/autoIP_cn.gif");
	else if (nvram_match("preferred_lang", "DE"))
		retval += websWrite(wp, "graph/autoIP_de.gif");
	else if (nvram_match("preferred_lang", "JP"))
		retval += websWrite(wp, "graph/autoIP_jp.gif");
	else
		retval += websWrite(wp, "graph/autoIP_en.gif");

	return retval;
}

int
ej_ap_select(int eid, webs_t wp, int argc, char_t **argv)
{
	nvram_set("ap_selecting", "1");
	int unit=0;
	int ret, i, j, k;
	wl_scan_results_t *result;
	wl_bss_info_t *info;
//	wl_bss_info_107_t *info;
	char *info_b;
	unsigned char *bssid;
	unsigned char bssid2[6];
	char uppermac[17];
	int rssi=-32768;
	int retval=0;
	int ssid_len=0;
	int apinfo_idx=0;
	int apinfo_idx_same=-1;
	int count=0;
	char ssid_str[256];

	wlc_ssid_t wst = {0, ""};
	wl_scan_params_asus wsp;
	wsp.ssid=wst;
	wsp.bssid.octet[0]=0xff;
	wsp.bssid.octet[1]=0xff;
	wsp.bssid.octet[2]=0xff;
	wsp.bssid.octet[3]=0xff;
	wsp.bssid.octet[4]=0xff;
	wsp.bssid.octet[5]=0xff;
	wsp.bss_type=DOT11_BSSTYPE_ANY;
//	wsp.scan_type=DOT11_SCANTYPE_PASSIVE;
	wsp.scan_type=DOT11_SCANTYPE_ACTIVE;
	wsp.nprobes=wsp.active_time=wsp.passive_time=wsp.home_time=-1;
	wsp.channel_num=0;
	wsp.channel_list[0]=1;
	wsp.channel_list[1]=2;
	wsp.channel_list[2]=3;
	wsp.channel_list[3]=4;
	wsp.channel_list[4]=5;
	wsp.channel_list[5]=6;
	wsp.channel_list[6]=7;
	wsp.channel_list[7]=8;
	wsp.channel_list[8]=9;
	wsp.channel_list[9]=10;
	wsp.channel_list[10]=11;
	wsp.channel_list[11]=12;
	wsp.channel_list[12]=13;
	wsp.channel_list[13]=14;

	result = (wl_scan_results_t *)buf;
	result->buflen=WLC_IOCTL_MAXLEN - sizeof(result);
	result->version=WL_BSS_INFO_VERSION;
	result->count=0;

	int SSID_valid=1;

	while ((ret=wl_ioctl(WIF, WLC_SCAN, &wsp, sizeof(wsp))) < 0 && count<2)
	{
		count++;
		sleep(1);
	}
//	sleep(3);
	usleep(3500*1000);
	if (ret==0)
	{
		count=0;
		while ((ret=wl_ioctl(WIF, WLC_SCAN_RESULTS, result, WLC_IOCTL_MAXLEN)) < 0 && count<2)
		{
			count++;
			sleep(1);
		}
		if (ret==0)
		{
			info = &(result->bss_info[0]);
			info_b = (unsigned char *)info;
			
			for(i=0;i<result->count;i++)
			{
				SSID_valid=1;
				for(j=0;j<info->SSID_len;j++)
				{
					if(info->SSID[j]<32 || info->SSID[j]>126)
					{
						SSID_valid=0;
						break;
					}
				}
				if(!SSID_valid)
					goto next_info;

				int left;
				struct bss_ie_hdr *ie;

				bssid = (unsigned char *)&info->BSSID;
				sprintf(uppermac, "%02X:%02X:%02X:%02X:%02X:%02X", (unsigned char)bssid[0], (unsigned char)bssid[1], (unsigned char)bssid[2], (unsigned char)bssid[3], (unsigned char)bssid[4], (unsigned char)bssid[5]);

				apinfo_idx_same=-1;
				for (k=0;k<apinfo_idx;k++)
				{
					if(strcmp(apinfos[k].BSSID, uppermac)==0 && strcmp(apinfos[k].SSID, info->SSID)==0)
					{
						apinfo_idx_same=k;
						break;
					}
				}

				if (apinfo_idx_same!=-1)
				{
					apinfos[apinfo_idx_same].RSSI=info->RSSI;
				} else
				{
					strcpy(apinfos[apinfo_idx].BSSID, uppermac);
					strcpy(apinfos[apinfo_idx].SSID, info->SSID);
					apinfos[apinfo_idx].RSSI=info->RSSI;
//					apinfos[apinfo_idx].channel=info->ctl_ch;
					apinfos[apinfo_idx].channel=((wl_bss_info_107_t *) info)->channel;

//					apinfos[apinfo_idx].channel=info->channel;
					if ((info->capability & 0x10) == 0x10)
						apinfos[apinfo_idx].wep=1;
					else
						apinfos[apinfo_idx].wep=0;
					apinfos[apinfo_idx].wpa=0;
					apinfo_idx++;
				}

				ie = (struct bss_ie_hdr *) ((u8 *) info + sizeof(*info));
				for (left = info->ie_length; left > 0; 
					left -= (ie->len + 2), ie = (struct bss_ie_hdr *) ((u8 *) ie + 2 + ie->len)) 
				{
					if (!(ie->elem_id == 0xdd) && !(ie->elem_id == 0x30))
						continue;

					if (wpa_parse_wpa_ie(&ie->elem_id, ie->len+2, &apinfos[apinfo_idx-1].wid)==0)
					{
						apinfos[apinfo_idx-1].wpa=1;
						break;
					}
				}
next_info:
				info = (wl_bss_info_t *) ((u8 *) info + info->length);
//				info = (wl_bss_info_107_t *) ((u8 *) info + info->length);
			}
		}
	}
end:
	retval += websWrite(wp, "<input type=\"hidden\" name=\"apinfo_n\" value=\"%d\" readonly=\"1\">", apinfo_idx);
	if (apinfo_idx==0)
	{
		retval += websWrite(wp, "<tr><td class='content_header_td_ap2'>&nbsp;</td>");
//		retval += websWrite(wp, "<td class='content_header_td_ap2'><font color='#FF0000'>Finds no AP!</font></td>");

		FILE *fp;
		char noap_str[32];
		if (nvram_match("preferred_lang", "TW"))
			fp=fopen("/www/noap_TW.asp", "r");
		else if (nvram_match("preferred_lang", "CN"))
			fp=fopen("/www/noap_CN.asp", "r");
		if (fp!=NULL)
		{
			if (fgets(noap_str, 32, fp)==NULL)
				sprintf(noap_str, "Finds no AP!");
			fclose(fp);
		}

		if (nvram_match("preferred_lang", "EN"))
			retval += websWrite(wp, "<td class='content_header_td_ap2'><font color='#FF0000'>Finds no AP!</font></td>");
		else if (nvram_match("preferred_lang", "DE"))
			retval += websWrite(wp, "<td class='content_header_td_ap2'><font color='#FF0000'>Es wurde kein AP gefunden!</font></td>");
		else
			retval += websWrite(wp, "<td class='content_header_td_ap2'><font color='#FF0000'>%s</font></td>", noap_str);

		retval += websWrite(wp, "<td class='content_header_td_ap2'>&nbsp;</td>");
		retval += websWrite(wp, "<td class='content_header_td_ap2'>&nbsp;</td>");
		retval += websWrite(wp, "<td class='content_header_td_ap2'>&nbsp;</td></tr>");
	}
	else
	for (k=0;k<apinfo_idx;k++)
	{
		retval += websWrite(wp, "<tr><td class='content_header_td_ap2'><a href=\"javascript:copyBSSID('%s')\">%s</a>", apinfos[k].BSSID, apinfos[k].BSSID);

		if (strlen(apinfos[k].SSID)==0)
		{
			retval += websWrite(wp, "<td nowarp=\"nowarp\" class='content_header_td_ap2'>&nbsp;</td><input type=\"hidden\" class=\"content_input_fd\" name=\"RSSID\" value=\"\" readonly=\"1\"><input type=\"hidden\" name=\"RSSIDE\" value=\"\" readonly=\"1\">");
		}
		else
		{
			retval += websWrite(wp, "<td nowarp=\"nowarp\" class='content_header_td_ap2'><input type=\"text\" class=\"content_input_fd\" name=\"RSSID\" value=\"\" readonly=\"1\">");
			memset(ssid_str, 0, sizeof(ssid_str));
			char_to_ascii(ssid_str, apinfos[k].SSID);
			retval += websWrite(wp, "<input type=\"hidden\" name=\"RSSIDE\" value=\"%s\" readonly=\"1\">", ssid_str);
		}

		if (apinfos[k].wpa==1)
			retval += websWrite(wp, "<td class='content_header_td_ap2'>%s (%s)</td>", wpa_cipher_txt(apinfos[k].wid.pairwise_cipher), wpa_key_mgmt_txt(apinfos[k].wid.key_mgmt, apinfos[k].wid.proto));
		else if (apinfos[k].wep==1)
			retval += websWrite(wp, "<td class='content_header_td_ap2'>WEP</td>");
		else
			retval += websWrite(wp, "<td class='content_header_td_ap2'>Off</td>");
		retval += websWrite(wp, "<td class='content_header_td_ap2'>%d</td>", apinfos[k].channel);
		retval += websWrite(wp, "<td class='content_header_td_ap2'>%d dBm</td></tr>", apinfos[k].RSSI);
		
	}
	nvram_set("ap_selecting", "0");
	return retval;
}

char *nvram_get_i2(char *name, int idx)
{
	char tmpstr[64];

	if (idx!=-1)
	{
		sprintf(tmpstr, "%s%d", name, idx);
	}
	else
	{
		sprintf(tmpstr, "%s", name);
	}

	return(nvram_get(tmpstr));
}

struct mssidinfo
{
	int index;
	char ssid[32];
	int vlanid;
	char auth_mode[6];
	char crypto[8];
	char wpa_psk[64];
	int wep_x;
	char wl_phrase_x[64];
	char wl_key1[26];
	char wl_key2[26];
	char wl_key3[26];
	char wl_key4[26];
	int wl_key;
	int wl_wpa_gtk_rekey;
	int enable;
}mssidinfos[4];

int
ej_mssid(int eid, webs_t wp, int argc, char_t **argv)
{
	int retval=0;
	int mssid_num=0;
	int i;
	int vlanid_pri=1;
	int idx_mssid[4];
	
	for (i=1; i<4; i++)
		idx_mssid[i]=-1;
	
	mssid_num=atoi(nvram_get("mssidnum_x"));

	for (i=0; i<mssid_num; i++)
	{
		mssidinfos[i].index=atoi(nvram_get_i2("mssid_index_x", i));
		if (mssidinfos[i].index==0)
		{
			vlanid_pri=atoi(nvram_get_i2("mssid_vlanid_x", i));
			continue;
		}
		else if (mssidinfos[i].index==1)
			idx_mssid[1]=i;
		else if (mssidinfos[i].index==2)
			idx_mssid[2]=i;
		else if (mssidinfos[i].index==3)
			idx_mssid[3]=i;
		strcpy(mssidinfos[i].ssid, nvram_get_i2("mssid_ssid_x", i));
		mssidinfos[i].vlanid=atoi(nvram_get_i2("mssid_vlanid_x", i));
		strcpy(mssidinfos[i].auth_mode, nvram_get_i2("mssid_auth_mode_x", i));
		mssidinfos[i].enable=atoi(nvram_get_i2("mssid_enable_x", i));
	}

	retval += websWrite(wp, "<tr><td class='content_header_td_ap2'>Primary SSID</td>");
//	retval += websWrite(wp, "<td class='content_header_td_ap2'>%s</td>", nvram_get("wl_ssid"));
	retval += websWrite(wp, "<td class='content_header_td_ap2'><input type=\"text\" maxlength=\"32\" class=\"content_input_fd\" size=\"32\" name=\"mssid_0\" value=\"\" readonly=\"1\"></td>");
	if (strcmp(nvram_get("wl_auth_mode"), "open")==0)
		retval += websWrite(wp, "<td class='content_header_td_ap2'>Open System</td>");
	else if (strcmp(nvram_get("wl_auth_mode"), "shared")==0)
		retval += websWrite(wp, "<td class='content_header_td_ap2'>Shared Key</td>");
	else if (strcmp(nvram_get("wl_auth_mode"), "psk")==0)
		retval += websWrite(wp, "<td class='content_header_td_ap2'>WPA-PSK/WPA2-PSK</td>");
	else if (strcmp(nvram_get("wl_auth_mode"), "wpa")==0)
		retval += websWrite(wp, "<td class='content_header_td_ap2'>WPA-Enterprise</td>");
	else if (strcmp(nvram_get("wl_auth_mode"), "wpa2")==0)
		retval += websWrite(wp, "<td class='content_header_td_ap2'>WPA2-Enterprise</td>");
	else if (strcmp(nvram_get("wl_auth_mode"), "radius")==0)
		retval += websWrite(wp, "<td class='content_header_td_ap2'>Radius with 802.1x</td>");
	if (strcmp(nvram_get("vlans_enable"), "1")==0)
		retval += websWrite(wp, "<td class='content_header_td_ap2'>%d</td>", vlanid_pri);
	else
		retval += websWrite(wp, "<td class='content_header_td_ap2'>OFF</td>");
	retval += websWrite(wp, "<td class='content_header_td_ap2'>Enabled</td></tr>");

	for (i=1; i<4; i++)
	{
		if (i==1)
			retval += websWrite(wp, "<tr><td class='content_header_td_ap2'>Multi-SSID1</td>");
		else if (i==2)
			retval += websWrite(wp, "<tr><td class='content_header_td_ap2'>Multi-SSID2</td>");
		else if (i==3)
			retval += websWrite(wp, "<tr><td class='content_header_td_ap2'>Multi-SSID3</td>");
		
//		retval += websWrite(wp, "<td class='content_header_td_ap2'>%s</td>", mssidinfos[idx_mssid[i]].ssid);

		if (i==1)
			retval += websWrite(wp, "<td class='content_header_td_ap2'><input type=\"text\" maxlength=\"32\" class=\"content_input_fd\" size=\"32\" name=\"mssid_1\" value=\"\" readonly=\"1\"></td>");
		else if (i==2)
			retval += websWrite(wp, "<td class='content_header_td_ap2'><input type=\"text\" maxlength=\"32\" class=\"content_input_fd\" size=\"32\" name=\"mssid_2\" value=\"\" readonly=\"1\"></td>");
		else if (i==3)
			retval += websWrite(wp, "<td class='content_header_td_ap2'><input type=\"text\" maxlength=\"32\" class=\"content_input_fd\" size=\"32\" name=\"mssid_3\" value=\"\" readonly=\"1\"></td>");
		
		if (strcmp(mssidinfos[idx_mssid[i]].auth_mode, "open")==0)
			retval += websWrite(wp, "<td class='content_header_td_ap2'>Open System</td>");
		else if (strcmp(mssidinfos[idx_mssid[i]].auth_mode, "shared")==0)
			retval += websWrite(wp, "<td class='content_header_td_ap2'>Shared Key</td>");
		else if (strcmp(mssidinfos[idx_mssid[i]].auth_mode, "psk")==0)
			retval += websWrite(wp, "<td class='content_header_td_ap2'>WPA-PSK/WPA2-PSK</td>");
			
		if (strcmp(nvram_get("vlans_enable"), "1")==0)
			retval += websWrite(wp, "<td class='content_header_td_ap2'>%d</td>", mssidinfos[idx_mssid[i]].vlanid);
		else
			retval += websWrite(wp, "<td class='content_header_td_ap2'>OFF</td>");

		if (mssidinfos[idx_mssid[i]].enable==1)
			retval += websWrite(wp, "<td class='content_header_td_ap2'><font color='#0000FF'>Enabled</font></td>");
		else
			retval += websWrite(wp, "<td class='content_header_td_ap2'><font color='#FF0000'>Disabled</font></td>");
	}

	return retval;
}

int
ej_profile(int eid, webs_t wp, int argc, char_t **argv)
{
	int retval=0;
	int pf_num=0;
	int i;
	char nvram_str1[32];
	char nvram_str2[32];
	char nvram_str3[32];
	
	for (i=0; i<4; i++)
	{
		memset(nvram_str1, 0, sizeof(nvram_str1));
		memset(nvram_str2, 0, sizeof(nvram_str2));
		memset(nvram_str3, 0, sizeof(nvram_str3));

		sprintf(nvram_str1, "pf%d_ssid", i+1);
		sprintf(nvram_str2, "pf%d_auth_mode", i+1);
		sprintf(nvram_str3, "pf%d_wpa_mode", i+1);

		if (nvram_invmatch(nvram_str1, ""))
		{
			pf_num++;
			retval += websWrite(wp, "<tr><td class='content_header_td_ap2'><center>%d</center></td>", pf_num);
			retval += websWrite(wp, "<td class='content_header_td_ap2'><input type=\"text\" maxlength=\"32\" class=\"content_input_fd\" size=\"32\" name=\"pf_ssid\" value=\"\" readonly=\"1\"></td>");
		
			if (nvram_match(nvram_str2, "open"))
				retval += websWrite(wp, "<td class='content_header_td_ap2'><center>Open System</center></td>");
			else if (nvram_match(nvram_str2, "shared"))
				retval += websWrite(wp, "<td class='content_header_td_ap2'><center>Shared Key</center></td>");
			else if (nvram_match(nvram_str2, "psk"))
			{
				if (nvram_match(nvram_str3, "1"))
					retval += websWrite(wp, "<td class='content_header_td_ap2'><center>WPA-Personal</center></td>");
				else if (nvram_match(nvram_str3, "2"))
					retval += websWrite(wp, "<td class='content_header_td_ap2'><center>WPA2-Personal</center></td>");
				else
					retval += websWrite(wp, "<td class='content_header_td_ap2'><center>????????</center></td>");
			}

			retval += websWrite(wp, "<td class='content_header_td_ap2'><center><input type=\"checkbox\" name=\"pf%d_delete\" value=\"\" onClick=\"return change_common_radio(this, 'WLANConfig11b', 'pf%d_delete', '1')\"></center></td></tr>", i+1, i+1);
		}
	}

	if (pf_num==0)
	{
		retval += websWrite(wp, "<tr><td class='content_header_td_ap2'>&nbsp;</td>");
		retval += websWrite(wp, "<td class='content_header_td_ap2'><center><font color='#FF0000'><span id=\"profile_list\">No profile in the list!</span></font></center></td>");
		retval += websWrite(wp, "<td class='content_header_td_ap2'>&nbsp;</td>");
		retval += websWrite(wp, "<td class='content_header_td_ap2'>&nbsp;</td></tr>");
	}

	return retval;
}