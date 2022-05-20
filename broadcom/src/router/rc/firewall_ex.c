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
 * $Id: firewall_ex.c,v 1.1.1.1 2007/01/25 12:52:21 jiahao_jhou Exp $
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <dirent.h>

#include <bcmnvram.h>
#include <shutils.h>
#include <rc.h>
#include <netconf.h>
#include <nvparse.h>

#define foreach_x(x)	for(i=0; i<atoi(nvram_safe_get(x)); i++)

char *g_buf;
char g_buf_pool[1024];

void g_buf_init()
{
	g_buf = g_buf_pool;
}

char *g_buf_alloc(char *g_buf_now)
{
	g_buf += strlen(g_buf_now)+1;

	return(g_buf_now);
}

void nvram_unsets(char *name, int count)
{
	char itemname_arr[32];
	int i;

	for(i=0; i<count; i++)
	{
		sprintf(itemname_arr, "%s%d", name, i);
		nvram_unset(itemname_arr);
	}
}

char *proto_conv(char *proto_name, int idx)
{	
	char *proto;
	char itemname_arr[32];
	
	sprintf(itemname_arr,"%s%d", proto_name, idx);
	proto=nvram_get(itemname_arr);
		
	if(!strncasecmp(proto, "Both", 3)) strcpy(g_buf, "both");
	else if(!strncasecmp(proto, "TCP", 3)) strcpy(g_buf, "tcp");
	else if(!strncasecmp(proto, "UDP", 3)) strcpy(g_buf, "udp");
	else if(!strncasecmp(proto, "OTHER", 5)) strcpy(g_buf, "other");
	else strcpy(g_buf,"tcp");
	return (g_buf_alloc(g_buf));
}

char *protoflag_conv(char *proto_name, int idx, int isFlag)
{	
	char *proto;
	char itemname_arr[32];
	
	sprintf(itemname_arr,"%s%d", proto_name, idx);
	proto=nvram_get(itemname_arr);

	strcpy(g_buf, "");
	
	if (!isFlag)
	{		
		if(strncasecmp(proto, "UDP", 3)==0) strcpy(g_buf, "udp");
		else strcpy(g_buf, "tcp");
	}
	else
	{	
		if(strlen(proto)>3)
		{
			strcpy(g_buf, proto+4);
		}			
		else strcpy(g_buf,"");
	}	
	return (g_buf_alloc(g_buf));
}

char *portrange_ex_conv(char *port_name, int idx)
{
	char *port, *strptr;
	char itemname_arr[32];
	
	sprintf(itemname_arr,"%s%d", port_name, idx);
	port=nvram_get(itemname_arr);

	strcpy(g_buf, "");
	
	if(!strncmp(port, ">", 1)) {
		sprintf(g_buf, "%d-65535", atoi(port+1) + 1);
	}
	else if(!strncmp(port, "=", 1)) {
		sprintf(g_buf, "%d-%d", atoi(port+1), atoi(port+1));
	}
	else if(!strncmp(port, "<", 1)) {
		sprintf(g_buf, "1-%d", atoi(port+1) - 1);
	}
	else if(strptr=strchr(port, ':'))
	{		
		strcpy(itemname_arr, port);
		strptr = strchr(itemname_arr, ':');
		sprintf(g_buf, "%d-%d", atoi(itemname_arr), atoi(strptr+1));		
	}
	else if(*port)
	{
		sprintf(g_buf, "%d-%d", atoi(port), atoi(port));
	}
	else
	{
		sprintf(g_buf, "");
	}
	
	return(g_buf_alloc(g_buf));
}

char *portrange_ex2_conv(char *port_name, int idx, int *start, int *end)
{
	char *port, *strptr;
	char itemname_arr[32];
	
	sprintf(itemname_arr,"%s%d", port_name, idx);
	port=nvram_get(itemname_arr);

	strcpy(g_buf, "");
	
	if(!strncmp(port, ">", 1)) 
	{
		sprintf(g_buf, "%d-65535", atoi(port+1) + 1);
		*start=atoi(port+1);
		*end=65535;
	}
	else if(!strncmp(port, "=", 1)) 
	{
		sprintf(g_buf, "%d-%d", atoi(port+1), atoi(port+1));
		*start=*end=atoi(port+1);
	}
	else if(!strncmp(port, "<", 1)) 
	{
		sprintf(g_buf, "1-%d", atoi(port+1) - 1);
		*start=1;
		*end=atoi(port+1);
	}
	else if(strptr=strchr(port, ':'))
	{		
		strcpy(itemname_arr, port);
		strptr = strchr(itemname_arr, ':');
		sprintf(g_buf, "%d-%d", atoi(itemname_arr), atoi(strptr+1));	
		*start=atoi(itemname_arr);
		*end=atoi(strptr+1);
	}
	else if(*port)
	{
		sprintf(g_buf, "%d-%d", atoi(port), atoi(port));
		*start=atoi(port);
		*end=atoi(port);
	}
	else
	{
		sprintf(g_buf, "");
		*start=0;
		*end=0;
	}
	
	return(g_buf_alloc(g_buf));
}

char *portrange_conv(char *port_name, int idx)
{
	char itemname_arr[32];
	
	sprintf(itemname_arr,"%s%d", port_name, idx);
	strcpy(g_buf, nvram_safe_get(itemname_arr));	
	
	return(g_buf_alloc(g_buf));
}

char *iprange_conv(char *ip_name, int idx)
{
	char *ip;
	char itemname_arr[32];
	char startip[16], endip[16];
	int i, j, k;
	
	sprintf(itemname_arr,"%s%d", ip_name, idx);
	ip=nvram_safe_get(itemname_arr);
	strcpy(g_buf, "");
	
	// scan all ip string
	i=j=k=0;
	
	while(*(ip+i))
	{
		if (*(ip+i)=='*') 
		{
			startip[j++] = '1';
			endip[k++] = '2';
			endip[k++] = '5';
			endip[k++] = '4';
			// 255 is for broadcast
		}
		else 
		{
			startip[j++] = *(ip+i);
			endip[k++] = *(ip+i);
		}
		i++;
	}	
	
	startip[j++] = 0;
	endip[k++] = 0;

	if (strcmp(startip, endip)==0)
		sprintf(g_buf, "%s", startip);
	else
		sprintf(g_buf, "%s-%s", startip, endip);
	return(g_buf_alloc(g_buf));
}

char *iprange_ex_conv(char *ip_name, int idx)
{
	char *ip;
	char itemname_arr[32];
	char startip[16], endip[16];
	int i, j, k;
	int mask;
	
	sprintf(itemname_arr,"%s%d", ip_name, idx);
	ip=nvram_safe_get(itemname_arr);
	strcpy(g_buf, "");
	
	// scan all ip string
	i=j=k=0;
	mask=32;
	
	while(*(ip+i))
	{
		if (*(ip+i)=='*') 
		{
			startip[j++] = '0';
			endip[k++] = '0';
			// 255 is for broadcast
			mask-=8;
		}
		else 
		{
			startip[j++] = *(ip+i);
			endip[k++] = *(ip+i);
		}
		i++;
	}	
	
	startip[j++] = 0;
	endip[k++] = 0;

	if (mask==32)
		sprintf(g_buf, "%s", startip);
	else if(mask==0)
		strcpy(g_buf, "");
	else sprintf(g_buf, "%s/%d", startip, mask);

	return(g_buf_alloc(g_buf));
}

