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
 * Network services
 *
 * Copyright 2004, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: network.c,v 1.1.1.1 2007/01/25 12:52:21 jiahao_jhou Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <syslog.h>															
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if_arp.h>
#include <signal.h>
typedef u_int64_t u64;
typedef u_int32_t u32;
typedef u_int16_t u16;
typedef u_int8_t u8;
#include <linux/sockios.h>
#include <linux/ethtool.h>
#include <bcmnvram.h>
#include <netconf.h>
#include <shutils.h>
#include <wlutils.h>
#include <nvparse.h>
#include <rc.h>
#include <bcmutils.h>
#include <etioctl.h>
#include <bcmparams.h>

static int
add_routes(char *prefix, char *var, char *ifname)
{
	char word[80], *next;
	char *ipaddr, *netmask, *gateway, *metric;
	char tmp[100];

	foreach(word, nvram_safe_get(strcat_r(prefix, var, tmp)), next) {

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

		dprintf("\n\n\nadd %s %d %s %s %s\n\n\n", ifname, atoi(metric), ipaddr, gateway, netmask);
		
		route_add(ifname, atoi(metric) + 1, ipaddr, gateway, netmask);
	}

	return 0;
}

static int
del_routes(char *prefix, char *var, char *ifname)
{
	char word[80], *next;
	char *ipaddr, *netmask, *gateway, *metric;
	char tmp[100];
	
	foreach(word, nvram_safe_get(strcat_r(prefix, var, tmp)), next) {
		dprintf("add %s\n", word);
		
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
		
		dprintf("add %s\n", ifname);
		
		route_del(ifname, atoi(metric) + 1, ipaddr, gateway, netmask);
	}

	return 0;
}

static int
add_lan_routes(char *lan_ifname)
{
	return add_routes("lan_", "route", lan_ifname);
}

static int
del_lan_routes(char *lan_ifname)
{
	return del_routes("lan_", "route", lan_ifname);
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
#define WPA_KEY_MGMT_PSK_ BIT(1)
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

#define WIF "eth1"
char buf_ap[WLC_IOCTL_MAXLEN];

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
		return WPA_KEY_MGMT_IEEE8021X_;
	if (memcmp(s, RSN_AUTH_KEY_MGMT_PSK_OVER_802_1X, RSN_SELECTOR_LEN) ==
	    0)
		return WPA_KEY_MGMT_PSK_;
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
	data->key_mgmt = WPA_KEY_MGMT_IEEE8021X_;
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
	int idx;
	struct wpa_ie_data wid;
}apinfos[32];

struct profileinfo
{
	int onAir;
	int RSSI;
	int wep;
	int wpa;
	int match_ssid;
}profileinfos[4];

int
ap_scan()
{
	FILE *fp=NULL;
	int ret, i, j, k;
	wl_scan_results_t *result;
//	wl_bss_info_107_t *info;
	wl_bss_info_t *info;
	char *info_b;
	unsigned char *bssid;
	char uppermac[17];
	int apinfo_idx=0;
	int apinfo_idx_same=-1;
	int count=0;
	char nvram_str[32];
	char nvram_str2[32];
	char nvram_str3[32];
	char ssid_str[256];

	wlc_ssid_t wst = {0, ""};
	wl_scan_params_asus wsp;
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
	
	fprintf(stderr, "***************************************\n");
	fprintf(stderr, "**********   AP Scanning   ************\n");
	fprintf(stderr, "***************************************\n");

	
	fp = fopen("/tmp/apscan", "w");

	if (	nvram_match("pf1_ssid", "") &&
		nvram_match("pf2_ssid", "") &&
		nvram_match("pf3_ssid", "") &&
		nvram_match("pf4_ssid", "")
	)
	{
		if (fp)
		{
			fprintf(fp, "profile list is empty!\n");
		}

		if (nvram_match("wl_connect_none_default", "1"))
		{
			j=0;
			goto end;
		}

		if (fp)
		{
			fclose(fp);
		}

		return -1;
	}

	result = (wl_scan_results_t *)buf_ap;
	result->buflen=WLC_IOCTL_MAXLEN - sizeof(result);
	result->version=WL_BSS_INFO_VERSION;
	result->count=0;

	for (i=0; i<4; i++)
	{
		fprintf(fp, "IDX: %d\n", i+1);
		
		profileinfos[i].onAir=0;
		profileinfos[i].RSSI=-32768;
		profileinfos[i].wep=0;
		profileinfos[i].wpa=0;
		profileinfos[i].match_ssid=0;
		memset(nvram_str, 0, sizeof(nvram_str));
		sprintf(nvram_str, "pf%d_ssid", i+1);

		fprintf(fp, "SSID: %s\n", nvram_safe_get(nvram_str));

		memset(nvram_str2, 0, sizeof(nvram_str2));
		sprintf(nvram_str2, "pf%d_auth_mode", i+1);
		memset(nvram_str3, 0, sizeof(nvram_str3));
		sprintf(nvram_str3, "pf%d_wep_x", i+1);
		if (nvram_match(nvram_str2, "open") && nvram_invmatch(nvram_str3, "0"))
			profileinfos[i].wep=1;
		else if (nvram_match(nvram_str2, "shared"))
			profileinfos[i].wep=1;
		else if (nvram_match(nvram_str2, "psk"))
		{
			profileinfos[i].wep=1;
			profileinfos[i].wpa=1;
		}

		fprintf(fp, "wep: %d wpa: %d\n\n", profileinfos[i].wep, profileinfos[i].wpa);

		if (nvram_invmatch(nvram_str, ""))
		{
			
			wsp.ssid.SSID_len=strlen(nvram_safe_get(nvram_str));
			memset(wsp.ssid.SSID, 0, sizeof(wsp.ssid.SSID));
			strcpy(wsp.ssid.SSID, nvram_safe_get(nvram_str));
			result->count=0;
			apinfo_idx=0;

			count=0;
			while ((ret=wl_ioctl(WIF, WLC_SCAN, &wsp, sizeof(wsp))) < 0 && count<2)
			{
				count++;
				sleep(1);
			}
			sleep(3);
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
					
					for(j=0;j<result->count;j++)
					{
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
						}
						else
						{
							strcpy(apinfos[apinfo_idx].BSSID, uppermac);
							strcpy(apinfos[apinfo_idx].SSID, info->SSID);
							apinfos[apinfo_idx].RSSI=info->RSSI;
//							apinfos[apinfo_idx].channel=info->channel;
							apinfos[apinfo_idx].channel=((wl_bss_info_107_t *) info)->channel;
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

//						info = (wl_bss_info_107_t *) ((u8 *) info + info->length);
						info = (wl_bss_info_t *) ((u8 *) info + info->length);
					}
				}
			}

			for (k=0;k<apinfo_idx;k++)
			{
				fprintf(fp, "bssid: %s\n", apinfos[k].BSSID);
				fprintf(fp, "rssi: %d dBm\n", apinfos[k].RSSI);
				fprintf(fp, "wep: %d wpa: %d\n\n", apinfos[k].wep, apinfos[k].wpa);
				profileinfos[i].match_ssid++;
				if ( profileinfos[i].wep==apinfos[k].wep && profileinfos[i].wpa==apinfos[k].wpa)
				{
					profileinfos[i].onAir=1;
					if (apinfos[k].RSSI>profileinfos[i].RSSI)
						profileinfos[i].RSSI=apinfos[k].RSSI;

					if (nvram_match("wl_ap_order", "0"))
					{
						fprintf(fp, "\n***Selected IDX: %d***\n\n\n", i+1);
						if (fp)
							fclose(fp);
						return i+1;
					}
				}
			}

			fprintf(fp, "***rssi: %d dBm\n\n", profileinfos[i].RSSI);
		}
		else
			fprintf(fp, "***\n\n");
	}

	j=0;
	k=-32768;
	for (i=0; i<4; i++)
	{
		if (profileinfos[i].RSSI>k)
		{
			j=i+1;
			k=profileinfos[i].RSSI;
		}
	}

	fprintf(fp, "\n***Selected IDX: %d***\n\n\n", j);

	if (j==0 && nvram_match("wl_connect_none_default", "1"))
	{
end:
		result = (wl_scan_results_t *)buf_ap;
		result->buflen=WLC_IOCTL_MAXLEN - sizeof(result);
		result->version=WL_BSS_INFO_VERSION;
		result->count=0;

		wsp.ssid=wst;
		apinfo_idx=0;

		count=0;
		while ((ret=wl_ioctl(WIF, WLC_SCAN, &wsp, sizeof(wsp))) < 0 && count<2)
		{
			count++;
			sleep(1);
		}
		sleep(3);
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
				
				for(j=0;j<result->count;j++)
				{
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
					}
					else
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
		}
		
		int rssi_max=-32768;
		int idx_max=-1;
		for (k=0;k<apinfo_idx;k++)
		{
			fprintf(fp, "ssid: %s\n", apinfos[k].SSID);
			fprintf(fp, "rssi: %d dBm\n", apinfos[k].RSSI);
			fprintf(fp, "wep: %d wpa: %d\n\n", apinfos[k].wep, apinfos[k].wpa);
			if ( apinfos[k].wep==0 && apinfos[k].wpa==0)
			{
				if (apinfos[k].RSSI>rssi_max)
				{
					rssi_max=apinfos[k].RSSI;
					idx_max=k;
				}
			}
		}

		if (idx_max!=-1)
		{
			j=5;

   			nvram_set("pf5_ssid", apinfos[idx_max].SSID);
   			memset(ssid_str, 0, sizeof(ssid_str));
   			char_to_ascii(ssid_str, apinfos[idx_max].SSID);
   			nvram_set("pf5_ssid2", ssid_str);
   			nvram_set("pf5_auth_mode", "open");
   			nvram_set("pf5_crypto", "tkip");
   			nvram_set("pf5_wpa_psk", "");
   			nvram_set("pf5_wep_x", "0");
   			nvram_set("pf5_phrase", "");
   			nvram_set("pf5_key1", "");
   			nvram_set("pf5_key2", "");
   			nvram_set("pf5_key3", "");
   			nvram_set("pf5_key4", "");
   			nvram_set("pf5_key", "1");
//   			nvram_set("pf5_wpa_gtk_rekey", "0");

			fprintf(fp, "\n***Selected none-preferred AP: %s***\n\n\n", apinfos[idx_max].SSID);
		}
		else
		{
			j=0;

   			nvram_unset("pf5_ssid");
   			nvram_unset("pf5_ssid2");
   			nvram_unset("pf5_auth_mode");
   			nvram_unset("pf5_crypto");
   			nvram_unset("pf5_wpa_psk");
   			nvram_unset("pf5_wep_x");
   			nvram_unset("pf5_phrase");
   			nvram_unset("pf5_key1");
   			nvram_unset("pf5_key2");
   			nvram_unset("pf5_key3");
   			nvram_unset("pf5_key4");
   			nvram_unset("pf5_key");
//   			nvram_unset("pf5_wpa_gtk_rekey");

			fprintf(fp, "\n***No none-preferred AP is selected!!!\n\n\n");
		}
	}

	if (j==0)	// pick a profile if there is a SSID matching
	{
		for (i=0; i<4; i++)
		{
			if (profileinfos[i].match_ssid>0)
			{
				j=i+1;
				break;
			}
		}
	}

	if (j==0)	// pick a profile anyway
	{
		if (nvram_invmatch("pf1_ssid", ""))
			j=1;
		if (nvram_invmatch("pf2_ssid", ""))
			j=2;
		if (nvram_invmatch("pf3_ssid", ""))
			j=3;
		if (nvram_invmatch("pf4_ssid", ""))
			j=4;
	}

	if (fp)
		fclose(fp);

	return j;
}

