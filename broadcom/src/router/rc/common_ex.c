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
 * $Id: common_ex.c,v 1.1.1.1 2007/01/25 12:52:21 jiahao_jhou Exp $
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

	FILE *logofp;
	FILE *settingfp;
	unsigned long *logolen;
	unsigned long *settinglen;
	unsigned long logolen_bak=0;
	char odmPtr[20];
	char odm_productid[13];
	char *logobuf=NULL;
	int size=4096;
	unsigned long count=0;

	strcpy(productid, "WLHDD");
	strcpy(fwver, "0.1.0.1");
	nvram_set("productid_odm", "");

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
//		fclose(fp);

		if (fseek(fp, *imagelen, SEEK_SET)!=0)
			goto read_logo_end;

		memset(odmPtr, 0, sizeof(odmPtr));
		if (!fread(odmPtr, 1, 20, fp))
			goto read_logo_end;

		if (odmPtr[0]==0)
		{
			goto read_logo_end;
		}

		strncpy(odm_productid, odmPtr, 12);
		odm_productid[12] = 0;
		nvram_set("productid_odm", trim_r(odm_productid));
		logolen = (unsigned long *) (odmPtr+12);
		settinglen = (unsigned long *) (odmPtr+16);
		logolen_bak=*logolen;

		if ((logobuf=malloc(size))==NULL)
			goto read_logo_end;

		if ((logofp = fopen("/tmp/model.jpg", "wb"))!=NULL)
		{
			while (logolen && *logolen) {
				count = (unsigned long)fread(logobuf, 1, size, fp);
				if (!count)
					break;

				if (count<=*logolen)
					*logolen-=count;
				else
				{
					count=*logolen;
					*logolen=0;
				}
				fwrite(logobuf, 1, count, logofp);
			}
			fclose(logofp);
		}

		if (fseek(fp, *imagelen + 20 + logolen_bak, SEEK_SET)!=0)
			goto read_logo_end;

		if ((settingfp = fopen("/tmp/odm_setting", "wb"))!=NULL)
		{
			while (settinglen && *settinglen) {
				count = (unsigned long)fread(logobuf, 1, size, fp);
				if (!count)
					break;

				if (count<=*settinglen)
					*settinglen-=count;
				else
				{
					count=*settinglen;
					*settinglen=0;
				}
				fwrite(logobuf, 1, count, settingfp);
			}
			fclose(settingfp);
		}

		fclose(fp);
		fp=NULL;
		free(logobuf);
	}
write_ver:
read_logo_end:
	if (fp)
		fclose(fp);

	// its a ugle solution for Product ID
	if (nvram_match("productid_odm", ""))
	{
		if (strstr(productid, "WL500gx"))
			nvram_set("productid", "WL500g.Deluxe");
		else
			nvram_set("productid", trim_r(productid));
	}
	else
		nvram_set("productid", nvram_get("productid_odm"));

	nvram_set("productid_own", trim_r(productid));
		

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
			nvram_set("wan0_ipaddr", nvram_safe_get("wan_ipaddr"));
			nvram_set("wan0_netmask", nvram_safe_get("wan_netmask"));
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

char *nvram_get_i3(char *name, int idx)
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

	return(nvram_safe_get(tmpstr));
}

int nvram_match_i2(char *name, int idx, char *match) {
	const char *value = nvram_get_i3(name, idx);
	return (value && !strcmp(value, match));
}

void char_to_ascii(char *output, char *input)/* Transfer Char to ASCII */
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

