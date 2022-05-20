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
 * $Id: common_ex.c,v 1.1.1.1 2007/01/25 12:52:51 jiahao_jhou Exp $
 */


#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<signal.h>
#include<bcmnvram.h>
#include<shutils.h>
#include<netconf.h>
#include<wlioctl.h>
#include<sys/time.h>
#include<syslog.h>
#include <stdarg.h>

#define XSTR(s) STR(s)
#define STR(s) #s

static char list[2048];

/* remove space in the end of string */
char *trim_r(char *str)
{
	int i;

	i=strlen(str);

	while(i>=1)
	{
		if (*(str+i-1) == ' ' || *(str+i-1) == 0x0a || *(str+i-1) == 0x0d) *(str+i-1)=0x0;
		else break;
		i--;
	}
	return(str);
}

/* convert mac address format from XXXXXXXXXXXX to XX:XX:XX:XX:XX:XX */
char *mac_conv(char *mac_name, int idx, char *buf)
{
	char *mac, name[32];
	int i, j;

	if (idx!=-1)	
		sprintf(name, "%s%d", mac_name, idx);
	else sprintf(name, "%s", mac_name);

	mac = nvram_safe_get(name);

	if (strlen(mac)==0) 
	{
		buf[0] = 0;
	}
	else
	{
		j=0;	
		for(i=0; i<12; i++)
		{		
			if (i!=0&&i%2==0) buf[j++] = ':';
			buf[j++] = mac[i];
		}
	}
	buf[j] = 0;

	dprintf("mac: %s\n", buf);

	return(buf);
}

void getsyspara(void)
{
	FILE *fp;
	char buf[1];
	unsigned long *imagelen;
	char dataPtr[4];
	char verPtr[64];
	char productid[13];
	char fwver[12];
	int i;

	strcpy(productid, "WLHDD");
	strcpy(fwver, "0.1.0.1");

	if ((fp = fopen("/dev/mtd/1", "rb"))!=NULL)
	{
		if (fseek(fp, 4, SEEK_SET)!=0) goto write_ver;
		fread(dataPtr, 1, 4, fp);
		imagelen = (unsigned long *)dataPtr;

		dprintf("image len %x\n", *imagelen);
		if (fseek(fp, *imagelen - 64, SEEK_SET)!=0) goto write_ver;
		dprintf("seek\n");
		if (!fread(verPtr, 1, 64, fp)) goto write_ver;
		dprintf("ver %x %x", verPtr[0], verPtr[1]);
		strncpy(productid, verPtr + 4, 12);
		productid[12] = 0;
		sprintf(fwver, "%d.%d.%d.%d", verPtr[0], verPtr[1], verPtr[2], verPtr[3]);

		dprintf("get fw ver: %s\n", productid);
		fclose(fp);
	}
write_ver:
	// its a ugle solution for Product ID
	if (strstr(productid, "WL500gx"))
		nvram_set("productid", "WL500g.Deluxe");
	else
		nvram_set("productid", trim_r(productid));
	nvram_set("firmver", trim_r(fwver));
}

void wan_netmask_check(void)
{
	unsigned int ip, gw, nm, lip, lnm;

	if (nvram_match("wan0_proto", "static") ||
	    nvram_match("wan0_proto", "pptp"))
	{
		ip = inet_addr(nvram_safe_get("wan_ipaddr"));
		gw = inet_addr(nvram_safe_get("wan_gateway"));
		nm = inet_addr(nvram_safe_get("wan_netmask"));

		lip = inet_addr(nvram_safe_get("lan_ipaddr"));
		lnm = inet_addr(nvram_safe_get("lan_netmask"));

		dprintf("ip : %x %x %x\n", ip, gw, nm);

		if (ip==0x0 || (ip&lnm)==(lip&lnm))
		{
			nvram_set("wan_ipaddr", "1.1.1.1");
			nvram_set("wan_netmask", "255.0.0.0");	
		}

		// check netmask here
		if (gw==0 || gw==0xffffffff || (ip&nm)==(gw&nm))
		{
			nvram_set("wan0_netmask", nvram_safe_get("wan_netmask"));
		}
		else
		{		
			for(nm=0xffffffff;nm!=0;nm=(nm>>8))
			{
				if ((ip&nm)==(gw&nm)) break;
			}

			dprintf("nm: %x\n", nm);

			if (nm==0xffffffff) nvram_set("wan0_netmask", "255.255.255.255");
			else if (nm==0xffffff) nvram_set("wan0_netmask", "255.255.255.0");
			else if (nm==0xffff) nvram_set("wan0_netmask", "255.255.0.0");
			else if (nm==0xff) nvram_set("wan0_netmask", "255.0.0.0");
			else nvram_set("wan0_netmask", "0.0.0.0");
		}

		nvram_set("wanx_ipaddr", nvram_safe_get("wan0_ipaddr"));
		nvram_set("wanx_netmask", nvram_safe_get("wan0_netmask"));
		nvram_set("wanx_gateway", nvram_safe_get("wan0_gateway"));
	}
}

