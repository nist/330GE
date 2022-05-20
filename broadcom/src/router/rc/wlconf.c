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
 * Wireless Network Adapter Configuration Utility
 *
 * Copyright 2005, ASUSTek Inc.
 * All Rights Reserved.                
 *                                     
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ASUSTek Inc.;   
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior      
 * written permission of ASUSTek Inc..                            
 *
 * $Id: wlconf.c,v 1.1.1.1 2007/01/25 12:52:21 jiahao_jhou Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <typedefs.h>
#include <bcmnvram.h>
#include <bcmutils.h>
#include <shutils.h>
#include <wlutils.h>

/* phy types */
#define	PHY_TYPE_A		0
#define	PHY_TYPE_B		1
#define	PHY_TYPE_G		2
#define	PHY_TYPE_NULL		0xf

/* parts of an idcode: */
#define	IDCODE_MFG_MASK		0x00000fff
#define	IDCODE_MFG_SHIFT	0
#define	IDCODE_ID_MASK		0x0ffff000
#define	IDCODE_ID_SHIFT		12
#define	IDCODE_REV_MASK		0xf0000000
#define	IDCODE_REV_SHIFT	28

#define WL_IOCTL(name, cmd, buf, len) (ret = wl_ioctl((name), (cmd), (buf), (len)))

/* set WEP key */
static int
wlconf_set_wep_key(char *name, char *prefix, int i)
{
	wl_wsec_key_t key;
	char wl_key[] = "wlXXXXXXXXXX_keyXXXXXXXXXX";
	char *keystr, hex[] = "XX";
	unsigned char *data = key.data;
	int ret = 0;

	memset(&key, 0, sizeof(key));
	key.index = i - 1;
	sprintf(wl_key, "%skey%d", prefix, i);
	keystr = nvram_safe_get(wl_key);

	switch (strlen(keystr)) {
	case WEP1_KEY_SIZE:
	case WEP128_KEY_SIZE:
		key.len = strlen(keystr);
		strcpy(key.data, keystr);
		break;
	case WEP1_KEY_HEX_SIZE:
	case WEP128_KEY_HEX_SIZE:
		key.len = strlen(keystr) / 2;
		while (*keystr) {
			strncpy(hex, keystr, 2);
			*data++ = (unsigned char) strtoul(hex, NULL, 16);
			keystr += 2;
		}
		break;
	default:
		key.len = 0;
		break;
	}

	/* Set current WEP key */
	if (key.len && i == atoi(nvram_safe_get(strcat_r(prefix, "key", wl_key))))
		key.flags = WL_PRIMARY_KEY;

	WL_IOCTL(name, WLC_SET_KEY, &key, sizeof(key));

	return ret;
}

static int 
wlconf_akm_options(char *prefix)
{
        char comb[32];
        char *wl_akm;
        int akm_ret_val = 0;
        char akm[32];
        char *next;

        wl_akm = nvram_safe_get(strcat_r(prefix, "akm", comb));
	foreach(akm, wl_akm, next) {
		if (!strcmp(akm, "wpa"))
			akm_ret_val |= WPA_AUTH_UNSPECIFIED;
		if (!strcmp(akm, "psk"))
			akm_ret_val |= WPA_AUTH_PSK;
		if (!strcmp(akm, "wpa2"))
			akm_ret_val |= WPA2_AUTH_UNSPECIFIED;
		if (!strcmp(akm, "psk2"))
			akm_ret_val |= WPA2_AUTH_PSK;
	}
	return akm_ret_val;
}

