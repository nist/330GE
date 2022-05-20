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
/************************************************************************************************************

 * File Name	: qosutile.c
 * Description  : modify from WRT54GS-v4.70.6:voip_qos.c. Called by speedtest.
 * Auther       : Wendel Huang.
 * History	: 2006.05.22 modify to post on WL500gP.
		  2006.07.18 Post to WL700g, and remove execution message from release version.
		  2006.08.08 modify check_wan_link() to ckeck WL700g wan status.
		  2006.08.14 modify get_dns_list() to get WL700g's DNS setting.
 ************************************************************************************************************/

#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <linux/if_ether.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/sockios.h>

#include "shutils.h"

#ifdef WL600g
#include "asusbcmnvram.h"
#include "dbapi.h"
#include "ifcwanapi.h"
#include "syscall.h"
#include "bcmadsl.h"    //for ADSL_CONNECTION_INFO

#else	//WL500gP, WL550gE
#include "bcmnvram.h"

#define IFC_TINY_LEN 	16
#endif

#include "qosutils.h"


#ifdef WL600g	//For WL600g

#define ADSL_LINK_CHECK 10

/**************************************************************************
 * Function Name: check_wan_link()
 * Description  : Check Wan interface's connection status
 * Parameters   : int num -> wan interface index
 * Returns      : wan status
			0 : WAN link fail
			1 : WAN link UP
 **************************************************************************/
int check_wan_link(int num)
{
	int wan_link = 0;
	int wait_dhcp = 0;
        int adslstatus = 0 ;
	char * interface[IFC_TINY_LEN];

	WAN_CON_ID wanId;
	WAN_CON_INFO wanInfo;

        //wait for ADSL ready
        int loop = 0;
        while( (adslstatus=bcmGetAdslStatus())!=BCM_ADSL_LINK_UP && loop<ADSL_LINK_CHECK) {
                sleep(10);
                loop++;
                //printf("        check_wan_link() ** ADSL line not connect\n");
        }
                                                                                                 
        if(adslstatus != BCM_ADSL_LINK_UP) {
 		nvram_set("qos_ubw_status", "fail");
		nvram_set("qos_ubw_reason", "Cable is not attached");
                return wan_link;
	}

	loop = 0;
	do {
	    wanId.conId=wanId.vpi=wanId.vci=0;
 	    if( BcmDb_getWanInfoNext(&wanId, &wanInfo)==DB_WAN_GET_OK ) {

		switch( BcmWan_getStatus(&wanId) ) {
	   	    case IFC_STATUS_DHCPC_PROGRESS:
#ifdef QOS_DEBUG
                	printf("        check_wan_link() ** DHCP\n");
#endif
			wait_dhcp = 1 ;
			sleep(10);
        		break; 
	   	    case IFC_STATUS_UP:
#ifdef QOS_DEBUG
                	printf("        check_wan_link() ** UP\n");
#endif
	 		nvram_set("qos_ubw_status", "initialing");
			BcmDb_getWanInterfaceName(&wanId, wanInfo.protocol, interface);
			nvram_set("wan0_ifname", interface);
			wan_link = 1;
			wait_dhcp = 0;
	           	break;
		    case IFC_STATUS_AUTH_ERR:
		    case IFC_STATUS_DOWN:
 			nvram_set("qos_ubw_reason", "PPP Error");
		    default:
#ifdef QOS_DEBUG
                	printf("        check_wan_link() ** fail\n");
#endif
 			nvram_set("qos_ubw_status", "fail");
			wan_link = 0;
			wait_dhcp = 0;
			sleep(10);
			break;
		}

	    }
	    loop++;
	}while(!wan_link && loop<ADSL_LINK_CHECK);

	return wan_link;
}

/**************************************************************************
 * Function Name: get_dns_list()
 * Description  : get DNS server ip from user configuration or retrieved from ISP
 * Parameters   : int no -> indicate using static or dynamaic dns preferentially
			no = 0 : Static DNS
			no = 1 : dynamic DNS
 * Returns      : struct dns_lists *  -> contain DNS IP infomation
 **************************************************************************/
struct dns_lists *get_dns_list(int no)
{
        //Get DNS Address from static configuration or dynamically retrive from ISP
	struct dns_lists *dns_list=NULL;
        char ip[IFC_TINY_LEN];
	int index;