/* This function is used to map nvram value from asus to Broadcom */
void convert_asus_values(int skipflag)
{	
	char tmpstr[32], tmpstr1[32], macbuf[36];
	char servers[64];
	char ifnames[36];
	char sbuf[64];
	int i, num;
	char *ptr;
	FILE *fp;

	if(!skipflag)
	{
	getsyspara();
	/* convert country code from regulation_domain */
	convert_country();
	}

#ifdef CHIP5352
	{	
		int radio=atoi(nvram_safe_get("wl_radio_power_x"));
	
		//early_convert_asus_values();
		if (radio>=13)
			nvram_set("wl_radio_power_x", "9");
	}
#endif

#ifdef RT2400_SUPPORT
	write_rt2400_conf();
#endif

#ifdef WOB
	// add for 4712/5350 which have no eeprom
	//nvram_set("il0macaddr", nvram_safe_get("et0macaddr"));
	nvram_unset("wl0_hwaddr");
#endif

	if (nvram_match("productid", "WL500b") || 
	    nvram_match("productid", "WL500bv2"))
	{
		nvram_set("wl_gmode", "0");
	}

	cprintf("read from nvram\n");

	/* Wireless Section */
	/* Country Code */
	nvram_set("wl0_country_code", nvram_safe_get("wl_country_code"));

	/* GMODE */
	nvram_set("wl0_gmode", nvram_safe_get("wl_gmode"));

	if (nvram_match("wl_gmode_protection_x", "1"))
	{
		//cprintf("set to auto\n");
		nvram_set("wl_gmode_protection", "auto");
		nvram_set("wl0_gmode_protection", "auto");
	}
	else
	{
		//cprintf("set to off");
		nvram_set("wl_gmode_protection", "off");
		nvram_set("wl0_gmode_protection", "off");
	}	

	if (nvram_match("wl_wep_x", "0"))
		nvram_set("wl0_wep", "disabled");
	else nvram_set("wl0_wep", "enabled");

	if (nvram_match("wl_auth_mode", "shared"))
		nvram_set("wl0_auth", "1");
	else nvram_set("wl0_auth", "0");


#ifdef WPA2_WMM	
	if (nvram_match("wl_auth_mode", "wpa"))
	{
		nvram_set("wl_akm", "wpa wpa2");
		nvram_set("wl0_akm", "wpa wpa2");
	}
	else if (nvram_match("wl_auth_mode", "psk"))
	{
		nvram_set("wl_akm", "psk psk2");
		nvram_set("wl0_akm", "psk psk2");
	}	
	else 
	{
		nvram_set("wl_akm", "");
		nvram_set("wl0_akm", "");
	}
	// thanks for Oleg
	nvram_set("wl0_auth_mode", nvram_match("wl_auth_mode", "radius") ? "radius" : "none");
	
	nvram_set("wl0_preauth", nvram_safe_get("wl_preauth"));
	nvram_set("wl0_net_reauth", nvram_safe_get("wl_net_reauth"));
	nvram_set("wl0_wme", nvram_safe_get("wl_wme"));
	nvram_set("wl0_wme_no_ack", nvram_safe_get("wl_wme_no_ack"));
	nvram_set("wl0_wme_sta_bk", nvram_safe_get("wl_wme_sta_bk"));
	nvram_set("wl0_wme_sta_be", nvram_safe_get("wl_wme_sta_be"));
	nvram_set("wl0_wme_sta_vi", nvram_safe_get("wl_wme_sta_vi"));
	nvram_set("wl0_wme_sta_vo", nvram_safe_get("wl_wme_sta_vo"));
	nvram_set("wl0_wme_ap_bk", nvram_safe_get("wl_wme_ap_bk"));
	nvram_set("wl0_wme_ap_be", nvram_safe_get("wl_wme_ap_be"));
	nvram_set("wl0_wme_ap_vi", nvram_safe_get("wl_wme_ap_vi"));
	nvram_set("wl0_wme_ap_vo", nvram_safe_get("wl_wme_ap_vo"));
#else
	nvram_set("wl0_auth_mode", nvram_safe_get("wl_auth_mode"));
	nvram_set("wl_akm", "");
	nvram_set("wl0_akm", "");
	nvram_set("wl0_wme", "off");
#endif

	nvram_set("wl0_ssid", nvram_safe_get("wl_ssid"));
	nvram_set("wl0_channel", nvram_safe_get("wl_channel"));
	nvram_set("wl0_country_code", nvram_safe_get("wl_country_code"));
	nvram_set("wl0_rate", nvram_safe_get("wl_rate"));
	nvram_set("wl0_rateset", nvram_safe_get("wl_rateset"));
	nvram_set("wl0_frag", nvram_safe_get("wl_frag"));
	nvram_set("wl0_rts", nvram_safe_get("wl_rts"));
	nvram_set("wl0_dtim", nvram_safe_get("wl_dtim"));
	nvram_set("wl0_bcn", nvram_safe_get("wl_bcn"));
	nvram_set("wl0_plcphdr", nvram_safe_get("wl_plcphdr"));
	nvram_set("wl0_crypto", nvram_safe_get("wl_crypto"));
	nvram_set("wl0_wpa_psk", nvram_safe_get("wl_wpa_psk"));
	nvram_set("wl0_key", nvram_safe_get("wl_key"));
	nvram_set("wl0_key1", nvram_safe_get("wl_key1"));
	nvram_set("wl0_key2", nvram_safe_get("wl_key2"));
	nvram_set("wl0_key3", nvram_safe_get("wl_key3"));
	nvram_set("wl0_key4", nvram_safe_get("wl_key4"));
	nvram_set("wl0_closed", nvram_safe_get("wl_closed"));
	nvram_set("wl0_frameburst", nvram_safe_get("wl_frameburst"));
	nvram_set("wl0_afterburner", nvram_safe_get("wl_afterburner"));
	nvram_set("wl0_ap_isolate", nvram_safe_get("wl_ap_isolate"));
	nvram_set("wl0_radio", nvram_safe_get("wl_radio_x"));

#if !defined(CHIP5352) && !defined(CHIP5350)
	if(nvram_match("wl_radio_x", "0"))
	{
		nvram_set("wl0gpio0", "1");
	}	
	else
	{
		nvram_unset("wl0gpio0");
	}
#endif

	nvram_set("wl0_radius_ipaddr", nvram_safe_get("wl_radius_ipaddr"));
	nvram_set("wl0_radius_port", nvram_safe_get("wl_radius_port"));
	nvram_set("wl0_radius_key", nvram_safe_get("wl_radius_key"));
	nvram_set("wl0_wpa_gtk_rekey", nvram_safe_get("wl_wpa_gtk_rekey"));


	if (nvram_invmatch("wl_mode_ex", "ap"))
	{
		int wepidx=atoi(nvram_safe_get("wl0_key"));
		char wepkey[64], wepname[16];

		sprintf(sbuf, "wl join \"%s\"", nvram_safe_get("wl0_ssid"));

		// key ??
		if (nvram_match("wl0_auth_mode", "psk"))
		{
			sprintf(sbuf, "%s key %s", sbuf, nvram_safe_get("wl0_wpa_psk"));
		}
		else if (nvram_match("wl0_wep", "enabled") && wepidx>=1 && wepidx<=4)
		{
			sprintf(wepname, "wl0_key%d", wepidx);
			sprintf(sbuf, "%s key %s", sbuf, nvram_safe_get(wepname));
		}
		
		sprintf(sbuf, "%s imode bss", sbuf);
		
		if (nvram_match("wl_auth_mode", "psk"))
			sprintf(sbuf, "%s amode wpapsk", sbuf);
		else if (nvram_match("wl_auth_mode", "shared"))
			sprintf(sbuf, "%s amode shared", sbuf);
		else sprintf(sbuf, "%s amode open", sbuf);

		nvram_set("wl0_join", sbuf);
		/* Station or Ethernet Bridge Mode */
		nvram_set("wl0_mode", nvram_safe_get("wl_mode_ex"));
	}
	else
	{
		/* WDS control */
		if (nvram_match("wl_mode_x", "1")) 
			nvram_set("wl0_mode", "wds");
		else nvram_set("wl0_mode", "ap");

		nvram_set("wl0_lazywds", nvram_safe_get("wl_lazywds"));
	}

	if (nvram_match("wl_wdsapply_x", "1"))
	{
		num = atoi(nvram_safe_get("wl_wdsnum_x"));
		list[0]=0;

#ifdef RT2400_SUPPORT
		fp = fopen("/tmp/RT2400AP.dat", "a+");
		if (fp) fprintf(fp, "WdsList=");
#endif

		for(i=0;i<num;i++)
		{
			sprintf(list, "%s %s", list, mac_conv("wl_wdslist_x", i, macbuf));

#ifdef RT2400_SUPPORT
			if (fp) 
			{
				fprintf(fp, "%s;", mac_conv("wl_wdslist_x", i, macbuf));
			}
#endif
		}

#ifdef RT2400_SUPPORT
		fprintf(fp, "\n");
		fclose(fp);
#endif
		dprintf("wds list %s %x\n", list, num);

		nvram_set("wl0_wds", list);
	}
	else nvram_set("wl0_wds", "");

	/* Mac filter */
	nvram_set("wl0_macmode", nvram_safe_get("wl_macmode"));

	if (strcmp(tmpstr, "disabled"))
	{
		num = atoi(nvram_safe_get("wl_macnum_x"));
		list[0]=0;

#ifdef RT2400_SUPPORT
		fp = fopen("/tmp/RT2400AP.dat", "a+");
		if (fp) fprintf(fp, "AclList=");
#endif

		for(i=0;i<num;i++)
		{
			sprintf(list, "%s %s", list, mac_conv("wl_maclist_x", i, macbuf));
			
#ifdef RT2400_SUPPORT
			if (fp) fprintf(fp, "%s;", mac_conv("wl_maclist_x", i, macbuf));
#endif
		}

#ifdef RT2400_SUPPORT		
		fprintf(fp, "\n");
		fclose(fp);
#endif
		//printf("mac list %s %x\n", list, num);

		nvram_set("wl0_maclist", list);
	}


#ifdef GUEST_ACCOUNT
if(!skipflag)
{
	if (nvram_match("wl_guest_enable", "1"))
	{
		nvram_set("wl0.1_guest","1");
		nvram_set("wl0.1_ifname", "wl0.1");
		nvram_set("wl0.1_mode", "ap");
		nvram_set("wl0.1_radio", "1");
		nvram_set("wl0.1_ipconfig_index","1");
		//nvram_set("unbridged_ifnames", "wl0.1");
		nvram_set("wl0_vifs", "wl0.1");
		nvram_set("wl0.1_ssid", nvram_safe_get("wl_guest_ssid_1"));
		nvram_set("wl0.1_crypto", nvram_safe_get("wl_guest_crypto_1"));
		nvram_set("wl0.1_wpa_psk", nvram_safe_get("wl_guest_wpa_psk_1"));
		nvram_set("wl0.1_key", nvram_safe_get("wl_guest_key_1"));
		nvram_set("wl0.1_key1", nvram_safe_get("wl_guest_key1_1"));
		nvram_set("wl0.1_key2", nvram_safe_get("wl_guest_key2_1"));
		nvram_set("wl0.1_key3", nvram_safe_get("wl_guest_key3_1"));
		nvram_set("wl0.1_key4", nvram_safe_get("wl_guest_key4_1"));

		if (nvram_match("wl_guest_wep_x_1", "0"))
			nvram_set("wl0.1_wep", "disabled");
		else nvram_set("wl0.1_wep", "enabled");

		if (nvram_match("wl_guest_auth_mode_1", "shared"))
			nvram_set("wl0.1_auth", "1");
		else nvram_set("wl0.1_auth", "0");

		if (nvram_match("wl_guest_auth_mode_1", "wpa"))
		{
			nvram_set("wl0.1_akm", "wpa wpa2");
		}
		else if (nvram_match("wl_guest_auth_mode_1", "psk"))
		{
			nvram_set("wl0.1_akm", "psk psk2");
		}
		else
		{
			nvram_set("wl0.1_akm", "");
		}

		nvram_set("wl0.1_auth_mode", nvram_match("wl_guest_auth_mode_1", "radius") ? "radius" : "none");
		nvram_set("wl0.1_gmode", nvram_safe_get("wl0_gmode"));
		nvram_set("wl0.1_gmode_protection", nvram_safe_get("wl0_gmode_protection"));
		nvram_set("wl0.1_rate", nvram_safe_get("wl_rate"));
		nvram_set("wl0.1_rateset", nvram_safe_get("wl_rateset"));
		nvram_set("wl0.1_frag", nvram_safe_get("wl_frag"));
		nvram_set("wl0.1_rts", nvram_safe_get("wl_rts"));
		nvram_set("wl0.1_dtim", nvram_safe_get("wl_dtim"));
		nvram_set("wl0.1_bcn", nvram_safe_get("wl_bcn"));
		nvram_set("wl0.1_plcphdr", nvram_safe_get("wl_plcphdr"));
		nvram_set("wl0.1_closed", nvram_safe_get("wl_closed"));
		nvram_set("wl0.1_frameburst", nvram_safe_get("wl_frameburst"));
		nvram_set("wl0.1_afterburner", nvram_safe_get("wl_afterburner"));
		nvram_set("wl0.1_ap_isolate", nvram_safe_get("wl_ap_isolate"));
		nvram_set("wl0.1_radio", nvram_safe_get("wl_radio_x"));
		nvram_set("wl0.1_preauth", nvram_safe_get("wl_preauth"));
		nvram_set("wl0.1_net_reauth", nvram_safe_get("wl_net_reauth"));
		nvram_set("wl0.1_wme", nvram_safe_get("wl_wme"));
		nvram_set("wl0.1_wme_no_ack", nvram_safe_get("wl_wme_no_ack"));
		nvram_set("wl0.1_wme_sta_bk", nvram_safe_get("wl_wme_sta_bk"));
		nvram_set("wl0.1_wme_sta_be", nvram_safe_get("wl_wme_sta_be"));
		nvram_set("wl0.1_wme_sta_vi", nvram_safe_get("wl_wme_sta_vi"));
		nvram_set("wl0.1_wme_sta_vo", nvram_safe_get("wl_wme_sta_vo"));
		nvram_set("wl0.1_wme_ap_bk", nvram_safe_get("wl_wme_ap_bk"));
		nvram_set("wl0.1_wme_ap_be", nvram_safe_get("wl_wme_ap_be"));
		nvram_set("wl0.1_wme_ap_vi", nvram_safe_get("wl_wme_ap_vi"));
		nvram_set("wl0.1_wme_ap_vo", nvram_safe_get("wl_wme_ap_vo"));

		nvram_set("lan1_ifname", "wl0.1");

		if (nvram_match("dhcp1_enable_x", "1"))
		{
			nvram_set("lan1_proto", "dhcp");
		}
		else
		{
			nvram_set("lan1_proto", "static");
		}
	}
	else
	{
		nvram_unset("wl0.1_ifname");
		nvram_unset("wl0.1_guest");
		nvram_unset("wl0_vifs");
		nvram_unset("lan1_ifname");
	}
}
#endif

	/* Direct copy value */
	/* LAN Section */
	if (nvram_match("dhcp_enable_x", "1"))
		nvram_set("lan_proto", "dhcp");
	else nvram_set("lan_proto", "static");


	nvram_set("wan0_proto", nvram_safe_get("wan_proto"));
	nvram_set("wan0_ipaddr", nvram_safe_get("wan_ipaddr"));
	nvram_set("wan0_gateway", nvram_safe_get("wan_gateway"));

	nvram_set("wan_ipaddr_t", "");
	nvram_set("wan_netmask_t", "");
	nvram_set("wan_gateway_t", "");
	nvram_set("wan_dns_t", "");
	nvram_set("wan_status_t", "Disconnected");

	wan_netmask_check();


	if (nvram_match("wan_proto", "pppoe") || nvram_match("wan_proto", "pptp"))
	{
		nvram_set("wan0_pppoe_ifname", "ppp0");
		nvram_set("upnp_wan_proto", "pppoe");
		nvram_set("wan0_pppoe_username", nvram_safe_get("wan_pppoe_username"));
		nvram_set("wan0_pppoe_passwd", nvram_safe_get("wan_pppoe_passwd"));
		nvram_set("wan0_pppoe_idletime", nvram_safe_get("wan_pppoe_idletime"));
		nvram_set("wan0_pppoe_txonly_x", nvram_safe_get("wan_pppoe_txonly_x"));
		nvram_set("wan0_pppoe_mtu", nvram_safe_get("wan_pppoe_mtu"));
		nvram_set("wan0_pppoe_mru", nvram_safe_get("wan_pppoe_mru"));
		nvram_set("wan0_pppoe_service", nvram_safe_get("wan_pppoe_service"));
		nvram_set("wan0_pppoe_ac", nvram_safe_get("wan_pppoe_ac"));
#ifdef REMOVE
		nvram_set("wan0_pppoe_demand", "1");
		nvram_set("wan0_pppoe_keepalive", "1");
#endif
		nvram_set("wan0_pppoe_ipaddr", nvram_safe_get("wan_ipaddr"));
		nvram_set("wan0_pppoe_netmask", nvram_safe_get("wan_netmask"));
		nvram_set("wan0_pppoe_gateway", nvram_safe_get("wan_gateway"));
	}
	nvram_set("wan0_hostname", nvram_safe_get("wan_hostname"));

	if (nvram_invmatch("wan_hwaddr_x", ""))
	{
		nvram_set("wan_hwaddr", mac_conv("wan_hwaddr_x", -1, macbuf));
		nvram_set("wan0_hwaddr", mac_conv("wan_hwaddr_x", -1, macbuf));
	}

	nvram_set("wan0_dnsenable_x", nvram_safe_get("wan_dnsenable_x"));

	convert_routes();

	memset(servers, 0, sizeof(servers));

	if (nvram_invmatch("ntp_server0", ""))
		sprintf(servers, "%s%s ", servers, nvram_safe_get("ntp_server0"));
	if (nvram_invmatch("ntp_server1", ""))
		sprintf(servers, "%s%s ", servers, nvram_safe_get("ntp_server1"));

	nvram_set("ntp_servers", servers);

	if (nvram_match("wan_nat_x", "0") && nvram_match("wan_route_x", "IP_Bridged"))
	{
		sprintf(ifnames, "%s", nvram_safe_get("lan_ifnames"));
		sprintf(ifnames, "%s %s", ifnames, nvram_safe_get("wan_ifnames"));
		nvram_set("lan_ifnames_t", ifnames);
		nvram_set("router_disable", "1");
		nvram_set("vlan_enable", "0");
	}
#ifdef WIRELESS_WAN
	else if (nvram_invmatch("wl_mode_ex", "ap")) // thanks for Oleg
	{
		char name[80], *next;
		
		char *wl_ifname=nvram_safe_get("wl0_ifname");

		/* remove wl_ifname from the ifnames */
		strcpy(ifnames, nvram_safe_get("wan_ifnames"));
		foreach(name, nvram_safe_get("lan_ifnames"), next) {
			if (strcmp(name, wl_ifname)) {
				sprintf(ifnames, "%s %s", ifnames, name);
			}
		}
		nvram_set("lan_ifnames_t", ifnames);
		nvram_set("router_disable", "0");
		nvram_set("vlan_enable", "1");
	}
#endif
	else 
	{ 
		nvram_set("lan_ifnames_t", nvram_safe_get("lan_ifnames"));
		nvram_set("router_disable", "0");
		nvram_set("vlan_enable", "1");
	}

#ifdef USB_SUPPORT
	// clean some temp variables
	nvram_set("usb_device", "");
	nvram_set("usb_ftp_device", "");
	nvram_set("usb_storage_device", "");
	nvram_set("usb_web_device", "");
	nvram_set("usb_audio_device", "");
	nvram_set("usb_webdriver_x", "");
	nvram_set("usb_web_flag", "");
#endif
	nvram_set("no_br", "0");

	if(nvram_invmatch("sp_battle_ips", "0") && !skipflag)
	{
		eval("insmod", "ip_nat_starcraft.o");
		eval("insmod", "ipt_NETMAP.o");
	}

	update_lan_status(1);

#ifdef NOIPTABLES
	if (nvram_match("misc_http_x", "1"))
	{
		if (nvram_invmatch("misc_httpport_x", ""))
			nvram_set("http_wanport", nvram_safe_get("misc_httpport_x"));
		else nvram_set("http_wanport", "8080");
	}
	else nvram_set("http_wanport", "");

	if (nvram_invmatch("fw_enable_x", "0"))
	{
		nvram_set("fw_disable", "0");
	}
	else
	{
		nvram_set("fw_disable", "1");
	}

	num = 0;	
	if (nvram_match("fw_log_x", "accept") ||
		nvram_match("fw_log_x", "both"))
		num |= 2;
		
	if (nvram_match("fw_log_x", "drop") ||
		nvram_match("fw_log_x", "both"))
		num |= 1;
	
	sprintf(tmpstr, "%d", num);
	nvram_set("log_level", tmpstr);	
#endif

	strcpy(tmpstr, nvram_safe_get("time_zone"));
	/* replace . with : */
	if ((ptr=strchr(tmpstr, '.'))!=NULL) *ptr = ':';
	/* remove *_? */
	if ((ptr=strchr(tmpstr, '_'))!=NULL) *ptr = 0x0;
	nvram_set("time_zone_x", tmpstr);
	dprintf("end map\n");
}