/* Set up wsec */
static int
wlconf_set_wsec(char *ifname, char *prefix)
{
	char tmp[100];
	int val = 0, ret;
	int akm_val;

	/* Set wsec bitvec */
	akm_val = wlconf_akm_options(prefix); 
	if (akm_val != 0) {
		if (nvram_match(strcat_r(prefix, "crypto", tmp), "tkip"))
			val = TKIP_ENABLED;
		else if (nvram_match(strcat_r(prefix, "crypto", tmp), "aes"))
			val = AES_ENABLED;
		else if (nvram_match(strcat_r(prefix, "crypto", tmp), "tkip+aes"))
			val = TKIP_ENABLED | AES_ENABLED;
	}
	if (nvram_match(strcat_r(prefix, "wep", tmp), "enabled"))
		val |= WEP_ENABLED;
	WL_IOCTL(ifname, WLC_SET_WSEC, &val, sizeof(val));
	/* Set wsec restrict if WSEC_ENABLED */
	val = val ? 1 : 0;
	WL_IOCTL(ifname, WLC_SET_WEP_RESTRICT, &val, sizeof(val));

	return 0;
}

static int 
wlconf_set_preauth(char *name, int preauth)
{
	int8 cap[WPA_CAP_LEN];
	int ret_val;


	ret_val =  wl_get_val(name, "wpa_cap", cap, WPA_CAP_LEN);
	if (ret_val != 0) return -1;

	if (preauth)
		cap[0] |= WPA_CAP_WPA2_PREAUTH;
	else
		cap[0] &= ~WPA_CAP_WPA2_PREAUTH; 

	return (wl_set_val(name, "wpa_cap", cap, WPA_CAP_LEN));

}

/* Set up WME */
static void
wlconf_set_wme(char *name, char *prefix)
{
	int i, j, k;
	int val, ret;
	int phytype, gmode, no_ack;
	wme_acparam_t *acparams;
	char buf[WLC_IOCTL_MAXLEN];
	char *v, *nv_value, nv[100];
	char nv_name[] = "%swme_%s_%s";
	char *ac[] = {"be", "bk", "vi", "vo"};
	char *cwmin, *cwmax, *aifsn, *txop_b, *txop_ag, *admin_forced;
	char **locals[] = {&cwmin, &cwmax, &aifsn, &txop_b, &txop_ag, &admin_forced};
	struct {char *req; char *str;} mode[] = {{"wme_ac_sta", "sta"}, {"wme_ac_ap", "ap"}};

	/* query the phy type */
	WL_IOCTL(name, WLC_GET_PHYTYPE, &phytype, sizeof(phytype));
	/* get gmode */
	gmode = atoi(nvram_safe_get(strcat_r(prefix, "gmode", nv)));

	/* WME sta setting first */
	for (i = 0; i < 2; i++) {
		/* build request block */
		memset(buf, 0, sizeof(buf));
		strcpy(buf, mode[i].req);
		/* put push wmeac params after "wme-ac" in buf */
		acparams = (wme_acparam_t *)(buf + strlen(buf) + 1);
		for (j = 0; j < AC_MAX; j++) {
			/* get packed nvram parameter */
			snprintf(nv, sizeof(nv), nv_name, prefix, mode[i].str, ac[j]);
			nv_value = nvram_safe_get(nv);
			strcpy(nv, nv_value);
			/* unpack it */
			for (v = nv, k = 0; (int)(v - nv) < strlen(nv_value) && 
				k < (sizeof(locals)/sizeof(locals[0])); k++) {
				*locals[k] = v;
				while((int)(v - nv) < strlen(nv_value) && *v != ' ')
					v++;
				*v = 0;
				v++;
			}
			if (k != (sizeof(locals)/sizeof(locals[0])))
				return;

			/* update CWmin */
			acparams->ECW &= ~WME_CWMIN_MASK;
			val = atoi(cwmin);
			for (val++, k = 0; val; val >>= 1, k++);
			acparams->ECW |= (k ? k - 1 : 0) & WME_CWMIN_MASK;
			/* update CWmax */
			acparams->ECW &= ~WME_CWMAX_MASK;
			val = atoi(cwmax);
			for (val++, k = 0; val; val >>= 1, k++);
			acparams->ECW |= ((k ? k - 1 : 0) << WME_CWMAX_SHIFT) & WME_CWMAX_MASK;
			/* update AIFSN */
			acparams->ACI &= ~WME_AIFS_MASK;
			acparams->ACI |= atoi(aifsn) & WME_AIFS_MASK;
			/* update ac */
			acparams->ACI &= ~WME_ACI_MASK;
			acparams->ACI |= j << WME_ACI_SHIFT;
			/* update TXOP */
			if (phytype == PHY_TYPE_B || gmode == 0)
				val = atoi(txop_b);
			else
				val = atoi(txop_ag);
			acparams->TXOP = val / 32;
			/* update acm */
			acparams->ACI &= ~WME_ACM_MASK;
			val = strcmp(admin_forced, "on") ? 0 : 1;
			acparams->ACI |= val << 4;

			/* configure driver */
			WL_IOCTL(name, WLC_SET_VAR, buf, sizeof(buf));
		}
	}

	/* set no-ack */
	v = nvram_safe_get(strcat_r(prefix, "wme_no_ack", nv));
	no_ack = strcmp(v, "on") ? 0 : 1;
	strcpy(buf, "wme_noack");
	*(int *)&buf[strlen(buf) + 1] = no_ack;
	WL_IOCTL(name, WLC_SET_VAR, buf, sizeof(buf));
}