	index = 0;
	dns_list = (struct dns_lists *)malloc(sizeof(struct dns_lists));
	memset(dns_list, 0, sizeof(struct dns_lists));

        bcmGetDns(ip);
        if( !strchr(ip, ' ') || strcmp(ip, "") || strcmp(ip, "0.0.0.0") ) {
#ifdef QOS_DEBUG
                printf("        get_dns_list() ** Pirm DNS ip = %s\n", ip);
#endif
		strcpy(dns_list->dns_server[index], ip);
		index++;
        }

        bcmGetDns2(ip);
        if( !strchr(ip, ' ') || strcmp(ip, "") || strcmp(ip, "0.0.0.0") )  {
#ifdef QOS_DEBUG
                printf("        get_dns_list() ** 2'th DNS ip = %s\n", ip);
#endif
		strcpy(dns_list->dns_server[index], ip);
		index++;
        }
	
	dns_list->num_servers = index;

	return dns_list;
}

/**************************************************************************
 * Function Name: get_gw_ip()
 * Description  : get LAN IP
 * Parameters   : None
 * Returns      : char*  -> LAN IP String
 **************************************************************************/
char *get_gw_ip(void) {
	char *addr=NULL;

	addr = (char *)malloc(IFC_TINY_LEN);
	
	bcmGetIfDestAddr("br0", addr);
	//bcmGetIfDestAddr(IFC_BRIDGE_NAME, addr);

	return addr;
}
#endif


#ifdef WL700G

#ifdef MYGCC
#include "ethtool-util.h"
#undef SIOCETHTOOL
#define SIOCETHTOOL 0x8946
                                                                                                                                               
int is_phyconnected(char *wan_if)
{
        int fd, err;
        struct ifreq ifr;
        struct ethtool_cmd ecmd;
                                                                                                                                               
        memset(&ifr, 0, sizeof(ifr));
        strcpy(ifr.ifr_name, wan_if);
        fd=socket(AF_INET, SOCK_DGRAM, 0);
        if (fd<0)
        {
                fprintf(stderr, "phy: error\n");
                return 0;
        }
        ecmd.cmd=ETHTOOL_GSET;
        ifr.ifr_data = (caddr_t)&ecmd;
        err=ioctl(fd, SIOCETHTOOL, &ifr);
        close(fd);
        if (err==0)
        {
                //fprintf(stderr, "phy: %x\n", ecmd.phy_address);
                if (ecmd.phy_address&(0x01)) return 1;
        }
        return 0;
}
#endif

/**************************************************************************
 * Function Name: check_wan_link()
 * Description  : Check Wan interface's connection status on WL700gE 
 * Parameters   : int num -> wan interface index
 * Returns      : wan status
			0 : WAN link fail
			1 : WAN link UP
 **************************************************************************/
int check_wan_link(int num)
{
	FILE *fp;
	char statusstr[64], line[64], *reason;
        int wan_link = 0, len;

	if (!is_phyconnected(nvram_safe_get("wan_ifname")))
	{
		wan_link=0;
 		nvram_set("qos_ubw_status", "fail");
		nvram_set("qos_ubw_reason", "Cable not attached");
#ifdef QOS_DEBUG
		printf("check_wan_link() ** wan_link = %d reason=%s\n", wan_link, "Cable not attached");
#endif
	}
	else if(nvram_match("wan_ipaddr", "") || nvram_match("wan_stopped", "1"))
	{
		wan_link = 0;
 		nvram_set("qos_ubw_status", "fail");
		if (nvram_match("wan_proto", "pppoe") || nvram_match("wan_proto", "pptp"))
		{ 
			fp = fopen("/tmp/wanstatus.log", "r");
			if (fp!=NULL)
			{
				fgets(line, sizeof(line), fp);
				reason = strchr(line, ',');
				if (reason) 
				{
					strcpy(statusstr, reason+1);
					len=0;
					for(len=0;len<strlen(statusstr);len++)
					{
						if(statusstr[len]=='\n'||statusstr[len]=='\r') break;
					}
					statusstr[len]=0x0;
					nvram_set("qos_ubw_reason", statusstr);
				}	
				fclose(fp);
			}
		}
		if (!strlen(statusstr))
			nvram_set("qos_ubw_reason", "Disconnected");
	}
	else
	{
		wan_link = 1;
 		nvram_set("qos_ubw_status", "initialing");
	}
	
	return wan_link;
}

