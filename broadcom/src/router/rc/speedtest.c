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
/*****************************************************************************************************************
 * Filr Name	: speedtest.c
 * Description  : modify from ping.c
 * related file : qosutils.h qosutils.c
 * Auther       : Wendel Huang
 * History	: 2006.05.22 	post to WL500gP
		  2006.07.18 	Post to WL700g, and remove execution message form release version
		  2006.08.01    Debug the error in MER mode
 *****************************************************************************************************************/
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/signal.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "qosutils.h"

#ifdef WL600g
#include "asusbcmnvram.h"
#include "dbapi.h"
#include "ifcwanapi.h"
#include "syscall.h"
#include "bcmadsl.h"	//for ADSL_CONNECTION_INFO
#else	//WL500gP, WL550gE
#include "bcmnvram.h"
#endif

#ifdef WL600g
extern ADSL_CONNECTION_INFO glbAdslInfo;// global ADSL info variable is declared in syscall.c
#endif

#define STRUCT_LEN(name)    sizeof(name)/sizeof(name[0])
#define MAX_BANDWIDTH	15359	// S = (1500-64)*8/1024 (kbits)
				// S *1000 / (thelta t) * 1.369 (weight) = B

static const int DEFDATALEN = 56;
static const int MAXIPLEN = 60;
static const int MAXICMPLEN = 76;
static const int MAXWAIT = 10;
static const int ADSLMXLEN = 1458;
static const int HEADOVERHEAD = 42;		//14(frame header)+20(ip header)+8(icmp header)

#define	A(bit)		rcvd_tbl[(bit)>>3]	/* identify byte in array */
#define	B(bit)		(1 << ((bit) & 0x07))	/* identify bit in byte */
#define	SET(bit)	(A(bit) |= B(bit))
#define	CLR(bit)	(A(bit) &= (~B(bit)))
#define	TST(bit)	(A(bit) & B(bit))

enum { WAN_RATE_SUCCESS, WAN_LINK_FAIL };

struct icmp_map_t{
	unsigned difference;
	unsigned rate;
} icmp_map[] = {
	{3000, MAX_BANDWIDTH},
	{1638, 2048},
	{819, 1024},
	{600, 640},
	{410, 512},
	{204, 256},
	{102, 128},
	{51, 64},
	{27, 33},
};//difference = rate * 0.8

struct wan_link_rate {
	unsigned ds;
	unsigned us;
};

/* common routines */
static int in_cksum(unsigned short *buf, int sz)
{
	int nleft = sz;
	int sum = 0;
	unsigned short *w = buf;
	unsigned short ans = 0;

	while (nleft > 1) {
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1) {
		*(unsigned char *) (&ans) = *(unsigned char *) w;
		sum += ans;
	}

	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	ans = ~sum;
	return (ans);
}

static struct sockaddr_in pingaddr;
static int pingsock = -1;
int datalen; /* intentionally uninitialized to work around gcc bug */

// brcm: changed default value of pingcount from 0 to 4.
static long ntransmitted=0, nreceived=0;
int pingcount=10;
static int myid;
static struct timeval tstart, tend;

struct hostent *hostent;

static void sendping(int);
static void unpack(char *, int, struct sockaddr_in *);

/**************************************************************************/