#ifdef CHIP5352
/* This function is used to map nvram value from asus to Broadcom */
void early_convert_asus_values()
{
	if (nvram_match ("wl_radio_power_x", "12"))
	{
		nvram_set("pa0maxpwr", "79");
		//nvram_set("opo","16");
	}
	else if (nvram_invmatch ("wl_radio_power_x", "12"))
        {
                nvram_set("pa0maxpwr", "78");
                //nvram_set("opo","8");
        }
	//else
	//{
        //       nvram_set("pa0maxpwr", "72");
        //       nvram_set("opo","8");
	//}
}	
#endif


char *findpattern(char *target, char *pattern)
{
	char *find;
	int len;

	//printf("find : %s %s\n", target, pattern);

	if ((find=strstr(target, pattern)))
	{
		len = strlen(pattern);
		if (find[len]==';' || find[len]==0)
		{
			return find;
		}
	}
	return NULL;
}

void update_lan_status(int isup)
{
	if (isup)
	{	
		nvram_set("lan_ipaddr_t", nvram_safe_get("lan_ipaddr"));
		nvram_set("lan_netmask_t", nvram_safe_get("lan_netmask"));

		if (nvram_match("wan_route_x", "IP_Routed"))
		{
			if (nvram_match("lan_proto", "dhcp"))
			{
				if (nvram_invmatch("dhcp_gateway_x", ""))
					nvram_set("lan_gateway_t", nvram_safe_get("dhcp_gateway_x"));
				else nvram_set("lan_gateway_t", nvram_safe_get("lan_ipaddr"));
			}
			else nvram_set("lan_gateway_t", nvram_safe_get("lan_ipaddr"));
		}
		else nvram_set("lan_gateway_t", nvram_safe_get("lan_gateway"));
	}
	else
	{
		nvram_set("lan_ipaddr_t", "");
		nvram_set("lan_netmask_t", "");
		nvram_set("lan_gateway_t", "");
	}
}