char *ip_conv(char *ip_name, int idx)
{	
	char itemname_arr[32];

	sprintf(itemname_arr,"%s%d", ip_name, idx);
	sprintf(g_buf, "%s", nvram_safe_get(itemname_arr));
	return(g_buf_alloc(g_buf));
}


char *general_conv(char *ip_name, int idx)
{	
	char itemname_arr[32];

	sprintf(itemname_arr,"%s%d", ip_name, idx);
	sprintf(g_buf, "%s", nvram_safe_get(itemname_arr));
	return(g_buf_alloc(g_buf));
}

char *filter_conv(char *proto, char *flag, char *srcip, char *srcport, char *dstip, char *dstport)
{
	char newstr[64];

	//printf("filter : %s,%s,%s,%s,%s,%s\n", proto, flag, srcip, srcport, dstip, dstport);
	
	strcpy(g_buf, "");		
				                        			
        if (strcmp(proto, "")!=0)
	{
		sprintf(newstr, " -p %s", proto);
        	strcat(g_buf, newstr);
        }				
            			
        if (strcmp(flag, "")!=0)
        {
//        	sprintf(newstr, " --tcp-flags %s RST", flag);
		sprintf(newstr, " --tcp-flags %s %s", flag, flag);
		strcat(g_buf, newstr);
	}			
                 
	if (strcmp(srcip, "")!=0)
	{
		if (strchr(srcip , '-'))
			sprintf(newstr, " --src-range %s", srcip);
		else	
			sprintf(newstr, " -s %s", srcip);
		strcat(g_buf, newstr);
	}				
            			
	if (strcmp(srcport, "")!=0)
	{
		sprintf(newstr, " --sport %s", srcport);
		strcat(g_buf, newstr);
        }			
            			
        if (strcmp(dstip, "")!=0)
        {
		if (strchr(dstip, '-'))
			sprintf(newstr, " --dst-range %s", dstip);
		else	
        		sprintf(newstr, " -d %s", dstip);
		strcat(g_buf, newstr);
        }
			
        if (strcmp(dstport, "")!=0)
        {
        	sprintf(newstr, " --dport %s", dstport);
		strcat(g_buf, newstr);
	}
	return(g_buf_alloc(g_buf));
	//printf("str: %s\n", g_buf);
}