char *nvram_get_i4(char *name, int idx)
{
	char tmpstr1[64];

	memset(tmpstr1, 0, sizeof(tmpstr1));
	sprintf(tmpstr1, "pf%d_%s", idx , name);
	return (nvram_safe_get(tmpstr1));
}

int
nvram_match_i(int idx, char *name, char *value)
{
	return (!strcmp(nvram_get_i4(name, idx), value));
}

void
start_lan(void)
{
	char *lan_ifname = nvram_safe_get("lan_ifname");
	char br0_ifnames[255];
	char name[80], *next;
	char tmpstr[48];
	int i, j;
	int s;
	struct ifreq ifr;
#ifdef WL330GE
	int profile_idx=-1;
	int ap_idx=1;
	char sbuf[64];
	char nvram_str[32];
	char ssidstr[128];
#endif

	dprintf("%s\n", lan_ifname);

#ifdef GUEST_ACCOUNT
	memset(br0_ifnames,0,sizeof(br0_ifnames));
	
	nvram_unset("unbridged_ifnames");
	nvram_unset("br0_ifnames");

	/* If we're a travel router... then we need to make sure we get
		 the primary wireless interface up before trying to attach slave
		 interface(s) to the bridge */
#endif

#ifdef WL330GE
	nvram_set("no_profile", "1");
//	nvram_set("disassoc", "1");
	if (nvram_invmatch("wl_mode_EX", "ap"))
	{
		nvram_set("wl0_ssid", "");
		nvram_set("wl0_ssid2", "");

		if (nvram_match("wl_connect_selected", "1"))
		{
			if (	nvram_invmatch("wl_ap_ssid", "") &&
				( nvram_invmatch("pf1_ssid", "") || nvram_invmatch("pf2_ssid", "") || nvram_invmatch("pf3_ssid", "") || nvram_invmatch("pf4_ssid", "") )
			)
			{
				if ( strcmp(nvram_safe_get("wl_ap_ssid"), nvram_safe_get("pf1_ssid"))==0 )
					ap_idx=1;
				else if ( strcmp(nvram_safe_get("wl_ap_ssid"), nvram_safe_get("pf2_ssid"))==0 )
					ap_idx=2;
				else if ( strcmp(nvram_safe_get("wl_ap_ssid"), nvram_safe_get("pf3_ssid"))==0 )
					ap_idx=3;
				else if ( strcmp(nvram_safe_get("wl_ap_ssid"), nvram_safe_get("pf4_ssid"))==0 )
					ap_idx=4;
				else
				{
					if (nvram_invmatch("pf1_ssid", ""))
						ap_idx=1;
					else if (nvram_invmatch("pf2_ssid", ""))
						ap_idx=2;
					else if (nvram_invmatch("pf3_ssid", ""))
						ap_idx=3;
					else if (nvram_invmatch("pf4_ssid", ""))
						ap_idx=4;

					memset(nvram_str, 0, sizeof(nvram_str));
					sprintf(nvram_str, "pf%d_ssid", ap_idx);
					nvram_set("wl_ap_ssid", nvram_safe_get(nvram_str));
				}

				profile_idx=ap_idx;
			}
			fprintf(stderr, "***************************************\n");
			fprintf(stderr, "***** Specified Network Index: %d *****\n", profile_idx);
			fprintf(stderr, "***************************************\n");
		}
		else
			profile_idx=ap_scan();

		if ( profile_idx > 0 )
		{
			nvram_set("no_profile", "0");

			fprintf(stderr, "ssid:         %s\n", nvram_get_i4("ssid", profile_idx));
			fprintf(stderr, "auth_mode:    %s\n", nvram_get_i4("auth_mode", profile_idx));
			fprintf(stderr, "wpa_mode:     %s\n", nvram_get_i4("wpa_mode", profile_idx));
			fprintf(stderr, "crypto:       %s\n", nvram_get_i4("crypto", profile_idx));
			fprintf(stderr, "wpa_psk:      %s\n", nvram_get_i4("wpa_psk", profile_idx));
			fprintf(stderr, "wep_x:        %s\n", nvram_get_i4("wep_x", profile_idx));
			fprintf(stderr, "phrase:       %s\n", nvram_get_i4("phrase", profile_idx));
			fprintf(stderr, "key1:         %s\n", nvram_get_i4("key1", profile_idx));
			fprintf(stderr, "key2:         %s\n", nvram_get_i4("key2", profile_idx));
			fprintf(stderr, "key3:         %s\n", nvram_get_i4("key3", profile_idx));
			fprintf(stderr, "key4:         %s\n", nvram_get_i4("key4", profile_idx));
			fprintf(stderr, "key:          %s\n", nvram_get_i4("key", profile_idx));
//			fprintf(stderr, "wpa_gtk_rekey:%s\n", nvram_get_i4("wpa_gtk_rekey", profile_idx));
			fprintf(stderr, "***************************************\n");

			nvram_set("wl0_ssid", nvram_get_i4("ssid", profile_idx));
			
   			memset(ssidstr, 0, sizeof(ssidstr));
   			char_to_ascii(ssidstr, nvram_get_i4("ssid", profile_idx));
			nvram_set("wl0_ssid2", ssidstr);

			if (nvram_match_i(profile_idx, "wep_x", "0") || nvram_match_i(profile_idx, "auth_mode", "psk"))
				nvram_set("wl0_wep", "disabled");
			else
				nvram_set("wl0_wep", "enabled");

			if (nvram_match_i(profile_idx, "auth_mode", "shared"))
				nvram_set("wl0_auth", "1");
			else
				nvram_set("wl0_auth", "0");

			if (nvram_match_i(profile_idx, "auth_mode", "psk"))
			{
				if (nvram_match_i(profile_idx, "wpa_mode", "1"))
				{
					nvram_set("wl_akm", "psk");
					nvram_set("wl0_akm", "psk");
				}
				else
				{
					nvram_set("wl_akm", "psk2");
					nvram_set("wl0_akm", "psk2");
				}
/*
				if (nvram_match_i(profile_idx, "crypto", "tkip"))
				{
					nvram_set("wl0_akm", "psk");
				}
				else
				{
					nvram_set("wl0_akm", "psk2");
				}
*/
			}	
			else 
			{
				nvram_set("wl_akm", "");
				nvram_set("wl0_akm", "");
			}	

			nvram_set("wl0_auth_mode", "none");

			if (nvram_match("wl_mode_EX", "re"))
			{
				if (nvram_match_i(profile_idx, "crypto", "tkip+aes"))
					nvram_set("wl0_crypto", "tkip");
				else
					nvram_set("wl0_crypto", nvram_get_i4("crypto", profile_idx));
			}
			else
			{
				if (nvram_match_i(profile_idx, "crypto", "tkip+aes"))
					nvram_set("wl0_crypto", "aes");
				else
					nvram_set("wl0_crypto", nvram_get_i4("crypto", profile_idx));
			}
			nvram_set("wl0_wpa_psk", nvram_get_i4("wpa_psk", profile_idx));
			nvram_set("wl0_key", nvram_get_i4("key", profile_idx));
			nvram_set("wl0_key1", nvram_get_i4("key1", profile_idx));
			nvram_set("wl0_key2", nvram_get_i4("key2", profile_idx));
			nvram_set("wl0_key3", nvram_get_i4("key3", profile_idx));
			nvram_set("wl0_key4", nvram_get_i4("key4", profile_idx));

			int wepidx=atoi(nvram_safe_get("wl0_key"));
			char wepkey[64], wepname[16];
			sprintf(sbuf, "wl join \"%s\"", nvram_safe_get("wl0_ssid"));

//			if (nvram_match("wl0_auth_mode", "psk"))
			if (nvram_match_i(profile_idx, "auth_mode", "psk"))
			{
				sprintf(sbuf, "%s key %s", sbuf, nvram_safe_get("wl0_wpa_psk"));
			}
			else if (nvram_match("wl0_wep", "enabled") && wepidx>=1 && wepidx<=4)
			{
				sprintf(wepname, "wl0_key%d", wepidx);
				sprintf(sbuf, "%s key %s", sbuf, nvram_safe_get(wepname));
			}
		
			sprintf(sbuf, "%s imode bss", sbuf);

//			if (nvram_match("wl0_auth_mode", "psk"))
			if (nvram_match_i(profile_idx, "auth_mode", "psk"))
			{
				if (nvram_match_i(profile_idx, "wpa_mode", "1"))
					sprintf(sbuf, "%s amode wpapsk", sbuf);
				else
					sprintf(sbuf, "%s amode wpa2psk", sbuf);
			}
//			else if (nvram_match("wl0_auth_mode", "shared"))
			else if (nvram_match_i(profile_idx, "auth_mode", "shared"))
				sprintf(sbuf, "%s amode shared", sbuf);
			else sprintf(sbuf, "%s amode open", sbuf);

			nvram_set("wl0_join", sbuf);

			if (nvram_match("wl_mode_EX", "re") && nvram_match("wl_individual_ure", "0"))
			{
				nvram_set("wl0.1_ssid", nvram_get_i4("ssid", profile_idx));
				nvram_set("wl0.1_crypto", nvram_get_i4("crypto", profile_idx));
				nvram_set("wl0.1_wpa_psk", nvram_get_i4("wpa_psk", profile_idx));
				nvram_set("wl0.1_key", nvram_get_i4("key", profile_idx));
				nvram_set("wl0.1_key1", nvram_get_i4("key1", profile_idx));
				nvram_set("wl0.1_key2", nvram_get_i4("key2", profile_idx));
				nvram_set("wl0.1_key3", nvram_get_i4("key3", profile_idx));
				nvram_set("wl0.1_key4", nvram_get_i4("key4", profile_idx));

				if (nvram_match_i(profile_idx, "wep_x", "0") || nvram_match_i(profile_idx, "auth_mode", "psk"))
					nvram_set("wl0.1_wep", "disabled");
				else
					nvram_set("wl0.1_wep", "enabled");
		
				if (nvram_match_i(profile_idx, "auth_mode", "shared"))
					nvram_set("wl0.1_auth", "1");
				else
					nvram_set("wl0.1_auth", "0");
		
				if (nvram_match_i(profile_idx, "auth_mode", "psk"))
				{
					if (nvram_match_i(profile_idx, "wpa_mode", "1"))
					{
						nvram_set("wl0.1_akm", "psk");
					}
					else if (nvram_match_i(profile_idx, "wpa_mode", "2"))
					{
						nvram_set("wl0.1_akm", "psk2");
					}
					else
					{
						nvram_set("wl0.1_akm", "");
					}
/*
					if (nvram_match("wl0.1_crypto", "tkip"))
					{
						nvram_set("wl0.1_akm", "psk");
					}
					else if (nvram_match("wl0.1_crypto", "aes"))
					{
						nvram_set("wl0.1_akm", "psk2");
					}
					else
					{
						nvram_set("wl0.1_akm", "psk psk2");
					}
*/
				}
				else
				{
					nvram_set("wl0.1_akm", "");
				}

				nvram_set("wl0.1_auth_mode", "none");
			}
		}
//		else
//			nvram_set("disassoc", "0");
	}
		
#endif

#ifdef URE
	if(nvram_match("ure_disable", "0") && nvram_match("router_disable", "0"))
	{
		eval("wlconf", nvram_get("wan0_ifname"), "up");
		if(nvram_match("no_profile", "1"))
			eval("wl", "bss", "-C", "0", "down");
	}
#endif

 	/* Bring up bridged interfaces */
	if (strncmp(lan_ifname, "br", 2) == 0) {
		eval("brctl", "addbr", lan_ifname);
		eval("brctl", "setfd", lan_ifname, "0");
//		if (nvram_match("router_disable", "1") || nvram_match("lan_stp", "0"))
//		if (nvram_match("lan_stp", "0"))
			eval("brctl", "stp", lan_ifname, "dis");

#ifdef WL330GE
		if (nvram_match("wan_nat_X", "1") && nvram_match("wl_mode_EX", "ap") && nvram_match("wan0_proto", "dhcp"))	// for WL-330gE Gateway mode
			eval("brctl", "addif", lan_ifname, "eth0");
#endif
#ifdef ASUS_EXT
		foreach(name, nvram_safe_get("lan_ifnames_t"), next) {
#else
		foreach(name, nvram_safe_get("lan_ifnames"), next) {
#endif
			/* Bring up interface */
			ifconfig(name, IFUP, NULL, NULL);
			/* Set the logical bridge address to that of the first interface */
			if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
				continue;
			strncpy(ifr.ifr_name, lan_ifname, IFNAMSIZ);
			if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0 &&
			    memcmp(ifr.ifr_hwaddr.sa_data, "\0\0\0\0\0\0", ETHER_ADDR_LEN) == 0) {
				strncpy(ifr.ifr_name, name, IFNAMSIZ);
				if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0) {
					strncpy(ifr.ifr_name, lan_ifname, IFNAMSIZ);
					ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
					ioctl(s, SIOCSIFHWADDR, &ifr);
				}
			}
			close(s);
			/* If not a wl i/f then simply add it to the bridge */
//			if (eval("wlconf", name, "up"))
			if (strcmp(name, "eth0")==0 || strcmp(name, "eth2")==0)
			{
#ifdef RT2400_SUPPORT
				if (strcmp(name, "eth2")==0)
				{
					// added by Joey for WL500b + WL127
					if (nvram_match("wl_channel", "0"))
						nvram_set("wl_channel", "6");

					sprintf(tmpstr, "mac_address=%s", nvram_safe_get("et0macaddr"));
					eval("insmod","rt2400.o",tmpstr);
					eval("brctl","addif",lan_ifname,"ra0");
					ifconfig("ra0",IFUP,NULL,NULL);
					nvram_set("nobr","1");

					j = atoi(nvram_safe_get("wl_wdsnum_x"));
					for(i=1;i<=j;i++)
					{
						sprintf(tmpstr, "ra%d", i);							ifconfig(tmpstr, IFUP, NULL, NULL);
						eval("brctl", "addif",lan_ifname,tmpstr);
					}
				}
				else
#endif

#ifdef GUEST_ACCOUNT
				{
					if (eval("brctl", "addif", lan_ifname, name))
						perror("brctl");
					else{
						char buf[255],*ptr;
						ptr = nvram_get("br0_ifnames");
						if (ptr)
							snprintf(buf,sizeof(buf),"%s %s",ptr,name);
						else
							strncpy(buf,name,sizeof(buf));
						nvram_set("br0_ifnames",buf);
					}
				}
#else
				eval("brctl", "addif", lan_ifname, name);
#endif

			}
			else 
			{

				if (	nvram_match("wl_mode_EX", "ap") ||
					strcmp(name, "eth1") ||
					(!strcmp(name, "eth1") && nvram_match("no_profile", "0")) ||
					(!strcmp(name, "eth1") && nvram_match("no_profile", "1") && nvram_match("wl_mode_EX", "re") && nvram_match("wan_nat_X", "0"))
				)
				{
					eval("wlconf", name, "up");

					if (!strcmp(name, "eth1") && nvram_match("no_profile", "1") && nvram_match("wl_mode_EX", "re") && nvram_match("wan_nat_X", "0"))
						eval("wl", "bss", "-C", "0", "down");		
				}
#ifdef GUEST_ACCOUNT
				char wl_guest[] = "wlXXXXXXXXXX_guest";
				char wl_vifs[]= "wlXXXXXXXXXX_vifs";
				char mode[] = "wlXXXXXXXXXX_mode";
				char *vifs;
#endif

				/* get the instance number of the wl i/f */
				char wl_name[] = "wlXXXXXXXXXX_mode";
				int unit;
#ifdef ASUS_EXT
				sync_mac(name, nvram_safe_get("et0macaddr"));
#endif
				wl_ioctl(name, WLC_GET_INSTANCE, &unit, sizeof(unit));
				snprintf(wl_name, sizeof(wl_name), "wl%d_mode", unit);

#ifdef GUEST_ACCOUNT
				snprintf(wl_guest, sizeof(wl_guest), "wl%d_guest", unit);
				snprintf(wl_vifs, sizeof(wl_vifs), "wl%d_vifs", unit);
					
				/* Multi-SSID specific configuration */
				/* Virtual interfaces are created with  the master interface 
				   by wlconf() . Only copy those that have wlX.Y_guest set*/
					   
				vifs = nvram_get(wl_vifs);
					
				if (vifs){
					char buf[255];
					char name[32];
					char vif_guest[32];
					char *ptr=NULL,*next=NULL;
						
					memset(buf,0,sizeof(buf));
						
					ptr = nvram_get("unbridged_ifnames");
						
					if (ptr) snprintf(buf,sizeof(buf),"%s",ptr);
						
					/*Loop thru wlX_vifs to get the virtual interfaces
				          wlX.Y_guest must be set for IP to configure it*/
					foreach(name,vifs,next){
						snprintf(vif_guest,sizeof(vif_guest),"%s_guest",name);
						cprintf("vif_guest=%s\n", vif_guest);
						if (nvram_match(vif_guest,"1")){
							if(*buf)
								snprintf(buf,sizeof(buf),"%s %s",buf,name);
							else
								strncpy(buf,name,sizeof(buf));
						} 
					}

					if (*buf) nvram_set("unbridged_ifnames",buf);
				}
#endif
				/* Receive all multicast frames in WET mode */
				if (nvram_match(wl_name, "wet"))
				{
					ifconfig(name, IFUP | IFF_ALLMULTI, NULL, NULL);
#ifdef GUEST_ACCOUNT
					/* Enable host DHCP relay */
					if (nvram_match("lan_dhcp", "1"))
						wl_iovar_set(name, "wet_host_mac", ifr.ifr_hwaddr.sa_data, ETHER_ADDR_LEN);
#endif
				}

				/* Do not attach the main wl i/f if in wds mode */
#ifdef GUEST_ACCOUNT
				if ( !nvram_match(mode, "wds") && !nvram_match(wl_guest, "1")){
					char buf[255],*ptr;

//#ifdef WL330GE
//					if (nvram_match("wl_mode_EX", "ap"))
//						eval("brctl", "addif", lan_ifname, name);
//#else
					eval("brctl", "addif", lan_ifname, name);
//#endif
					
					ptr = nvram_get("br0_ifnames");
					if (ptr)
						snprintf(buf,sizeof(buf),"%s %s",ptr,name);
					else
						strncpy(buf,name,sizeof(buf));
					nvram_set("br0_ifnames",buf);
				}
#else
				if (nvram_invmatch(wl_name, "wds"))
					eval("brctl", "addif",lan_ifname,name);
#endif
			}
		}
	}
	/* specific non-bridged lan i/f */
	else if (strcmp(lan_ifname, "")) {
		/* Bring up interface */
		ifconfig(lan_ifname, IFUP, NULL, NULL);
		/* config wireless i/f */
		if (!eval("wlconf", lan_ifname, "up")) {
			char tmp[100], prefix[] = "wanXXXXXXXXXX_";
			int unit;
			/* get the instance number of the wl i/f */
			wl_ioctl(lan_ifname, WLC_GET_INSTANCE, &unit, sizeof(unit));
			snprintf(prefix, sizeof(prefix), "wl%d_", unit);
			/* Receive all multicast frames in WET mode */
			if (nvram_match(strcat_r(prefix, "mode", tmp), "wet"))
				ifconfig(lan_ifname, IFUP | IFF_ALLMULTI, NULL, NULL);
		}
	}