void update_wan_status(int isup)
{
	char *proto;
	char dns_str[36];

	proto = nvram_safe_get("wan_proto");

//printf("update wan status %d %s\n", isup, proto);

	if (!strcmp(proto, "static")) nvram_set("wan_proto_t", "Static");
	else if (!strcmp(proto, "dhcp")) nvram_set("wan_proto_t", "Automatic IP");
	else if (!strcmp(proto, "pppoe")) nvram_set("wan_proto_t", "PPPoE");	
	else if (!strcmp(proto, "pptp")) nvram_set("wan_proto_t", "PPTP");
	else if (!strcmp(proto, "bigpond")) nvram_set("wan_proto_t", "BigPond");
#ifdef CDMA
	else if (!strcmp(proto, "cdma")) nvram_set("wan_proto_t", "CDMA");
#endif


	if (!isup)
	{
		nvram_set("wan_ipaddr_t", "");
		nvram_set("wan_netmask_t", "");
		nvram_set("wan_gateway_t", "");
		nvram_set("wan_dns_t", "");
		nvram_set("wan_ifname_t", "");
		nvram_set("wan_status_t", "Disconnected");
	}	
	else
	{
		nvram_set("wan_ipaddr_t", nvram_safe_get("wan0_ipaddr"));
		nvram_set("wan_netmask_t", nvram_safe_get("wan0_netmask"));
		nvram_set("wan_gateway_t", nvram_safe_get("wan0_gateway"));


		if (nvram_invmatch("wan_dnsenable_x", "1"))	
		{		
			if (nvram_invmatch("wan_dns1_x",""))
				sprintf(dns_str, "%s", nvram_safe_get("wan_dns1_x"));		
			if (nvram_invmatch("wan_dns2_x",""))
				sprintf(dns_str, " %s", nvram_safe_get("wan_dns2_x"));		
			nvram_set("wan_dns_t", dns_str);
		}
		else nvram_set("wan_dns_t", nvram_safe_get("wan0_dns"));
		nvram_set("wan_status_t", "Connected");
	}
}

