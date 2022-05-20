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
#ifdef BTN_SETUP
// * Man in Middle Attack
//
//             -Attacker<-
//            /           \
//           /             \
//          V               \	     
//         AP  <----x---- Client
//  ==> the same as
//         AP  <--------- Anonymous Client
//  ==> solved by using in very short range
//
// * Key generation(Diffie-Hellman)
//
// AP 				
//   	p: random in 100 predefined primes		
//	q: 5
//	public_ap/private_ap: DH_generate_key(p, q, private)
//
// Client
//	p: random in 100 predfined primses, rand in SSID
//	q: 5
//	public_client/private_client: DH_generate_key(p, q, private)
//
// * Process
// 
// (a1) Press Button for 3 seconds
//
// (a2) Generate Public Key by using:
//      CreatePubPrivKey()
//
// (a3) Change SSID to
//      ASUS_OTSx_zzz_iiii
//      x : setting in default or not
//      zzz : rand seed for primes number
//      iiii : default ip, if no dhcp server is provided
//
//                                  (c1) Survey AP with OTS....
//				    (c2) Generate Public Key by using:
//                                       CreatePubPrivKey()
//				    (c3) Start Session one with PackSetPubKey
//    <-----OTSInit(SetPubKey)---------
// (a4) UnpackSetPubKey
// (a4-1) For other connection, set into log
//
// (a5) PackSetPubKeyRes
//    ------OTSInitAck(SetPubKeyRes)-->
//                                  (c4) UnpackSetPubKeyAck
//				    (c5) Close Session one socket
//
// (a7) CreateSharedKey()	    (c6) CreateSharedKey
// (a8) Set to WPA-PSK w/ 
//      CreateSharedKey
//
//				    (c7) Start Session Two w/ PackSetInfoGWQuick
//    <---- OTSExchange(SetInfoGWQuick)- 
// (a9) UnpackSetInfoGWQuick
// (a9-1) For other connection, set into log
//   
// (a10-1) PackSetInfoGWQuickRes: 
//         Apply Setting with QuickFlag = None
//
//    ----- OTSExchangeRes(SetInfoGWQuickRes) -> Client
//                                  (c8) UnpackSetInfoGWQuickRes
//
// (a10-2) PackSetInfoGWQuickRes
//         Response Setting with QuickFlag = Wireless
//
//    ----- OTSExchangeRes(SetInfoGWQuickRes) -> Client
//                                  (c8) UnpackSetInfoGWQuickRes
//                                  (c9) close sesson two socket
//
// (a11) save setting and reboot
//
// * Timer
// - 120 seconds, button is pressed and no action is performed.
// - 20 seconds, button is pressed and OTSInit is sent
//
// * Functions
// DH *DH_new();
// int CreatePubPrivKey(DH *dh, int rand, char *pub, char *priv);
// int CreateSharedKey(DH *dh, char *pub, char *shared);
// int DH_free(DH *dh);
//
// Fully Support: ASUS cards, WZC
// Alert to WZC(WPA) : Centrino or other cards in XP SP2
// Alert to WZC(WEP) : Centrino or other cards in XP SP1
// Alert to Ethernet : Other cards in 98/ME/.....
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/fcntl.h>
#include <dirent.h>
#include <sys/mount.h>
#include <bcmnvram.h>
#include <netconf.h>
#include <shutils.h>
#include <rc.h>
#include <syslog.h>
#include <iboxcom.h>

#define logs(fmt, arg...) //syslog(LOG_NOTICE, fmt, ##arg)
#include <openssl/dh.h>
#include "crypto.c"
DH *dh;

typedef union {
    struct sockaddr sa;
    struct sockaddr_in sa_in;
} usockaddr;

#define ENCRYPTION 1
#define BTNSETUP_INIT_TIMEOUT 		120	// 3 minute
#define BTNSETUP_EXCHANGE_TIMEOUT 	300    	// 3 minutes
#define MAX_DHKEY_LEN 192
#define WEP64_LEN 10
#define WEP128_LEN 26
#define WPAPSK_LEN 63

PKT_SET_INFO_GW_QUICK pkt;
unsigned char pubkey[MAX_DHKEY_LEN];
unsigned char cpubkey[MAX_DHKEY_LEN];
unsigned char sharedkey[MAX_DHKEY_LEN];
char sharedkeystr[MAX_DHKEY_LEN*2+1];
TEMP_WIRELESS *tw; // Point to sharedkeystr


char ssid[32+1];
int bs_mode;
time_t bs_time;
int bs_timeout;
int bs_auth;
int bs_encrypt;

//#define OTS_LOG 1
//#define OTS_SIMU 1

#ifdef OTS_SIMU
int ots_simu(int stage)
{
	printf("ots check: %d %d\n", stage, atoi(nvram_safe_get("ots_simu_stage")));
	if(stage==atoi(nvram_safe_get("ots_simu_stage")))
	{
		nvram_set("ots_simu_stage", "");
		return 0;
        }
	return 1;
}
#endif

#ifdef OTS_LOG
char ots_buf[1024];

void ots_log(unsigned int flag, int save)
{
	if(flag==0) 
	{
		ots_buf[0]=0;
		nvram_set("ots_log", "0");
	}
	else
	{
		if (save>1)
			sprintf(ots_buf, "%s;[%02x]", nvram_safe_get("ots_log"), flag);
		else
			sprintf(ots_buf, "%s;%02x", nvram_safe_get("ots_log"), flag);
		nvram_set("ots_log", ots_buf);
	}

	if(save==1) nvram_commit();
}
#endif