#ifdef GUEST_ACCOUNT
	/* build unbridged ifnames NVRAM var from wl_guest list */
	if (nvram_match("wl_mode_EX", "ap"))
	foreach(name, nvram_safe_get("lan_ifnames"), next) {
	
		char wl_guest[] = "wlXXXXXXXXXX_guest";
		int unit;
		
		if (!eval("wlconf", name, "up")) {
			wl_ioctl(name, WLC_GET_INSTANCE, &unit, sizeof(unit));
			snprintf(wl_guest, sizeof(wl_guest), "wl%d_guest", unit);
			
			if (nvram_match(wl_guest,"1")){
				char buf[255],*ptr;
				
				ptr = nvram_get("unbridged_ifnames");
				if (ptr)
					snprintf(buf,sizeof(buf),"%s %s",ptr,name);
				else
					strncpy(buf,name,sizeof(buf));
					
				nvram_set("unbridged_ifnames",buf);
			}
		}
	}
#endif

	/* Get current LAN hardware address */
	if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) >= 0) {
		char eabuf[32];
		strncpy(ifr.ifr_name, lan_ifname, IFNAMSIZ);
		if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0)
			nvram_set("lan_hwaddr", ether_etoa(ifr.ifr_hwaddr.sa_data, eabuf));
		close(s);
	}

