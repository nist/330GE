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
#ifdef QOS
#include<stdio.h>
#include<stdlib.h>
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

#ifdef ASUS_RC
#include<asusbcmnvram.h>
#else
#include <epivers.h>
#include<bcmnvram.h>
#endif

#include <shutils.h>

/***************************************************************************************/
//  The definitions of all partations
//  1:1 first partation
//    1:10 partations for user specify, the child partations are 10:70, 10:71...etc.
//    1:20 partation for short ack
//    1:30 partation for psh ack
//    1:40 partation for tos flag
//    1:50 partation for download master and ftp on router
//    1:60 default class
/***************************************************************************************/


char tar[32];

char *Ch_conv(char *proto_name, int idx)
{
	char *proto;
	char qos_name_x[32];
	sprintf(qos_name_x, "%s%d", proto_name, idx);
	if (nvram_match(qos_name_x,""))
	{
		return NULL;
	}
	else 
	{
		proto=nvram_get(qos_name_x);
		return proto;
	}
}

char *num_conv(int idx)
{
	sprintf(tar, "%d", idx);
	return(tar);
}

void tc_qdisc(char *move, char *network, char *idx)
{
	char flowid[32];
	sprintf(flowid, "%s%s", "1", idx);
	eval("tc","qdisc",move,"dev",network,"root","handle","1:","htb", "default",flowid);
}
void tc_class_global(char *network, char *idx, char *minBW, char *maxBW, char *prio_level)
{
	char flowid[32];
        char filter_handle[32];
	char handle[32];

        sprintf(flowid, "%s%s", "1:", idx);
        sprintf(handle, "%s%s%s", "1", idx, ":");
        sprintf(filter_handle, "%s", idx);

        eval("tc","class","add","dev",network,"parent","1:10","classid",flowid,"htb","rate",minBW,"ceil",maxBW, "prio", prio_level);
        eval("tc","qdisc","add","dev",network,"parent",flowid,"handle",handle,"pfifo");
        eval("tc","filter","add","dev",network,"parent","1:", "protocol", "ip", "prio", prio_level,"handle", filter_handle, "fw", "classid", flowid);
#ifdef QOS_DEBUG
        syslog(LOG_NOTICE, "tc class add dev %s parent 1:10 classid %s htb rate %s ceil %s prio %s\n", network, flowid, minBW, maxBW, prio_level);
        syslog(LOG_NOTICE, "tc qdisc add dev %s parent %s handle %s pfifo\n", network, flowid, handle);
        syslog(LOG_NOTICE, "tc filter add dev %s parent 1: protocol ip prio %s handle %s fw classid %s\n", network, prio_level, filter_handle, flowid);
	printf("tc class add dev %s parent 1:10 classid %s htb rate %s ceil %s prio %s\n", network, flowid, minBW, maxBW, prio_level);
        printf("tc qdisc add dev %s parent %s handle %s pfifo\n", network, flowid, handle);
        printf("tc filter add dev %s parent 1: protocol ip prio %s handle %s fw classid %s\n", network, prio_level, filter_handle, flowid);
#endif

}
void tc_class(char *network, char *idx, char *minBW, char *maxBW, char *prio_level)
{
	char flowid[32];
        char filter_handle[32];
        char prio[32];
	char handle[32];

        sprintf(flowid, "%s%s", "1:11", idx);
        sprintf(handle, "%s%s%s", "11", idx, ":");
        sprintf(filter_handle, "%s%s", "11", idx);

        eval("tc","class","add","dev",network,"parent","1:10","classid",flowid,"htb","rate",minBW,"ceil",maxBW, "prio", prio_level);
        eval("tc","qdisc","add","dev",network,"parent",flowid,"handle",handle,"pfifo");
        eval("tc","filter","add","dev",network,"parent","1:", "protocol", "ip", "prio", prio_level,"handle", filter_handle, "fw", "classid", flowid);
#ifdef QOS_DEBUG
        syslog(LOG_NOTICE, "tc class add dev %s parent 1:1 classid %s htb rate %s ceil %s prio %s\n", network, flowid, minBW, maxBW, prio_level);
        syslog(LOG_NOTICE, "tc qdisc add dev %s parent %s handle %s pfifo\n", network, flowid, handle);
        syslog(LOG_NOTICE, "tc filter add dev %s parent 1: protocol ip prio %s handle %s fw classid %s\n", network, prio_level, filter_handle, flowid);
	printf("tc class add dev %s parent 1:1 classid %s htb rate %s ceil %s prio %s\n", network, flowid, minBW, maxBW, prio_level);
        printf("tc qdisc add dev %s parent %s handle %s pfifo\n", network, flowid, handle);
        printf("tc filter add dev %s parent 1: protocol ip prio %s handle %s fw classid %s\n", network, prio_level, filter_handle, flowid);
#endif

}