/*
 * logmessage
 *
 */
void logmessage(char *logheader, char *fmt, ...)
{
  va_list args;
  char buf[512];

  va_start(args, fmt);

  vsnprintf(buf, sizeof(buf), fmt, args);
  openlog(logheader, 0, 0);
  syslog(0, buf);
  closelog();
  va_end(args);
}


/*
 * wanmessage
 *
 */
void wanmessage(char *fmt, ...)
{
  va_list args;
  char buf[512];

  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  nvram_set("wan_reason_t", buf);
  va_end(args);
}

/* 
 * pppstatus
 * 
 */
char *pppstatus(char *buf)
{
   FILE *fp;
   char sline[128], *p;

   if ((fp=fopen("/tmp/wanstatus.log", "r")) && fgets(sline, sizeof(sline), fp))
   {
	p = strstr(sline, ",");
	strcpy(buf, p+1);
   }
   else
   {
	strcpy(buf, "unknown reason");
   }	
}


/* 
 * Kills process whose PID is stored in plaintext in pidfile
 * @param	pidfile	PID file, signal
 * @return	0 on success and errno on failure
 */
int
kill_pidfile_s(char *pidfile, int sig)
{
	FILE *fp = fopen(pidfile, "r");
	char buf[256];
	extern errno;

	if (fp && fgets(buf, sizeof(buf), fp)) {
		pid_t pid = strtoul(buf, NULL, 0);
		fclose(fp);
		return kill(pid, sig);
  	} else
		return errno;
}