int is_ots()
{
	char *ptr=nvram_safe_get("sharedkeystr");

	if(strlen(ptr)) return 1;
	else return 0;	
}

#ifdef ENCRYPTION
#define BLOCKLEN 16

void Encrypt(int klen, unsigned char *key, unsigned char *ptext, int tlen, unsigned char *ctext)
{
	unsigned char *pptr, *cptr;
	int i;

	i = 0;
	pptr = ptext;
	cptr = ctext;

	while(1)
	{
		aes_encrypt(klen, key, pptr, cptr);
		i+=16;
		if(i>=tlen) break;		
		pptr+=16;
		cptr+=16;
	}
}

void Decrypt(int klen, unsigned char *key, unsigned char *ptext, int tlen, unsigned char *ctext)
{
	unsigned char *pptr, *cptr;
	int i;

	i = 0;
	pptr = ptext;
	cptr = ctext;

	while(1)
	{
		aes_decrypt(klen, key, pptr, cptr);
		i+=16;
		if(i>=tlen) break;		
		pptr+=16;
		cptr+=16;
	}
}
#endif

// some utility
void nvram_set_ip(char *nvram_name, DWORD ip)
{
	struct in_addr in;

	if (ip!=0)
	{
		in.s_addr = ip;
		nvram_set(nvram_name, (char *)inet_ntoa(in));
	}
	else nvram_set(nvram_name, "");
}

void nvram_set_str(char *nvram_name, char *value, int size)
{
	char tmpbuf[256];

	tmpbuf[size] = 0;
	memcpy(tmpbuf, value, size);
	nvram_set(nvram_name, tmpbuf);
}

int
start_sdhcpd(void)
{
	FILE *fp;
	char *dhcpd_argv[] = {"udhcpd", "/tmp/udhcpd.conf", NULL, NULL};
	char *slease = "/tmp/udhcpd-br0.sleases";
	pid_t pid;

	if (nvram_match("lan_proto", "dhcp")) return 0;
	//ifconfig(nvram_safe_get("lan_ifname"), IFUP,
	//	 nvram_safe_get("lan_ipaddr"), "255.255.255.0");
	//dprintf("%s %s %s %s\n",
	//	nvram_safe_get("lan_ifname"),
	//	nvram_safe_get("dhcp_start"),
	//	nvram_safe_get("dhcp_end"),
	//	nvram_safe_get("lan_lease"));

	if (!(fp = fopen("/tmp/udhcpd-br0.leases", "a"))) {
		perror("/tmp/udhcpd-br0.leases");
		return errno;
	}
	fclose(fp);

	/* Write configuration file based on current information */
	if (!(fp = fopen("/tmp/udhcpd.conf", "w"))) {
		perror("/tmp/udhcpd.conf");
		return errno;
	}
	
	fprintf(fp, "pidfile /var/run/udhcpd-br0.pid\n");
	fprintf(fp, "start %s\n", nvram_safe_get("dhcp_start"));
	fprintf(fp, "end %s\n", nvram_safe_get("dhcp_end"));
	fprintf(fp, "interface %s\n", nvram_safe_get("lan_ifname"));
	fprintf(fp, "remaining yes\n");
	fprintf(fp, "lease_file /tmp/udhcpd-br0.leases\n");
	fprintf(fp, "option subnet %s\n", nvram_safe_get("lan_netmask"));
	fprintf(fp, "option router %s\n", nvram_safe_get("lan_ipaddr"));
	fprintf(fp, "option lease 3600\n");
	fclose(fp);
	
	_eval(dhcpd_argv, NULL, 0, &pid);
	//dprintf("done\n");
	return 0;
}

int btn_setup_get_setting(PKT_SET_INFO_GW_QUICK *pkt)
{
	char tmpbuf[256];
	int ret=0;

	memset(pkt, 0, sizeof(pkt));

	pkt->QuickFlag=QFCAP_WIRELESS;
	strcpy(tmpbuf, nvram_safe_get("wl_ssid"));
	memcpy(pkt->WSetting.SSID, tmpbuf, sizeof(pkt->WSetting.SSID));

	if(nvram_match("wl_auth_mode", "open"))
	{
		pkt->WSetting.Auth=AUTHENTICATION_OPEN;
		pkt->WSetting.Encrypt=atoi(nvram_safe_get("wl_wep_x"));
		if (pkt->WSetting.Encrypt>ENCRYPTION_DISABLE)
		{
			pkt->WSetting.DefaultKey = atoi(nvram_safe_get("wl_key"));
			sprintf(tmpbuf, "wl_key%d", pkt->WSetting.DefaultKey);
			strcpy(pkt->WSetting.Key, nvram_safe_get(tmpbuf));
		}
	}
	else if(nvram_match("wl_auth_mode", "shared"))
	{
		pkt->WSetting.Auth=AUTHENTICATION_SHARED;
		pkt->WSetting.Encrypt=atoi(nvram_safe_get("wl_wep_x"));
		if (pkt->WSetting.Encrypt>ENCRYPTION_DISABLE)
		{
			pkt->WSetting.DefaultKey = atoi(nvram_safe_get("wl_key"));
			sprintf(tmpbuf, "wl_key%d", pkt->WSetting.DefaultKey);
			strcpy(pkt->WSetting.Key, nvram_safe_get(tmpbuf));
		}	
	}
	else if(nvram_match("wl_auth_mode", "psk"))	// add "wl_wpa_mode" matching by Jiahao for WL-520gu
	{
		if(nvram_match("wl_wpa_mode", "1"))
		{
			pkt->WSetting.Auth=AUTHENTICATION_WPA_PSK;
			pkt->WSetting.Encrypt=ENCRYPTION_TKIP;
		}
		else if(nvram_match("wl_wpa_mode", "2"))
		{
			pkt->WSetting.Auth=AUTHENTICATION_WPA_PSK2;
			pkt->WSetting.Encrypt=ENCRYPTION_AES;
		}
		else
		{
			if (nvram_match("wl_crypto", "aes"))
			{
				pkt->WSetting.Auth=AUTHENTICATION_WPA_PSK2;
				pkt->WSetting.Encrypt=ENCRYPTION_AES;
			}
			else
			{
				pkt->WSetting.Auth=AUTHENTICATION_WPA_PSK;
				pkt->WSetting.Encrypt=ENCRYPTION_TKIP;
			}
		}
		strcpy(tmpbuf, nvram_safe_get("wl_wpa_psk"));
		memcpy(pkt->WSetting.Key, tmpbuf, sizeof(pkt->WSetting.Key));
		pkt->WSetting.DefaultKey=DEFAULT_KEY_1;
	}
	else goto fail;	

	ret = 1;
fail:
	return ret;
}