/* This function is used to map nvram value from asus to Broadcom */
void convert_asus_values(int skipflag)
{	
	char tmpstr[32], tmpstr1[32], macbuf[36];
	char servers[64];
	char ifnames[36];
	char sbuf[64];
	int i, num;
	char ssidstr[128];
	int mssid_num;
	char *ptr;
	FILE *fp;

	if(!skipflag)
	{
//	getsyspara();
	/* convert country code from regulation_domain */
	convert_country();
	}

#ifndef CDMA
	if(nvram_match("wan_proto", "cdma"))
		nvram_set("wan_proto", "dhcp");
#endif

#ifndef WL500GP
/*
	early_convert_asus_values();
	int radio=atoi(nvram_safe_get("wl_radio_power_x"));
	if (radio>=13)
		nvram_set("wl_radio_power_x", "9");
*/
#endif

#ifdef WL500GP
#if 0
//	if(nvram_get("preferred_lang")==NULL){
	if(nvram_match("preferred_lang", "")) {
		if ((nvram_match("regulation_domain","0x47TW"))||(nvram_match("regulation_domain","0X47TW")))
			nvram_set("preferred_lang","TW");
		else if ((nvram_match("regulation_domain","0x11HK"))||(nvram_match("regulation_domain","0X11HK")))
			nvram_set("preferred_lang","TW");
//		else if ((nvram_match("regulation_domain","0x45KR"))||(nvram_match("regulation_domain","0X45KR")))
//			nvram_set("preferred_lang","KR");
		else if ((nvram_match("regulation_domain","0x44CN"))||(nvram_match("regulation_domain","0X44CN")))
			nvram_set("preferred_lang","CN");
		else if ((nvram_match("regulation_domain","0x30DE"))||(nvram_match("regulation_domain","0X30DE")))
			nvram_set("preferred_lang","DE");

		else if ((nvram_match("regulation_domain","0x30GB"))||(nvram_match("regulation_domain","0X30GB")))
			nvram_set("preferred_lang","EN");
		else
			nvram_set("preferred_lang","EN");
	}
#endif
	if(nvram_invmatch("temp_lang", ""))
		nvram_set("preferred_lang", nvram_get("temp_lang"));

#endif

#ifdef WL500GP
	if(nvram_invmatch("httpd_die_reboot", ""))
		nvram_set("httpd_die_reboot", "");

//	nvram_set("opo","10");
//	eval("wl", "txpwr1", "-1");
/*
	int radio = atoi(nvram_safe_get("wl_radio_power_x"));
	if (radio < 16)
		eval("wl", "txpwr1", "-o", "-d", nvram_safe_get("wl_radio_power_x"));
	else
		eval("wl", "txpwr1", "-1");
*/

	/* Clean MFG test values when boot */
/*
	if (nvram_invmatch("asus_mfg", ""))
		nvram_set("asus_mfg", "");
	if (nvram_invmatch("asus_mfg_webcam", ""))
		nvram_set("asus_mfg_webcam", "");
	if (nvram_invmatch("asus_mfg_flash", ""))
		nvram_set("asus_mfg_flash", "");
	if (nvram_invmatch("btn_rst", ""))
		nvram_set("btn_rst", "");
	if (nvram_invmatch("btn_ez", ""))
		nvram_set("btn_ez", "");
*/

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

	if (nvram_match("wl_wep_x", "0") || nvram_match("wl_auth_mode", "psk"))
		nvram_set("wl0_wep", "disabled");
	else nvram_set("wl0_wep", "enabled");

	if (nvram_match("wl_auth_mode", "shared"))
		nvram_set("wl0_auth", "1");
	else nvram_set("wl0_auth", "0");


#ifdef WPA2_WMM	
	if (nvram_match("wl_auth_mode", "wpa"))
	{
		if(nvram_match("wl_mode_ex", "ap"))
		{
			nvram_set("wl_akm", "wpa");
			nvram_set("wl0_akm", "wpa");
		}
	}
	else if (nvram_match("wl_auth_mode", "wpa2"))
	{
		if(nvram_match("wl_mode_ex", "ap"))
		{
			nvram_set("wl_akm", "wpa2");
			nvram_set("wl0_akm", "wpa2");
		}
	}
	else if (nvram_match("wl_auth_mode", "psk"))
	{
		if(nvram_match("wl_mode_ex", "ap"))
		{
			if(nvram_match("wl_wpa_mode", "0"))
			{
				if (nvram_match("wl_crypto", "tkip"))
				{
					nvram_set("wl_akm", "psk");
					nvram_set("wl0_akm", "psk");
				}
				else if (nvram_match("wl_crypto", "aes"))
				{
					nvram_set("wl_akm", "psk2");
					nvram_set("wl0_akm", "psk2");
				}
				else
				{
					nvram_set("wl_akm", "psk psk2");
					nvram_set("wl0_akm", "psk psk2");
				}
			}
			else if(nvram_match("wl_wpa_mode", "1"))
			{
				nvram_set("wl_akm", "psk");
				nvram_set("wl0_akm", "psk");
			}
			else if(nvram_match("wl_wpa_mode", "2"))
			{
				nvram_set("wl_akm", "psk2");
				nvram_set("wl0_akm", "psk2");
			}
		}
		else
		{
			if(nvram_match("wl_wpa_mode", "1"))
			{
				nvram_set("wl_akm", "psk");
				nvram_set("wl0_akm", "psk");
			}
			else if(nvram_match("wl_wpa_mode", "2"))
			{
				nvram_set("wl_akm", "psk2");
				nvram_set("wl0_akm", "psk2");
			}
/*
			if (nvram_match("wl_crypto", "tkip"))
			{
				nvram_set("wl_akm", "psk");
				nvram_set("wl0_akm", "psk");
			}
			else
			{
				nvram_set("wl_akm", "psk2");
				nvram_set("wl0_akm", "psk2");
			}
*/
		}
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
	nvram_set("wl0_wme_apsd", nvram_safe_get("wl_wme_apsd"));
#else
	nvram_set("wl0_auth_mode", nvram_safe_get("wl_auth_mode"));
	nvram_set("wl_akm", "");
	nvram_set("wl0_akm", "");
	nvram_set("wl0_wme", "off");
#endif

	nvram_set("wl0_ssid", nvram_safe_get("wl_ssid"));
   	memset(ssidstr, 0, sizeof(ssidstr));
   	char_to_ascii(ssidstr, nvram_safe_get("wl_ssid"));
	nvram_set("wl0_ssid2", ssidstr);
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

	memset(ssidstr, 0, sizeof(ssidstr));
	char_to_ascii(ssidstr, nvram_safe_get("wl_ssid"));
	nvram_set("wl_ssid2", ssidstr);

	memset(ssidstr, 0, sizeof(ssidstr));
	char_to_ascii(ssidstr, nvram_safe_get("wl_guest_ssid_1"));
	nvram_set("wl_guest_ssid_2", ssidstr);

	memset(ssidstr, 0, sizeof(ssidstr));
	char_to_ascii(ssidstr, nvram_safe_get("mssid_ssid_x0"));
	nvram_set("mssid_ssid_2_x0", ssidstr);

	memset(ssidstr, 0, sizeof(ssidstr));
	char_to_ascii(ssidstr, nvram_safe_get("mssid_ssid_x1"));
	nvram_set("mssid_ssid_2_x1", ssidstr);

	memset(ssidstr, 0, sizeof(ssidstr));
	char_to_ascii(ssidstr, nvram_safe_get("mssid_ssid_x2"));
	nvram_set("mssid_ssid_2_x2", ssidstr);

	memset(ssidstr, 0, sizeof(ssidstr));
	char_to_ascii(ssidstr, nvram_safe_get("mssid_ssid_x3"));
	nvram_set("mssid_ssid_2_x3", ssidstr);

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
		//nvram_set("wl0_mode", nvram_safe_get("wl_mode_ex"));
#ifdef RANGE_EXTENDER
		if(nvram_match("wl_mode_ex", "re"))
		{
			if(nvram_match("wan_nat_x", "1"))
			{
				nvram_set("wl_mode", "sta"); // use wet mode
				nvram_set("wl0_mode", "sta"); // use wet mode
			}
			else
			{
				nvram_set("wl_mode", "wet"); // use wet mode
				nvram_set("wl0_mode", "wet"); // use wet mode
			}
		}
		else
#endif
		{
			nvram_set("wl_mode", "wet"); // use wet mode
			nvram_set("wl0_mode", "wet"); // use wet mode
		}

		nvram_set("wl0_lazywds", "0");
		nvram_set("wl0_wdsapply_x", "0");
	}
	else
	{
		/* WDS control */
		if (nvram_match("wl_mode_x", "1")) 
			nvram_set("wl0_mode", "wds");
		else
			nvram_set("wl0_mode", "ap");

		if (nvram_invmatch("wl_mode_x", "0"))
		{
			nvram_set("wl0_lazywds", nvram_safe_get("wl_lazywds"));
			nvram_set("wl0_wdsapply_x", nvram_safe_get("wl_wdsapply_x"));
		}
		else
		{
			nvram_set("wl0_lazywds", "0");
			nvram_set("wl0_wdsapply_x", "0");
		}
	}

//	if (nvram_match("wl_wdsapply_x", "1") && nvram_match("wl_mode_ex", "ap"))
	if (	nvram_match("wl_wdsapply_x", "1") &&
		nvram_match("wl_mode_ex", "ap") &&
		nvram_invmatch("wl_mode_x", "0")
	)
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

	if (nvram_invmatch("wl_macmode", "disabled"))
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

#ifdef RANGE_EXTENDER
	if(nvram_match("wl_mode_ex", "re"))
	{
		nvram_set("wl_ure", "1");
		nvram_set("wl0_ure", "1");
		nvram_set("ure_disable", "0");
		
		nvram_set("wl_guest_enable", "1");
	}
	else
	{
		nvram_set("wl_ure", "0");
		nvram_set("wl0_ure", "0");
		nvram_set("ure_disable", "1");
	}
#endif

#ifdef GUEST_ACCOUNT
if(!skipflag)
{
	if (nvram_match("mssid_enable", "1") && nvram_match("wan_nat_x", "0") && nvram_match("wl_mode_ex", "ap") && nvram_invmatch("wl_mode_x", "1"))
	{
		nvram_set("wl_guest_enable", "1");
		nvram_set("wl0_mssid_extended", "off");

		char tmpstr1[64];
		char tmpstr2[64];
		char tmpstr3[64];
		memset(tmpstr1, 0, sizeof(tmpstr1));
		memset(tmpstr2, 0, sizeof(tmpstr2));
		memset(tmpstr3, 0, sizeof(tmpstr3));
		int mssid_num=0;
		for(i=1;i<4;i++)
		{
			if(strcmp(nvram_get_i3("mssid_enable_x", i), "1")==0)
			{
				mssid_num++;
				if (strlen(tmpstr2)==0)
					sprintf(tmpstr1, "wl0.%d", i);
				else
					sprintf(tmpstr1, "%s wl0.%d", tmpstr2, i);
				strcpy(tmpstr2, tmpstr1);

				sprintf(tmpstr1, "wl0.%d_guest", i);
//				nvram_set(tmpstr1, "1");
				nvram_unset(tmpstr1);
				sprintf(tmpstr1, "wl0.%d_radio", i);
				nvram_set(tmpstr1, "1");
				sprintf(tmpstr1, "wl0.%d_mode", i);
				nvram_set(tmpstr1, "ap");
				
				sprintf(tmpstr1, "wl0.%d_ipconfig_index", i);
				sprintf(tmpstr3, "%d", mssid_num);
				nvram_set(tmpstr1, tmpstr3);
				
				sprintf(tmpstr1, "wl0.%d_ifname", i);
				sprintf(tmpstr3, "wl0.%d", i);
				nvram_set(tmpstr1, tmpstr3);


				sprintf(tmpstr1, "lan%d_ifname", i);
//				nvram_set(tmpstr1, tmpstr3);
 				nvram_unset(tmpstr1);
				sprintf(tmpstr1, "lan%d_ipaddr", i);
				nvram_unset(tmpstr1);
				sprintf(tmpstr1, "lan%d_netmask", i);
				nvram_unset(tmpstr1);
				sprintf(tmpstr1, "lan%d_dhcp", i);
//				nvram_set(tmpstr1, "0");
				nvram_unset(tmpstr1);
				
				sprintf(tmpstr1, "wl0.%d_ssid", i);
				nvram_set(tmpstr1, nvram_get_i3("mssid_ssid_x", i));
				sprintf(tmpstr1, "wl0.%d_crypto", i);
				nvram_set(tmpstr1, nvram_get_i3("mssid_crypto_x", i));
				sprintf(tmpstr1, "wl0.%d_wpa_psk", i);
				nvram_set(tmpstr1, nvram_get_i3("mssid_wpa_psk_x", i));
				sprintf(tmpstr1, "wl0.%d_key", i);
				nvram_set(tmpstr1, nvram_get_i3("mssid_key_x", i));
				sprintf(tmpstr1, "wl0.%d_key1", i);
				nvram_set(tmpstr1, nvram_get_i3("mssid_key1_x", i));
				sprintf(tmpstr1, "wl0.%d_key2", i);
				nvram_set(tmpstr1, nvram_get_i3("mssid_key2_x", i));
				sprintf(tmpstr1, "wl0.%d_key3", i);
				nvram_set(tmpstr1, nvram_get_i3("mssid_key3_x", i));
				sprintf(tmpstr1, "wl0.%d_key4", i);
				nvram_set(tmpstr1, nvram_get_i3("mssid_key4_x", i));

				sprintf(tmpstr1, "wl0.%d_wep", i);
				if (nvram_match_i2("mssid_wep_x", i, "0"))
					nvram_set(tmpstr1, "disabled");
				else
					nvram_set(tmpstr1, "enabled");
				sprintf(tmpstr1, "wl0.%d_auth", i);
				if (nvram_match_i2("mssid_auth_mode_x", i, "shared"))
					nvram_set(tmpstr1, "1");
				else
					nvram_set(tmpstr1, "0");
				sprintf(tmpstr1, "wl0.%d_akm", i);
				if (nvram_match_i2("mssid_auth_mode_x", i, "psk"))
				{
//					nvram_set(tmpstr1, "psk psk2");
					if (nvram_match_i2("mssid_crypto_x", i, "tkip"))
					{
						nvram_set(tmpstr1, "psk");
					}
					else if (nvram_match_i2("mssid_crypto_x", i, "aes"))
					{
						nvram_set(tmpstr1, "psk2");
					}
					else
					{
						nvram_set(tmpstr1, "psk psk2");
					}
				}
				else
					nvram_set(tmpstr1, "");

				if (i==1)
				{
					nvram_set("wl0.1_auth_mode", nvram_match_i2("mssid_auth_mode_x", i, "radius") ? "radius" : "none");
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
				}
				else if (i==2)
				{
					nvram_set("wl0.2_auth_mode", nvram_match_i2("mssid_auth_mode_x", i, "radius") ? "radius" : "none");
					nvram_set("wl0.2_gmode", nvram_safe_get("wl0_gmode"));
					nvram_set("wl0.2_gmode_protection", nvram_safe_get("wl0_gmode_protection"));
					nvram_set("wl0.2_rate", nvram_safe_get("wl_rate"));
					nvram_set("wl0.2_rateset", nvram_safe_get("wl_rateset"));
					nvram_set("wl0.2_frag", nvram_safe_get("wl_frag"));
					nvram_set("wl0.2_rts", nvram_safe_get("wl_rts"));
					nvram_set("wl0.2_dtim", nvram_safe_get("wl_dtim"));
					nvram_set("wl0.2_bcn", nvram_safe_get("wl_bcn"));
					nvram_set("wl0.2_plcphdr", nvram_safe_get("wl_plcphdr"));
					nvram_set("wl0.2_closed", nvram_safe_get("wl_closed"));
					nvram_set("wl0.2_frameburst", nvram_safe_get("wl_frameburst"));
					nvram_set("wl0.2_afterburner", nvram_safe_get("wl_afterburner"));
					nvram_set("wl0.2_ap_isolate", nvram_safe_get("wl_ap_isolate"));
					nvram_set("wl0.2_radio", nvram_safe_get("wl_radio_x"));
					nvram_set("wl0.2_preauth", nvram_safe_get("wl_preauth"));
					nvram_set("wl0.2_net_reauth", nvram_safe_get("wl_net_reauth"));
					nvram_set("wl0.2_wme", nvram_safe_get("wl_wme"));
					nvram_set("wl0.2_wme_no_ack", nvram_safe_get("wl_wme_no_ack"));
					nvram_set("wl0.2_wme_sta_bk", nvram_safe_get("wl_wme_sta_bk"));
					nvram_set("wl0.2_wme_sta_be", nvram_safe_get("wl_wme_sta_be"));
					nvram_set("wl0.2_wme_sta_vi", nvram_safe_get("wl_wme_sta_vi"));
					nvram_set("wl0.2_wme_sta_vo", nvram_safe_get("wl_wme_sta_vo"));
					nvram_set("wl0.2_wme_ap_bk", nvram_safe_get("wl_wme_ap_bk"));
					nvram_set("wl0.2_wme_ap_be", nvram_safe_get("wl_wme_ap_be"));
					nvram_set("wl0.2_wme_ap_vi", nvram_safe_get("wl_wme_ap_vi"));
					nvram_set("wl0.1_wme_ap_vo", nvram_safe_get("wl_wme_ap_vo"));
				}
				else if (i==3)
				{
					nvram_set("wl0.3_auth_mode", nvram_match_i2("mssid_auth_mode_x", i, "radius") ? "radius" : "none");
					nvram_set("wl0.3_gmode", nvram_safe_get("wl0_gmode"));
					nvram_set("wl0.3_gmode_protection", nvram_safe_get("wl0_gmode_protection"));
					nvram_set("wl0.3_rate", nvram_safe_get("wl_rate"));
					nvram_set("wl0.3_rateset", nvram_safe_get("wl_rateset"));
					nvram_set("wl0.3_frag", nvram_safe_get("wl_frag"));
					nvram_set("wl0.3_rts", nvram_safe_get("wl_rts"));
					nvram_set("wl0.3_dtim", nvram_safe_get("wl_dtim"));
					nvram_set("wl0.3_bcn", nvram_safe_get("wl_bcn"));
					nvram_set("wl0.3_plcphdr", nvram_safe_get("wl_plcphdr"));
					nvram_set("wl0.3_closed", nvram_safe_get("wl_closed"));
					nvram_set("wl0.3_frameburst", nvram_safe_get("wl_frameburst"));
					nvram_set("wl0.3_afterburner", nvram_safe_get("wl_afterburner"));
					nvram_set("wl0.3_ap_isolate", nvram_safe_get("wl_ap_isolate"));
					nvram_set("wl0.3_radio", nvram_safe_get("wl_radio_x"));
					nvram_set("wl0.3_preauth", nvram_safe_get("wl_preauth"));
					nvram_set("wl0.3_net_reauth", nvram_safe_get("wl_net_reauth"));
					nvram_set("wl0.3_wme", nvram_safe_get("wl_wme"));
					nvram_set("wl0.3_wme_no_ack", nvram_safe_get("wl_wme_no_ack"));
					nvram_set("wl0.3_wme_sta_bk", nvram_safe_get("wl_wme_sta_bk"));
					nvram_set("wl0.3_wme_sta_be", nvram_safe_get("wl_wme_sta_be"));
					nvram_set("wl0.3_wme_sta_vi", nvram_safe_get("wl_wme_sta_vi"));
					nvram_set("wl0.3_wme_sta_vo", nvram_safe_get("wl_wme_sta_vo"));
					nvram_set("wl0.3_wme_ap_bk", nvram_safe_get("wl_wme_ap_bk"));
					nvram_set("wl0.3_wme_ap_be", nvram_safe_get("wl_wme_ap_be"));
					nvram_set("wl0.3_wme_ap_vi", nvram_safe_get("wl_wme_ap_vi"));
					nvram_set("wl0.3_wme_ap_vo", nvram_safe_get("wl_wme_ap_vo"));
				}
			}
		}
		nvram_set("wl0_vifs", tmpstr2);
	}
	else if (
			(
			(nvram_match("wan_nat_x", "1") && nvram_match("wl_mode_ex", "ap") && nvram_match("wl_guest_enable", "1")) ||
			(nvram_match("wan_nat_x", "1") && nvram_match("wl_mode_ex", "re")) ||
			(nvram_match("wan_nat_x", "0") && nvram_match("wl_mode_ex", "re"))
			) && nvram_invmatch("wl_mode_x", "1")
		)
	{
		if (nvram_match("wl_mode_ex", "re"))
//			 && nvram_match("wan_nat_x", "1"))
		{
			nvram_unset("wl0.1_guest");
			nvram_set("wl0_mssid_extended", "off");
		}
		else
		{
			nvram_set("wl0.1_guest","1");
			nvram_set("wl0_mssid_extended", "on");
		}
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
		nvram_set("wl0.1_wpa_gtk_rekey", nvram_safe_get("wl_guest_wpa_gtk_rekey_1"));

		if (nvram_match("wl_guest_wep_x_1", "0"))
			nvram_set("wl0.1_wep", "disabled");
		else nvram_set("wl0.1_wep", "enabled");

		if (nvram_match("wl_guest_auth_mode_1", "shared"))
			nvram_set("wl0.1_auth", "1");
		else nvram_set("wl0.1_auth", "0");

		if (nvram_match("wl_guest_auth_mode_1", "wpa") && nvram_match("wl_mode_ex", "ap"))
		{
//			nvram_set("wl0.1_akm", "wpa wpa2");
			nvram_set("wl0.1_akm", "");
		}
		else if (nvram_match("wl_guest_auth_mode_1", "psk"))
		{
			if(nvram_match("wl_guest_wpa_mode", "0"))
			{
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
			}
			else if(nvram_match("wl_guest_wpa_mode", "1"))
			{
				nvram_set("wl0.1_akm", "psk");
			}
			else if(nvram_match("wl_guest_wpa_mode", "2"))
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
		
		if (nvram_match("wl_mode_ex", "re") && nvram_match("wl_akm", "wpa wpa2"))
		{
			nvram_set("wl_akm", "");
			nvram_set("wl0_akm", "");
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

		if (nvram_match("wl_mode_ex", "re") && nvram_match("wan_nat_x", "0"))
			nvram_unset("lan1_ifname");
		else
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
		nvram_set("mssid_enable", "0");
		nvram_set("wl_guest_enable", "0");
		nvram_set("wl0_mssid_extended", "off");
		
		nvram_unset("wl0_vifs");

		nvram_unset("wl0.1_guest");
		nvram_unset("wl0.1_radio");
		nvram_unset("wl0.1_mode");
		nvram_unset("wl0.1_ifname");
		
		nvram_unset("wl0.2_guest");
		nvram_unset("wl0.2_radio");
		nvram_unset("wl0.2_mode");
		nvram_unset("wl0.2_ifname");
		
		nvram_unset("wl0.3_guest");
		nvram_unset("wl0.3_radio");
		nvram_unset("wl0.3_mode");
		nvram_unset("wl0.3_ifname");
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

	if (nvram_invmatch("wan_hwaddr_x", "") && nvram_match("wan_nat_x", "1") && (nvram_match("wl_mode_ex", "ap")||nvram_match("wl_mode_ex", "sta")))
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

/*
	if (nvram_match("wan_nat_x", "0") && nvram_match("wan_nat_x", "0"))
	{
		sprintf(ifnames, "%s", nvram_safe_get("lan_ifnames"));
#ifndef WL500GP
		sprintf(ifnames, "%s %s", ifnames, nvram_safe_get("wan_ifnames"));
#endif
		nvram_set("lan_ifnames_t", ifnames);
		nvram_set("router_disable", "1");
		nvram_set("vlan_enable", "0");
	}
*/
#ifdef WL330GE
	if (nvram_match("wl_mode_ex", "ap") && nvram_match("wan_nat_x", "0") && nvram_match("mssid_enable", "1") && nvram_match("vlans_enable", "1"))
		nvram_set("lan_ifname", "eth0");
	else
		nvram_set("lan_ifname", "br0");

	nvram_set("vlan_enable", "0");

	if (nvram_match("wan_nat_x", "1"))
	{
		if (nvram_match("wl_mode_ex", "sta"))
			nvram_set("lan_ifnames", "eth0");
		else
			nvram_set("lan_ifnames", "eth1");
	}
	else
	{
		if (nvram_match("wl_mode_ex", "ap") && nvram_match("mssid_enable", "1") && nvram_match("vlans_enable", "1"))
			nvram_set("lan_ifnames", "eth1");
		else
			nvram_set("lan_ifnames", "eth0 eth1");
	}

	nvram_set("wan_ifnames", "eth0");	// returning from URE mode
	nvram_set("wan0_ifname", "eth0");	//
#endif
#ifdef RANGE_EXTENDER
	if (nvram_match("wl_mode_ex", "re"))	// URE mode
	{
		if (nvram_match("wan_nat_x", "1"))	// Travel Router mode
		{
			nvram_set("lan_ifnames_t", "eth0 wl0.1");
			nvram_set("wan_ifnames", "eth1");
			nvram_set("wan0_ifname", "eth1");
			nvram_set("router_disable", "0");
		}
		else	// Range Extender mode
		{
			nvram_set("lan_ifnames_t", "eth0 eth1 wl0.1");
			nvram_set("wan_ifnames", "");
			nvram_set("wan0_ifname", "");
			nvram_set("router_disable", "1");
		}
	}
	else
#endif
#ifdef WL330GE
	if (nvram_match("wan_nat_x", "0") && nvram_match("wan_nat_x", "0"))	// AP or STA mode
	{
		nvram_set("wan_ifnames", "");
		nvram_set("wan0_ifname", "");
		if (nvram_match("wl_mode_ex", "ap") && nvram_match("mssid_enable", "1"))
		{
			if (nvram_match("vlans_enable", "1"))
				nvram_set("lan_ifnames_t", "eth1");
			else
				nvram_set("lan_ifnames_t", "eth0 eth1");
		}
		else
		{
			sprintf(ifnames, "%s", nvram_safe_get("lan_ifnames"));
			sprintf(ifnames, "%s %s", ifnames, nvram_safe_get("wan_ifnames"));
			nvram_set("lan_ifnames_t", ifnames);
		}
		nvram_set("router_disable", "1");
	}
#endif
#ifdef WIRELESS_WAN
	else if (nvram_invmatch("wl_mode_ex", "ap")) // thanks for Oleg
	{
#ifdef WL330GE
		nvram_set("wan_ifnames", "eth1");
		nvram_set("wan0_ifname", "eth1");
		nvram_set("lan_ifnames_t", "eth0");
#endif
/*
		char name[80], *next;
		
		char *wl_ifname=nvram_safe_get("wl0_ifname");

		// remove wl_ifname from the ifnames
		strcpy(ifnames, nvram_safe_get("wan_ifnames"));
		foreach(name, nvram_safe_get("lan_ifnames"), next) {
			if (strcmp(name, wl_ifname)) {
				sprintf(ifnames, "%s %s", ifnames, name);
			}
		}
		nvram_set("lan_ifnames_t", ifnames);
*/
		nvram_set("router_disable", "0");
//		nvram_set("vlan_enable", "1");	// marked for WL330gE
	}
#endif
	else 
	{ 
		nvram_set("lan_ifnames_t", nvram_safe_get("lan_ifnames"));
		nvram_set("router_disable", "0");
//		nvram_set("vlan_enable", "1");	// marked for WL330gE
	}

#ifdef USB_SUPPORT
	// clean some temp variables
	if (!skipflag)
	{
	nvram_set("swap_on", "0");
	nvram_set("usb_device", "");
//	nvram_set("usb_ftp_device", ""); 	// marked by Jiahao for WL500gP
	nvram_set("usb_storage_device", "");
	nvram_set("usb_web_device", "");	// following lines are added by Jiahao for WL500gP
	nvram_set("usb_audio_device", "");
	nvram_set("usb_webdriver_x", "");
	nvram_set("usb_web_flag", "");
	nvram_set("usb_disc0_path0", "");
	nvram_set("usb_disc0_path1", "");
	nvram_set("usb_disc0_path2", "");
	nvram_set("usb_disc0_path3", "");
	nvram_set("usb_disc0_path4", "");
	nvram_set("usb_disc0_path5", "");
	nvram_set("usb_disc0_path6", "");
	nvram_set("usb_disc1_path0", "");
	nvram_set("usb_disc1_path1", "");
	nvram_set("usb_disc1_path2", "");
	nvram_set("usb_disc1_path3", "");
	nvram_set("usb_disc1_path4", "");
	nvram_set("usb_disc1_path5", "");
	nvram_set("usb_disc1_path6", "");
	nvram_set("usb_disc0_fs_path0", "");
	nvram_set("usb_disc0_fs_path1", "");
	nvram_set("usb_disc0_fs_path2", "");
	nvram_set("usb_disc0_fs_path3", "");
	nvram_set("usb_disc0_fs_path4", "");
	nvram_set("usb_disc0_fs_path5", "");
	nvram_set("usb_disc0_fs_path6", "");
	nvram_set("usb_disc1_fs_path0", "");
	nvram_set("usb_disc1_fs_path1", "");
	nvram_set("usb_disc1_fs_path2", "");
	nvram_set("usb_disc1_fs_path3", "");
	nvram_set("usb_disc1_fs_path4", "");
	nvram_set("usb_disc1_fs_path5", "");
	nvram_set("usb_disc1_fs_path6", "");
	nvram_set("usb_disc0_index", "0");
	nvram_set("usb_disc1_index", "0");
	nvram_set("usb_disc0_port", "0");
	nvram_set("usb_disc1_port", "0");
	nvram_set("usb_disc0_dev", "");
	nvram_set("usb_disc1_dev", "");
#ifdef DLM
	nvram_set("ftp_running", "0");
	nvram_set("samba_running", "0");
	nvram_set("app_running", "0");
	nvram_set("eject_from_web", "0");
	nvram_set("st_ftp_modex", nvram_get("st_ftp_mode"));
	nvram_set("st_samba_modex", nvram_get("st_samba_mode"));
	nvram_set("st_samba_mode_x", "-1");
	nvram_set("apps_dlx", nvram_get("apps_dl"));
	nvram_set("apps_dl_x", "-1");
	nvram_set("apps_dmsx", nvram_get("apps_dms"));
	nvram_set("apps_dms_usb_port_x", "-1");
	nvram_set("apps_dms_usb_port_x2", "-1");
	nvram_set("apps_status_checked", "0");
	nvram_set("usb_storage_busy", "0");
	nvram_set("usb_storage_busy2", "0");
	nvram_set("swapoff_failed", "0");
#endif
#ifdef WCN
        nvram_set("wcn_enable_x", nvram_get("wcn_enable"));
#endif
	}
#endif
	nvram_set("no_br", "0");

	if(nvram_match("wan_nat_x", "1"))
	{
	if(nvram_invmatch("sp_battle_ips", "0") && !skipflag)
	{
		eval("insmod", "ip_nat_starcraft.o");
		eval("insmod", "ipt_NETMAP.o");
	}

	//2005/09/22 insmod FTP module
	if (nvram_match("usb_ftpenable_x", "1") && atoi(nvram_get("usb_ftpport_x"))!=21)
	{
		char ports[32];

		sprintf(ports, "ports=21,%d", atoi(nvram_get("usb_ftpport_x")));
		eval("insmod", "ip_conntrack_ftp.o", ports);	
		eval("insmod", "ip_nat_ftp.o", ports);
	}
	else
	{
		eval("insmod", "ip_conntrack_ftp.o");
		eval("insmod", "ip_nat_ftp.o");
	}
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

	// pre mapping
	if(nvram_match("time_zone", "KST-9KDT"))
		nvram_set("time_zone", "UCT-9_1");
	else if(nvram_match("time_zone", "RFT-9RFTDST"))
		nvram_set("time_zone", "UCT-9_2");
	
	strcpy(tmpstr, nvram_safe_get("time_zone"));
	/* replace . with : */
	if ((ptr=strchr(tmpstr, '.'))!=NULL) *ptr = ':';
	/* remove *_? */
	if ((ptr=strchr(tmpstr, '_'))!=NULL) *ptr = 0x0;

	// special mapping
	if(nvram_match("time_zone", "JST"))
		nvram_set("time_zone_x", "UCT-9");
	else if(nvram_match("time_zone", "TST-10TDT"))
		nvram_set("time_zone_x", "UCT-10");
	else if(nvram_match("time_zone", "CST-9:30CDT"))
		nvram_set("time_zone_x", "UCT-9:30");
	else nvram_set("time_zone_x", tmpstr);

#ifdef CDMA
	nvram_set("support_cdma", "1");
	nvram_set("cdma_down", "99");
#else
	nvram_unset("support_cdma");
#endif
	nvram_set("reboot", "");
#ifdef WCN
        nvram_set("reboot_WCN", "");
#endif

	nvram_set("ddns_updated", "0");

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

	nvram_set("restore", "0");

//	if (!nvram_get("wl_nat"))
//		nvram_set("wl_nat", "0");

	nvram_set("ntp_ready", "0");

	dprintf("end map\n");
}

/* This function is used to map nvram value from asus to Broadcom */
void early_convert_asus_values()
{
/*
	if (nvram_match ("wl_radio_power_x", "12"))
	{
		nvram_set("pa0maxpwr", "79");
		nvram_set("opo","16");
	}
	else if (nvram_invmatch ("wl_radio_power_x", "12"))
        {
                nvram_set("pa0maxpwr", "78");
                nvram_set("opo","8");
	}
*/
}	


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

		if (nvram_match("wan_nat_x", "1"))
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


		if (nvram_invmatch("wan_dnsenable_x", "1") || nvram_match("wan0_proto", "static"))
		{
			memset(dns_str, 0, sizeof(dns_str));
			if (nvram_invmatch("wan_dns1_x",""))
				sprintf(dns_str, "%s", nvram_safe_get("wan_dns1_x"));
			if (nvram_invmatch("wan_dns2_x",""))
			{
				if (nvram_invmatch("wan_dns1_x",""))
					sprintf(dns_str, " %s", nvram_safe_get("wan_dns2_x"));
				else
					sprintf(dns_str, "%s", nvram_safe_get("wan_dns2_x"));
			}		
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