/**************************************************************************
 * Function Name: get_dns_list() for WL700g
 * Description  : get DNS server ip from user configuration or retrieved from ISP
 * Parameters   : int no -> indicate using static or dynamaic dns preferentially
                        no = 0 : Static DNS
                        no = 1 : dynamic DNS
 * Returns      : struct dns_lists *  -> contain DNS IP infomation
 **************************************************************************/
struct dns_lists *get_dns_list(int no)
{
        char list[254],prelist[254];
        char *next, word[254];
        struct dns_lists *dns_list = NULL;
        int i, match = 0;
        char got_dns[2][15]={"wan_dns1_x","wan_dns2_x"};
                                                                                                 
        dns_list = (struct dns_lists *)malloc(sizeof(struct dns_lists));
        memset(dns_list, 0, sizeof(struct dns_lists));
        dns_list->num_servers = 0;
                                                                                                 
        if(no == 0){
                snprintf(prelist, sizeof(prelist), "%s %s",nvram_safe_get(got_dns[0]), nvram_safe_get(got_dns[1]));
                snprintf(list, sizeof(list), "%s %s",prelist ,nvram_safe_get("wan_dns"));
	}
	else {
                snprintf(prelist, sizeof(prelist), "%s",nvram_safe_get("wan_dns"));
                snprintf(list, sizeof(list), "%s %s %s",prelist ,nvram_safe_get(got_dns[0]), nvram_safe_get(got_dns[1]));
	}
	printf("%s\n", list);

        foreach(word, list, next) {
                if(strcmp(word, "0.0.0.0") && strcmp(word, "")){
                       match = 0;
                       for(i=0 ; i<dns_list->num_servers ; i++){       // Skip same DNS
                                if(!strcmp(dns_list->dns_server[i], word))      match = 1;
                        }
                        if(!match){
                                snprintf(dns_list->dns_server[dns_list->num_servers], sizeof(dns_list->dns_server[dns_list->num_servers]), "%s", word);
				printf("%s \n", dns_list->dns_server[dns_list->num_servers]);
                                dns_list->num_servers ++ ;
                        }
                }
                if(dns_list->num_servers == 4)      break;      // We only need 4 counts DNS entry
        }

        return dns_list;
}

/**************************************************************************
 * Function Name: get_gw_ip()
 * Description  : get gateway IP of WL700g
 * Parameters   : None
 * Returns      : char*  -> Gateway IP String
 **************************************************************************/
char *get_gw_ip(void) {
	char *addr=NULL;

	addr = (char *)malloc(IFC_TINY_LEN);
	
	strcpy(addr, nvram_safe_get("lan_ipaddr"));	

	return addr;
}
#endif


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx


#ifdef WL500GP	//For WL500gP, WL550gE
/*
int file_to_buf(char *path, char *buf, int len)
{
        FILE *fp;
                                                                                                 
        memset(buf, 0 , len);
                                                                                                 
        if ((fp = fopen(path, "r"))) {
                fgets(buf, len, fp);
                fclose(fp);
                return 1;
        }
                                                                                                 
        return 0;
}
*/

int get_ppp_pid(char *conntype)
{
        int pid = -1;
        char tmp[80], tmp1[80];
        snprintf(tmp, sizeof(tmp), "/var/run/%s.pid", conntype);
#ifdef QOS_DEBUG                                                                                                                        
	printf("	get_ppp_pid() ** tmp = %s\n", tmp);
#endif
        file_to_buf(tmp, tmp1, sizeof(tmp1));
        pid = atoi(tmp1);
#ifdef QOS_DEBUG
	printf("	get_ppp_pid() ** pid = %d\n", pid);
#endif
        return pid;
}

/* Find process name by pid from /proc directory */
char *find_name_by_proc(int pid)
{
        FILE *fp;
        char line[254];
        char filename[80];
        static char name[80];
                                                                                                 
        snprintf(filename, sizeof(filename), "/proc/%d/status", pid);
                                                                                                 
        if((fp = fopen(filename, "r"))){
                fgets(line, sizeof(line), fp);
                /* Buffer should contain a string like "Name:   binary_name" */
                sscanf(line, "%*s %s", name);
#ifdef QOS_DEBUG
		printf("	find_name_by_proc() ** name=%s\n", name);
#endif
                fclose(fp);
                return name;
        }
                                                                                                 
        return "";
}