/************************************************************************************/
/* This function is to add the filter match user specify. For LAN IP address, the   */
/* interface should be br0. For WAN IP address, the interface should be eth0/ppp0   */
/************************************************************************************/

void tc_filter(char *network, char *ipaddr, char *port, char *idx)
{	
	char flowid[32];
	char filter_idx[32];
	sprintf(flowid, "%s%s", "1:1", idx);
	sprintf(filter_idx, "%s%s", "11", idx);
	FILE *fp=NULL;

	if ((fp=fopen("/tmp/mangle_rules", "a"))==NULL) return;
//	fprintf(fp, "*mangle\n");

	if (port == NULL)
	{
		fprintf(fp, "-A PREROUTING -i br0 -s %s -j MARK --set-mark %s\n", ipaddr, filter_idx);
/*
		eval("iptables", "-t", "mangle", "-A", "PREROUTING","-i", "br0", "-s", ipaddr,\
			"-j", "MARK", "--set-mark", filter_idx);
*/		
	}
	else if (ipaddr == NULL)
	{
		fprintf(fp, "-A PREROUTING -i br0 -p tcp --dport %s -j MARK --set-mark %s\n", port, filter_idx);
		fprintf(fp, "-A PREROUTING -i br0 -p udp --dport %s -j MARK --set-mark %s\n", port, filter_idx);
/*
		eval("iptables", "-t", "mangle", "-A", "PREROUTING", "-i", "br0","-p", "tcp",\
			"--dport", port, "-j", "MARK", "--set-mark", filter_idx);

		eval("iptables", "-t", "mangle", "-A", "PREROUTING", "-i", "br0","-p", "udp",\
			"--dport", port, "-j", "MARK", "--set-mark", filter_idx);
*/
	}
	else
	{
		fprintf(fp, "-A PREROUTING -i br0 -s %s -p tcp --dport %s -j MARK --set-mark %s\n", ipaddr, port, filter_idx);
		fprintf(fp, "-A PREROUTING -i br0 -s %s -p udp --dport %s -j MARK --set-mark %s\n",ipaddr, port, filter_idx);
/*
		eval("iptables", "-t", "mangle", "-A", "PREROUTING", "-i", "br0", "-s", ipaddr,\
			"-p", "tcp", "--dport", port, "-j", "MARK", "--set-mark", filter_idx);

		eval("iptables", "-t", "mangle", "-A", "PREROUTING", "-i", "br0", "-s", ipaddr,\
			"-p", "udp", "--dport", port, "-j", "MARK", "--set-mark", filter_idx);
*/			
	}
//	fprintf(fp, "COMMIT\n\n");
	fclose(fp);
//	eval("iptables-restore", "/tmp/mangle_rules");
}


/*************************************************************************************/
/* This function is called if global setting or common applicatin options are enable */
/*************************************************************************************/