/*
* For debugging only
*/
#define WLCONF_DBG(fmt, arg...)

#if defined(linux)
#include <unistd.h>
static void
sleep_ms(const unsigned int ms)
{
	usleep(1000*ms);
}
#endif

/*
* The following condition(s) must be met when Auto Channel Selection 
* is enabled.
*  - the I/F is up (change radio channel requires it is up?)
*  - the AP must not be associated (setting SSID to empty should 
*    make sure it for us)
*/
static uint8
wlconf_auto_channel(char *name)
{
	int chosen = 0;
	wl_uint32_list_t request;
	int phytype;
	/* query the phy type */
	wl_ioctl(name, WLC_GET_PHYTYPE, &phytype, sizeof(phytype));
	request.count = 0;	/* let the ioctl decide */
	if (!wl_ioctl(name, WLC_START_CHANNEL_SEL, &request, sizeof(request))) {
		sleep_ms(phytype == PHY_TYPE_A ? 1000 : 750);
		while (wl_ioctl(name, WLC_GET_CHANNEL_SEL, &chosen, sizeof(chosen)))
			sleep_ms(100);
	}
	WLCONF_DBG("interface %s: channel selected %d\n", name, chosen);
	return chosen;
}

/* PHY type/BAND conversion */
#define WLCONF_PHYTYPE2BAND(phy)	((phy) == PHY_TYPE_A ? WLC_BAND_A : WLC_BAND_B)
/* PHY type conversion */
#define WLCONF_PHYTYPE2STR(phy)	((phy) == PHY_TYPE_A ? "a" : \
				 (phy) == PHY_TYPE_B ? "b" : "g")
#define WLCONF_STR2PHYTYPE(phy)	((phy) && (phy)[0] == 'a' ? PHY_TYPE_A : \
				 (phy) && (phy)[0] == 'b' ? PHY_TYPE_B : PHY_TYPE_G)
				 