static void sendping(int junk)
{
#ifdef QOS_DEBUG
	printf("	Start of sendping()\n");
#endif
	
	struct icmp *pkt;
	int i;
	char packet[datalen + 8];

	pkt = (struct icmp *) packet;

	int repeat = 0;
	for ( ; repeat<pingcount ; repeat++ ) {

	    pkt->icmp_type = ICMP_ECHO;
	    pkt->icmp_code = 0;
	    pkt->icmp_cksum = 0;
	    pkt->icmp_seq = ntransmitted++;
	    pkt->icmp_id = myid;

	    gettimeofday((struct timeval *) &packet[8], NULL);
	    pkt->icmp_cksum = in_cksum((unsigned short *) pkt, sizeof(packet));

	    struct timeval tmshow;
	    memcpy(&tmshow, &packet[8], sizeof(struct timeval));

#ifdef QOS_DEBUG
	    printf("	sendping() ** sequence = %d 	sendtime= %ld\n", pkt->icmp_seq, tmshow.tv_usec);
#endif

	    i = sendto(pingsock, packet, sizeof(packet), 0,
	    		       (struct sockaddr *) &pingaddr, sizeof(struct sockaddr_in));
	    
	    //2006.08.01	This is for sending echo-request error in MER mode
 	    //printf("	sendping() ** ntransmitted=%ld : sendto = %d\n", ntransmitted, i);
	    //printf("QOS Detecting#%ld : sendto = %d\n", ntransmitted, i);
	    //printf("QOS \n");

#ifdef QOS_DEBUG
	    if(i<0) 
		switch(errno) {
		    case EBADF:
			printf("EBADF\n");
		    case EFAULT:
			printf("EFAULT\n");
		    case ENOTSOCK:
			printf("ENOTSOCK\n");
		    case EINTR:
			printf("EINTR\n");
		    case EAGAIN:
			printf("EAGAIN\n");
		    case ENOBUFS: 
			printf("ENOBUFS\n");
		    //case ENOMEN:
			//printf("ENOMEN\n");
		    case EINVAL:
			printf("EINVAL\n");
		    default:
			printf("sendto DNS error\n");
		}
#endif
		
	    if ( i<0 || (size_t)i != sizeof(packet) )
		    printf("	sendping() ** sendto Error !!\n");
	}
}

static void unpack(char *buf, int sz, struct sockaddr_in *from)
{
	struct icmp *icmppkt;
	struct iphdr *iphdr;
	struct timeval tv, *tp;
	int hlen;
	unsigned long int triptime;

	gettimeofday(&tv, NULL);

	/* check IP header */
	iphdr = (struct iphdr *) buf;
	hlen = iphdr->ihl << 2;
	/* discard if too short */
	//if (sz < (datalen + ICMP_MINLEN))
	//	return;

	sz -= hlen;
	icmppkt = (struct icmp *) (buf + hlen);

	if (icmppkt->icmp_type == ICMP_ECHOREPLY) {

		if (icmppkt->icmp_id != myid) {
#ifdef QOS_DEBUG
	    	    printf("	unpack() ** not my ping !! icmp->icmpid = %d : myid =%d \n", icmppkt->icmp_id, myid);	
#endif
	    	    return;				/* not our ping */
		}

	        ++nreceived;
		tp = (struct timeval *) icmppkt->icmp_data;

		if( icmppkt->icmp_seq==0 ) {
		    memcpy(&tstart, &tv, sizeof(struct timeval));
		}
		if ( icmppkt->icmp_seq==(pingcount-1) ) {
		    memcpy(&tend, &tv, sizeof(struct timeval));
		}

		if ((tv.tv_usec -= tp->tv_usec) < 0) {
			--tv.tv_sec;
			tv.tv_usec += 1000000;
		}
		tv.tv_sec -= tp->tv_sec;

		triptime = tv.tv_sec * 1000000 + tv.tv_usec;

		//nvram_set("qos_ubw_status", "initialing");
                /*brcm: changed display message to show actually receive data bytes length
                  rather than ICMP packet length which is ICMP_MINLEN+dataLen */
#ifdef QOS_DEBUG
		printf("	unpack() ** new sec = %ld	new usec = %ld\n", tv.tv_sec, tv.tv_usec);
#endif
	}
	else if (icmppkt->icmp_type == ICMP_UNREACH) {
		//if (icmppkt->icmp_code == ICMP_UNREACH_HOST) {
		    nvram_set("qos_ubw_reason", "Host Unreachable");	
#ifdef QOS_DEBUG
		    printf("	unpack() ** DNS Host Unreachable !!\n");
#endif
		    nvram_set("qos_ubw_status", "fail");
	}
	else {
		    nvram_set("qos_ubw_status", "fail");
#ifdef QOS_DEBUG
		    printf("	unpack() ** icmp->type : %d\n", icmppkt->icmp_type);
#endif
	}

}