void start_global_set(char *network){
	FILE *fp=NULL;

	if ((fp=fopen("/tmp/mangle_rules", "w"))==NULL) return;
	fprintf(fp, "*mangle\n");

	if (nvram_invmatch("qos_tos_prio","0"))
	{
	//	eval("iptables", "-t", "mangle", "-A", "POSTROUTING", "-p", "!", "icmp", "-m", "length","--length", ":256", "-j", "MARK", "--set-mark", "40");
		fprintf(fp, "-A POSTROUTING -p ! icmp -m tos --tos ! 0 -m length --length :512 -j MARK --set-mark 40\n");
/*
		eval("iptables", "-t", "mangle", "-A", "POSTROUTING", "-p", "!","icmp","-m", "tos","--tos", "!","0","-m","length","--length",":512", "-j", "MARK", "--set-mark", "40");
*/		
	}

	if (nvram_invmatch("qos_shortpkt_prio","0"))
	{
		fprintf(fp, "-A POSTROUTING -m length --length :128 -j MARK --set-mark 20\n");
/*
		eval("iptables", "-t", "mangle", "-A", "POSTROUTING", "-m", "length","--length", ":128",\
			 "-j", "MARK", "--set-mark", "20");
*/
	}

	if (nvram_invmatch("qos_pshack_prio","0"))
	{
		fprintf(fp, "-A POSTROUTING -p tcp --tcp-flags PSH PSH -m length --length :100 -j MARK --set-mark 30\n");
/*
		eval("iptables", "-t", "mangle", "-A", "POSTROUTING", "-p", "tcp", "--tcp-flags", "PSH", "PSH",\
			 "-m", "length", "--length", ":100", "-j", "MARK", "--set-mark", "30");
*/
	}
	
//	fprintf(fp, "COMMIT\n\n");
	fclose(fp);
//	eval("iptables-restore", "/tmp/mangle_rules");
}