int osl_ifflags(const char *ifname)
{
    int sockfd;
    struct ifreq ifreq;
    int flags = 0;
                                                                                                 
    if ((sockfd = socket( AF_INET, SOCK_DGRAM, 0 )) < 0) {
        perror("socket");
        return flags;
    }
                                                                                                 
    strncpy(ifreq.ifr_name, ifname, IFNAMSIZ);
    if (ioctl(sockfd, SIOCGIFFLAGS, &ifreq) < 0) {
            flags = 0;
    } else {
            flags = ifreq.ifr_flags;
    }
    close(sockfd);
    return flags;
}

//#ifdef MYTEST
// Added by Joey for ethtool
#include <net/if.h>
#include "ethtool-util.h"
#undef SIOCETHTOOL
#define	SIOCETHTOOL	0x8946
#define	SIOCGETCPHYRD	0x89FE
                                                                                                                                               
int is_phyconnected(char *wan_if)
{
        int fd, err;
        struct ifreq ifr;
        struct ethtool_cmd ecmd;
                                                                                                                                               
        memset(&ifr, 0, sizeof(ifr));
        strcpy(ifr.ifr_name, wan_if);
        fd=socket(AF_INET, SOCK_DGRAM, 0);
        if (fd<0)
        {
                return 0;
        }
        ecmd.cmd=ETHTOOL_GSET;
        ifr.ifr_data = (caddr_t)&ecmd;
        err=ioctl(fd, SIOCETHTOOL, &ifr);
//        close(fd);
//		printf("cable not attached 1\n");
        if (err==0)
        {
//		printf("cable not attached 2\n");

                // 2006_06_16_Roly
                // 2001.01.08 Modified for BCM5354 Platform
                // detect wan port connection

                int vecarg[2];
                int verbuf;
                                                                                                     
                vecarg[0] = strtoul("0x01", NULL, 0) << 16;
                vecarg[0] |= strtoul("0x00", NULL, 0) & 0xffff;
                                                                                                                             
                ifr.ifr_data = (caddr_t) vecarg;
                if (ioctl(fd, SIOCGETCPHYRD, (caddr_t)&ifr) < 0)
                        printf("etcphyrd error\n");
                printf("0x%04x\n", vecarg[1]);
                close(fd);
                                                                                                                             
                verbuf=vecarg[1];
                        
                if((verbuf & 0x0010) != 0)
                        return 1;
        }
        return 0;
}
//#endif

/**************************************************************************
 * Function Name: check_wan_link()
 * Description  : Check Wan interface's connection status on WL500gp 
 * Parameters   : int num -> wan interface index
 * Returns      : wan status
			0 : WAN link fail
			1 : WAN link UP
 **************************************************************************/