void btn_setup_save_setting(PKT_SET_INFO_GW_QUICK *pkt)
{
	char tmpbuf[256];

	if (pkt->QuickFlag&QFCAP_WIRELESS)
	{
	   //printf("Wireless\n");
	   if (!(pkt->QuickFlag&QFCAP_GET))
	   {
		printf("Set\n");
		// assign automatic generate value
		if (pkt->WSetting.SSID[0]==0)
		{
			strncpy(pkt->WSetting.SSID, tw->u.WirelessStruct.SuggestSSID, sizeof(pkt->WSetting.SSID));
		}
		// assign automatic generate value
		if (pkt->WSetting.Key[0]==0)
		{
			strncpy(pkt->WSetting.Key, tw->u.WirelessStruct.SuggestKey, sizeof(pkt->WSetting.Key));
		}

		if (pkt->WSetting.Encrypt==ENCRYPTION_WEP64)
			pkt->WSetting.Key[WEP64_LEN] = 0;
		else if (pkt->WSetting.Encrypt==ENCRYPTION_WEP128)
			pkt->WSetting.Key[WEP128_LEN] = 0;

		// wireless setting
		// 1. ssid
		nvram_set_str("wl_ssid", pkt->WSetting.SSID, sizeof(pkt->WSetting.SSID));
		memset(tmpbuf, 0, sizeof(tmpbuf));
		char_to_ascii(tmpbuf, pkt->WSetting.SSID);
		nvram_set_str("wl_ssid2", tmpbuf, sizeof(tmpbuf));
	
		if (pkt->WSetting.Auth==AUTHENTICATION_OPEN)
		{
			nvram_set("wl_auth_mode", "open");
			if (pkt->WSetting.Encrypt==ENCRYPTION_WEP64||
			    pkt->WSetting.Encrypt==ENCRYPTION_WEP128)
			{
				sprintf(tmpbuf, "%d", pkt->WSetting.Encrypt);
				nvram_set("wl_wep_x", tmpbuf);

				if(pkt->WSetting.DefaultKey>DEFAULT_KEY_4||
				   pkt->WSetting.DefaultKey<DEFAULT_KEY_1) 
				   pkt->WSetting.DefaultKey=DEFAULT_KEY_1;

				if(pkt->WSetting.DefaultKey==DEFAULT_KEY_1)
					nvram_set_str("wl_key1", pkt->WSetting.Key, sizeof(pkt->WSetting.Key));
				else if(pkt->WSetting.DefaultKey==DEFAULT_KEY_2)
					nvram_set_str("wl_key2", pkt->WSetting.Key, sizeof(pkt->WSetting.Key));
				else if(pkt->WSetting.DefaultKey==DEFAULT_KEY_3)
					nvram_set_str("wl_key3", pkt->WSetting.Key, sizeof(pkt->WSetting.Key));
				else if(pkt->WSetting.DefaultKey==DEFAULT_KEY_4)
					nvram_set_str("wl_key4", pkt->WSetting.Key, sizeof(pkt->WSetting.Key));
				sprintf(tmpbuf,"%d", pkt->WSetting.DefaultKey);
				nvram_set("wl_key", tmpbuf);
			}
			else 
			{
				nvram_set("wl_key", "1");
				nvram_set("wl_key1", "");
				nvram_set("wl_key2", "");
				nvram_set("wl_key3", "");
				nvram_set("wl_key4", "");
			}
		}
		else if(pkt->WSetting.Auth==AUTHENTICATION_SHARED)
		{	
			nvram_set("wl_auth_mode", "shared");
			if (pkt->WSetting.Encrypt==ENCRYPTION_WEP64 ||
			    pkt->WSetting.Encrypt==ENCRYPTION_WEP128)
			{
				sprintf(tmpbuf, "%d", pkt->WSetting.Encrypt);
				nvram_set("wl_wep_x", tmpbuf);
				if(pkt->WSetting.DefaultKey>DEFAULT_KEY_4 ||
				   pkt->WSetting.DefaultKey<DEFAULT_KEY_1) 
				   pkt->WSetting.DefaultKey=DEFAULT_KEY_1;
				if(pkt->WSetting.DefaultKey==DEFAULT_KEY_1)
					nvram_set_str("wl_key1", pkt->WSetting.Key, sizeof(pkt->WSetting.Key));
				else if(pkt->WSetting.DefaultKey==DEFAULT_KEY_2)
					nvram_set_str("wl_key2", pkt->WSetting.Key, sizeof(pkt->WSetting.Key));
				else if(pkt->WSetting.DefaultKey==DEFAULT_KEY_3)
					nvram_set_str("wl_key3", pkt->WSetting.Key, sizeof(pkt->WSetting.Key));
				else if(pkt->WSetting.DefaultKey==DEFAULT_KEY_4)
					nvram_set_str("wl_key4", pkt->WSetting.Key, sizeof(pkt->WSetting.Key));
				else goto fail;
				sprintf(tmpbuf,"%d", pkt->WSetting.DefaultKey);
				nvram_set("wl_key", tmpbuf);
			}
		}
		else if(pkt->WSetting.Auth==AUTHENTICATION_WPA_PSK)
		{
			nvram_set("wl_auth_mode", "psk");
			nvram_set("wl_wpa_mode", "0");
			if (pkt->WSetting.Encrypt==ENCRYPTION_TKIP)
				nvram_set("wl_crypto", "tkip");
			else if (pkt->WSetting.Encrypt==ENCRYPTION_AES)
				nvram_set("wl_crypto", "aes");
			else
				nvram_set("wl_crypto", "tkip+aes");

			nvram_set("wl_wep_x", "0");	
			nvram_set_str("wl_wpa_psk", pkt->WSetting.Key, WPAPSK_LEN);
			nvram_set("wl_key", "1");
			nvram_set("wl_key1", "");
			nvram_set("wl_key2", "");
			nvram_set("wl_key3", "");
			nvram_set("wl_key4", "");
		}
		
		else goto fail;

		nvram_set("x_Setting", "1");
		nvram_set("x_EZSetup", "1");
	   }
	}		
	if (pkt->QuickFlag&QFCAP_ISP)
	{
		// ISP setting
		if(pkt->ISPSetting.ISPType==ISP_TYPE_DHCPCLIENT)
		{
			nvram_set("wan_proto", "dhcp");	
			nvram_set_str("wan_hostname", pkt->ISPSetting.HostName, sizeof(pkt->ISPSetting.HostName));
			nvram_set_str("wan_hwaddr_x", pkt->ISPSetting.MAC, sizeof(pkt->ISPSetting.MAC));
			nvram_set_str("wan_heartbeat_x", "", sizeof(pkt->ISPSetting.BPServer));
		}
		else if(pkt->ISPSetting.ISPType==ISP_TYPE_PPPOE)
		{
			nvram_set("wan_proto", "pppoe");
			nvram_set_str("wan_pppoe_username", pkt->ISPSetting.UserName, sizeof(pkt->ISPSetting.UserName));
			nvram_set_str("wan_pppoe_passwd", pkt->ISPSetting.Password, sizeof(pkt->ISPSetting.Password));
		}	
		else if(pkt->ISPSetting.ISPType==ISP_TYPE_PPTP)
		{
			nvram_set("wan_proto", "pptp");
			nvram_set_str("wan_pppoe_username", pkt->ISPSetting.UserName, sizeof(pkt->ISPSetting.UserName));
			nvram_set_str("wan_pppoe_passwd", pkt->ISPSetting.Password, sizeof(pkt->ISPSetting.Password));
			nvram_set_ip("wan_ipaddr", pkt->ISPSetting.IPAddr);
			nvram_set_ip("wan_netmask", pkt->ISPSetting.Mask);
			nvram_set_ip("wan_gateway", pkt->ISPSetting.Gateway);
		}
		else if(pkt->ISPSetting.ISPType==ISP_TYPE_STATICIP)
		{
			nvram_set("wan_proto", "static");
			nvram_set_ip("wan_ipaddr", pkt->ISPSetting.IPAddr);
			nvram_set_ip("wan_netmask", pkt->ISPSetting.Mask);
			nvram_set_ip("wan_gateway", pkt->ISPSetting.Gateway);
		}
		else if(pkt->ISPSetting.ISPType==ISP_TYPE_BIGPOND)
		{
			nvram_set("wan_proto", "bigpond");
			nvram_set_str("wan_pppoe_username", pkt->ISPSetting.UserName, sizeof(pkt->ISPSetting.UserName));
			nvram_set_str("wan_pppoe_passwd", pkt->ISPSetting.Password, sizeof(pkt->ISPSetting.Password));
			nvram_set_ip("wan_ipaddr", pkt->ISPSetting.IPAddr);
			nvram_set_str("wan_hostname", pkt->ISPSetting.HostName, sizeof(pkt->ISPSetting.HostName));
			nvram_set_str("wan_hwaddr_x", pkt->ISPSetting.MAC, sizeof(pkt->ISPSetting.MAC));
			nvram_set_str("wan_heartbeat_x", pkt->ISPSetting.BPServer, sizeof(pkt->ISPSetting.BPServer));
		}
		if (pkt->ISPSetting.DNSServer1==0 && pkt->ISPSetting.DNSServer2==0)
		{
			nvram_set("wan_dnsenable_x", "1");
		}
		else
		{
			nvram_set("wan_dnsenable_x", "0");
			nvram_set_ip("wan_dns1_x", pkt->ISPSetting.DNSServer1);
			nvram_set_ip("wan_dns2_x", pkt->ISPSetting.DNSServer2);
		}	
		nvram_set("time_zone", pkt->ISPSetting.TimeZone);
		nvram_set("x_Setting", "1");
	}		
	if (pkt->QuickFlag&QFCAP_FINISH)
	{
		nvram_set("sharedkeystr", "");	
	}
	else if(pkt->QuickFlag&QFCAP_REBOOT) 
	{
	   	nvram_set("sharedkeystr", sharedkeystr);
	}
	//convert_asus_values();
	//nvram_commit();
fail:

}