void timematch_conv(char *mstr, char *nv_date, char *nv_time)
{	
	char *datestr[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	char timestart[6], timestop[6];
	char *time, *date;
	int i, head;

	date = nvram_safe_get(nv_date);
	time = nvram_safe_get(nv_time);

	if (strlen(date)!=7||strlen(time)!=8) goto no_match;

	if (strncmp(date, "1111111", 7)==0 &&
	    strncmp(time, "00002359", 8)==0) goto no_match;
	
	i=0;
	strncpy(timestart, time, 2);
	i+=2;
	timestart[i++] = ':';
	strncpy(timestart+i, time+2, 2);
	i+=2;
	timestart[i]=0;
	i=0;
	strncpy(timestop, time+4, 2);
	i+=2;
	timestop[i++] = ':';
	strncpy(timestop+i, time+6, 2);
	i+=2;
	timestop[i]=0;

	sprintf(mstr, "-m time --timestart %s:00 --timestop %s:00 --days ",
			timestart, timestop);

	head=1;

	for(i=0;i<7;i++)
	{
		if (*(date+i)=='1')
		{
			if (head)
			{
			    sprintf(mstr, "%s %s", mstr, datestr[i]);
			    head=0;
			}
			else
			{	
			    sprintf(mstr, "%s,%s", mstr, datestr[i]);
			}
		}
	}
	return;
	
no_match:
	sprintf(mstr, "");
	return;
}

void
p(int step)
{
	dprintf("P: %d %s\n", step, g_buf);
}

void 
ip2class(char *lan_ip, char *netmask, char *buf)
{
	unsigned int val, ip;
	struct in_addr in;
	int i=0;

	// only handle class A,B,C	
	val = (unsigned int)inet_addr(netmask);
	ip = (unsigned int)inet_addr(lan_ip);
	in.s_addr = ip & val;
	if (val==0xff00000) sprintf(buf, "%s/8", inet_ntoa(in));
	else if (val==0xffff0000) sprintf(buf, "%s/16", inet_ntoa(in));
	else sprintf(buf, "%s/24", inet_ntoa(in));
	dprintf(buf);	
}

void convert_routes(void)
{
	/* Disable Static if it's not enable */	
	if (nvram_match("sr_enable_x", "0"))
	{
		nvram_set("lan_route", "");
		nvram_set("wan0_route", "");
		return;
	}

	int i;
	char *ip, *netmask, *gateway, *matric, *interface;
	char wroutes[400], lroutes[400];

	wroutes[0] = 0;
	lroutes[0] = 0;	

	g_buf_init();
			
	foreach_x("sr_num_x")
	{
		ip = general_conv("sr_ipaddr_x", i);
		netmask = general_conv("sr_netmask_x", i);
		gateway = general_conv("sr_gateway_x", i);
		matric = general_conv("sr_matric_x", i);
		interface = general_conv("sr_if_x", i);


		dprintf("%x %s %s %s %s %s\n", i, ip, netmask, gateway, matric, interface);

		if (!strcmp(interface, "WAN"))
		{		
			sprintf(wroutes, "%s %s:%s:%s:%d", wroutes, ip, netmask, gateway, atoi(matric)+1);
		}
		else if (!strcmp(interface, "LAN"))
		{
			sprintf(lroutes, "%s %s:%s:%s:%d", lroutes, ip, netmask, gateway, atoi(matric)+1);
		}	
	}

	//printf("route: %s %s\n", lroutes, wroutes);
	nvram_set("lan_route", lroutes);
	nvram_set("wan0_route", wroutes);
}

void write_static_leases(char *file)
{
	FILE *fp;
	char *ip, *mac;
	int i;

	fp=fopen(file, "w");

	if (fp==NULL) return;
	
	g_buf_init();
			
       	foreach_x("dhcp_staticnum_x")
       	{	               			
            	ip = general_conv("dhcp_staticip_x", i);
		mac = general_conv("dhcp_staticmac_x", i);
		fprintf(fp, "%s,%s\r\n", ip, mac);
	}
	fclose(fp);
}

#ifndef NOIPTABLES
void
write_upnp_forward(FILE *fp, FILE *fp1, char *wan_if, char *wan_ip, char *lan_if, char *lan_ip, char *lan_class, char *logaccept, char *logdrop)
{
	char name[] = "forward_portXXXXXXXXXX", value[512];
        char *wan_port0, *wan_port1, *lan_ipaddr, *lan_port0, *lan_port1, *proto;
        char *enable, *desc;
	int i;

	/* Set wan_port0-wan_port1>lan_ipaddr:lan_port0-lan_port1,proto,enable,desc */
	for(i=0 ; i<15 ; i++)
	{
		snprintf(name, sizeof(name), "forward_port%d", i);

		strncpy(value, nvram_safe_get(name), sizeof(value));

		/* Check for LAN IP address specification */
		lan_ipaddr = value;
		wan_port0 = strsep(&lan_ipaddr, ">");
		if (!lan_ipaddr)
			continue;

		/* Check for LAN destination port specification */
		lan_port0 = lan_ipaddr;
		lan_ipaddr = strsep(&lan_port0, ":");
		if (!lan_port0)
			continue;

		/* Check for protocol specification */
		proto = lan_port0;
		lan_port0 = strsep(&proto, ":,");
		if (!proto)
			continue;

		/* Check for enable specification */
		enable = proto;
		proto = strsep(&enable, ":,");
		if (!enable)
			continue;

		/* Check for description specification (optional) */
		desc = enable;
		enable = strsep(&desc, ":,");

		/* Check for WAN destination port range (optional) */
		wan_port1 = wan_port0;
		wan_port0 = strsep(&wan_port1, "-");
		if (!wan_port1)
			wan_port1 = wan_port0;

		/* Check for LAN destination port range (optional) */
		lan_port1 = lan_port0;

		lan_port0 = strsep(&lan_port1, "-");
	        if (!lan_port1)
			lan_port1 = lan_port0;

		/* skip if it's disabled */
		if( strcmp(enable, "off") == 0 )
			continue;

		/* -A PREROUTING -p tcp -m tcp --dport 823 -j DNAT 
		                 --to-destination 192.168.1.88:23  */
		if( !strcmp(proto,"tcp") || !strcmp(proto,"both") )
		{
	   		fprintf(fp, "-A PREROUTING -p tcp -m tcp -d %s --dport %s "
				  "-j DNAT --to-destination %s:%s\n"
					, wan_ip, wan_port0, lan_ipaddr, lan_port0);

			fprintf(fp1, "-A FORWARD -p tcp "
				 "-m tcp -d %s --dport %s -j %s\n"
				 , lan_ipaddr, lan_port0, logaccept);
		}
		if( !strcmp(proto,"udp") || !strcmp(proto,"both") ){
	   		fprintf(fp, "-A PREROUTING -p udp -m udp -d %s --dport %s "
				  "-j DNAT --to-destination %s:%s\n"
				  	, wan_ip, wan_port0, lan_ipaddr, lan_port0);

			fprintf(fp1, "-A FORWARD -p udp -m udp -d %s --dport %s -j %s\n"
				 , lan_ipaddr, lan_port0, logaccept);
		}
	}
}

char *ipoffset(char *ip, int offset, char *tmp)
{
	unsigned int ip1, ip2, ip3, ip4;

	sscanf(ip, "%d.%d.%d.%d", &ip1, &ip2, &ip3, &ip4);
	sprintf(tmp, "%d.%d.%d.%d", ip1, ip2, ip3, ip4+offset);

	dprintf("ip : %s\n", tmp);
	return(tmp);
}

nat_setting(char *wan_if, char *wan_ip, char *lan_if, char *lan_ip, char *logaccept, char *logdrop)
{
	FILE *fp, *fp1;
        char *proto, *flag, *srcip, *srcport, *dstip, *dstport;
        char *setting, lan_class[32];
	int i;
	int wan_port;
	char *internal_ip, *internal_num;
	int num;
	char dstips[32], dstports[12];
     				
	
	if ((fp=fopen("/tmp/nat_rules", "w"))==NULL) return -1;
	if ((fp1=fopen("/tmp/nat_forward_rules", "w"))==NULL) return -1;

	fprintf(fp, "*nat\n"
	       	":PREROUTING ACCEPT [0:0]\n"
	       	":POSTROUTING ACCEPT [0:0]\n"
	  	":OUTPUT ACCEPT [0:0]\n");

	//Log	
   	//if(nvram_match("misc_natlog_x", "1"))
   	// 	fprintf(fp, "-A PREROUTING -i %s -j LOG --log-prefix ALERT --log-level 4\n", wan_if);

   	if (nvram_match("misc_http_x", "1"))
	{
		wan_port=8080;
   		if (nvram_invmatch("misc_httpport_x", ""))
      			wan_port=atoi(nvram_safe_get("misc_httpport_x")); 	
   		fprintf(fp, "-A PREROUTING -p tcp -m tcp -d %s --dport %d -j DNAT --to-destination %s:80\n", wan_ip, wan_port, lan_ip);
	}
	
   	if (nvram_match("apps_dl_share", "1"))
	{
   		fprintf(fp, "-A PREROUTING -p tcp -m tcp -d %s --dport %s:%s -j DNAT --to %s\n", wan_ip, nvram_safe_get("apps_dl_share_port_from"), nvram_safe_get("apps_dl_share_port_to"), lan_ip);
   		fprintf(fp, "-A PREROUTING -p udp -m udp -d %s --dport %s:%s -j DNAT --to %s\n", wan_ip, nvram_safe_get("apps_dl_share_port_from"), nvram_safe_get("apps_dl_share_port_to"), lan_ip);
	}

   	if (nvram_match("wan_nat_X", "1")&&nvram_invmatch("upnp_ENABLE", "0"))
   	{     		
		// upnp port forward
        	write_upnp_forward(fp, fp1, wan_if, wan_ip, lan_if, lan_ip, lan_class, logaccept, logdrop);
	}

	// Port forwarding or Virtual Server
   	if (nvram_match("wan_nat_X", "1") && nvram_match("vts_enable_x", "1"))
   	{     		
   		g_buf_init();
   				     		
     		foreach_x("vts_num_x")
     		{
     			char *proto;
			char *protono;
			char *port;
			char *lport;
			char *dstip;

     			proto = proto_conv("vts_proto_x", i);
			protono = portrange_conv("vts_protono_x", i);
			port = portrange_conv("vts_port_x", i);
			lport = portrange_conv("vts_lport_x", i);
			dstip = ip_conv("vts_ipaddr_x", i);

			if (lport!=NULL && strlen(lport)!=0) 
			{
				sprintf(dstips, "%s:%s", dstip, lport);
				sprintf(dstports, "%s", lport);
			}
			else
			{
				sprintf(dstips, "%s:%s", dstip, port);
				sprintf(dstports, "%s", port);
			}
     				
			if (strcmp(proto, "tcp")==0 || strcmp(proto, "both")==0)
			{
				if (lport!=NULL && strlen(lport)!=0) 
				{
					fprintf(fp, "-A PREROUTING -p tcp -m tcp -d %s --dport %s -j DNAT --to-destination %s\n", 
					wan_ip, port, dstips);
				}
				else
				{
					fprintf(fp, "-A PREROUTING -p tcp -m tcp -d %s --dport %s -j DNAT --to %s\n", 
					wan_ip, port, dstip);
				}

				fprintf(fp1, "-A FORWARD -p tcp -m tcp -d %s --dport %s -j %s\n", 
					dstip, dstports, logaccept);
			}     				
				
			if (strcmp(proto, "udp")==0 || strcmp(proto, "both")==0)
			{
				if (lport!=NULL && strlen(lport)!=0) 
				{
					fprintf(fp, "-A PREROUTING -p udp -m udp -d %s --dport %s -j DNAT --to-destination %s\n", 
					wan_ip, port, dstips);
				}
				else
				{
					fprintf(fp, "-A PREROUTING -p udp -m udp -d %s --dport %s -j DNAT --to %s\n", 
					wan_ip, port, dstip);
				}
				fprintf(fp1, "-A FORWARD -p udp -m udp -d %s --dport %s -j %s\n", 
					dstip, dstports, logaccept);
			}     				        			
			if (strcmp(proto, "other")==0)
			{
				fprintf(fp, "-A PREROUTING -p %s -d %s -j DNAT --to %s\n",
					protono, wan_ip, dstip);

				fprintf(fp1, "-A FORWARD -p %s -d %s -j %s\n",
					protono, dstip, logaccept);
			}     				        				}	
	}	

   	if (nvram_match("wan_nat_X", "1") && nvram_invmatch("sp_battle_ips", "0"))
	{
		#define BASEPORT 6112
		#define BASEPORT_NEW 10000

		ip2class(lan_ip, nvram_safe_get("lan_netmask"), lan_class);

		/* run starcraft patch anyway */
		fprintf(fp, "-A PREROUTING -p udp -d %s --sport %d -j NETMAP --to %s\n", wan_ip, BASEPORT, lan_class);

		fprintf(fp, "-A POSTROUTING -p udp -s %s --dport %d -j NETMAP --to %s\n", lan_class, BASEPORT, wan_ip);

		fprintf(fp1, "-A FORWARD -p udp --dport %d -j %s\n",
					BASEPORT, logaccept);
	}

   	// Exposed station	
   	if (nvram_match("wan_nat_X", "1") && nvram_invmatch("dmz_ip", ""))
   	{   		
      		fprintf(fp, "-A PREROUTING -d %s -j DNAT --to %s\n", 
			wan_ip, nvram_safe_get("dmz_ip"));      

      		fprintf(fp1, "-A FORWARD -d %s -j %s\n", 
			nvram_safe_get("dmz_ip"), logaccept);      
      	}

	if (nvram_match("wan_nat_X", "1"))
	{
   		fprintf(fp, "-A POSTROUTING -o %s -j MASQUERADE\n", wan_if);

		// masquerade lan to lan
		ip2class(lan_ip, nvram_safe_get("lan_netmask"), lan_class);
		fprintf(fp, "-A POSTROUTING -o %s -s %s -d %s -j MASQUERADE\n", lan_if, lan_class, lan_class);
	}

	fprintf(fp, "COMMIT\n");
	
	fclose(fp);
	fclose(fp1);

	eval("iptables-restore", "/tmp/nat_rules");
}
/* Rules for LW Filter and MAC Filter
 * MAC ACCEPT
 *     ACCEPT -> MACS
 *            	 -> LW Disabled
 *                  MACS ACCEPT
 *               -> LW Default Accept: 
 *                  MACS DROP in rules
 *                  MACS ACCEPT Default
 *               -> LW Default Drop: 
 *                  MACS ACCEPT in rules
 *                  MACS DROP Default
 *     DROP    -> FORWARD DROP 
 *
 * MAC DROP
 *     DROP -> FORWARD DROP
 *     ACCEPT -> FORWARD ACCEPT 
 */

int
filter_setting(char *wan_if, char *wan_ip, char *lan_if, char *lan_ip, char *logaccept, char *logdrop)
{

	FILE *fp, *fp1;
        char *proto, *flag, *srcip, *srcport, *dstip, *dstport;
        char *setting, line[256];
	char macaccept[32], chain[3];
	char *ftype, *dtype, *fftype;
	int num;
	int i;
	int wan_port;
	
	if ((fp=fopen("/tmp/filter_rules", "w"))==NULL) return -1;

	fprintf(fp, "*filter\n:INPUT ACCEPT [0:0]\n:FORWARD ACCEPT [0:0]\n:OUTPUT ACCEPT [0:0]\n:MACS - [0:0]\n:logaccept - [0:0]\n:logdrop - [0:0]\n");

	if( nvram_match("wan0_proto", "bigpond"))
	{
		   fprintf(fp, "-I INPUT -d %s -i %s -p udp --dport %d -j %s\n"
		   , nvram_safe_get("wan0_ipaddr")
		   , nvram_safe_get("wan0_ifname")
		   , 5050
		   , "ACCEPT");
	}

	strcpy(macaccept, "");

	// FILTER from LAN to WAN Source MAC
	if(nvram_invmatch("macfilter_enable_x", "0"))
	{   		
		// LAN/WAN filter		
		g_buf_init();

		if (nvram_match("macfilter_enable_x", "2"))
		{
			dtype = logaccept;
			ftype = logdrop;
			fftype = logdrop;
		}
		else
		{
			dtype = logdrop;
			ftype = logaccept;

			strcpy(macaccept, "MACS");
			fftype = macaccept;
		}
	
		num = atoi(nvram_safe_get("macfilter_num_x"));

		for(i=0;i<num;i++)
		{	
         		fprintf(fp, "-A INPUT -i %s -m mac --mac-source %s -j %s\n", lan_if, mac_conv("macfilter_list_x", i, line), ftype);
         		fprintf(fp, "-A FORWARD -i %s -m mac --mac-source %s -j %s\n", lan_if, mac_conv("macfilter_list_x", i, line), fftype);

#ifdef GUEST_ACCOUNT			
#ifdef RANGE_EXTENDER
			if(nvram_match("wl_mode_EX", "re"))
			{
				// do nothing
			}
			else
#endif	
			if(nvram_match("wl_guest_ENABLE", "1") && nvram_match("wl_mode_EX", "ap") && nvram_match("wan_nat_X", "1"))
			{
         			fprintf(fp, "-A INPUT -i %s -m mac --mac-source %s -j %s\n", "wl0.1", mac_conv("macfilter_list_x", i, line), ftype);
				fprintf(fp, "-A FORWARD -i %s -m mac --mac-source %s -j %s\n", "wl0.1", mac_conv("macfilter_list_x", i, line), fftype);
			}
#endif
		} 
	} 

	if (nvram_invmatch("fw_enable_x", "1"))
	{
		if (nvram_match("macfilter_enable_x", "1"))
		{
			/* Filter known SPI state */
			fprintf(fp, "-A INPUT -i %s -m state --state NEW -j %s\n"
			,lan_if, logdrop);

#ifdef GUEST_ACCOUNT
#ifdef RANGE_EXTENDER
			if(nvram_match("wl_mode_EX", "re"))
			{
				// do nothing
			}
			else
#endif	
			if(nvram_match("wl_guest_ENABLE", "1") && nvram_match("wl_mode_EX", "ap") && nvram_match("wan_nat_X", "1"))
			{
				fprintf(fp, "-I INPUT -i %s -m state --state NEW -j %s\n"
				, "wl0.1", logdrop);
			}
#endif
		}
	}
	else
	{	
		if (nvram_match("macfilter_enable_x", "1"))
		{
			/* Filter known SPI state */
			fprintf(fp, "-A INPUT -m state --state INVALID -j %s\n"
		          "-A INPUT -m state --state RELATED,ESTABLISHED -j %s\n"
	        	  "-A INPUT -i lo -m state --state NEW -j %s\n"
		          "-A INPUT -i %s -m state --state NEW -j %s\n"
			,logdrop, logaccept, "ACCEPT", lan_if, logdrop);
#ifdef GUEST_ACCOUNT
#ifdef RANGE_EXTENDER
			if(nvram_match("wl_mode_EX", "re"))
			{
				// do nothing
			}
			else
#endif				
			if(nvram_match("wl_guest_ENABLE", "1") && nvram_match("wl_mode_EX", "ap") && nvram_match("wan_nat_X", "1"))
			{
				fprintf(fp, "-I INPUT -i %s -m state --state NEW -j %s\n"
				, "wl0.1", logdrop);
			}
#endif
		}
		else
		{
			/* Filter known SPI state */
			fprintf(fp, "-A INPUT -m state --state INVALID -j %s\n"
		          "-A INPUT -m state --state RELATED,ESTABLISHED -j %s\n"
	        	  "-A INPUT -i lo -m state --state NEW -j %s\n"
		          "-A INPUT -i %s -m state --state NEW -j %s\n"
			,logdrop, logaccept, "ACCEPT", lan_if, "ACCEPT");

#ifdef GUEST_ACCOUNT			
#ifdef RANGE_EXTENDER
			if(nvram_match("wl_mode_EX", "re"))
			{
				// do nothing
			}
			else
#endif	
			if(nvram_match("wl_guest_ENABLE", "1") && nvram_match("wl_mode_EX", "ap")  && nvram_match("wan_nat_X", "1"))
			{
				fprintf(fp, "-I INPUT -i %s -m state --state NEW -j %s\n"
				, "wl0.1", "ACCEPT");
			}
#endif
		}

	// Thanks for Oleg
	/* enable incoming packets from broken dhcp servers, which are sending replies
	 * from addresses other than used for query, this could lead to lower level
	 * of security, but it does not work otherwise (conntrack does not work) :-( 
	 */
	if (nvram_match("wan0_proto", "dhcp") || nvram_match("wan0_proto", "bigpond"))
	{
		fprintf(fp, "-A INPUT -p udp --sport 67 --dport 68 -j %s\n", logaccept);
	}
 
#ifdef GUEST_ACCOUNT			
#ifdef RANGE_EXTENDER
	if(nvram_match("wl_mode_EX", "re"))
	{
		// do nothing
	}
	else
#endif	
	if(nvram_match("wl_guest_ENABLE", "1") && nvram_match("wl_mode_EX", "ap") && nvram_match("wan_nat_X", "1"))
	{
		fprintf(fp, "-I INPUT -i %s -p tcp -m tcp --dport 80 -j %s\n", "wl0.1", logdrop);

	        //fprintf(fp, "-I INPUT -i %s -p icmp -j %s\n", "wl0.1", logdrop);
	        fprintf(fp, "-I FORWARD -i %s -o br0 -j %s\n", "wl0.1", logdrop);
	        fprintf(fp, "-I FORWARD -i br0 -o %s -j %s\n", "wl0.1", logdrop);

#ifdef USB_SUPPORT
		fprintf(fp, "-I INPUT -i %s -p tcp -m tcp --dport %s -j %s\n", "wl0.1", nvram_safe_get("usb_webhttpport_x"), logdrop);

		fprintf(fp, "-I INPUT -i %s -p tcp -m tcp --dport %s -j %s\n", "wl0.1", nvram_safe_get("usb_ftpport_x"), logdrop);

		fprintf(fp, "-I INPUT -i %s -p tcp -m tcp --dport %s -j %s\n", "wl0.1", nvram_safe_get("usb_webactivex_x"), logdrop);

		fprintf(fp, "-I INPUT -i %s -p tcp -m tcp --dport %s -j %s\n", "wl0.1", "515", logdrop);

		fprintf(fp, "-I INPUT -i %s -p tcp -m tcp --dport %s -j %s\n", "wl0.1", "9100", logdrop);

		fprintf(fp, "-I INPUT -i %s -p tcp -m tcp --dport %s -j %s\n", "wl0.1", "3838", logdrop);
#endif
	}
#endif

	// Firewall between WAN and Local
	if (nvram_match("misc_http_x", "1"))
	{
		fprintf(fp, "-A INPUT -p tcp -m tcp -d %s --dport 80 -j %s\n", nvram_safe_get("lan_ipaddr"), logaccept);
		//fprintf(fp, "-A INPUT -p tcp -m tcp -d %s --dport %s -j %s\n", wan_ip, nvram_safe_get("misc_httpport_x"), logaccept);
	}

	if (nvram_match("usb_webenable_x", "2"))
	{
		fprintf(fp, "-A INPUT -p tcp -m tcp -d %s --dport %s -j %s\n", wan_ip, nvram_safe_get("usb_webhttpport_x"), logaccept);

		fprintf(fp, "-A INPUT -p tcp -m tcp -d %s --dport %s -j %s\n", wan_ip, nvram_safe_get("usb_webactivex_x"), logaccept);
	}

	if (nvram_invmatch("usb_ftpenable_x", "0"))
	{	
		fprintf(fp, "-A INPUT -p tcp -m tcp -d %s --dport %s -j %s\n", wan_ip, nvram_safe_get("usb_ftpport_x"), logaccept);
	}

	if (nvram_invmatch("misc_ping_x", "0"))
	{
		fprintf(fp, "-A INPUT -p icmp -d %s -j %s\n", wan_ip, logaccept);
	}

	if (nvram_invmatch("misc_lpr_x", "0"))
	{
		fprintf(fp, "-A INPUT -p tcp -m tcp -d %s --dport %d -j %s\n", wan_ip, 515, logaccept);
		fprintf(fp, "-A INPUT -p tcp -m tcp -d %s --dport %d -j %s\n", wan_ip, 9100, logaccept);
		fprintf(fp, "-A INPUT -p tcp -m tcp -d %s --dport %d -j %s\n", wan_ip, 3838, logaccept);
	}

	fprintf(fp, "-A INPUT -j %s\n", logdrop);
	}

	/* thanks for Oleg */
	fprintf(fp, "-A FORWARD -m state --state ESTABLISHED,RELATED -j %s\n", logaccept);
	if (strlen(macaccept)>0)
		fprintf(fp, "-A MACS -m state --state ESTABLISHED,RELATED -j %s\n", logaccept);

	/* Drop the wrong state, INVALID, packets */
	fprintf(fp, "-A FORWARD -m state --state INVALID -j %s\n", logdrop);
	if (strlen(macaccept)>0)
		fprintf(fp, "-A MACS -m state --state INVALID -j %s\n", logdrop);

	/* Accept the redirect, might be seen as INVALID, packets */
	fprintf(fp, "-A FORWARD -i %s -o %s -j %s\n", lan_if, lan_if, logaccept);	
	if (strlen(macaccept)>0)
	{
		fprintf(fp, "-A MACS -i %s -o %s -j %s\n", lan_if, lan_if, logaccept);
	
#ifdef GUEST_ACCOUNT
#ifdef RANGE_EXTENDER
		if(nvram_match("wl_mode_EX", "re"))
		{
			// do nothing
		}
		else
#endif				
		if(nvram_match("wl_guest_ENABLE", "1") && nvram_match("wl_mode_EX", "ap") && nvram_match("wan_nat_X", "1"))
		{
			fprintf(fp, "-A MACS -i wl0.1 -o wl0.1 -j %s\n", logaccept);
		}
#endif
	}
	/* Clamp TCP MSS to PMTU of WAN interface */
	if( nvram_match("wan_proto", "pppoe"))
	{
		fprintf(fp, "-I FORWARD -p tcp --tcp-flags SYN,RST SYN -m tcpmss --mss %d: -j TCPMSS "
			  "--set-mss %d\n", atoi(nvram_safe_get("wan_pppoe_mtu"))-39, atoi(nvram_safe_get("wan_pppoe_mtu"))-40);
		
		if (strlen(macaccept)>0)
			fprintf(fp, "-A MACS -p tcp --tcp-flags SYN,RST SYN -m tcpmss --mss %d: -j TCPMSS "
			  "--set-mss %d\n", atoi(nvram_safe_get("wan_pppoe_mtu"))-39, atoi(nvram_safe_get("wan_pppoe_mtu"))-40);
	}

	if (nvram_match("wan_proto", "pptp"))
	{
		fprintf(fp, "-A FORWARD -p tcp --syn -j TCPMSS --clamp-mss-to-pmtu\n");
		if (strlen(macaccept)>0)
			fprintf(fp, "-A MACS -p tcp --syn -j TCPMSS --clamp-mss-to-pmtu\n");
 	}
	
	if (nvram_match("fw_enable_x", "1"))
	{
   		// DoS attacks   
   		// sync-flood protection	
   		fprintf(fp, "-A FORWARD -i %s -p tcp --syn -m limit --limit 1/s -j %s\n", wan_if, logaccept);
   		// furtive port scanner
   		fprintf(fp, "-A FORWARD -i %s -p tcp --tcp-flags SYN,ACK,FIN,RST RST -m limit --limit 1/s -j %s\n", wan_if, logaccept);
   		// ping of death
   		fprintf(fp, "-A FORWARD -i %s -p icmp --icmp-type echo-request -m limit --limit 1/s -j %s\n", wan_if, logaccept);
	}


	// FILTER from LAN to WAN
        // Rules for MAC Filter and LAN to WAN Filter
        // Drop rules always before Accept
	if(nvram_match("macfilter_enable_x", "1"))
		strcpy(chain, "MACS");
	else strcpy(chain, "FORWARD");
		
	if(nvram_match("fw_lw_enable_x", "1"))
	{   		
		char lanwan_timematch[128];
		char ptr[32], *icmplist;
		char *ftype, *dtype;

		timematch_conv(lanwan_timematch, "filter_lw_date_x", "filter_lw_time_x");	   
 
		if (nvram_match("filter_lw_default_x", "DROP"))
		{
			dtype = logdrop;
			ftype = logaccept;

		}
		else
		{
			dtype = logaccept;
			ftype = logdrop;
		}
			
		// LAN/WAN filter		
		g_buf_init();

       		foreach_x("filter_lw_num_x")
       		{	               			
            		proto = protoflag_conv("filter_lw_proto_x", i, 0);
            		flag = protoflag_conv("filter_lw_proto_x", i, 1);
			srcip = iprange_ex_conv("filter_lw_srcip_x", i);
			srcport = portrange_conv("filter_lw_srcport_x", i);
			dstip = iprange_ex_conv("filter_lw_dstip_x", i);
			dstport = portrange_conv("filter_lw_dstport_x", i);	
			setting=filter_conv(proto, flag, srcip, srcport, dstip, dstport); 
         		fprintf(fp, "-A %s %s -i %s -o %s %s -j %s\n", chain, lanwan_timematch, lan_if, wan_if, setting, ftype);
		}            

       		// ICMP       	       			       			
       		foreach(ptr, nvram_safe_get("filter_lw_icmp_x"), icmplist)
		{	   	 		
	        	fprintf(fp, "-A %s %s -i %s -o %s -p icmp --icmp-type %s -j %s\n", chain, lanwan_timematch, lan_if, wan_if, ptr, ftype);
	        }	

		// Default
	        fprintf(fp, "-A %s -i %s -o %s -j %s\n", chain, lan_if, wan_if, dtype);
	} 
	else if (nvram_match("macfilter_enable_x", "1"))
	{
         	fprintf(fp, "-A FORWARD -i %s -o %s -j %s\n", lan_if, wan_if, logdrop);
         	fprintf(fp, "-A MACS -i %s -o %s -j %s\n", lan_if, wan_if, logaccept);
	}

	// Filter from WAN to LAN
	if(nvram_match("fw_wl_enable_x", "1"))
	{   		
		char wanlan_timematch[128];
		char ptr[32], *icmplist;
		char *dtype, *ftype;

		timematch_conv(wanlan_timematch, "filter_wl_date_x", "filter_wl_time_x");
		g_buf_init();
	
		if (nvram_match("filter_wl_default_x", "DROP"))
		{
			dtype = logdrop;
			ftype = logaccept;
		}
		else
		{
			dtype = logaccept;
			ftype = logdrop;
		}
			
       		foreach_x("filter_wl_num_x")
       		{	               			
            		proto = protoflag_conv("filter_wl_proto_x", i, 0);
            		flag = protoflag_conv("filter_wl_proto_x", i, 1);
			srcip = iprange_ex_conv("filter_wl_srcip_x", i);
			srcport = portrange_conv("filter_wl_srcport_x", i);
			dstip = iprange_ex_conv("filter_wl_dstip_x", i);
			dstport = portrange_conv("filter_wl_dstport_x", i);	
			setting=filter_conv(proto, flag, srcip, srcport, dstip, dstport);

         		fprintf(fp, "-A FORWARD %s -i %s -o %s %s -j %s\n", wanlan_timematch, wan_if, lan_if, setting, ftype);
		}            
       			 
       		// ICMP       	       			       			
       		foreach(ptr, nvram_safe_get("filter_wl_icmp_x"), icmplist)
		{	   	 		
	        	fprintf(fp, "-A FORWARD %s -i %s -o %s -p icmp --icmp-type %s -j %s\n", wanlan_timematch, wan_if, lan_if, ptr, ftype);
	        }	
         	
		// thanks for Oleg
		// Default
		// fprintf(fp, "-A FORWARD -i %s -o %s -j %s\n", wan_if, lan_if, dtype);
       	}


	/* Write forward chain rules of NAT */
	if ((fp1 = fopen("/tmp/nat_forward_rules", "r"))!=NULL)
	{
		while(fgets(line, sizeof(line), fp1))
		{
			fprintf(fp, "%s", line);
		}

		fclose(fp1);
	}

	if(nvram_match("fw_wl_enable_x", "1")) // Thanks for Oleg
	{   		
		// Default
		fprintf(fp, "-A FORWARD -i %s -o %s -j %s\n", wan_if, lan_if, 
			nvram_match("filter_wl_default_x", "DROP") ? logdrop : logaccept);
	}

	/* logaccept chain */
	fprintf(fp, "-A logaccept -m state --state NEW -j LOG --log-prefix \"ACCEPT \" "
		  "--log-tcp-sequence --log-tcp-options --log-ip-options\n"
		  "-A logaccept -j ACCEPT\n");

	/* logdrop chain */
	fprintf(fp,"-A logdrop -m state --state NEW -j LOG --log-prefix \"DROP\" "
		  "--log-tcp-sequence --log-tcp-options --log-ip-options\n"
		  "-A logdrop -j DROP\n");
	
	fprintf(fp, "COMMIT\n\n");
       	fclose(fp);

	eval("iptables-restore", "/tmp/filter_rules");
}

int porttrigger_setting()
{
	netconf_app_t apptarget, *app;
	int i;
	char *out_proto, *in_proto, *out_port, *in_port, *desc;
	int  out_start, out_end, in_start, in_end;

	if (nvram_invmatch("wan_nat_X", "1") ||
	    nvram_invmatch("autofw_enable_x", "1")) 
		return -1;
	
	g_buf_init();
   				     		
     	foreach_x("autofw_num_x")
     	{	
     		out_proto = proto_conv("autofw_outproto_x", i);
     		out_port = portrange_ex2_conv("autofw_outport_x", i, &out_start, &out_end);
     		in_proto = proto_conv("autofw_inproto_x", i);
		in_port = portrange_ex2_conv("autofw_inport_x", i, &in_start, &in_end);
		desc = general_conv("autofw_desc_x", i);

		app = &apptarget;
		memset(app, 0, sizeof(netconf_app_t));

		/* Parse outbound protocol */
		if (!strncasecmp(out_proto, "tcp", 3))
			app->match.ipproto = IPPROTO_TCP;
		else if (!strncasecmp(out_proto, "udp", 3))
			app->match.ipproto = IPPROTO_UDP;
		else continue;

		/* Parse outbound port range */
		app->match.dst.ports[0] = htons(out_start);
		app->match.dst.ports[1] = htons(out_end);

		/* Parse related protocol */
		if (!strncasecmp(in_proto, "tcp", 3))
			app->proto = IPPROTO_TCP;
		else if (!strncasecmp(in_proto, "udp", 3))
			app->proto = IPPROTO_UDP;
		else continue;

		/* Parse related destination port range */
		app->dport[0] = htons(in_start);
		app->dport[1] = htons(in_end);

		/* Parse mapped destination port range */
		app->to[0] = htons(in_start);
		app->to[1] = htons(in_end);

		/* Parse description */
		if (desc)
			strncpy(app->desc, desc, sizeof(app->desc));

		/* Set interface name (match packets entering LAN interface) */
		strncpy(app->match.in.name, nvram_safe_get("lan_ifname"), IFNAMSIZ);

		/* Set LAN source port range (match packets from any source port) */
		app->match.src.ports[1] = htons(0xffff);

		/* Set target (application specific port forward) */
		app->target = NETCONF_APP;

		if (valid_autofw_port(app))
		{	
			netconf_add_fw((netconf_fw_t *)app);
		}
	}
}

int
start_firewall_ex(char *wan_if, char *wan_ip, char *lan_if, char *lan_ip)
{
	DIR *dir;
	struct dirent *file;
	FILE *fp;
	char name[NAME_MAX];
	char logaccept[32], logdrop[32];
	
	/* Block obviously spoofed IP addresses */
	if (!(dir = opendir("/proc/sys/net/ipv4/conf")))
		perror("/proc/sys/net/ipv4/conf");
	while (dir && (file = readdir(dir))) {
		if (strncmp(file->d_name, ".", NAME_MAX) != 0 &&
		    strncmp(file->d_name, "..", NAME_MAX) != 0) {
			sprintf(name, "/proc/sys/net/ipv4/conf/%s/rp_filter", file->d_name);
			if (!(fp = fopen(name, "r+"))) {
				perror(name);
				break;
			}
			fputc('1', fp);
			fclose(fp);
		}
	}
	closedir(dir);


	/* Determine the log type */
	if (nvram_match("fw_log_x", "accept") || nvram_match("fw_log_x", "both"))
		strcpy(logaccept, "logaccept");
	else strcpy(logaccept, "ACCEPT");

	if (nvram_match("fw_log_x", "drop") || nvram_match("fw_log_x", "both"))
		strcpy(logdrop, "logdrop");
	else strcpy(logdrop, "DROP");
		
	/* nat setting */
	nat_setting(wan_if, wan_ip, lan_if, lan_ip, logaccept, logdrop);

	/* Filter setting */
	filter_setting(wan_if, wan_ip, lan_if, lan_ip, logaccept, logdrop);

	/* Trigger port setting */
	porttrigger_setting();

	/* Add max conntrack as 4096, thanks for Oleg */
	if ((fp=fopen("/proc/sys/net/ipv4/ip_conntrack_max", "r+")))
	{
		if (nvram_get("misc_conntrack_x")==NULL) fputs("4096", fp);
		else fputs(nvram_safe_get("misc_conntrack_x"), fp);
		fclose(fp);
	}

#ifdef XBOX_SUPPORT
	if( (fp=fopen("/proc/sys/net/ipv4/ip_conntrack_udp_timeouts", "r+")) ){
		fprintf(fp, "%d %d", 65, 180);
		fclose(fp);
	} else
		perror("/proc/sys/net/ipv4/ip_conntrack_udp_timeouts");
#endif

	if( (fp=fopen("/proc/sys/net/ipv4/ip_forward", "r+")) ){
		fputc('1', fp);
		fclose(fp);
	} else
		perror("/proc/sys/net/ipv4/ip_forward");


	if ((fp=fopen("/proc/sys/net/ipv4/tcp_max_syn_backlog", "w+")))
	{
		fputs("256", fp);
		fclose(fp);
	}

	if ((fp=fopen("/proc/sys/net/ipv4/conf/all/accept_redirects", "w+")))
	{
		fputs("0", fp);
		fclose(fp);
	}

	if ((fp=fopen("/proc/sys/net/ipv4/conf/all/rp_filter", "w+")))
	{
		fputs("1", fp);
		fclose(fp);
	}

	if ((fp=fopen("/proc/sys/net/ipv4/conf/all/send_redirects", "w+")))
	{
		fputs("0", fp);
		fclose(fp);
	}

	if ((fp=fopen("/proc/sys/net/ipv4/tcp_fin_timeout", "w+")))
	{
		fputs("30", fp);
		fclose(fp);
	}

	if ((fp=fopen("/proc/sys/net/ipv4/tcp_keepalive_intvl", "w+")))
	{
		fputs("30", fp);
		fclose(fp);
	}

	if ((fp=fopen("/proc/sys/net/ipv4/tcp_keepalive_probes", "w+")))
	{
		fputs("3", fp);
		fclose(fp);
	}

	if ((fp=fopen("/proc/sys/net/ipv4/tcp_keepalive_time", "w+")))
	{
		fputs("1800", fp);
		fclose(fp);
	}

	if ((fp=fopen("/proc/sys/net/ipv4/tcp_retries2", "w+")))
	{
		fputs("5", fp);
		fclose(fp);
	}

	if ((fp=fopen("/proc/sys/net/ipv4/tcp_rfc1337", "w+")))
	{
		fputs("1", fp);
		fclose(fp);
	}

	if ((fp=fopen("/proc/sys/net/ipv4/tcp_syn_retries", "w+")))
	{
		fputs("3", fp);
		fclose(fp);
	}

	if ((fp=fopen("/proc/sys/net/ipv4/tcp_synack_retries", "w+")))
	{
		fputs("3", fp);
		fclose(fp);
	}

	if ((fp=fopen("/proc/sys/net/ipv4/tcp_syncookies", "w+")))
	{
		fputs("1", fp);
		fclose(fp);
	}

	if ((fp=fopen("/proc/sys/net/ipv4/tcp_timestamps", "w+")))
	{
		fputs("0", fp);
		fclose(fp);
	}

	if ((fp=fopen("/proc/sys/net/ipv4/tcp_tw_recycle", "w+")))
	{
		fputs("1", fp);
		fclose(fp);
	}

	if ((fp=fopen("/proc/sys/net/ipv4/tcp_tw_reuse", "w+")))
	{
		fputs("1", fp);
		fclose(fp);
	}

	if ((fp=fopen("/proc/sys/net/ipv4/tcp_abort_on_overflow", "w+")))
	{
		fputs("1", fp);
		fclose(fp);
	}


	return ;
}


portmapping_main(int argc, char *argv[])
{	
	char actionname[32], portname[32], ipname[32];
	// Check wan interface
	// argv[1] = Set or Unset	
	// argv[2] = Port
	// argv[3] = IP
	// argv[4] = Item
	sprintf(actionname, "4_MappedAction_%S", argv[4]);
	sprintf(ipname, "4_MappedIP_%s", argv[4]);
	sprintf(portname, "4_MappedInPort_%s", argv[4]); 
	
	if (strcmp(argv[1], "Set")==0)
	{		
		nvram_set(actionname, argv[1]);
		nvram_set(portname, argv[2]);
		nvram_set(ipname, argv[3]);
	}
	else
	{
		nvram_set(actionname, argv[1]);
	}
	
	if (nvram_match("wan_proto", "pppoe"))
	{
		start_firewall_ex("ppp0", "", "br0", "");
	}	
	else
	{
		if (nvram_match("wl_mode_EX", "ap"))
			start_firewall_ex("eth0", "", "br0", "");
		else
			start_firewall_ex("eth1", "", "br0", "");
	}
}

/*
#ifdef USB_SUPPORT
void write_ftp_banip(FILE *fp)
{
	char *ip;
	int i;

	g_buf_init();
			
       	foreach_x("usb_bannum_x")
       	{	               			
            	ip = general_conv("usb_ftpbanip_x", i);
		fprintf(fp, "ban=%s\n", ip);
	}
}


void write_ftp_userlist(FILE *fp)
{
	char *user, *pass, *max, *rights;
	int i, maxuser;
	char passwd[32];
	char dir[64];

	g_buf_init();

	mkdir_if_none("/tmp/harddisk/ftp_pub");
	mkdir_if_none("/tmp/harddisk/ftp_pvt");
		
       	foreach_x("usb_ftpnum_x")
       	{	               			
            	user = general_conv("usb_ftpusername_x", i);
            	pass = general_conv("usb_ftppasswd_x", i);
            	max = general_conv("usb_ftpmaxuser_x", i);
            	rights = general_conv("usb_ftprights_x", i);

		if (strlen(max)==0) maxuser=0;
		else maxuser=atoi(max);

		if (strlen(pass)==0) strcpy(passwd, "*");
		else strcpy(passwd, pass);

		if (strcmp(rights, "Private")==0)
		{
			fprintf(fp, "user=%s %s /ftp_pvt/%s/ %d A\n", user, passwd, user, maxuser);
			sprintf(dir, "/tmp/harddisk/ftp_pvt/%s", user);
			mkdir_if_none(dir);
		}
		else if (strcmp(rights, "Private(WO)")==0)
		{
			fprintf(fp, "user=%s %s /ftp_pvt/%s/ %d U\n", user, passwd, user, maxuser);
			sprintf(dir, "/tmp/harddisk/ftp_pvt/%s", user);
			mkdir_if_none(dir);
		}
		else if (strcmp(rights, "Read/Write/Erase")==0)
			fprintf(fp, "user=%s %s /ftp_pub/ %d A\n", user, passwd, maxuser);
		else if (strcmp(rights, "Read/Write")==0)
			fprintf(fp, "user=%s %s /ftp_pub/ %d DUM\n", user, passwd, maxuser);
		else if (strcmp(rights, "Read Only")==0)
			fprintf(fp, "user=%s %s /ftp_pub/ %d DM\n", user, passwd, maxuser);
		else if (strcmp(rights, "Write Only")==0)
			fprintf(fp, "user=%s %s /ftp_pub/ %d U\n", user, passwd, maxuser);
		else	fprintf(fp, "user=%s %s /ftp_pub/ %d -\n", user, passwd, maxuser);
	}
}
#endif
*/
#endif