int check_wan_link(int num)
{
        int wan_link = 0;
	char conntype[10];
        char wan_if[2][20]={"wan_ifname","wan_ifnames_t"};

	//Check WAN Cable connect
	if( is_phyconnected(nvram_safe_get("wan_ifname"))==0 ) {
	//if ( 	strcmp( nvram_safe_get("wan_status_t"), "Disconnected")==0 
	//	&& strcmp( nvram_safe_get("wan_reason_t"), "Cable is not attached")==0 ) {
		wan_link=0;
 		nvram_set("qos_ubw_status", "fail");
		nvram_set("qos_ubw_reason", "Cable is not attached");
		return wan_link;
	}

        if(nvram_match("wan_proto", "pptp")
        || nvram_match("wan_proto", "PPTP")
        || nvram_match("wan_proto", "l2tp")
        || nvram_match("wan_proto", "pppoe")
        || nvram_match("wan_proto", "PPPoE")
        || nvram_match("wan_proto", "BigPong")
        || nvram_match("wan_proto", "heartbeat")
        ){
                FILE *fp;
                char filename[80];
		//char conntype[10];
		char buf1[256];
                char *name;
		char* pos;
		char* cpos;
                
                if(num == 0) {
			system("ls /tmp/ppp > /tmp/ppp/filelist");

			if ((fp = fopen("/tmp/ppp/filelist", "r"))) {
			    while( fgets(buf1,256,fp) ) 
				if ( (pos=strstr(buf1, "link")) ) {

				    sprintf(filename, "/tmp/ppp/%s", buf1); 
				    cpos = strchr(filename, '\n');
				    *cpos = '\0';
#ifdef QOS_DEBUG
				    printf("	check_wan_link() ** filename = %s\n", filename);
#endif

				    strcpy(conntype, pos+5);
				    cpos = strchr(conntype, '\n');
				    *cpos = '\0';
#ifdef QOS_DEBUG
				    printf("	check_wan_link() ** conntype = %s\n", conntype);
#endif
				    break;
				}
			}
			else { 
 			    nvram_set("qos_ubw_status", "fail");
			    return wan_link;
			}

			fclose(fp); 
			unlink("/tmp/ppp/filelist");
		}

                if ((fp = fopen(filename, "r"))){
                        int pid = -1;
                        fclose(fp);

                        if(nvram_match("wan_proto", "heartbeat")){
                                char buf[20];
                                file_to_buf(filename, buf, sizeof(buf));
                                pid = atoi(buf);
                        }
                        else
                                pid = get_ppp_pid(conntype);
                                                                                                 
                        name = find_name_by_proc(pid);
                        if(!strncmp(name, "pppoecd", 7) ||      // for PPPoE
                           !strncmp(name, "pppd", 4) ||         // for PPTP
                           !strncmp(name, "bpalogin", 8)) {      // for HeartBeat
                                wan_link = 1;     //connect
 				nvram_set("qos_ubw_status", "initialing");
			}
                        else{
#ifdef QOS_DEBUG
                                printf("The %s had been died, remove %s\n", nvram_safe_get("wan_proto"), filename);
#endif
                                wan_link = 0;   
				// For some reason, the pppoed had been died, by link file still exist.
                                unlink(filename);
                        }
                }
        }
        else{
                if(nvram_invmatch("wan_ipaddr", "0.0.0.0")){
                        wan_link = 1;
 			nvram_set("qos_ubw_status", "initialing");
		}
	}

                                                                                                 
#ifdef QOS_DEBUG
	printf("	check_wan_link() ** wan_link = %d\n", wan_link);
#endif
        return wan_link;
}

/**************************************************************************
 * Function Name: get_dns_list()
 * Description  : get DNS server ip from user configuration or retrieved from ISP
 * Parameters   : int no -> indicate using static or dynamaic dns preferentially
                        no = 0 : Static DNS
                        no = 1 : dynamic DNS
 * Returns      : struct dns_lists *  -> contain DNS IP infomation
 **************************************************************************/
struct dns_lists *get_dns_list(int no)
{
        char list[254], prelist[254];
        char *next, word[254];
        struct dns_lists *dns_list = NULL;
        int i, match = 0;
        char got_dns[2][15]={"wan_dns1_x","wan_dns2_x"};
                                                                                                 
        dns_list = (struct dns_lists *)malloc(sizeof(struct dns_lists));
        memset(dns_list, 0, sizeof(struct dns_lists));
        dns_list->num_servers = 0;

        if(no == 0){
                snprintf(prelist, sizeof(prelist), "%s %s",nvram_safe_get(got_dns[0]), nvram_safe_get(got_dns[1]));
                snprintf(list, sizeof(list), "%s %s",prelist ,nvram_safe_get("wan0_dns"));
	}
	else {
                snprintf(prelist, sizeof(prelist), "%s",nvram_safe_get("wan0_dns"));
                snprintf(list, sizeof(list), "%s %s %s",prelist ,nvram_safe_get(got_dns[0]), nvram_safe_get(got_dns[1]));
	}
                                                                                                 
        foreach(word, list, next) {
                if(strcmp(word, "0.0.0.0") && strcmp(word, "")){
                        match = 0;
                       for(i=0 ; i<dns_list->num_servers ; i++){       // Skip same DNS
                                if(!strcmp(dns_list->dns_server[i], word))      match = 1;
                        }
                        if(!match){
                                snprintf(dns_list->dns_server[dns_list->num_servers], sizeof(dns_list->dns_server[dns_list->num_servers]), "%s", word);
                                dns_list->num_servers ++ ;
                        }
                }
                if(dns_list->num_servers == 4)      break;      // We only need 3 counts DNS entry
        }
        return dns_list;

}

/**************************************************************************
 * Function Name: get_gw_ip()
 * Description  : get gateway IP
 * Parameters   : None
 * Returns      : char*  -> Gateway IP String
 **************************************************************************/
char *get_gw_ip(void) {
	char *addr=NULL;

	addr = (char *)malloc(IFC_TINY_LEN);
	
	strcpy(addr, nvram_safe_get("lan_ipaddr"));	

	return addr;
}
#endif