#ifdef RT2400_SUPPORT
void write_rt2400_conf(void)
{
	
	FILE *fp;
	char *tmpstr;

	// create hostapd.conf
	fp=fopen("/tmp/RT2400AP.dat","w");
	if (fp==NULL)
	{
		return;	
	}


	fprintf(fp, "[Default]\n");
	fprintf(fp, "SSID=%s\n", nvram_safe_get("wl_ssid"));
	if (nvram_match("wl_channel", "0")) 
		fprintf(fp, "Channel=6\n");
	else
		fprintf(fp, "Channel=%s\n", nvram_safe_get("wl_channel"));	

	fprintf(fp, "HIDESSID=%s\n", nvram_safe_get("wl_closed"));

	fprintf(fp, "BeaconPeriod=%s\n", nvram_safe_get("wl_bcn")); 	
	fprintf(fp, "RTSThreshold=%s\n", nvram_safe_get("wl_rts")); 		
	fprintf(fp, "FragThreshold=%s\n", nvram_safe_get("wl_frag")); 


	if (nvram_invmatch("wl_wep_x","0"))
	{	
		fprintf(fp, "DefaultKeyID=%s\n", nvram_safe_get("wl_key")); 	

		fprintf(fp, "Key1Type=0\n");		
		fprintf(fp, "Key1Str=%s\n", nvram_safe_get("wl_key1")); 	
		fprintf(fp, "Key2Type=0\n");		
		fprintf(fp, "Key2Str=%s\n", nvram_safe_get("wl_key2")); 	
		fprintf(fp, "Key3Type=0\n");		
		fprintf(fp, "Key3Str=%s\n", nvram_safe_get("wl_key3")); 	
		fprintf(fp, "Key4Type=0\n");		
		fprintf(fp, "Key4Str=%s\n", nvram_safe_get("wl_key4")); 	
	}


	if (nvram_match("wl_auth_mode","shared"))
	{
		fprintf(fp, "AuthMode=shared\n"); 
		fprintf(fp, "EncrypType=wep\n");
	}
	else if (nvram_match("wl_auth_mode","psk"))
	{
		fprintf(fp, "AuthMode=wpapsk\n"); 
		fprintf(fp, "EncrypType=tkip\n");
		fprintf(fp, "WPAPSK=%s\n", nvram_safe_get("wl_wpa_psk"));
		fprintf(fp, "RekeyInterval=%s\n", nvram_safe_get("wl_wpa_gtk_rekey"));
		fprintf(fp, "RekeyMethod=time\n");
	}
	else 
	{
		fprintf(fp, "AuthMode=open\n"); 
		if (nvram_invmatch("wl_wep_x","0"))	
			fprintf(fp, "EncrypType=wep\n");
		else fprintf(fp, "EncrypType=none\n");
	}

	if (nvram_match("wl_macmode", "allow"))
	{	
		fprintf(fp, "AclEnable=1\n");
	}
	else if (nvram_match("wl_macmode", "deny"))
	{	
		fprintf(fp, "AclEnable=2\n");
	}
	else fprintf(fp, "AclEnable=0\n");

	if (nvram_match("wl_mode_x", "0"))
		fprintf(fp, "WdsEnable=0\n");
	else fprintf(fp, "WdsEnable=1\n");
	fclose(fp);
}
#endif