void start_qos(char *wan_ipaddr)
{
	int rulenum=0;
	int idx_class=0, idx_filter=0, idx_class_S=0; // control index
	int idx=0;
	int ubw=0, other_ubw=0, userspec_ubw=0, service_ubw=0;
	int shortpkt_ubw=0, pshack_ubw=0, tospkt_ubw=0;
	char net_name[32];
	char qos_shortpkt_ubw[32], qos_other_ubw[32], qos_shortpkt_max[32], qos_pshack_ubw[32], qos_other_max[32], qos_tospkt_ubw[32];
	char qos_userspec_ubw[32];
	char qos_name[32];
	char qos_ubw[32], qos_service_ubw[32];
	char dfragment_size[32];
	FILE *fp=NULL;

	nvram_set("qos_XXX", "1");

	if(nvram_match("qos_enable", "1")) //Start QoS
	{
		rulenum=atoi(nvram_safe_get("qos_rulenum_x"))-1;

		/* Get interface name */
		if (nvram_match("wan0_proto", "pppoe") || nvram_match("wan0_proto","pptp"))
		strcpy (net_name, nvram_safe_get("wan0_pppoe_ifname"));
		else	
		strcpy (net_name, nvram_safe_get("wan0_ifname"));

		/* Reset all qdisc first */
		eval ("tc","qdisc","del", "dev", net_name, "root", "2>/dev/null");

		/* Add new qdisc for wan classify */
		eval("tc","qdisc","add", "dev", net_name, "root", "handle", "1:",\
			 "htb", "default", "60");

	#ifdef GPQOS	
		/* Add root class 1:1 and BW = unlimited */
		eval("tc", "class", "add", "dev", net_name, "parent", "1:", "classid", "1:1",\
			 "htb", "rate", "100000kbit", "ceil", "100000kbit");

		/* Add class 1:80 for local web access */
        	char lan_ipaddr[32];
	        sprintf(lan_ipaddr, "%s%s", nvram_get("lan_ipaddr"), "/24");

		eval("tc", "class", "add", "dev", net_name, "parent", "1:1", "classid", "1:80",\
			 "htb", "rate", "90000kbit", "ceil", "90000kbit");

		eval("tc", "qdisc", "add", "dev", net_name, "parent", "1:80", "handle", "180:", "pfifo");

		eval("tc", "filter", "add", "dev", net_name, "parent", "1:", "protocol", "ip", "prio", "100", \
			 "handle", "80", "fw", "classid", "1:80");

                //eval("iptables", "-t", "mangle", "-A", "POSTROUTING", "-p", "tcp", "--sport", "80",
//                eval("iptables", "-t", "mangle", "-A", "POSTROUTING",\
//                        "-d", lan_ipaddr, "-j", "MARK", "--set-mark", "80");
	#endif


		/* Add class for global settings / user specify / server services */

		// Decide all the max/min bandwidth for each classes
		ubw = atoi(nvram_get("qos_ubw"))*0.9;
		sprintf(qos_ubw, "%d%s", ubw, "kbit");
		

		if (ubw > 128)
		{
			if (nvram_invmatch("qos_pshack_prio", "0"))
			{ 
				pshack_ubw = 96; //24kbits(3kbytes)*4 = 96 means support 4 game users play at the same time
				nvram_set("qos_shortpkt_prio","1");//game need short ack at the same time
			}
			else
				pshack_ubw = 0;

			if (nvram_invmatch("qos_tos_prio", "0")) 
				tospkt_ubw = 32;//support 1 user on voip phone
			else
				tospkt_ubw = 0;
			
			other_ubw = ubw - pshack_ubw - tospkt_ubw;

			if (nvram_invmatch("qos_shortpkt_prio", "0"))
				shortpkt_ubw = other_ubw/3;
			else 
				shortpkt_ubw = 0;
			other_ubw = other_ubw - shortpkt_ubw;
		
			service_ubw = other_ubw*atoi(nvram_get("qos_service_ubw"))/100;
			sprintf(qos_service_ubw, "%d%s", service_ubw, "kbit");


			if (nvram_match("qos_service_enable", "1"))
				other_ubw -= service_ubw;

		} 
		else if (ubw > 64 && ubw < 128)
		{

			if (nvram_invmatch("qos_pshack_prio", "0"))
			{ 
				pshack_ubw = 48; //24kbits(3kbytes)*2 = 48 means support 2 game users play at the same time
				nvram_set("qos_shortpkt_prio","1");//game need short ack at the same time
			}
			else
				pshack_ubw = 0;

			if (nvram_invmatch("qos_tos_prio", "0")) 
				tospkt_ubw = 20;//support 1 user on voip phone
			else
				tospkt_ubw = 0;
			
			other_ubw = ubw - pshack_ubw - tospkt_ubw;

			if (nvram_invmatch("qos_shortpkt_prio", "0"))
				shortpkt_ubw = other_ubw/3;
			else 
				shortpkt_ubw = 0;
			other_ubw = other_ubw - shortpkt_ubw;
		
			service_ubw = other_ubw*atoi(nvram_get("qos_service_ubw"))/100;
			sprintf(qos_service_ubw, "%d%s", service_ubw, "kbit");


			if (nvram_match("qos_service_enable", "1"))
				other_ubw -= service_ubw;

		} 
		else
		{


			if (nvram_invmatch("qos_pshack_prio", "0"))
			{ 
				pshack_ubw = 24; //24kbits(3kbytes)*4 = 96 means support 4 game users play at the same time
				nvram_set("qos_shortpkt_prio","1");//game need short ack at the same time
			}
			else
				pshack_ubw = 0;

			if (nvram_invmatch("qos_tos_prio", "0")) 
				tospkt_ubw = 12;//support 1 user on voip phone
			else
				tospkt_ubw = 0;
			
			other_ubw = ubw - pshack_ubw - tospkt_ubw;

			if (nvram_invmatch("qos_shortpkt_prio", "0"))
				shortpkt_ubw = other_ubw/3;
			else 
				shortpkt_ubw = 0;
			other_ubw = other_ubw - shortpkt_ubw;
		
			service_ubw = other_ubw*atoi(nvram_get("qos_service_ubw"))/100;
			sprintf(qos_service_ubw, "%d%s", service_ubw, "kbit");


			if (nvram_match("qos_service_enable", "1"))
				other_ubw -= service_ubw;


		}
			sprintf (qos_shortpkt_ubw, "%d%s", shortpkt_ubw, "kbit");
			sprintf (qos_shortpkt_max, "%d%s", shortpkt_ubw, "kbit");
 			sprintf (qos_pshack_ubw, "%d%s", pshack_ubw, "kbit");
 			sprintf (qos_tospkt_ubw, "%d%s", tospkt_ubw, "kbit");
			sprintf (qos_other_max, "%d%s", other_ubw, "kbit");
			sprintf (qos_other_ubw, "%d%s", other_ubw, "kbit");



		eval("tc", "class", "add", "dev", net_name, "parent", "1:", "classid", "1:10",\
			 "htb", "rate", qos_ubw, "ceil", qos_ubw);

		// Add class for Global options, 1:20, 1:30 and 1:40.

		if (nvram_match("qos_global_enable", "1"))
		{
			if (nvram_invmatch("qos_shortpkt_prio","0"))
			{
				tc_class_global(net_name, "20", qos_shortpkt_ubw, qos_shortpkt_max,\
					 nvram_get("qos_shortpkt_prio"));
			}

			if(nvram_invmatch("qos_pshack_prio", "0"))
			{
				tc_class_global(net_name, "30", qos_pshack_ubw, qos_pshack_ubw,\
					 nvram_get("qos_pshack_prio"));
			}

			//voice 
			if( nvram_invmatch("qos_tos_prio", "0") )
			{
				tc_class_global(net_name, "40", qos_tospkt_ubw, qos_tospkt_ubw,\
					 nvram_get("qos_tos_prio"));
			}
		}

		// Add class for user specify, 10:110, 10:111...etc.
		if (nvram_match("qos_userspec_app","1"))
		{
			/* Determine the other upload bandwidth priority                               */
			/* This value should be Max = Total - short_ack(if reserve)                    */
			/* The minumum should be Min =(Total - short_ack - pshack)/(ulenum + default) */
 
			userspec_ubw = other_ubw/(rulenum+2) ;
			sprintf (qos_userspec_ubw, "%d%s", userspec_ubw, "kbit");

			/* Add path 1:10 for user specific */
			for(idx_class=0; idx_class < rulenum+1; idx_class++)
			{
				tc_class(net_name, num_conv(idx_class),\
					 qos_userspec_ubw, qos_other_max, Ch_conv("qos_prio_x", idx_class));
			}

			/* Add the default class. The bandwidth are share with user specific applications */
			eval("tc", "class", "add", "dev", net_name, "parent", "1:10", "classid","1:60", "htb",\
				 "rate", qos_userspec_ubw, "ceil", qos_other_max, "prio", "2");
        		eval("tc","qdisc","add","dev",net_name,"parent","1:60","handle","160:","pfifo");
		}
		else {
		// Add class for default class, 1:60(if no user specify)
			eval("tc", "class", "add", "dev", net_name, "parent", "1:10", "classid",\
				 "1:60", "htb", "rate", qos_other_ubw, "ceil", qos_other_max, "prio", "2");
        		eval("tc","qdisc","add","dev",net_name,"parent","1:60","handle","160:","pfifo");
		}

		
		// Add class for services on router 1:50
		if (nvram_match("qos_service_enable", "1"))
		{
			tc_class_global(net_name, "50", qos_service_ubw, qos_other_max, "4");

		}
		/* Enable filter for each settings */

		// Clean iptables
/*
		eval("iptables", "-F", "-t", "mangle");
*/
		if ((fp=fopen("/tmp/mangle_rules", "w"))==NULL) return;
		fprintf(fp, "*mangle\n");
		fprintf(fp, "-F\n");
		fprintf(fp, "COMMIT\n\n");
		fclose(fp);
		eval("iptables-restore", "/tmp/mangle_rules");

		/* Global Settings */
		if(nvram_match("qos_global_enable","1"))
			start_global_set(net_name);

	#ifdef GPQOS

		/* Due to port 80 on LAN port */
 	        //eval("iptables", "-t", "mangle", "-A", "POSTROUTING", "-p", "tcp", "--sport", "80",
/*
 	        eval("iptables", "-t", "mangle", "-A", "POSTROUTING",\
                        "-d", lan_ipaddr, "-j", "MARK", "--set-mark", "80");
*/
		if ((fp=fopen("/tmp/mangle_rules", "a"))==NULL) return;
//		fprintf(fp, "*mangle\n");
		fprintf(fp, "-A POSTROUTING -d %s -j MARK --set-mark 80\n", lan_ipaddr);
//		fprintf(fp, "COMMIT\n\n");
		fclose(fp);
//		eval("iptables-restore", "/tmp/mangle_rules");
	#endif
		/* User define settings */
		if (nvram_match("qos_userdef_enable", "1"))
		{ 
			/* if user specify supports */
			if(nvram_match("qos_userspec_app","1"))
			{
				for(idx_filter=0; idx_filter < rulenum+1; idx_filter++) //fit to download classes
                		{
			        	tc_filter(net_name, Ch_conv("qos_ip_x", idx_filter), \
                        			Ch_conv("qos_port_x", idx_filter), num_conv(idx_filter));
                		}
			} //end qos_userspec_app

			/* if qos_asus_services */
			//wendel
			if(nvram_match("qos_service_enable","1")){
				if ((fp=fopen("/tmp/mangle_rules", "a"))==NULL) return;
//				fprintf(fp, "*mangle\n");
				fprintf(fp, "-A OUTPUT -p tcp --dport 1:65535 -j MARK --set-mark 50\n");
//				fprintf(fp, "COMMIT\n\n");
				fclose(fp);
//				eval("iptables-restore", "/tmp/mangle_rules");
/*
				eval("iptables", "-t", "mangle", "-A", "OUTPUT", "-p", "tcp", "--dport", "1:65535",
					"-j", "MARK", "--set-mark", "50");
*/
			}
		}// end qos_userspec

		//Enable Dynamic Fragmentation function
		if( nvram_match("qos_dfragment_enable", "1"))
		{
			int mss_size = 1200;
//			float pct = atoi(nvram_get("qos_dfragment_size"));
//			pct = pct/100;
//			mss_size = 536 + (1200-536)*pct;
			mss_size = atoi(nvram_get("qos_dfragment_size"));
		
			//packet size % 4 Must equl 0 (no padding octets)
			if(mss_size%4)
				mss_size = (mss_size/4 +1) *4;

			//536 =  standard MTU(576) - IP header(20) - TCP header(20)
			//1200 = suggested trasmmit packet size
			sprintf(dfragment_size, "%d", mss_size);

			if ((fp=fopen("/tmp/mangle_rules", "a"))==NULL) return;
//			fprintf(fp, "*mangle\n");
			fprintf(fp, "-I FORWARD -p tcp --tcp-flags SYN,ACK SYN,ACK -j TCPMSS --set-mss %s\n", dfragment_size);
			fprintf(fp, "COMMIT\n\n");
			fclose(fp);
			eval("iptables-restore", "/tmp/mangle_rules");
/*
			eval("iptables", "-t", "mangle", "-I", "FORWARD", "-p", "tcp", "--tcp-flags", "SYN,ACK", "SYN,ACK", "-j", "TCPMSS", "--set-mss", dfragment_size);
*/
		}

	} //end qos_enable
}
#endif