/* configure the specified wireless interface */
int
wlconf(char *name)
{
	int restore_defaults, val, unit, phytype, gmode = 0, ret = 0;
	char tmp[100], prefix[] = "wlXXXXXXXXXX_";
	char var[80], *next, phy[] = "a", *str;
	unsigned char buf[WLC_IOCTL_MAXLEN];
	char *country;
	wlc_rev_info_t rev;
	channel_info_t ci;
	struct maclist *maclist;
	struct ether_addr *ea;
	wlc_ssid_t ssid;
	wl_rateset_t rs;
	unsigned int i;
	char eaddr[32];
	int ap, wds, sta = 0, wet = 0;
	char country_code[4];
	int nas_will_run = 0;
	char *afterburner;
	char *wme;
	char *preauth;
	int set_preauth;

	/* Check interface (fail silently for non-wl interfaces) */
	if ((ret = wl_probe(name)))
		return ret;

	/* Get MAC address */
	(void) wl_hwaddr(name, buf);

	/* Get instance */
	WL_IOCTL(name, WLC_GET_INSTANCE, &unit, sizeof(unit));
	snprintf(prefix, sizeof(prefix), "wl%d_", unit);

	/* Restore defaults if per-interface parameters do not exist */
	restore_defaults = !nvram_get(strcat_r(prefix, "ifname", tmp));
	wlconf_validate_all(prefix, restore_defaults);
	nvram_set(strcat_r(prefix, "ifname", tmp), name);
	nvram_set(strcat_r(prefix, "hwaddr", tmp), ether_etoa(buf, eaddr));
	snprintf(buf, sizeof(buf), "%d", unit);
	nvram_set(strcat_r(prefix, "unit", tmp), buf);

	/*
	* Nuke SSID first so that the AP won't be associated when WLC_UP.
	* This must be done here if Auto Channel Selection is enabled.
	*/
	WL_IOCTL(name, WLC_GET_UP, &val, sizeof(val));
	if (val) {
		/* Nuke SSID  */
		ssid.SSID_len = 0;
		ssid.SSID[0] = '\0';
		WL_IOCTL(name, WLC_SET_SSID, &ssid, sizeof(ssid));

		/* Bring the interface down */
		WL_IOCTL(name, WLC_DOWN, NULL, sizeof(val));
	}

	/* Set mode : AP, STA */
	ap = nvram_match(strcat_r(prefix, "mode", tmp), "ap");
	wds = nvram_match(strcat_r(prefix, "mode", tmp), "wds");
	val = (ap + wds) ? 1 : 0;
	WL_IOCTL(name, WLC_SET_AP, &val, sizeof(val));

	/* Set STA specific parameters */
	if (!ap) {
		/* Set mode: WET */
		if ((wet = nvram_match(strcat_r(prefix, "mode", tmp), "wet")))
			WL_IOCTL(name, WLC_SET_WET, &wet, sizeof(wet));
		/* Set infra: BSS/IBSS */
		if (wet || (sta = nvram_match(strcat_r(prefix, "mode", tmp), "sta"))) {
			val = atoi(nvram_safe_get(strcat_r(prefix, "infra", tmp)));
			WL_IOCTL(name, WLC_SET_INFRA, &val, sizeof(val));
		}
	}
	/* Set The AP MAX Associations Limit */
	if (ap) {
		 val = atoi(nvram_safe_get(strcat_r(prefix, "maxassoc", tmp)));
		 if (val > 0)
		 	wl_set_int(name, "maxassoc", val);

		/* 
		 * Set The WPA2 Pre auth cap. only reason we are doing it here is the driver is down
		 * if we do it in the NAS we need to bring down the interface and up to make 
		 * it affect in the  beacons 
		 * */
		set_preauth = 1;
		preauth = nvram_safe_get(strcat_r(prefix, "preauth", tmp));
		if (strlen (preauth) != 0) {
			set_preauth = atoi(preauth);
		}
		wlconf_set_preauth(name, set_preauth);
	}

	val = 1 // set ap closed
	WL_IOCTL(name, WLC_SET_CLOSED, &val, sizeof(val));

        /* Set the ap isolate mode */
	val = 1;
	wl_set_int(name, "ap_isolate", val);
	
	/* Set the MAC list */
	maclist = (struct maclist *) buf;
	maclist->count = 0;
	WL_IOCTL(name, WLC_SET_MACLIST, buf, sizeof(buf));
	val = WLC_MACMODE_DISABLED;
	WL_IOCTL(name, WLC_SET_MACMODE, &val, sizeof(val));
	val = 1;
	WL_IOCTL(name, WLC_SET_RADIO, &val, sizeof(val));

		int control = WLC_G_PROTECTION_CTL_OFF;

		/* Set gmode */
		gmode = atoi(nvram_safe_get(strcat_r(prefix, "gmode", tmp)));
		WL_IOCTL(name, WLC_SET_GMODE, &gmode, sizeof(gmode));

		/* Set gmode protection override and control algorithm */
		strcat_r(prefix, "gmode_protection", tmp);
		if (nvram_match(tmp, "auto")) {
			override = WLC_G_PROTECTION_AUTO;
			control = WLC_G_PROTECTION_CTL_OVERLAP;
		}
		WL_IOCTL(name, WLC_SET_GMODE_PROTECTION_OVERRIDE, &override, sizeof(override));
		WL_IOCTL(name, WLC_SET_GMODE_PROTECTION_CONTROL, &control, sizeof(control));
	}
	val = 0;
	wl_set_val(name, "wme", &val, sizeof(val));
	if (val)
		wlconf_set_wme(name, prefix);
	
	/* Set WSEC */
	/*
	* Need to check errors (card may have changed) and change to
	* defaults since the new chip may not support the requested 
	* encryptions after the card has been changed.
	*/
	if (wlconf_set_wsec(name, prefix)) {
		/* change nvram only, code below will pass them on */
		wlconf_restore_var(prefix, "auth_mode");
		wlconf_restore_var(prefix, "auth");
		/* reset wep to default */
		wlconf_restore_var(prefix, "crypto");
		wlconf_restore_var(prefix, "wep");
		wlconf_set_wsec(name, prefix);
	}

 	val = wlconf_akm_options(prefix);
	if (val & WPA_AUTH_PSK) {
		/* In wet mode enable in driver wpa supplicant */
		if (wet) {
			wsec_pmk_t psk;
			char *key;

			if (((key = nvram_get(strcat_r(prefix, "wpa_psk", tmp))) != NULL)
					&& (strlen(key) < WSEC_MAX_PSK_LEN)) {
				psk.key_len = (ushort) strlen(key);
				psk.flags = WSEC_PASSPHRASE;
				strcpy(psk.key, key);
				WL_IOCTL(name, WLC_SET_WSEC_PMK, &psk, sizeof(psk));
			}
			wl_set_int(name, "sup_wpa", 1);
		}
	}
	WL_IOCTL(name, WLC_SET_WPA_AUTH, &val, sizeof(val));

	if ( (val != 0) || (nvram_match(strcat_r(prefix, "auth_mode", tmp), "radius")))
		val = 1;
	else 
		val = 0;

	nas_will_run =  val;

	WL_IOCTL(name, WLC_SET_EAP_RESTRICT, &val, sizeof(val));
	
	/* Set WEP keys */
	if (nvram_match(strcat_r(prefix, "wep", tmp), "enabled")) {
		for (i = 1; i <= DOT11_MAX_DEFAULT_KEYS; i++)
			wlconf_set_wep_key(name, prefix, i);
	}

	/* Set 802.11 authentication mode - open/shared */
	val = atoi(nvram_safe_get(strcat_r(prefix, "auth", tmp)));
	WL_IOCTL(name, WLC_SET_AUTH, &val, sizeof(val));

	/*
	 * Set SSID/Join BSS
	 *
	 * AP: Set SSID to bring AP up only when nas will not run
	 * STA: Join the BSS regardless.
	 */
	if ((ap && !nas_will_run) || sta || wet) {
		strcat_r(prefix, "ssid", tmp);
		ssid.SSID_len = strlen(nvram_safe_get(tmp));
		if (ssid.SSID_len > sizeof(ssid.SSID))
			ssid.SSID_len = sizeof(ssid.SSID);
		strncpy(ssid.SSID, nvram_safe_get(tmp), ssid.SSID_len);
	}
	/* A zero length SSID turns off the AP */
	else {
		ssid.SSID_len = 0;
		ssid.SSID[0] = '\0';
	}
	WL_IOCTL(name, WLC_SET_SSID, &ssid, sizeof(ssid));
	
	return 0;
}