int OTSStart(int flag)
{
	// stop other service

	if (flag)
	{
		//stop_service_main(1);
		start_sdhcpd();
		strcpy(sharedkeystr, nvram_safe_get("sharedkeystr"));
		tw = (TEMP_WIRELESS *)sharedkeystr;
		nvram_set("sharedkeystr", "");
		nvram_commit();
		time(&bs_time);
		bs_mode=BTNSETUP_DATAEXCHANGE_EXTEND;
		bs_timeout = BTNSETUP_EXCHANGE_TIMEOUT;
	}
	else
	{
		stop_service_main(1);
		start_sdhcpd();

		BN_register_RAND(ots_rand);

		dh = NULL;
		dh = DH_init(p1536, 192, 5);
		if (!DH_generate_key(pubkey,dh)) goto err;

		/* Start button setup process */
		/* SSID : [ProductID]_OTS[Default]_[Prime]*/
		if (nvram_match("x_Setting", "1")) // not in default
			sprintf(ssid, "%s_OTS1", nvram_safe_get("productid")); 
		else sprintf(ssid, "%s_OTS0", nvram_safe_get("productid")); 

		/* +++ Cherry Cho modified in 2007/2/12. +++ */
                eval("wlconf","wsec","off");		
                eval("wlconf","auth", "0"); 		
                eval("wlconf","wpa_auth", "off");
		eval("wlconf","eap","off");
		eval("wlconf","ssid", "ASUS_OTS");
		/* --- Cherry Cho modified in 2007/2/12. --- */

		// reduce power
		// eval("wl", "txpwr", "1");
		bs_mode = BTNSETUP_START;
		bs_timeout = BTNSETUP_INIT_TIMEOUT;
		time(&bs_time);
	}
	return 1;

err:
	if (dh) 
	{
		DH_free(dh);
		dh=NULL;
	}
	return 0;
}