#ifdef WPA2_WMM
	/* Set QoS mode */
	if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) >= 0) {
		int i, qos;
		caddr_t ifrdata;
		struct ethtool_drvinfo info;

		qos = (strcmp(nvram_safe_get("wl_wme"), "on")) ? 0 : 1;
		for (i = 1; i <= DEV_NUMIFS; i ++) {
			ifr.ifr_ifindex = i;
			if (ioctl(s, SIOCGIFNAME, &ifr))
				continue;
			if (ioctl(s, SIOCGIFHWADDR, &ifr))
				continue;
			if (ifr.ifr_hwaddr.sa_family != ARPHRD_ETHER)
				continue;
			/* get flags */
			if (ioctl(s, SIOCGIFFLAGS, &ifr))
				continue;
			/* if up(wan not up yet at this point) */
			if (ifr.ifr_flags & IFF_UP) {
				ifrdata = ifr.ifr_data;
				memset(&info, 0, sizeof(info));
				info.cmd = ETHTOOL_GDRVINFO;
				ifr.ifr_data = (caddr_t)&info;
				if (ioctl(s, SIOCETHTOOL, &ifr) >= 0) {
					/* currently only need to set QoS to et devices */
					if (!strncmp(info.driver, "et", 2)) {
						ifr.ifr_data = (caddr_t)&qos;
						ioctl(s, SIOCSETCQOS, &ifr);
					}
				}
				ifr.ifr_data = ifrdata;
			}
		}
	}
#endif
	if (nvram_match("lan_ipaddr", ""))
	{
		nvram_set("lan_ipaddr", "192.168.1.220");
		nvram_set("lan_netmask", "255.255.255.0");
	}
	else if (nvram_match("lan_netmask", ""))
		nvram_set("lan_netmask", "255.255.255.0");

#ifdef ASUS_EXT
#ifndef FLASH2M
	/* 
	* Configure DHCP connection. The DHCP client will run 
	* 'udhcpc bound'/'udhcpc deconfig' upon finishing IP address 
	* renew and release.
	*/
	if (nvram_match("router_disable", "1"))
	{
		if (nvram_match("lan_proto_x", "1")) 
		{
			char *dhcp_argv[] = { "udhcpc",
					      "-i", lan_ifname,
					      "-p", "/var/run/udhcpc_lan.pid",
					      "-s", "/tmp/landhcpc",
					      NULL
			};
			pid_t pid;


			/* Bring up and configure LAN interface */
			ifconfig(lan_ifname, IFUP,
		 		nvram_safe_get("lan_ipaddr"), nvram_safe_get("lan_netmask"));

			symlink("/sbin/rc", "/tmp/landhcpc");

			/* Start dhcp daemon */
			_eval(dhcp_argv, NULL, 0, &pid);
		}
		else
		{
			/* Bring up and configure LAN interface */
			ifconfig(lan_ifname, IFUP,
		 		nvram_safe_get("lan_ipaddr"), nvram_safe_get("lan_netmask"));
			lan_up(lan_ifname);

			update_lan_status(1);
		}
	}
	else
#endif // end of FLASH2M
	{
		/* Bring up and configure LAN interface */
		ifconfig(lan_ifname, IFUP,
		 	nvram_safe_get("lan_ipaddr"), nvram_safe_get("lan_netmask"));
		/* Install lan specific static routes */
		add_lan_routes(lan_ifname);

		update_lan_status(1);
	}

/*
	if (nvram_match("wan_nat_X", "1") && nvram_match("wl_mode_EX", "ap") && nvram_match("wan0_proto", "dhcp"))
	{
			char *dhcp_argv[] = { "udhcpc",
					      "-i", lan_ifname,
					      "-p", "/var/run/udhcpc_lan.pid",
					      "-s", "/tmp/landhcpc",
					      NULL
			};
			pid_t pid;

			symlink("/sbin/rc", "/tmp/landhcpc");

			// Start dhcp daemon
			_eval(dhcp_argv, NULL, 0, &pid);
	}
*	

#else
	/* Bring up and configure LAN interface */
	ifconfig(lan_ifname, IFUP,
		 nvram_safe_get("lan_ipaddr"), nvram_safe_get("lan_netmask"));

	/* Install lan specific static routes */
	add_lan_routes(lan_ifname);
#endif

#ifdef GUEST_ACCOUNT
#ifdef RANGE_EXTENDER
	if(nvram_match("wl_mode_EX", "re"))
	{
	}
	else
#endif
	/* Bring up unbridged LAN interfaces (if they exist)*/
	{
	char *interfaces,*ifname, *ptr;
	char word[64], *next;
	int index =1 ;
	
	interfaces = nvram_get("unbridged_ifnames");
	if (interfaces)
		foreach(word,interfaces,next){
		
		 	char interface[32], mask[32];
		 	int s;
		 
		 	ptr=word;
			ifname = word;
			index =  get_ipconfig_index(ifname);
			
			if ( index < 0) {
				cprintf("Cannot find index for interface:%s\n",ifname);
				continue;
			}

			snprintf(interface,sizeof(interface),"lan%d_ipaddr",index);
		 	snprintf(mask,sizeof(mask),"lan%d_netmask",index);
		 	ifconfig(ifname, IFUP,nvram_safe_get(interface), nvram_safe_get(mask)); 
		 
		 	/* Get Ethernet hardware address. Note this value NOT is committed to NVRAM */
		 	if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) >= 0) {
		 		struct ifreq ifr;
				char macaddr[]="00:00:00:00:00:00";
				char mac[32];
			
				memset(&ifr,0,sizeof(ifr));
				strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
				if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0){
					ether_etoa(ifr.ifr_hwaddr.sa_data, macaddr);
					snprintf(mac,sizeof(mac),"lan%d_hwaddr",index);	
					nvram_set(mac,macaddr);
				}
				close(s);
			}
		 
		}
	}
#endif