extern void deltatime(const char *host, struct timeval *deltaval)
{
#ifdef QOS_DEBUG
	printf("	Start of deltatime()\n");
#endif

	datalen = ADSLMXLEN;
	char packet[datalen + MAXIPLEN + MAXICMPLEN];
	unsigned long int ul_hostip;
	struct timeval socktv;

	myid = getpid() & 0xFFFF;
	
	if( (pingsock=socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0 ) { 
#ifdef QOS_DEBUG
		printf("	deltatime() ** socket() Error!!\n");
#endif
		exit(2);
	}
	
	socktv.tv_sec = 5;
	socktv.tv_usec = 0;
	setsockopt(pingsock, SOL_SOCKET, SO_RCVTIMEO, &socktv, sizeof(socktv));

	memset(&pingaddr, 0, sizeof(struct sockaddr_in));

	pingaddr.sin_family = AF_INET;
	
	if( (ul_hostip=inet_addr(host)) !=INADDR_NONE ) 
		pingaddr.sin_addr.s_addr = ul_hostip;
	else if( (hostent=gethostbyname(host)) ) 
		memcpy(&pingaddr.sin_addr, hostent->h_addr, sizeof(pingaddr.sin_addr));
#ifdef QOS_DEBUG
	else
		printf("	deltatime() ** bad host ip or name!!\n");
#endif

#ifdef QOS_DEBUG
	printf("	deltatime() ** host = %s\n", inet_ntoa(pingaddr.sin_addr));
#endif
	
	/* start the ping's going ... */
	signal(SIGALRM, sendping);
	alarm(1);

	/* listen for replies */
	int runtimes = 1;
	int errotime = 0;
	while (1) {
#ifdef 	QOS_DEBUG
		printf("\n	receive exec times = %d\n", runtimes);
#endif
		runtimes++;

		struct sockaddr_in from;
		socklen_t fromlen = (socklen_t) sizeof(from);
		int c = 0;

		c = recvfrom(pingsock, packet, sizeof(packet), 0,
						  (struct sockaddr *) &from, &fromlen);
		if (c < 0) {
			errotime++;		
	
			if (errno == EINTR) continue;

			if( errotime > 3 ) {
				nvram_set("qos_ubw_status", "fail");		
				nvram_set("qos_ubw_reason", "DNS server fail");
			}

#ifdef QOS_DEBUG	
			printf("	deltatime() ** recvfrom small than sendto !!\n");
#endif

			if (pingcount > 0 && runtimes>=pingcount)
				break;
		        else
				continue;
		}
		else if(c == 0) {
			nvram_set("qos_ubw_status", "fail");
		}
		
		unpack(packet, c, &from);

		if (pingcount > 0 && nreceived >= pingcount)
			break;

		if ( strcmp(nvram_safe_get("qos_ubw_status"), "fail") == 0 )
			exit(0);
	}
	
	close(pingsock);

	if ((tend.tv_usec -= tstart.tv_usec) < 0) {
		--tend.tv_sec;
		tend.tv_usec += 1000000;
	}
	tend.tv_sec -= tstart.tv_sec;
#ifdef QOS_DEBUG
	printf("	deltatime() ** tv_sec=%ld : tv_usec=%ld\n", tend.tv_sec, tend.tv_usec);
#endif

	memcpy(deltaval, &tend, sizeof(struct timeval));
}

/**************************************************************************
 * Function Name: qos_get_wan_rate
 * Description  : get the ADSL line rate
 * Parameters   : None
 * Returns      : Is successful when get the ADSL data rate? 
			0 : Success
			1 : Link fail
 **************************************************************************/
int qos_get_wan_rate(void)
{
	static struct wan_link_rate wanrate;
	char us[256], ds[256] ;
	char * gw_ip=NULL;
	struct timeval deltaval;
       	double delta_us = 0;

	wanrate.us = wanrate.ds = 0;

	char *ip = nvram_safe_get("wan_dns_t");

	if( !check_wan_link(0) )
		return WAN_LINK_FAIL;

	gw_ip = get_gw_ip();

	if((strchr(ip, ' ')) ||(!strcmp(ip, "")))
	{
		struct dns_lists *dns_list = get_dns_list(1);
		int i;
		
		for(i=0 ; i<dns_list->num_servers ; i++){
			ip = dns_list->dns_server[i];
			if(    (!strchr(ip, ' ')) 
			    && (strcmp(ip, ""))
			    && (strcmp(ip, gw_ip))) //WL600g auto assign LAN IP as DNS Server 
				break;
		}
		free(dns_list);
	}
	
	if( (strchr(ip, ' ')) || (!strcmp(ip, "")) || (!strcmp(ip, gw_ip)) ) {
#ifdef QOS_DEBUG                                                
		printf("	qos_get_wan_rate() ** GET DNS IP Error\n");
#endif
		nvram_set("qos_ubw_status", "fail");
		nvram_set("qos_ubw_reason", "DNS server fail");
	}
	else 
	        deltatime(ip, &deltaval);
	
	free(gw_ip);

	if( strcmp(nvram_safe_get("qos_ubw_status"), "fail") == 0 ) { 
#ifdef WL600g
		if( !strcmp(nvram_safe_get("qos_ubw_reason"), "DNS server fail") )
		{
			//DownStream
        		if ( glbAdslInfo.ulInterleavedDnStreamRate != 0 )
         		    wanrate.ds = glbAdslInfo.ulInterleavedDnStreamRate / 1000;
        		else
            		    wanrate.ds =  glbAdslInfo.ulFastDnStreamRate / 1000;

			//UpStream
        		if ( glbAdslInfo.ulInterleavedUpStreamRate != 0 )
		            wanrate.us = glbAdslInfo.ulInterleavedUpStreamRate / 1000;
		        else
            		    wanrate.us = glbAdslInfo.ulFastUpStreamRate / 1000;

			//sprintf(ds, "%u", wanrate.ds);
			//nvram_set("wan_ds", ds);
			sprintf(us, "%u", wanrate.us);
			nvram_set("qos_ubw", us);
	
			nvram_set("qos_ubw_status", "success");

			return WAN_RATE_SUCCESS;	
		}
		else
			return WAN_LINK_FAIL;
#else		
		return WAN_LINK_FAIL;
#endif
	}

#ifdef QOS_DEBUG
        printf("\n      start time ** tv_sec=%ld: tv_usec=%ld\n",tstart.tv_sec, tstart.tv_usec);
        printf("        end time   ** tv_sec=%ld : tv_usec=%ld\n", tend.tv_sec, tend.tv_usec);
        printf("                      delta_sec=%ld : delta_usec=%ld\n", deltaval.tv_sec*1000000, deltaval.tv_usec);
#endif
                                                                                                 
        delta_us = (int)deltaval.tv_sec * 1000000 + deltaval.tv_usec;
                                                                                                 
        wanrate.us = ( (( (datalen+42)*pingcount*8 )/delta_us) )*1000000/1024;

	if( wanrate.us!=0 ) 
		nvram_set("qos_ubw_status", "success");
	
#ifdef 	QOS_DEBUG
        printf("\nUpload Bandwidth = %.0u kbps\n", wanrate.us);
#endif
	
	wanrate.ds = 0;

	int i=0;
	for (i = 0; i < STRUCT_LEN(icmp_map); i++)
		if (wanrate.us >= icmp_map[i].difference) {
		    wanrate.us =icmp_map[i].rate;
		    break;
		}

	//sprintf(ds, "%u", wanrate.ds);
	//nvram_set("wan_ds", ds);
	sprintf(us, "%u", wanrate.us);
	nvram_set("qos_ubw", us);	

	return WAN_RATE_SUCCESS;	
}

#ifdef WL600g
//**************************************************************************
// Function Name: Speedtest_init
// Description  : init function, called by BcmNtwk_init() in ifcntwkapi.cpp
// Returns      : 0
//**************************************************************************
int Speedtest_init(void) {
        FILE *fp;

        /* Run it under background */
        switch (fork()) {
        case -1:
                exit(0);
                break;
        case 0:
                // start in a new session
                (void) setsid();
                break;
        default:
                // parent process should just die
                return 0;
        }

        /* write pid */
        if ((fp=fopen("/var/run/speedtest.pid", "w"))!=NULL)
        {
                fprintf(fp, "%d", getpid());
                fclose(fp);
        }
	
	nvram_set("qos_ubw_status", "initialing");

	sleep(60);
	if( nvram_match("qos_enable", "1") ) {

		qos_get_wan_rate();
	
		if(nvram_match("qos_ubw_status", "success") )
			start_qos();
	}

	while(1){
		pause();
	}

        return 0;
}
#else
//extern void start_qos(void);


int Speedtest_Init(void) {
/*
	nvram_set("qos_enable", "1");

	nvram_set("qos_dfragment_enable", "1");
	nvram_set("qos_dfragment_size", "10");

	nvram_set("qos_global_enable", "1");

	nvram_set("qos_service_enable", "1");
	nvram_set("qos_service_ubw", "10");

	nvram_set("qos_tos_prio", "1");
	nvram_set("qos_pshack_prio", "1");
	nvram_set("qos_shortpkt_prio", "1");

	nvram_set("qos_userdef_enable", "1");
	nvram_set("qos_userspec_app", "1");
	nvram_set("qos_rulenum_x", "2");
	
	nvram_set("qos_ip_x0", "192.168.6.54");
	nvram_set("qos_port_x0", "3333");
	nvram_set("qos_prio_x0", "3");

	nvram_set("qos_ip_x1", "192.168.6.54");
	nvram_set("qos_port_x1", "8888");
	nvram_set("qos_prio_x1", "5");
*/


                nvram_set("qos_userspec_app", "0");
                nvram_set("qos_global_enable", "0");
                nvram_set("qos_userdef_enable", "0");
                nvram_set("qos_enable", "0");

        if ( nvram_invmatch("qos_rulenum_x", "0"))
                nvram_set("qos_userspec_app", "1");

        if( nvram_match("qos_tos_prio", "1") || nvram_match("qos_pshack_prio", "1") || nvram_match("qos_shortpkt_prio", "1") )
                nvram_set("qos_global_enable", "1");

        if( nvram_match("qos_userspec_app", "1") || nvram_match("qos_dfragment_enable", "1") || nvram_match("qos_service_enable", "1") )
                nvram_set("qos_userdef_enable", "1");
                                                                                                                                               
        if( nvram_match("qos_global_enable", "1") || nvram_match("qos_userdef_enable", "1") ) {
//                sleep(3);
                nvram_set("qos_enable", "1");
                qos_get_wan_rate();
                if(nvram_match("qos_ubw_status", "success") )
                        start_qos();
        }
                                                                                                                                               
	return 0;
}
#endif 

#ifdef MYGCC
int main(int argc, char** argv)
{
	int i = 0;
	char *thisarg;

	printf("	%-20s	%-20s\n", "index", "value");

	while( i<argc ) {
	    printf("	%-20d	%-20s\n", i, argv[i]);
	    i++;
	}

	argc--;
	argv++;
	while( argc>0 && **argv=='-' ) {
	    	thisarg = *argv;
		thisarg++;
		switch (*thisarg) {
		case 'c':
			if (--argc <= 0)
			        exit(1);
			argv++;
			pingcount = atoi(*argv);
			break;
		case 's':
			if (--argc <= 0)
			        exit(1);
			argv++;
			datalen = atoi(*argv);
			break;
		default:
			exit(1);
		}
		argc--;
		argv++;
	}

	if( argc<0 ) 	exit(1);
	
        struct timeval deltaval;

        deltatime(*argv, &deltaval);
	
	printf("\n	start time ** tv_sec=%u : tv_usec=%u\n",tstart.tv_sec, tstart.tv_usec);
	printf("	end time   ** tv_sec=%u : tv_usec=%u\n", tend.tv_sec, tend.tv_usec);
        printf("        	      delta_sec=%u : delta_usec=%u\n", deltaval.tv_sec*1000000, deltaval.tv_usec);
        
	double delta_us = 0;
	delta_us = (int)deltaval.tv_sec * 1000000 + deltaval.tv_usec;
        printf("        delta interval = %f us\n", delta_us);
                                                                                                 
        double bw = ( (( (datalen+42)*pingcount*8 )/delta_us) )*1000000/1024;
        printf("\n	bandwidth = %.0f kbps\n", bw);
	
	return 0;
}
#endif 