int
OTSExchange(int auth, int encrypt)
{
	int ret = 0;
	int i;
	char SSID[32+1];
	char Key[64+1];

	if (auth==-1&&encrypt==-1) 
	{
		return ret;
	}

	// generate shared key
	if (!DH_compute_key(sharedkey, cpubkey, MAX_DHKEY_LEN, dh)) 
	{
		goto err;
	}

	sharedkeystr[0] = 0;
	for(i=0;i<MAX_DHKEY_LEN;i++)
	{
		 sprintf(sharedkeystr, "%s%02X", sharedkeystr, (unsigned char )sharedkey[i]);
	}

	tw = sharedkeystr;
	strncpy(SSID, tw->u.WirelessStruct.TempSSID, sizeof(SSID));
	SSID[32]=0;
	strncpy(Key, tw->u.WirelessStruct.TempKey, sizeof(Key));
	Key[64]=0;
	
#ifdef ENCRYPTION
	// using layer 3 encryption
#else
	if(auth==AUTHENTICATION_SHARED)
		eval("wl", "auth", "shared");
	else if(auth==AUTHENTICATION_WPA_PSK)
		eval("wl", "auth", "psk");
	else
		eval("wl", "auth", "open");

	if (encrypt==ENCRYPTION_WEP64)
	{
		Key[WEP64_LEN]=0;
		eval("wl","wep", Key);
	}			
	else if (encrypt==ENCRYPTION_WEP128)
	{
		Key[WEP128_LEN]=0;
		eval("wl","wep", Key);
	}
	else if (encrypt==ENCRYPTION_TKIP)
	{
		Key[WPAPSK_LEN] = 0;
		eval("wl", "tkip", Key);
	}

	eval("wl","ssid", SSID);

#endif
	ret = 1;
	return ret;
err:
	if (dh) 
	{
		DH_free(dh);
		dh=NULL;
	}
	return ret;
}

static int
OTS_socket_init( usockaddr* usaP )
{
    int listen_fd;
    int i;

    memset( usaP, 0, sizeof(usockaddr) );
    usaP->sa.sa_family = AF_INET;
    usaP->sa_in.sin_addr.s_addr = htonl( INADDR_ANY );
    usaP->sa_in.sin_port = htons(OTSPORT);

    listen_fd = socket( usaP->sa.sa_family, SOCK_STREAM, 0 );

    if ( listen_fd < 0 )
    {
	perror( "socket" );
	return -1;
    }

    i = 1;
    if ( setsockopt( listen_fd, SOL_SOCKET, SO_REUSEADDR, (char*) &i, sizeof(i) ) < 0 )
	{
	perror( "setsockopt" );
	return -1;
	}
    if ( bind( listen_fd, &usaP->sa, sizeof(struct sockaddr_in) ) < 0 )
	{
	perror( "bind" );
	return -1;
	}
    if ( listen( listen_fd, 5) < 0 )
	{
	perror( "listen" );
	return -1;
	}
    return listen_fd;
}