#ifndef ASUS_EXT
	/* Start syslogd if either log_ipaddr or log_ram_enable is set */
	if (nvram_invmatch("log_ipaddr", "") || nvram_match("log_ram_enable", "1")) {
		char *argv[] = {
			"syslogd",
			NULL, 		/* -C */
			NULL, NULL,	/* -R host */
			NULL
		};
		int pid;
		int argc = 1;
		
		if (nvram_match("log_ram_enable", "1")) {
			argv[argc++] = "-C";
		}
		else if (!nvram_match("log_ram_enable", "0")) {
			nvram_set("log_ram_enable", "0");
		}
				
		if (nvram_invmatch("log_ipaddr", "")) {
			argv[argc++] = "-R";
			argv[argc++] = nvram_get("log_ipaddr");
		}

		_eval(argv, NULL, 0, &pid);
	}
#endif

	dprintf("%s %s\n",
		nvram_safe_get("lan_ipaddr"),
		nvram_safe_get("lan_netmask"));
}

void
stop_lan(void)
{
	char *lan_ifname = nvram_safe_get("lan_ifname");
	char name[80], *next;

	dprintf("%s\n", lan_ifname);

	/* Stop the syslogd daemon */
	eval("killall", "syslogd");

	/* Remove static routes */
	del_lan_routes(lan_ifname);

	/* Bring down LAN interface */
	ifconfig(lan_ifname, 0, NULL, NULL);

	/* Bring down bridged interfaces */
	if (strncmp(lan_ifname, "br", 2) == 0) {
#ifdef ASUS_EXT
		foreach(name, nvram_safe_get("lan_ifnames_t"), next) {
#else
		foreach(name, nvram_safe_get("lan_ifnames"), next) {
#endif
			eval("wlconf", name, "down");
			ifconfig(name, 0, NULL, NULL);
			eval("brctl", "delif", lan_ifname, name);
		}
		eval("brctl", "delbr", lan_ifname);
	}
	/* Bring down specific interface */
	else if (strcmp(lan_ifname, ""))
		eval("wlconf", lan_ifname, "down");
	
	dprintf("done\n");
}

static int
wan_prefix(char *ifname, char *prefix)
{
	int unit;
	
	if ((unit = wan_ifunit(ifname)) < 0)
		return -1;

	sprintf(prefix, "wan%d_", unit);
	return 0;
}

static int
add_wan_routes(char *wan_ifname)
{
	char prefix[] = "wanXXXXXXXXXX_";

	/* Figure out nvram variable name prefix for this i/f */
	if (wan_prefix(wan_ifname, prefix) < 0)
		return -1;

	return add_routes(prefix, "route", wan_ifname);
}

static int
del_wan_routes(char *wan_ifname)
{
	char prefix[] = "wanXXXXXXXXXX_";

	/* Figure out nvram variable name prefix for this i/f */
	if (wan_prefix(wan_ifname, prefix) < 0)
		return -1;

	return del_routes(prefix, "route", wan_ifname);
}

int
wan_valid(char *ifname)
{
	char name[80], *next;
	
	foreach(name, nvram_safe_get("wan_ifnames"), next)
		if (ifname && !strcmp(ifname, name))
			return 1;

#ifndef RANGE_EXTENDER
#ifdef WIRELESS_WAN
	if (nvram_invmatch("wl_mode_EX", "ap")) {
		return nvram_match("wl0_ifname", ifname);
	}	
#endif
#endif
	return 0;
}

void
start_wan(void)
{
	char *wan_ifname;
	char *wan_proto;
	int unit;
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";
	char eabuf[32];
	int s;
	struct ifreq ifr;
	pid_t pid;

	/* check if we need to setup WAN */
	if (nvram_match("router_disable", "1")
#ifdef BTN_SETUP
//	|| is_ots()/* Cherry Cho unmarked in 2007/2/13. */
#endif
)
		return;

#ifdef ASUS_EXT
	update_wan_status(0);
	/* start connection independent firewall */
	start_firewall();
#else
	/* start connection independent firewall */
	start_firewall();
#endif

	/* Create links */
	mkdir("/tmp/ppp", 0777);
	mkdir("/tmp/ppp/peers", 0777);
	symlink("/sbin/rc", "/tmp/ppp/ip-up");
	symlink("/sbin/rc", "/tmp/ppp/ip-down");	
	symlink("/sbin/rc", "/tmp/udhcpc");

	//symlink("/dev/null", "/tmp/ppp/connect-errors");

	/* Start each configured and enabled wan connection and its undelying i/f */
	for (unit = 0; unit < MAX_NVPARSE; unit ++) 
	{
#ifdef ASUS_EXT // Only multiple pppoe is allowed 
		if (unit>0 && nvram_invmatch("wan_proto", "pppoe")) break;
#endif

		snprintf(prefix, sizeof(prefix), "wan%d_", unit);

		/* make sure the connection exists and is enabled */ 
		wan_ifname = nvram_get(strcat_r(prefix, "ifname", tmp));
		if (!wan_ifname)
			continue;
		wan_proto = nvram_get(strcat_r(prefix, "proto", tmp));
		if (!wan_proto || !strcmp(wan_proto, "disabled"))
			continue;

		/* disable the connection if the i/f is not in wan_ifnames */
		if (!wan_valid(wan_ifname)) {
			nvram_set(strcat_r(prefix, "proto", tmp), "disabled");
			continue;
		}

		dprintf("%s %s\n\n\n\n\n", wan_ifname, wan_proto);

		/* Set i/f hardware address before bringing it up */
		if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
			continue;

		strncpy(ifr.ifr_name, wan_ifname, IFNAMSIZ);

		/* Since WAN interface may be already turned up (by vlan.c),
		   if WAN hardware address is specified (and different than the current one),
		   we need to make it down for synchronizing hwaddr. */
		if (ioctl(s, SIOCGIFHWADDR, &ifr)) {
			close(s);
			continue;
		}

		ether_atoe(nvram_safe_get(strcat_r(prefix, "hwaddr", tmp)), eabuf);
		if (bcmp(eabuf, ifr.ifr_hwaddr.sa_data, ETHER_ADDR_LEN))
		{
			/* current hardware address is different than user specified */
			ifconfig(wan_ifname, 0, NULL, NULL);
		}

		/* Configure i/f only once, specially for wireless i/f shared by multiple connections */
		if (ioctl(s, SIOCGIFFLAGS, &ifr)) {
			close(s);
			continue;
		}
		if (!(ifr.ifr_flags & IFF_UP)) {
			/* Sync connection nvram address and i/f hardware address */
			memset(ifr.ifr_hwaddr.sa_data, 0, ETHER_ADDR_LEN);

			if (!nvram_invmatch(strcat_r(prefix, "hwaddr", tmp), "") ||
			    !ether_atoe(nvram_safe_get(strcat_r(prefix, "hwaddr", tmp)), ifr.ifr_hwaddr.sa_data) ||
			    !memcmp(ifr.ifr_hwaddr.sa_data, "\0\0\0\0\0\0", ETHER_ADDR_LEN)) {
				if (ioctl(s, SIOCGIFHWADDR, &ifr)) {
					close(s);
					continue;
				}
				nvram_set(strcat_r(prefix, "hwaddr", tmp), ether_etoa(ifr.ifr_hwaddr.sa_data, eabuf));
			}
			else {
				ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
				ioctl(s, SIOCSIFHWADDR, &ifr);
			}

			/* Bring up i/f */
			ifconfig(wan_ifname, IFUP, NULL, NULL);

#ifdef URE
			/* do wireless specific config */
			if(nvram_match("ure_disable", "1")) 
			{
				eval("wlconf", wan_ifname, "up");
			}
#else
			/* do wireless specific config */
			eval("wlconf", wan_ifname, "up");
#endif
		}
	
		close(s);


#ifdef ASUS_EXT
		if (unit==0) 
		{		
			FILE *fp;

			setup_ethernet(nvram_safe_get("wan_ifname"));
			start_pppoe_relay(nvram_safe_get("wan0_ifname"));	// for A3 board

			/* Enable Forwarding */
			if ((fp = fopen("/proc/sys/net/ipv4/ip_forward", "r+"))) 			{
				fputc('1', fp);
				fclose(fp);
			} else
			{	
				perror("/proc/sys/net/ipv4/ip_forward");
			}
		}

		/* 
		* Configure PPPoE connection. The PPPoE client will run 
		* ip-up/ip-down scripts upon link's connect/disconnect.
		*/
#ifdef CDMA
		if (strcmp(wan_proto, "cdma")==0)
        	{
			if(nvram_match("cdma_down", "99"))
			{
				nvram_set("cdma_down", "1");
			}
                	else 
			{
				nvram_set("cdma_down", "0");
				start_cdma();
			}
                	update_wan_status(0);
        	}
		else
#endif

#ifdef RPPPPOE
		if (nvram_match("wan_proto", "pppoe") || (nvram_match("wan_proto", "pptp")
#ifdef DHCP_PPTP
&& nvram_invmatch(strcat_r(prefix, "pppoe_gateway", tmp), "")
#endif
))
		{

			int demand = atoi(nvram_safe_get(strcat_r(prefix, "pppoe_idletime", tmp)));

			/* update demand option */
			nvram_set(strcat_r(prefix, "pppoe_demand", tmp), demand ? "1" : "0");
			/* Bring up  WAN interface */
			if (strcmp(wan_proto, "pptp") == 0) {
				ifconfig(wan_ifname, IFUP, 
					nvram_safe_get("wan0_pppoe_ipaddr"), 
					nvram_safe_get("wan0_pppoe_netmask"));

				/* add pptp server route */
				if (nvram_invmatch("wan_heartbeat_x", "")) {
					route_add(wan_ifname, 0, nvram_safe_get("wan_heartbeat_x"),
						nvram_get(strcat_r(prefix, "pppoe_gateway", tmp)), "255.255.255.255");
				} else {
					route_add(wan_ifname, 0, nvram_safe_get(strcat_r(prefix, "pppoe_gateway", tmp)),
						NULL, "255.255.255.255");
				}
 
		 	} else {
		 		/* do not use safe_get here, values are optional */
				ifconfig(wan_ifname, IFUP, 
					nvram_get("wan_ipaddr"), 
					nvram_get("wan_netmask"));
		 	}
		 	
		 	/* setup static wan routes via physical device */
			add_routes("wan_", "route", wan_ifname);
			
			start_pppd(prefix);

			/* ppp interface name is referenced from this point on */
			wan_ifname = nvram_safe_get(strcat_r(prefix, "pppoe_ifname", tmp));
			
			/* Pretend that the WAN interface is up */
			if (nvram_match(strcat_r(prefix, "pppoe_demand", tmp), "1")) 
			{
				int timeout = 5;
				/* Wait for pppx to be created */
				while (ifconfig(wan_ifname, IFUP, NULL, NULL) && timeout--)
					sleep(1);

				/* Retrieve IP info */
				if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
					continue;
				strncpy(ifr.ifr_name, wan_ifname, IFNAMSIZ);

				/* Set temporary IP address */
				if (ioctl(s, SIOCGIFADDR, &ifr))
					perror(wan_ifname);

				nvram_set(strcat_r(prefix, "ipaddr", tmp), inet_ntoa(sin_addr(&ifr.ifr_addr)));
				nvram_set(strcat_r(prefix, "netmask", tmp), "255.255.255.255");

				/* Set temporary P-t-P address */
				if (ioctl(s, SIOCGIFDSTADDR, &ifr))
					perror(wan_ifname);
				nvram_set(strcat_r(prefix, "gateway", tmp), inet_ntoa(sin_addr(&ifr.ifr_dstaddr)));

				close(s);

				/* 
				* Preset routes so that traffic can be sent to proper pppx even before 
				* the link is brought up.
				*/
				preset_wan_routes(wan_ifname);
			}
		
		}
#else
		if (strcmp(wan_proto, "pppoe") == 0) {
			char *pppoe_argv[] = { "pppoecd",
					       nvram_safe_get(strcat_r(prefix, "ifname", tmp)),
					       "-u", nvram_safe_get(strcat_r(prefix, "pppoe_username", tmp)),
					       "-p", nvram_safe_get(strcat_r(prefix, "pppoe_passwd", tmp)),
					       "-r", nvram_safe_get(strcat_r(prefix, "pppoe_mru", tmp)),
					       "-t", nvram_safe_get(strcat_r(prefix, "pppoe_mtu", tmp)),
					       "-i", nvram_match(strcat_r(prefix, "pppoe_demand", tmp), "1") ?
					       		nvram_safe_get(strcat_r(prefix, "pppoe_idletime", tmp)) : "0",
#ifdef ASUS_EXT
						NULL, NULL,
						NULL, NULL,
						NULL, NULL,
#endif
					       NULL, NULL,	/* pppoe_service */
					       NULL, NULL,	/* pppoe_ac */
					       NULL,		/* pppoe_keepalive */
					       NULL, NULL,	/* ppp unit requested */
					       NULL
			}, **arg;
			int timeout = 5;
			char pppunit[] = "XXXXXXXXXXXX";

			/* Add optional arguments */

			for (arg = pppoe_argv; *arg; arg++);

#ifdef ASUS_EXT
			if (nvram_invmatch(strcat_r(prefix, "pppoe_idletime", tmp), "0"))
			{
				*arg++ = "-I";
				*arg++ = "30";

				*arg++ = "-T";
				*arg++ = "9";

				*arg++ = "-N";
				*arg++ = "10";
			}

			if (nvram_invmatch(strcat_r(prefix, "pppoe_txonly_x", tmp), "0")) {
				*arg++ = "-o";
			}
#endif
			if (nvram_invmatch(strcat_r(prefix, "pppoe_service", tmp), "")) {
				*arg++ = "-s";
				*arg++ = nvram_safe_get(strcat_r(prefix, "pppoe_service", tmp));
			}
			if (nvram_invmatch(strcat_r(prefix, "pppoe_ac", tmp), "")) {
				*arg++ = "-a";
				*arg++ = nvram_safe_get(strcat_r(prefix, "pppoe_ac", tmp));
			}

#ifndef ASUS_EXT	// keep alive anyway
			if (nvram_match(strcat_r(prefix, "pppoe_demand", tmp), "1") || 
			    nvram_match(strcat_r(prefix, "pppoe_keepalive", tmp), "1"))
#endif
				*arg++ = "-k";
			snprintf(pppunit, sizeof(pppunit), "%d", unit);
			*arg++ = "-U";
			*arg++ = pppunit;

			/* launch pppoe client daemon */
			_eval(pppoe_argv, NULL, 0, &pid);

			/* ppp interface name is referenced from this point on */
			wan_ifname = nvram_safe_get(strcat_r(prefix, "pppoe_ifname", tmp));
			
			/* Pretend that the WAN interface is up */
			if (nvram_match(strcat_r(prefix, "pppoe_demand", tmp), "1")) {
				/* Wait for pppx to be created */
				while (ifconfig(wan_ifname, IFUP, NULL, NULL) && timeout--)
					sleep(1);

				/* Retrieve IP info */
				if ((s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
					continue;
				strncpy(ifr.ifr_name, wan_ifname, IFNAMSIZ);

				/* Set temporary IP address */
				if (ioctl(s, SIOCGIFADDR, &ifr))
					perror(wan_ifname);
				nvram_set(strcat_r(prefix, "ipaddr", tmp), inet_ntoa(sin_addr(&ifr.ifr_addr)));
				nvram_set(strcat_r(prefix, "netmask", tmp), "255.255.255.255");

				/* Set temporary P-t-P address */
				if (ioctl(s, SIOCGIFDSTADDR, &ifr))
					perror(wan_ifname);
				nvram_set(strcat_r(prefix, "gateway", tmp), inet_ntoa(sin_addr(&ifr.ifr_dstaddr)));

				close(s);

				/* 
				* Preset routes so that traffic can be sent to proper pppx even before 
				* the link is brought up.
				*/

				preset_wan_routes(wan_ifname);
			}
#ifdef ASUS_EXT
			nvram_set("wan_ifname_t", wan_ifname);
#endif
		}
#endif // FLASH2M
#endif
		/* 
		* Configure DHCP connection. The DHCP client will run 
		* 'udhcpc bound'/'udhcpc deconfig' upon finishing IP address 
		* renew and release.
		*/
		else if (strcmp(wan_proto, "dhcp") == 0 ||
			 strcmp(wan_proto, "bigpond") == 0 
#ifdef DHCP_PPTP
|| (strcmp(wan_proto,"pptp")==0 && nvram_match(strcat_r(prefix, "pppoe_gateway", tmp), ""))
#endif
) {
			char *wan_hostname = nvram_get(strcat_r(prefix, "hostname", tmp));
			char *dhcp_argv[] = { "udhcpc",
					      "-i", wan_ifname,
					      "-p", (sprintf(tmp, "/var/run/udhcpc%d.pid", unit), tmp),
					      "-s", "/tmp/udhcpc",
					      wan_hostname && *wan_hostname ? "-H" : NULL,
					      wan_hostname && *wan_hostname ? wan_hostname : NULL,
					      NULL
			};
			/* Start dhcp daemon */

//			if ( !(nvram_invmatch("wl_mode_EX", "ap") && nvram_match("no_profile", "1")) )
			_eval(dhcp_argv, NULL, 0, &pid);

			if (nvram_match("wan_nat_X", "1") && nvram_match("wl_mode_EX", "ap") && nvram_match("wan0_proto", "dhcp"))
			{
				char *dhcp_argv2[] = { "udhcpc",
					      "-i", "br0",
					      "-p", "/var/run/udhcpc_fake.pid",
					      "-s", "/tmp/udhcpc",
					      NULL
				};

				// Start dhcp daemon
				_eval(dhcp_argv2, NULL, 0, &pid);
			}

#ifdef ASUS_EXT
			wanmessage("Can not get IP from server");
			nvram_set("wan_ifname_t", wan_ifname);
#endif
		}
		/* Configure static IP connection. */
		else if (strcmp(wan_proto, "static") == 0) {
			/* Assign static IP address to i/f */
			ifconfig(wan_ifname, IFUP,
				 nvram_safe_get(strcat_r(prefix, "ipaddr", tmp)), 
				 nvram_safe_get(strcat_r(prefix, "netmask", tmp)));
			/* We are done configuration */
			wan_up(wan_ifname);
#ifdef ASUS_EXT
			nvram_set("wan_ifname_t", wan_ifname);
#endif
		}
#if 0
                else if (strcmp(wan_proto, "dhcp") == 0 ||
                         strcmp(wan_proto, "bigpond") == 0
//#endif
#ifdef DHCP_PPTP
 		if (strcmp(wan_proto,"pptp")==0 && nvram_match(strcat_r(prefix, "pppoe_gateway", tmp), ""))
 		{
                        char *wan_hostname = nvram_get(strcat_r(prefix, "hostname", tmp));
                        char *dhcp_argv[] = { "udhcpc",
                                              "-i", wan_ifname,
                                              "-p", (sprintf(tmp, "/var/run/udhcpc%d.pid", unit), tmp),
                                              "-s", "/tmp/udhcpc",
                                              wan_hostname && *wan_hostname ? "-H" : NULL,
                                              wan_hostname && *wan_hostname ? wan_hostname : NULL,
                                              NULL
                        };
                        /* Start dhcp daemon */
                        _eval(dhcp_argv, NULL, 0, &pid);
#ifdef ASUS_EXT
                        wanmessage("Can not get IP from server");
                        nvram_set("wan_ifname_t", wan_ifname);
#endif
                }
#endif
#endif

#ifndef ASUS_EXT
		/* Start connection dependent firewall */
		start_firewall2(wan_ifname);
#endif

		dprintf("%s %s\n",
			nvram_safe_get(strcat_r(prefix, "ipaddr", tmp)),
			nvram_safe_get(strcat_r(prefix, "netmask", tmp)));
	}

	/* Report stats */
	if (nvram_invmatch("stats_server", "")) {
		char *stats_argv[] = { "stats", nvram_get("stats_server"), NULL };
		_eval(stats_argv, NULL, 5, NULL);
	}
}

void
stop_wan(void)
{
	char name[80], *next, signal[] = "XXXX";
	
	eval("killall", "stats");
	eval("killall", "ntp");
	eval("killall", "ntpclient");

	/* Shutdown and kill all possible tasks */
	eval("killall", "ip-up");
	eval("killall", "ip-down");
	snprintf(signal, sizeof(signal), "-%d", SIGHUP);
	eval("killall", signal, "pppoecd");
	eval("killall", signal, "pppd");
	eval("killall", "pppoecd");
	eval("killall", "pppd");
#ifdef CDMA
	eval("killall", "chat");
#endif
	snprintf(signal, sizeof(signal), "-%d", SIGUSR2);
	eval("killall", signal, "udhcpc");
	eval("killall", "udhcpc");

	/* Bring down WAN interfaces */
	foreach(name, nvram_safe_get("wan_ifnames"), next)
	{
		ifconfig(name, 0, NULL, NULL);
	}

	/* Remove dynamically created links */
	unlink("/tmp/udhcpc");
	
	unlink("/tmp/ppp/ip-up");
	unlink("/tmp/ppp/ip-down");
	unlink("/tmp/ppp/options");
	rmdir("/tmp/ppp");

#ifdef ASUS_EXT
	update_wan_status(0);
#endif

	dprintf("done\n");
}

void
stop_wan2(void)
{
	char name[80], *next, signal[] = "XXXX";
	
	eval("killall", "stats");
	eval("killall", "ntp");
	eval("killall", "ntpclient");

	/* Shutdown and kill all possible tasks */
	eval("killall", "ip-up");
	eval("killall", "ip-down");
	snprintf(signal, sizeof(signal), "-%d", SIGHUP);
	eval("killall", signal, "pppoecd");
	eval("killall", signal, "pppd");
	eval("killall", "pppoecd");
	eval("killall", "pppd");

	snprintf(signal, sizeof(signal), "-%d", SIGUSR2);
	eval("killall", signal, "udhcpc");
	eval("killall", "udhcpc");

	/* Remove dynamically created links */
	unlink("/tmp/udhcpc");
	
	unlink("/tmp/ppp/ip-up");
	unlink("/tmp/ppp/ip-down");
	unlink("/tmp/ppp/options");
	rmdir("/tmp/ppp");

#ifdef ASUS_EXT
	if(nvram_invmatch("wan_ifname_t", "")) wan_down(nvram_safe_get("wan_ifname_t"));
#endif

	dprintf("done\n");
}

static int
add_ns(char *wan_ifname)
{
	FILE *fp;
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";
	char word[100], *next;
	char line[100];

	/* Figure out nvram variable name prefix for this i/f */
	if (wan_prefix(wan_ifname, prefix) < 0)
		return -1;

	/* Open resolv.conf to read */
	if (!(fp = fopen("/tmp/resolv.conf", "r+"))) {
		perror("/tmp/resolv.conf");
		return errno;
	}

	/* Append only those not in the original list */
	foreach(word, nvram_safe_get(strcat_r(prefix, "dns", tmp)), next) 
	{
		fseek(fp, 0, SEEK_SET);
		while (fgets(line, sizeof(line), fp)) {
			char *token = strtok(line, " \t\n");

			if (!token || strcmp(token, "nameserver") != 0)
				continue;
			if (!(token = strtok(NULL, " \t\n")))
				continue;

			if (!strcmp(token, word))
				break;
		}
		if (feof(fp))
			fprintf(fp, "nameserver %s\n", word);
	}
	fclose(fp);

#ifdef ASUS_EXT
	stop_dns();
	start_dns();
#else
	/* notify dnsmasq */
	snprintf(tmp, sizeof(tmp), "-%d", SIGHUP);
	eval("killall", tmp, "dnsmasq");
#endif
	
	return 0;
}

static int
del_ns(char *wan_ifname)
{
	FILE *fp, *fp2;
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";
	char word[100], *next;
	char line[100];

	/* Figure out nvram variable name prefix for this i/f */
	if (wan_prefix(wan_ifname, prefix) < 0)
		return -1;

	/* Open resolv.conf to read */
	if (!(fp = fopen("/tmp/resolv.conf", "r"))) {
		perror("fopen /tmp/resolv.conf");
		return errno;
	}
	/* Open resolv.tmp to save updated name server list */
	if (!(fp2 = fopen("/tmp/resolv.tmp", "w"))) {
		perror("fopen /tmp/resolv.tmp");
		fclose(fp);
		return errno;
	}
	/* Copy updated name servers */
	while (fgets(line, sizeof(line), fp)) {
		char *token = strtok(line, " \t\n");

		if (!token || strcmp(token, "nameserver") != 0)
			continue;
		if (!(token = strtok(NULL, " \t\n")))
			continue;

		foreach(word, nvram_safe_get(strcat_r(prefix, "dns", tmp)), next)
			if (!strcmp(word, token))
				break;
		if (!next)
			fprintf(fp2, "nameserver %s\n", token);
	}
	fclose(fp);
	fclose(fp2);
	/* Use updated file as resolv.conf */
	unlink("/tmp/resolv.conf");
	rename("/tmp/resolv.tmp", "/tmp/resolv.conf");

#ifdef ASUS_EXT
	stop_dns();
	start_dns();
#else	
	/* notify dnsmasq */
	snprintf(tmp, sizeof(tmp), "-%d", SIGHUP);
	eval("killall", tmp, "dnsmasq");
#endif
	
	return 0;
}

void
wan_up(char *wan_ifname)
{
	char tmp[100], prefix[] = "wanXXXXXXXXXX_", tmp1[128];
	char *wan_proto;

	/* Figure out nvram variable name prefix for this i/f */
	if (wan_prefix(wan_ifname, prefix) < 0)
		return;

	wan_proto = nvram_safe_get(strcat_r(prefix, "proto", tmp));	

	dprintf("%s %s\n", wan_ifname, wan_proto);

	/* Set default route to gateway if specified */
	if (nvram_match(strcat_r(prefix, "primary", tmp), "1"))
		route_add(wan_ifname, 0, "0.0.0.0", 
			nvram_safe_get(strcat_r(prefix, "gateway", tmp)),
			"0.0.0.0");

	/* Install interface dependent static routes */
	add_wan_routes(wan_ifname);

	/* Add dns servers to resolv.conf */
	add_ns(wan_ifname);

	start_ddns();

	/* Sync time */
	stop_ntpc();
	start_ntpc();

#ifdef ASUS_EXT
	update_wan_status(1);

#ifdef NOIPTABLES
	start_firewall2(wan_ifname);
#else
	start_firewall_ex(wan_ifname, nvram_safe_get(strcat_r(prefix, "ipaddr", tmp)), "br0", nvram_safe_get("lan_ipaddr"));
#endif

	stop_upnp();
	start_upnp();		
	if (strcmp(wan_proto, "bigpond")==0)	
	{
		stop_bpalogin();
		start_bpalogin();
	}
#endif
#ifdef CDMA
	if ((strcmp(wan_proto, "cdma")==0))
	{
		nvram_set("cdma_down", "2");
	}
#endif

#ifdef DHCP_PPTP
        if(strcmp(wan_proto, "pptp")==0 && nvram_match(strcat_r(prefix, "pppoe_gateway", tmp), ""))
        {
                //stop_wan();
                nvram_set(strcat_r(prefix, "pppoe_ipaddr", tmp), nvram_safe_get(strcat_r(prefix, "ipaddr", tmp1)));
                nvram_set(strcat_r(prefix, "pppoe_netmask", tmp), nvram_safe_get(strcat_r(prefix, "netmask", tmp1)));
                nvram_set(strcat_r(prefix, "pppoe_gateway", tmp), nvram_safe_get(strcat_r(prefix, "gateway", tmp1)));
                start_wan(); // go through pptp
        }
#endif

	
#ifdef QOS
	// start qos related 
	// start_qos(nvram_safe_get("wan_ipaddr_t"));
	if (nvram_match("router_disable", "0"))
		Speedtest_Init();
#endif

	dprintf("done\n");
}

void
wan_down(char *wan_ifname)
{
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";
	char *wan_proto;

	/* Figure out nvram variable name prefix for this i/f */
	if (wan_prefix(wan_ifname, prefix) < 0)
		return;

	wan_proto = nvram_safe_get(strcat_r(prefix, "proto", tmp));
	
	dprintf("%s %s\n", wan_ifname, wan_proto);

	/* Remove default route to gateway if specified */
	if (nvram_match(strcat_r(prefix, "primary", tmp), "1"))
		route_del(wan_ifname, 0, "0.0.0.0", 
			nvram_safe_get(strcat_r(prefix, "gateway", tmp)),
			"0.0.0.0");

	/* Remove interface dependent static routes */
	del_wan_routes(wan_ifname);

	/* Update resolv.conf */
	del_ns(wan_ifname);

	if (strcmp(wan_proto, "static")==0)
		ifconfig(wan_ifname, IFUP, NULL, NULL);

#ifdef ASUS_EXT
	//printf("update wan status\n");
	update_wan_status(0);

	if (strcmp(wan_proto, "bigpond")==0) stop_bpalogin();
#endif

#ifdef CDMA
	if ((strcmp(wan_proto, "cdma")==0))
	{
		stop_cdma();
		nvram_set("cdma_down", "1");
	}
#endif

	dprintf("done\n");
}

/* Enable WET DHCP relay for ethernet clients */
static int
enable_dhcprelay(char *ifname)
{
	char name[80], *next;

	dprintf("%s\n", ifname);
	
	/* WET interface is meaningful only in bridged environment */
	if (strncmp(ifname, "br", 2) == 0) {
		foreach(name, nvram_safe_get("lan_ifnames"), next) {
			char mode[] = "wlXXXXXXXXXX_mode";
			int unit;

			/* make sure the interface is indeed of wl */
			if (wl_probe(name))
				continue;
			
			/* get the instance number of the wl i/f */
			wl_ioctl(name, WLC_GET_INSTANCE, &unit, sizeof(unit));
			snprintf(mode, sizeof(mode), "wl%d_mode", unit);

			/* enable DHCP relay, there should be only one WET i/f */
			if (nvram_match(mode, "wet")) {
				uint32 ip;
				inet_aton(nvram_safe_get("lan_ipaddr"), (struct in_addr *)&ip);
				if (wl_iovar_setint(name, "wet_host_ipv4", ip))
					perror("wet_host_ipv4");
				break;
			}
		}
	}
	return 0;
}

#ifdef ASUS_EXT
#ifndef FLASH2M
void
lan_up(char *lan_ifname)
{
	FILE *fp;
	char word[100], *next;
	char line[100];

	/* Set default route to gateway if specified */
	route_add(lan_ifname, 0, "0.0.0.0", 
			nvram_safe_get("lan_gateway"),
			"0.0.0.0");

	/* Enable WET DHCP relay if requested */
	if (nvram_match("ure_disable", "0"))
		enable_dhcprelay(lan_ifname);

	/* Open resolv.conf to read */
	if (!(fp = fopen("/tmp/resolv.conf", "w"))) {
		perror("/tmp/resolv.conf");
		return errno;
	}

	if (nvram_invmatch("lan_gateway", ""))
		fprintf(fp, "nameserver %s\n", nvram_safe_get("lan_gateway"));

	foreach(word, nvram_safe_get("lan_dns"), next)
	{
		fprintf(fp, "nameserver %s\n", word);
	}
	fclose(fp);

	/* Sync time */
	stop_ntpc();
	start_ntpc();
}

void
lan_down(char *lan_ifname)
{
	/* Remove default route to gateway if specified */
	route_del(lan_ifname, 0, "0.0.0.0", 
			nvram_safe_get("lan_gateway"),
			"0.0.0.0");

	/* remove resolv.conf */
	unlink("/tmp/resolv.conf");
}


void
lan_up_ex(char *lan_ifname)
{
	FILE *fp;
	char word[100], *next;
	char line[100];

	/* Set default route to gateway if specified */
	route_add(lan_ifname, 0, "0.0.0.0", 
			nvram_safe_get("lan_gateway_t"),
			"0.0.0.0");

	/* Open resolv.conf to read */
	if (!(fp = fopen("/tmp/resolv.conf", "w"))) {
		perror("/tmp/resolv.conf");
		return errno;
	}

	if (nvram_invmatch("lan_gateway_t", ""))
		fprintf(fp, "nameserver %s\n", nvram_safe_get("lan_gateway_t"));

	foreach(word, nvram_safe_get("lan_dns_t"), next)
	{
		fprintf(fp, "nameserver %s\n", word);
	}
	fclose(fp);

	/* Sync time */
	stop_ntpc();
	start_ntpc();
	//update_lan_status(1);
}

void
lan_down_ex(char *lan_ifname)
{
	/* Remove default route to gateway if specified */
	route_del(lan_ifname, 0, "0.0.0.0", 
			nvram_safe_get("lan_gateway_t"),
			"0.0.0.0");

	/* remove resolv.conf */
	unlink("/tmp/resolv.conf");

	update_lan_status(0);
}
#endif
#endif

static int
notify_nas(char *type, char *ifname, char *action)
{
	char *argv[] = {"nas4not", type, ifname, action, 
			NULL,	/* role */
			NULL,	/* crypto */
			NULL,	/* auth */
			NULL,	/* passphrase */
			NULL,	/* ssid */
			NULL};
	char *str = NULL;
	int retries = 10;
	char tmp[100], prefix[] = "wlXXXXXXXXXX_";
	int unit;
	char remote[ETHER_ADDR_LEN];
	char ssid[48], pass[80], auth[16], crypto[16], role[8];
	int i;

	/* the wireless interface must be configured to run NAS */
	wl_ioctl(ifname, WLC_GET_INSTANCE, &unit, sizeof(unit));
	snprintf(prefix, sizeof(prefix), "wl%d_", unit);
	if (nvram_match(strcat_r(prefix, "akm", tmp), "") &&
	    nvram_match(strcat_r(prefix, "auth_mode", tmp), "none"))
		return 0;

	/* find WDS link configuration */
	wl_ioctl(ifname, WLC_WDS_GET_REMOTE_HWADDR, remote, ETHER_ADDR_LEN);
	for (i = 0; i < MAX_NVPARSE; i ++) {
		char mac[ETHER_ADDR_STR_LEN];
		uint8 ea[ETHER_ADDR_LEN];

		if (get_wds_wsec(unit, i, mac, role, crypto, auth, ssid, pass) &&
		    ether_atoe(mac, ea) && !bcmp(ea, remote, ETHER_ADDR_LEN)) {
			argv[4] = role;
			argv[5] = crypto;
			argv[6] = auth;
			argv[7] = pass;
			argv[8] = ssid;
			break;
		}
	}

	/* did not find WDS link configuration, use wireless' */
	if (i == MAX_NVPARSE) {
		/* role */
		argv[4] = "auto";
		/* crypto */
		argv[5] = nvram_safe_get(strcat_r(prefix, "crypto", tmp));
		/* auth mode */
		argv[6] = nvram_safe_get(strcat_r(prefix, "akm", tmp));
		/* passphrase */
		argv[7] = nvram_safe_get(strcat_r(prefix, "wpa_psk", tmp));
		/* ssid */
		argv[8] = nvram_safe_get(strcat_r(prefix, "ssid", tmp));
	}

	/* wait till nas is started */
	while (retries -- > 0 && !(str = file2str("/tmp/nas.lan.pid")))
		sleep(1);
	if (str) {
		int pid;
		free(str);
		return _eval(argv, ">/dev/console", 0, &pid);
	}
	return -1;
}

int
hotplug_net(void)
{
	char *lan_ifname = nvram_safe_get("lan_ifname");
	char *interface, *action;

	if (!(interface = getenv("INTERFACE")) ||
	    !(action = getenv("ACTION")))
		return EINVAL;

	if (strncmp(interface, "wds", 3))
		return 0;

	if (!strcmp(action, "register")) {
		/* Bring up the interface and add to the bridge */
		ifconfig(interface, IFUP, NULL, NULL);
		
		/* Bridge WDS interfaces */
		if (!strncmp(lan_ifname, "br", 2) && 
		    eval("brctl", "addif", lan_ifname, interface))
		    return 0;

		/* Notify NAS of adding the interface */
		notify_nas("lan", interface, "up");
	}
	return 0;
}


int
wan_ifunit(char *wan_ifname)
{
	int unit;
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";

	if ((unit = ppp_ifunit(wan_ifname)) >= 0)
		return unit;
	else {
		for (unit = 0; unit < MAX_NVPARSE; unit ++) {
			snprintf(prefix, sizeof(prefix), "wan%d_", unit);
			if (nvram_match(strcat_r(prefix, "ifname", tmp), wan_ifname) &&
			    (nvram_match(strcat_r(prefix, "proto", tmp), "dhcp") ||
			     nvram_match(strcat_r(prefix, "proto", tmp), "bigpond") ||
#ifdef DHCP_PPTP
                             nvram_match(strcat_r(prefix, "proto", tmp), "pptp") ||
#endif
#ifdef CDMA
			     nvram_match(strcat_r(prefix, "proto", tmp), "cdma") ||
#endif
			     nvram_match(strcat_r(prefix, "proto", tmp), "static")))
				return unit;
		}
	}
	return -1;
}

int
preset_wan_routes(char *wan_ifname)
{
	char tmp[100], prefix[] = "wanXXXXXXXXXX_";


	/* Figure out nvram variable name prefix for this i/f */
	if (wan_prefix(wan_ifname, prefix) < 0)
		return -1;

	/* Set default route to gateway if specified */
	if (nvram_match(strcat_r(prefix, "primary", tmp), "1"))
	{
		route_add(wan_ifname, 0, "0.0.0.0", "0.0.0.0", "0.0.0.0");
	}

	/* Install interface dependent static routes */
	add_wan_routes(wan_ifname);
	return 0;
}

int
wan_primary_ifunit(void)
{
	int unit;
	
	for (unit = 0; unit < MAX_NVPARSE; unit ++) {
		char tmp[100], prefix[] = "wanXXXXXXXXXX_";
		snprintf(prefix, sizeof(prefix), "wan%d_", unit);
		if (nvram_match(strcat_r(prefix, "primary", tmp), "1"))
			return unit;
	}

	return 0;
}