OTSFinish(int fd, int flag)
{
    	shutdown(fd, 2);
    	close(fd);

	//if (flag) kill(1, SIGHUP);
	//else kill(1, SIGTERM);
	kill(1, SIGTERM);
}

char pdubuf[INFO_PDU_LENGTH];	
char pdubuf_res[INFO_PDU_LENGTH];

static int 
waitsock(int fd, int sec, int usec)
{
	struct timeval tv;
	fd_set fdvar;
	int res;
	
	FD_ZERO(&fdvar);
	FD_SET(fd, &fdvar);
	tv.tv_sec = sec;
	tv.tv_usec = usec;
	res = select(fd+1, &fdvar, NULL, NULL, &tv);

	return res;
}

int OTSPacketHandler(int sockfd)
{
    IBOX_COMM_PKT_HDR_EX *phdr;
    IBOX_COMM_PKT_RES_EX *phdr_res;
    char tmpbuf[INFO_PDU_LENGTH];
	
    int i, len;
    char *buf;

    if (waitsock(sockfd, 2, 0)<=0)
    {
	syslog(LOG_NOTICE, "Connect Timeout %x\n", bs_mode);
	close(sockfd); 
	return NULL;
    }

    buf = pdubuf; 
    len = sizeof(pdubuf);
    /* Parse headers */

    while ((i=read(sockfd, buf, len))&&len>0)
    {
	len-=i;
	buf+=i;
    }

#ifdef DEBUG
    dprintf("recv: %x\n", len);
    for(i=0;i<sizeof(pdubuf);i++)
    {
	if(i%16==0) dprintf("\n");
	dprintf("%02x ", (unsigned char *)pdubuf[i]);
    }
#endif

#ifdef ENCRYPTION
    phdr = (IBOX_COMM_PKT_HDR_EX *)pdubuf;
    phdr_res = (IBOX_COMM_PKT_RES_EX *)pdubuf_res;
    
    if (bs_mode>BTNSETUP_START &&
       !( bs_mode==BTNSETUP_DATAEXCHANGE &&
	phdr->ServiceID==NET_SERVICE_ID_IBOX_INFO &&//Second Chance,2005/07/18
        phdr->PacketType==NET_PACKET_TYPE_CMD &&
        (phdr->OpCode==NET_CMD_ID_EZPROBE || phdr->OpCode==NET_CMD_ID_SETKEY_EX)
	)
) 
    {	
    	Decrypt(sizeof(tw->u.WirelessStruct.TempKey),
		tw->u.WirelessStruct.TempKey,
		pdubuf, INFO_PDU_LENGTH,
		tmpbuf);

    	phdr = (IBOX_COMM_PKT_HDR_EX *)tmpbuf;
    	phdr_res = (IBOX_COMM_PKT_RES_EX *)pdubuf_res;
    } 	
    else
    {
    	phdr = (IBOX_COMM_PKT_HDR_EX *)pdubuf;
    	phdr_res = (IBOX_COMM_PKT_RES_EX *)pdubuf_res;
    }
#else
    phdr = (IBOX_COMM_PKT_HDR_EX *)pdubuf;
    phdr_res = (IBOX_COMM_PKT_RES_EX *)pdubuf_res;
#endif

    //syslog(LOG_NOTICE, "Data Packet XXX %x %x %x %x\n", phdr->ServiceID, phdr->PacketType, bs_mode, phdr->OpCode);

    if (phdr->ServiceID==NET_SERVICE_ID_IBOX_INFO &&
        phdr->PacketType==NET_PACKET_TYPE_CMD)
    {	    	
	phdr_res->ServiceID=NET_SERVICE_ID_IBOX_INFO;
	phdr_res->PacketType=NET_PACKET_TYPE_RES;
	phdr_res->OpCode =phdr->OpCode;
	phdr_res->Info = phdr->Info;
	memcpy(phdr_res->MacAddress, phdr->MacAddress, sizeof(phdr_res->MacAddress));
#ifdef OTS_LOG
	if (phdr->OpCode!=NET_CMD_ID_EZPROBE)
		ots_log(phdr->OpCode+0x90, 0);
#endif
	//syslog(LOG_NOTICE, "Data Packet %x %x\n", bs_mode, phdr->OpCode);

	switch(phdr->OpCode)
	{
		case NET_CMD_ID_EZPROBE:
		{
		     PKT_EZPROBE_INFO *ezprobe_res;
		     
		     ezprobe_res = (PKT_EZPROBE_INFO *)(pdubuf_res+sizeof(IBOX_COMM_PKT_RES_EX));		

		     ezprobe_res->isNotDefault = atoi(nvram_safe_get("x_Setting"));
		     ezprobe_res->isSetByOts = atoi(nvram_safe_get("x_EZSetup"));
		     ezprobe_res->isWAN = is_phyconnected(nvram_safe_get("wan_ifname"));
		     ezprobe_res->isDHCP = 0;
		     ezprobe_res->isPPPOE = 0;

		     if (nvram_match("wl_auth_mode", "shared"))
				ezprobe_res->Auth = AUTHENTICATION_SHARED;
		     else if (nvram_match("wl_auth_mode", "psk"))
				ezprobe_res->Auth = AUTHENTICATION_WPA_PSK;
		     else if (nvram_match("wl_auth_mode", "wpa"))
				ezprobe_res->Auth = AUTHENTICATION_WPA;
		     else if (nvram_match("wl_auth_mode", "radius"))
				ezprobe_res->Auth = AUTHENTICATION_8021X;
		     else ezprobe_res->Auth = AUTHENTICATION_OPEN;

		     if (nvram_match("wl_macmode", "allow"))
				ezprobe_res->Acl = ACL_MODE_ACCEPT;
		     else if (nvram_match("wl_macmode", "deny"))
				ezprobe_res->Acl = ACL_MODE_REJECT;
		     else ezprobe_res->Acl = ACL_MODE_DISABLE;

		     if (nvram_match("wl_mode_x", "1"))
				ezprobe_res->Wds = WDS_MODE_WDS_ONLY;
		     else if (nvram_match("wl_mode_x", "2"))
				ezprobe_res->Wds = WDS_MODE_HYBRID;
		     else ezprobe_res->Wds = WDS_MODE_AP_ONLY;

		     strcpy(ezprobe_res->ProductID, nvram_safe_get("productid")); 
		     strcpy(ezprobe_res->FirmwareVersion, nvram_safe_get("firmver"));
		     time(&bs_time); // reset timer only
		     bs_auth=-1;
		     bs_encrypt=-1;

#ifdef OTS_SIMU
		     if(!ots_simu(1)) return INFO_PDU_LENGTH;	
#endif
		     send(sockfd, pdubuf_res, sizeof(pdubuf_res), NULL);
		     return INFO_PDU_LENGTH;		  
		}	
		case NET_CMD_ID_SETKEY_EX:
		{
		     PKT_SET_INFO_GW_QUICK_KEY *pkey;
		     PKT_SET_INFO_GW_QUICK_KEY *pkey_res;

		     if (bs_mode!=BTNSETUP_START 
			&& bs_mode != BTNSETUP_DATAEXCHANGE // allow second change, 2005/07/18, Chen-I
			) 
		     {	
		     	  bs_auth=-1;
		     	  bs_encrypt=-1;
		  	  return NULL;
                     }

		     pkey=(PKT_SET_INFO_GW_QUICK_KEY *)(pdubuf+sizeof(IBOX_COMM_PKT_HDR_EX));
		     pkey_res = (PKT_SET_INFO_GW_QUICK_KEY *)(pdubuf_res+sizeof(IBOX_COMM_PKT_RES_EX));

		     if(pkey->KeyLen==0) return NULL;
		     else memcpy(cpubkey, pkey->Key, MAX_DHKEY_LEN);

		     bs_mode = BTNSETUP_DATAEXCHANGE;
		     time(&bs_time);
		     bs_timeout=BTNSETUP_EXCHANGE_TIMEOUT;
		     bs_auth=pkey->Auth;
		     bs_encrypt=pkey->Encrypt;
		     pkey_res->Auth = pkey->Auth;
		     pkey_res->Encrypt = pkey->Encrypt;
		     pkey_res->KeyLen = MAX_DHKEY_LEN;
		     memcpy(pkey_res->Key, pubkey, MAX_DHKEY_LEN);
#ifdef OTS_SIMU
		     if(!ots_simu(2)) return INFO_PDU_LENGTH;
#endif
		     send(sockfd, pdubuf_res, sizeof(pdubuf_res), NULL);
		     return INFO_PDU_LENGTH;
		}
		case NET_CMD_ID_QUICKGW_EX:
		{
		     PKT_SET_INFO_GW_QUICK *gwquick;
		     PKT_SET_INFO_GW_QUICK *gwquick_res;

		     if (bs_mode!=BTNSETUP_DATAEXCHANGE && bs_mode!=BTNSETUP_DATAEXCHANGE_EXTEND) 
		     {
			 return NULL;
                     }
#ifdef ENCRYPTION
		     gwquick=(PKT_SET_INFO_GW_QUICK *)(tmpbuf+sizeof(IBOX_COMM_PKT_HDR_EX));
#else
		     gwquick=(PKT_SET_INFO_GW_QUICK *)(pdubuf+sizeof(IBOX_COMM_PKT_HDR_EX));
#endif

		     gwquick_res = (PKT_SET_INFO_GW_QUICK *)(pdubuf_res+sizeof(IBOX_COMM_PKT_RES_EX));

		     //printf("Flag: %x\n", gwquick->QuickFlag);
		     btn_setup_save_setting(gwquick);

#ifdef OTS_LOG
	if (phdr->OpCode!=NET_CMD_ID_EZPROBE)
		ots_log(phdr->OpCode + gwquick->QuickFlag, 2);
#endif

		     if((gwquick->QuickFlag&QFCAP_WIRELESS)&&
			(gwquick->QuickFlag&QFCAP_GET)) // get setting
		     {	
			 btn_setup_get_setting(gwquick_res);
		     	 gwquick_res->QuickFlag = QFCAP_WIRELESS;
		     }	
		     else
		     {	
			 memcpy(gwquick_res, gwquick, sizeof(PKT_SET_INFO_GW_QUICK));
		     }	

		     if((gwquick->QuickFlag&QFCAP_FINISH)) // finish
		     {	
		     	 bs_mode = BTNSETUP_FINISH;
		     	 bs_timeout=0;
#ifdef OTS_SIMU
		     	 if(!ots_simu(3)) return INFO_PDU_LENGTH;
#endif
		     }
		     else if((gwquick->QuickFlag&QFCAP_REBOOT)) //reboot 
		     {
			 bs_mode = BTNSETUP_DATAEXCHANGE_FINISH;
		     	 bs_timeout=BTNSETUP_EXCHANGE_TIMEOUT;
#ifdef OTS_SIMU
		     	 if(!ots_simu(4)) return INFO_PDU_LENGTH;
#endif
		     }	
		     else
		     {
#ifdef OTS_SIMU
		     	 if(!ots_simu(5)) return INFO_PDU_LENGTH;
#endif
                     }
		     time(&bs_time);

#ifdef OTS_LOG
	if (phdr->OpCode!=NET_CMD_ID_EZPROBE)
		ots_log(phdr->OpCode + gwquick->QuickFlag, 3);
#endif


#ifdef ENCRYPTION
		     Encrypt(sizeof(tw->u.WirelessStruct.TempKey),
				tw->u.WirelessStruct.TempKey,
				pdubuf_res, INFO_PDU_LENGTH,
				tmpbuf);
		     i=send(sockfd, tmpbuf, INFO_PDU_LENGTH, 0);
 
#else
		     i=send(sockfd, pdubuf_res, INFO_PDU_LENGTH, 0);
#endif

#ifdef OTS_LOG
	 	     if(i>=0) ots_log(i, 4);
		     else ots_log(errno, 4);
#endif

		     return INFO_PDU_LENGTH;
		}
		default:
			return NULL;	
	}
    }
    return NULL;
}

int 
start_ots(void)
{ 
	char *ots_argv[] = {"ots", NULL};
	pid_t pid;
	
	_eval(ots_argv, NULL, 0, &pid);
	return 0;
}

int
stop_ots(void)
{
	int ret1 = eval("killall", "ots");
	dprintf("done\n");
	return(ret1);
}

int 
ots_main(int argc, char *argv[])
{
	FILE *fp;
    	usockaddr usa;
    	int listen_fd;
    	int conn_fd;
    	socklen_t sz = sizeof(usa);
    	IBOX_COMM_PKT_HDR*  phdr;
    	int                 iLen , iRes , iCount , iRcv;
    	int                 fromlen;
    	char                *hdr;
    	char                pdubuf[INFO_PDU_LENGTH];
    	struct sockaddr_in  from_addr;
	time_t now;
	int flag=0;
	int ret;

	/* write pid */
	if ((fp=fopen("/var/run/ots.pid", "w"))!=NULL)
	{
		fprintf(fp, "%d", getpid());
		fclose(fp);
	}

	if (nvram_invmatch("sharedkeystr", "")) 
	{
		OTSStart(1);
	}
	else 
	{
#ifdef OTS_LOG
		ots_log(0, 0);
#endif
		OTSStart(0);
	}

	/* Initialize listen socket */
    	if ((listen_fd = OTS_socket_init(&usa)) < 0) {
		fprintf(stderr, "can't bind to any address\n" );
		return 0;
    	} 	

    	/* Loop forever handling requests */
    	for (;;) 
    	{
    		ret=waitsock(listen_fd, bs_timeout, 0);

		if (ret==0) goto finish;
		else if(ret<0) continue;

		if ((conn_fd = accept(listen_fd, &usa.sa, &sz)) < 0) {
			perror("accept");
#ifdef OTS_LOG
			ots_log(bs_mode+0x10, 0);
#endif
			continue;
			//return errno;
		}

		if (!OTSPacketHandler(conn_fd))
		{
			syslog(LOG_NOTICE, "Error Packets: %x %x %x", bs_mode, bs_auth, bs_encrypt);
		}
	        
		if (bs_mode==BTNSETUP_DATAEXCHANGE) 
		{
		      if(!OTSExchange(bs_auth, bs_encrypt)) 
		      {	
			  //continue;
		      }	
		}
		else if(bs_mode==BTNSETUP_DATAEXCHANGE_FINISH)
		{
		      sleep(2);
		      flag = 1;
		      goto finish;		
		}
		time(&now);
		if (bs_mode>=BTNSETUP_FINISH) goto finish;
		if ((now-bs_time)>bs_timeout) goto finish;
    	}
finish:
	if (bs_mode==BTNSETUP_FINISH)
	{
#ifdef OTS_LOG
		ots_log(bs_mode + 0x30, 0);
#endif
		if (dh) 
		{
			DH_free(dh);
			dh=NULL;
		}
    		shutdown(listen_fd, 2);
    		close(listen_fd);
		nvram_set("bs_mode", "1");
		bs_mode=BTNSETUP_NONE;
		sleep(2);
		stop_wan();
		stop_dhcpd();
		convert_asus_values(1);
		nvram_commit();
		start_dhcpd();
		start_wan();
	}
	else 
	{
#ifdef OTS_LOG
		ots_log(bs_mode + 0x40, 1);
#endif
		if (dh) 
		{
			DH_free(dh);
			dh=NULL;
		}

		convert_asus_values(1);
		nvram_commit();
		OTSFinish(listen_fd, flag);
	}
    	return 1;
}
#endif
/*
void char_to_ascii(char *output, char *input)// Transfer Char to ASCII 
{                                                   // Cherry_Cho added in 2006/9/29
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
*/

