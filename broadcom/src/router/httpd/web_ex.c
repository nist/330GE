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
 * Copyright 2001, ASUSTek Inc.
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ASUSTek Inc.;
 * the contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of ASUSTek Inc..
 *
 * $Id: web_ex.c,v 1.1.1.1 2006/04/25 06:09:26 jiahao_jhou Exp $
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
#include <httpd.h>
#endif /* WEBS */

#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/klog.h>
#include <sys/wait.h>
#include <dirent.h>
#include <wlutils.h>
#ifdef WRITE2FLASH
#include <linux/mtd/mtd.h>
#include <trxhdr.h>
#endif
#include "typedefs.h"
#include "shutils.h"
#include "bcmutils.h"
#include "bcmnvram_f.h"
#include "common.h"

#define sys_restart() kill(1, SIGHUP)
#define sys_reboot() nvram_set("reboot", "1")

#ifdef WEBS
#define init_cgi(query)
#define do_file(webs, file)
#endif

#define sys_upgrade(image) eval("write", image, "/dev/mtd/1")
#define sys_upload(image) eval("nvram", "restore", image)
#define sys_download(file) eval("nvram", "save", file)
#define sys_stats(url) eval("stats", (url))
#define sys_restore(sid) eval("nvram_x","get",(sid))
#define sys_commit(sid) nvram_commit();
#define sys_default()   eval("erase", "/dev/mtd/3")
#define sys_nvram_set(param) eval("nvram", "set", param)

#define UPNP_E_SUCCESS 0
#define UPNP_E_INVALID_ARGUMENT -1

#define GROUP_FLAG_REFRESH 	0
#define GROUP_FLAG_DELETE 	1
#define GROUP_FLAG_ADD 		2
#define GROUP_FLAG_REMOVE 	3

#define IMAGE_HEADER 	"HDR0"
#define PROFILE_HEADER 	"HDR1"

static int apply_cgi_group(webs_t wp, int sid, struct variable *var, char *groupName, int flag);
static int nvram_generate_table(webs_t wp, char *serviceId, char *groupName);

static int ej_select_country(int eid, webs_t wp, int argc, char_t **argv);

char ibuf2[8192];

static int ezc_error = 0;

#define ACTION_UPGRADE_OK   0
#define ACTION_UPGRADE_FAIL 1

int action;

char *serviceId;
#define MAX_GROUP_ITEM 10
#define MAX_GROUP_COUNT 300
#define MAX_LINE_SIZE 512
char *groupItem[MAX_GROUP_ITEM];
char urlcache[128];
char urlcache2[128];
char *next_host;
int delMap[MAX_GROUP_COUNT];
char SystemCmd[64];
char UserID[32]="";
char UserPass[32]="";
char ProductID[32]="";

extern int redirect;

char current_url_glocal[256];

#ifdef WRITE2FLASH
char *flashblock=NULL;
int mtd_fd = -1;
int mtd_count;
mtd_info_t mtd_info;
erase_info_t erase_info;

int flog(char *log)
{
	FILE *fp;

	fp=fopen("/tmp/log", "a+");

	if (fp)
	{	
		fprintf(fp, "log : %d %d %s", erase_info.start, erase_info.length, log);
		fclose(fp);
	}	
}

int flashwrite(char *buf, int count, int filelen)
{
	int len;
	int offset;
	int size;

	// first time, the trx header
	if (mtd_fd==-1)
	{
		/* Open MTD device and get sector size */
		if ((mtd_fd = open("/dev/mtd/1", O_RDWR)) < 0 ||
		    ioctl(mtd_fd, MEMGETINFO, &mtd_info) != 0 ||
		    mtd_info.erasesize < sizeof(struct trx_header)) {
			flog("1\r\n");
			return 0;
		}

		erase_info.start=0;
		erase_info.length=mtd_info.erasesize;
		mtd_count = 0;

		/* Allocate temporary buffer */
		if (!(flashblock = malloc(erase_info.length))) {
			flog("2\r\n");
			goto fail;
		}
	}
	
	offset=0;
	do {
		if ((mtd_count+(count-offset))>=erase_info.length)
		{
			memcpy(flashblock+mtd_count, buf+offset, erase_info.length-mtd_count);
			offset+=(erase_info.length-mtd_count);
			mtd_count=erase_info.length;
		}
		else 
		{
			memcpy(flashblock+mtd_count, buf+offset, count-offset);
			mtd_count+=(count-offset);
			if (filelen!=0) return 1;
		}

		(void) ioctl(mtd_fd, MEMUNLOCK, &erase_info);
		if (ioctl(mtd_fd, MEMERASE, &erase_info) != 0 ||
	    		write(mtd_fd, flashblock, mtd_count) != mtd_count) {
			flog("3\r\n");
			goto fail;
		}
		erase_info.start+=mtd_count;
		mtd_count=0;
		flog("4\r\n");
		if (filelen==0&&(count-offset)==0) break;
	} while(1);

fail:
	/* if end of file */
	if (flashblock) {
		/* Dummy read to ensure chip(s) are out of lock/suspend state */
		(void) read(mtd_fd, flashblock, 2);
		free(flashblock);
	}

	if (mtd_fd >= 0)
		close(mtd_fd);
	return 1;
}
#endif

char *
rfctime(const time_t *timep)
{
	static char s[201];
	struct tm tm;

	setenv("TZ", nvram_safe_get_x("", "time_zone_x"), 1);
	memcpy(&tm, localtime(timep), sizeof(struct tm));
	strftime(s, 200, "%a, %d %b %Y %H:%M:%S %z", &tm);
	return s;
}

char *
reltime(unsigned int seconds)
{
	static char s[] = "XXXXX days, XX hours, XX minutes, XX seconds";
	char *c = s;

#ifdef SHOWALL
	if (seconds > 60*60*24) {
		c += sprintf(c, "%d days, ", seconds / (60*60*24));
		seconds %= 60*60*24;
	}
	if (seconds > 60*60) {
		c += sprintf(c, "%d hours, ", seconds / (60*60));
		seconds %= 60*60;
	}
	if (seconds > 60) {
		c += sprintf(c, "%d minutes, ", seconds / 60);
		seconds %= 60;
	}
	c += sprintf(c, "%d seconds", seconds);
#else
	c += sprintf(c, "%d seconds", seconds);
#endif

	return s;
}

#ifndef WEBS
/******************************************************************************/
/*
 *	Redirect the user to another webs page
 */
 
char *getip(FILE *fp)
{     
    //getpeername(fd, &addr, &addrlen);
    if (next_host==NULL || strcmp(next_host, "")==0)    
    {
	
       return(nvram_get_x("BRIPAddress","lan_ipaddr"));	
    }
    else
{
	
       return(next_host);
}
} 

void websRedirect(webs_t wp, char_t *url)
{	
	//printf("Redirect to : %s\n", url);	
	websWrite(wp, T("<html><head>\r\n"));
        websWrite(wp, T("<meta http-equiv=\"refresh\" content=\"0; url=http://%s/%s\">\r\n"), getip((FILE *)wp), url);
        websWrite(wp, T("<meta http-equiv=\"Content-Type\" content=\"text/html\">\r\n"));
        websWrite(wp, T("</head></html>\r\n"));      
	
#ifdef REMOVE_WL600	
	websWrite(wp, T("HTTP/1.0 302 Redirect\r\n"));
	websWrite(wp, T("Server: Asus Server\r\n"));
	websWrite(wp, T("Pragma: no-cache\r\n"));
	websWrite(wp, T("Cache-Control: no-cache\r\n"));
	websWrite(wp, T("Content-Type: text//html\r\n"));
	websWrite(wp, T("Location: http://%s/%s"), /*websGetVar(wp, T("HTTP_HOST"), "")*/"192.168.123.249", url);			

	websWrite(wp, T("<html><head></head><body>\r\n\
		This document has moved to a new <a href=\"http://%s/%s\">location</a>.\r\n\
		Please update your documents to reflect the new location.\r\n\
		</body></html>\r\n"), /*websGetVar(wp, T("HTTP_HOST"), "")*/"192.168.123.249", url);
        websWrite(wp, T("HTTP/1.0 200 OK\r\n"));	
#endif        
        websDone(wp, 200);	
}
#endif

void sys_script(char *name)
{
     char scmd[64];
	
     sprintf(scmd, "/tmp/%s", name);
     //printf("run %s %d %s\n", name, strlen(name), scmd);
     
     //handle special scirpt first

     if (strcmp(name,"syscmd.sh")==0)
     {
	   if (strcmp(SystemCmd, "")!=0)
	   {
	   	sprintf(SystemCmd, "%s > /tmp/syscmd.log 2>&1\n", SystemCmd);
	   	system(SystemCmd);
	   }	
	   else
	   {
	   	system("echo None > /tmp/syscmd.log\n");
	   }
     }
     else if (strcmp(name, "syslog.sh")==0)
     {
	   // to nothing
     }	
     else if (strcmp(name, "wan.sh")==0)
     {
	   kill_pidfile_s("/var/run/infosvr.pid", SIGUSR1);
     }
     else if (strcmp(name, "printer.sh")==0)
     {	
	   // update status of printer
	   kill_pidfile_s("/var/run/infosvr.pid", SIGUSR1);
     }
     else if (strcmp(name, "lpr_remove")==0)
     {
	   kill_pidfile_s("/var/run/lpdparent.pid", SIGUSR2);
     }
     else if (strcmp(name, "wlan11a.sh")==0 || strcmp(name,"wlan11b.sh")==0)
     {
	  // do nothing	
     }
     else if (strcmp(name,"leases.sh")==0 || strcmp(name,"dleases.sh")==0)
     {
		sys_refresh_lease();	
     }
     else if (strcmp(name,"iptable.sh")==0) 
     {
		// TODO	
     }
     else if (strcmp(name,"route.sh")==0)
     {
		// TODO
     }
     else if (strcmp(name,"radio_disable")==0)
     {
		//eval("wl", "radio", "off");
		eval("radioctrl", "0");
     }
     else if (strcmp(name,"radio_enable")==0)
     {
		//eval("wl", "radio", "on");	
		eval("radioctrl", "1");
     }
     else if (strcmp(name,"dhcpc_renew")==0)
     {
                nvram_set("updating", "1");
                sys_renew();
                if(!strcmp(nvram_get_x("", "wan0_proto"), "static"))
                        sleep(4);
                else
                        sleep(1);
                nvram_set("updating", "0");
     }
     else if (strcmp(name,"dhcpc_release")==0)
     {
                nvram_set("updating", "1");
                sys_release();
                if(!strcmp(nvram_get_x("", "wan0_proto"), "static"))
                        sleep(3);
                else
                        sleep(1);
                nvram_set("updating", "0");
     }
     else if (strcmp(name,"eject-usb.sh")==0)
     {
		eval("rmstorage");
     }
     else if (strcmp(name,"ddnsclient")==0)
     {
		eval("start_ddns");
     }
     else if (strstr(scmd, " ")==0) // no parameter, run script with eval
     {	
         	eval(scmd);
     }
     else system(scmd);  
}


/* 
 *   Byte 0 : Firmware Major
 *   Byte 1 : Firmware Minor 
 *   Byte 2 : File system Major
 *   Byte 3 : File system Minor
 *   Byte 4~15 : Product ID
 *   Byte 16~31: Hardware compatible list( 4 pairs)
 *      16+x*4: Minimum Hardware Major
 *      17+x*4: Minimum Hardware Minor
 *      18+x*4: Maximum Hardware Major
 *      19+x*4: Maximum Hardware Minor
 *      x= 0~3
 */
char checkVersion(char *version, unsigned char major, unsigned char minor)
{
        #define VINT(ver) (((((ver)[0])&0xff)<<8)|(((ver)[1])&0xff))        
        int i;
        unsigned int ver, min, max;
        char *clist;
        
        clist = version+16;
        ver = major << 8 | minor;
        
        //printf("ver: %d %d %d\n", major, minor, ver);
        i = 0;
        
        while(VINT(clist+i) && i<16)
        {
            min = VINT(clist+i); 	
            max = VINT(clist+i+2);
                        
            //printf("List: %x %x %x %x %x %x %x %x\n", i, ver, min, max, clist[i], clist[i+1], clist[i+2], clist[i+3]);
           
            if (ver>=min && (max==0 || ver<=max)) return 1; 
            i+=4;	
        }         
        if (i==0 || ver==0) return 1;                              	
        else return 0;
}

void websScan(char_t *str)
{
	unsigned int i, flag;
        char_t *v1, *v2, *v3, *sp;
        char_t groupid[64];
        char_t value[MAX_LINE_SIZE];
        char_t name[MAX_LINE_SIZE];
        
        v1 = strchr(str, '?');
                        
        i = 0;
        flag = 0;
                                     		
	while(v1!=NULL)
	{	   	    	
	    v2 = strchr(v1+1, '=');
	    v3 = strchr(v1+1, '&');
	    
	    if (v3!=NULL)
	    {
	       strncpy(value, v2+1, v3-v2-1);
	       value[v3-v2-1] = 0;  
	    }  
	    else
	    {
	       strcpy(value, v2+1);
	    }
	    
	    strncpy(name, v1+1, v2-v1-1);
	    name[v2-v1-1] = 0;
	    /*printf("Value: %s %s\n", name, value);*/
	    
	    if (v2 != NULL && ((sp = strchr(v1+1, ' ')) == NULL || (sp > v2))) 
	    {	    	
	       if (flag && strncmp(v1+1, groupid, strlen(groupid))==0)
	       {	    		    	   
	           delMap[i] = atoi(value);
	           /*printf("Del Scan : %x\n", delMap[i]);*/
	           if (delMap[i]==-1)  break;	           	           
	           i++;
	       }	
               else if (strncmp(v1+1,"group_id", 8)==0)
               {            	                               
                   sprintf(groupid, "%s_s", value);
                   flag = 1;
               }   
            }
	    v1 = strchr(v1+1, '&');
	} 
	delMap[i] = -1;
	return;
}


void websApply(webs_t wp, char_t *url)
{
#ifdef WL550gE
        do_ej (url, wp);
        websDone(wp, 200);
#else
     FILE *fp;
     char buf[MAX_LINE_SIZE];

     fp = fopen(url, "r");
     
     if (fp==NULL) return;
     
     while(fgets(buf, sizeof(buf), fp))
     {
	websWrite(wp, buf);
     } 
     
     websDone(wp, 200);	
     fclose(fp);
#endif
}


/*
 * Example: 
 * lan_ipaddr=192.168.1.1
 * <% nvram_get_x("lan_ipaddr"); %> produces "192.168.1.1"
 * <% nvram_get_x("undefined"); %> produces ""
 */
static int
ej_nvram_get_x(int eid, webs_t wp, int argc, char_t **argv)
{
	char *sid, *name, *c;
	int ret = 0;

	if (ejArgs(argc, argv, "%s %s", &sid, &name) < 2) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	for (c = nvram_safe_get_x(sid, name); *c; c++) {
		if (isprint(*c) &&
		    *c != '"' && *c != '&' && *c != '<' && *c != '>')
			ret += websWrite(wp, "%c", *c);
		else
			ret += websWrite(wp, "&#%d", *c);
	}

	return ret;
}

/*
 * Example: 
 * lan_ipaddr=192.168.1.1
 * <% nvram_get_x("lan_ipaddr"); %> produces "192.168.1.1"
 * <% nvram_get_x("undefined"); %> produces ""
 */
static int
ej_nvram_get_f(int eid, webs_t wp, int argc, char_t **argv)
{
	char *file, *field, *c, buf[64];
	int ret = 0;

	if (ejArgs(argc, argv, "%s %s", &file, &field) < 2) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
			
	strcpy(buf, nvram_safe_get_f(file, field));		
        for (c = buf; *c; c++) {
		if (isprint(*c) &&
		    *c != '"' && *c != '&' && *c != '<' && *c != '>')
			ret += websWrite(wp, "%c", *c);
		else
			ret += websWrite(wp, "&#%d", *c);
	}

	return ret;
}

/*
 * Example: 
 * wan_proto=dhcp
 * <% nvram_match("wan_proto", "dhcp", "selected"); %> produces "selected"
 * <% nvram_match("wan_proto", "static", "selected"); %> does not produce
 */
static int
ej_nvram_match_x(int eid, webs_t wp, int argc, char_t **argv)
{
	char *sid, *name, *match, *output;

	if (ejArgs(argc, argv, "%s %s %s %s", &sid, &name, &match, &output) < 4) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
	
	if (nvram_match_x(sid, name, match))
	{
		return websWrite(wp, output);
	}	

	return 0;
}	

static int
ej_nvram_double_match_x(int eid, webs_t wp, int argc, char_t **argv)
{
	char *sid, *name, *match, *output;
	char *sid2, *name2, *match2;

	if (ejArgs(argc, argv, "%s %s %s %s %s %s %s", &sid, &name, &match, &sid2, &name2, &match2, &output) < 7) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
	
	if (nvram_match_x(sid, name, match) && nvram_match_x(sid2, name2, match2))
	{
		return websWrite(wp, output);
	}	

	return 0;
}

/*
 * Example: 
 * wan_proto=dhcp
 * <% nvram_match("wan_proto", "dhcp", "selected"); %> produces "selected"
 * <% nvram_match("wan_proto", "static", "selected"); %> does not produce
 */
static int
ej_nvram_match_both_x(int eid, webs_t wp, int argc, char_t **argv)
{
	char *sid, *name, *match, *output, *output_not;

	if (ejArgs(argc, argv, "%s %s %s %s %s", &sid, &name, &match, &output, &output_not) < 5) 
        {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
	
	if (nvram_match_x(sid, name, match))
	{
		return websWrite(wp, output);
	}
        else
        {
                return websWrite(wp, output_not);
        }	
}

/*
 * Example: 
 * wan_proto=dhcp
 * <% nvram_match_both("wan_proto", "dhcp", "selected", "not"); %> produces "selected"
 * <% nvram_match_both("wan_proto", "static", "selected", "not"); %> produces "not"
 */
static int
ej_nvram_match_list_both_x(int eid, webs_t wp, int argc, char_t **argv)
{
	char *sid, *name, *match, *output, *output_ex;
	int index;

	if (ejArgs(argc, argv, "%s %s %s %s %s %d", &sid, &name, &match, &output, &output_ex, &index) < 6) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	if (nvram_match_list_x(sid, name, match, index))
		return websWrite(wp, output);
	else
	        return websWrite(wp, output_ex);		
}	


/*
 * Example: 
 * lan_ipaddr=192.168.1.1 192.168.39.248
 * <% nvram_get_list("lan_ipaddr", 0); %> produces "192.168.1.1"
 * <% nvram_get_list("lan_ipaddr", 1); %> produces "192.168.39.248"
 */
static int
ej_nvram_get_list_x(int eid, webs_t wp, int argc, char_t **argv)
{
	char *sid, *name;
	int which;	
	int ret = 0;

	if (ejArgs(argc, argv, "%s %s %d", &sid, &name, &which) < 3) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	ret += websWrite(wp, nvram_get_list_x(sid, name, which));
	return ret;
}

/*
 * Example: 
 * lan_ipaddr=192.168.1.1 192.168.39.248
 * <% nvram_get_list("lan_ipaddr", 0); %> produces "192.168.1.1"
 * <% nvram_get_list("lan_ipaddr", 1); %> produces "192.168.39.248"
 */
static int
ej_nvram_get_buf_x(int eid, webs_t wp, int argc, char_t **argv)
{
	char *sid, *name;
	int which;		

	if (ejArgs(argc, argv, "%s %s %d", &sid, &name, &which) < 3) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	
	return 0;
}

/*
 * Example: 
 * lan_ipaddr=192.168.1.1 192.168.39.248
 * <% nvram_get_table_x("lan_ipaddr"); %> produces "192.168.1.1"
 * <% nvram_get_table_x("lan_ipaddr"); %> produces "192.168.39.248"
 */
static int
ej_nvram_get_table_x(int eid, webs_t wp, int argc, char_t **argv)
{
	char *sid, *name;
	int ret = 0;

	if (ejArgs(argc, argv, "%s %s", &sid, &name) < 2) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
		        
	ret += nvram_generate_table(wp, sid, name);	
	return ret;
}

/*
 * Example: 
 * wan_proto=dhcp;dns
 * <% nvram_match_list("wan_proto", "dhcp", "selected", 0); %> produces "selected"
 * <% nvram_match_list("wan_proto", "static", "selected", 1); %> does not produce
 */
static int
ej_nvram_match_list_x(int eid, webs_t wp, int argc, char_t **argv)
{
	char *sid, *name, *match, *output;
	int which;

	if (ejArgs(argc, argv, "%s %s %s %s %d", &sid, &name, &match, &output, &which) < 5) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
	
	if (nvram_match_list_x(sid, name, match, which))
		return websWrite(wp, output);
	else
	        return 0;		
}	


static int
ej_select_channel(int eid, webs_t wp, int argc, char_t **argv)
{
	char *sid, chstr[32];
	int ret = 0;	
	int i, channel, channels[32];
	FILE *fp = NULL;
	char auto_str[32];
	
	if (ejArgs(argc, argv, "%s", &sid) < 1) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

        if (nvram_match_x("", "preferred_lang", "TR"))
	{
		memset(auto_str, 0x0, 32);
                fp=fopen("/www/auto_TR.asp", "r");
        	if (fp)
        	{
                	if (fgets(auto_str, 32, fp)==NULL)
                        	strcpy(auto_str, "Auto");
                	fclose(fp);
		}
		else
			strcpy(auto_str, "Auto");
	}
	else
		strcpy(auto_str, "Auto");
         
        channel=atoi(nvram_safe_get_x("", "wl_channel"));

	if (!wl_channels_in_country(nvram_safe_get_x("", "wl_country_code"), channels))
	{
		wl_channels_in_country_asus(nvram_safe_get_x("", "wl_country_code"), channels);
	}

	i = 0;

	while(channels[i]!=-1)
	{
		if (channels[i] == 0)
//			strcpy(chstr, "Auto");
			strcpy(chstr, auto_str);
		else sprintf(chstr, "%d", channels[i]);

        	if (channel==channels[i])
            		ret += websWrite(wp, "<option value=\"%d\" selected>%s</option>", channels[i], chstr);
        	else
            		ret += websWrite(wp, "<option value=\"%d\">%s</option>", channels[i], chstr);
		i++;
	} 
	return ret;
}


static int
ej_select_profile(int eid, webs_t wp, int argc, char_t **argv)
{
	char *sid, nvram_str[32];
	int ret = 0;	
	int i, idx, idx_current;
	
	if (ejArgs(argc, argv, "%s", &sid) < 1) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	if (	!strcmp(nvram_get_x("", "pf1_ssid"), "") &&
		!strcmp(nvram_get_x("", "pf2_ssid"), "") &&
		!strcmp(nvram_get_x("", "pf3_ssid"), "") &&
		!strcmp(nvram_get_x("", "pf4_ssid"), "")
	)
	{
		ret += websWrite(wp, "<option value=\"\" selected>&nbsp;</option>");
		return ret;
	}

	idx = 1;
	for (i=0; i<4; i++)
	{
		memset(nvram_str, 0, sizeof(nvram_str));
		sprintf(nvram_str, "pf%d_ssid", i+1);

		if (strcmp(nvram_get_x("", nvram_str), ""))
		{
			if (strcmp(nvram_get_x("", nvram_str), nvram_get_x("", "wl_ap_ssid"))==0)
			{
				ret += websWrite(wp, "<option value=\"%s\" selected>%s</option>", nvram_get_x("", nvram_str), nvram_get_x("", nvram_str));
				idx++;
			}
			else
			{
				ret += websWrite(wp, "<option value=\"%s\">%s</option>", nvram_get_x("", nvram_str), nvram_get_x("", nvram_str));
				idx++;
			}
		}
	}

/*
	idx_current = atoi(nvram_get_x("", "wl_ap_index"));
	memset(nvram_str, 0, sizeof(nvram_str));
	sprintf(nvram_str, "pf%d_ssid", idx_current);
	if (!strcmp(nvram_get_x("", nvram_str), ""))
		for (i=0; i<4; i++)
		{
			memset(nvram_str, 0, sizeof(nvram_str));
			sprintf(nvram_str, "pf%d_ssid", i+1);
			if (strcmp(nvram_get_x("", nvram_str), ""))
			{
				idx_current=i+1;
				break;
			}
		}

	idx = 1;
	for (i=0; i<4; i++)
	{
		memset(nvram_str, 0, sizeof(nvram_str));
		sprintf(nvram_str, "pf%d_ssid", i+1);

		if (strcmp(nvram_get_x("", nvram_str), ""))
		{
			if (idx_current==i+1)
			{
				ret += websWrite(wp, "<option value=\"%d\" selected>%d</option>", idx, idx);
				idx++;
			}
			else
			{
				ret += websWrite(wp, "<option value=\"%d\">%d</option>", idx, idx);
				idx++;
			}
		}
	}
*/
	return ret;
}

static int
ej_nvram_char_to_ascii(int eid, webs_t wp, int argc, char_t **argv)
{
	char *sid, *name, *c;
	int ret = 0;

	if (ejArgs(argc, argv, "%s %s", &sid, &name) < 2) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	char tmpstr[256];
	memset(tmpstr, 0x0, sizeof(tmpstr));
	char_to_ascii(tmpstr, nvram_safe_get_x(sid, name));
	ret += websWrite(wp, "%s", tmpstr);

	return ret;
}

static int
ej_urlcache(int eid, webs_t wp, int argc, char_t **argv)
{
	int flag;

	if (strcmp(urlcache, "Main_Operation3.asp")==0 || strcmp(urlcache, "Main_Operation4.asp")==0)	// for WL320g to reload treeFrame
	   flag = 4;
	else if (strcmp(urlcache, "Main_Operation2.asp")==0)	// for WL320g to reload treeFrame
	   flag = 3;
	else if (strcmp(urlcache, "Main_Operation.asp")==0)
	   flag = 2;
	else if (strcmp(urlcache, "Main_Content.asp")==0)
	   flag = 1;		  		
	else
	   flag = 0;
	   
	cprintf("Url:%s %d\n", urlcache, flag);

	if (strcmp(nvram_safe_get_x("IPConnection", "wan_route_x"), "IP_Routed")==0)
	{	
	    if (strcmp(nvram_safe_get_x("IPConnection", "wan_nat_x"), "1")==0)
	    {	   
	   	/* disable to redirect page */
	   	if (redirect)
	   	{
			if (!strcmp(nvram_get("preferred_lang"), "TW"))
				websWrite(wp, "Basic_GOperation_Content_TW.asp");
			else if (!strcmp(nvram_get("preferred_lang"), "CN"))
				websWrite(wp, "Basic_GOperation_Content_CN.asp");
			else
				websWrite(wp, "Basic_GOperation_Content.asp");
			redirect=0;
           	}
           	else if (flag==4)
           		websWrite(wp, "Basic_HomeGateway_SaveRestart.asp");
           	else if (flag==3)
           		websWrite(wp, "SaveRestart.asp");
	   	else if (flag==2)
	   	{
	   		if (!strcmp(nvram_get("preferred_lang"), "TW"))
				websWrite(wp, "Basic_GOperation_Content_TW.asp");
	   		else if (!strcmp(nvram_get("preferred_lang"), "CN"))
				websWrite(wp, "Basic_GOperation_Content_CN.asp");
			else
				websWrite(wp, "Basic_GOperation_Content.asp");
		}
	   	else if (flag==1)
			websWrite(wp, "Basic_HomeGateway_SaveRestart.asp");
	   	else
	   	{
	    		if (strcmp(urlcache2, "Advanced_WAdvanced_Content.asp")==0)
	    			websWrite(wp, "SaveRestart.asp");
	    		else
	    			websWrite(wp, "Main_Index_HomeGateway.asp");
	    	}
//	   		websWrite(wp, "Main_Index_HomeGateway.asp");
	    }
	    else
	    {	
	   	/* disable to redirect page */
	   	if (redirect)
	   	{	
			websWrite(wp, "Basic_ROperation_Content.asp");
			redirect=0;
           	}
	   	else if (flag==2)
			websWrite(wp, "Basic_ROperation_Content.asp");
	   	else if (flag==1)
			websWrite(wp, "Basic_Router_SaveRestart.asp");
	   	else
	   		websWrite(wp, "Main_Index_Router.asp");
            }	
	}      
	else
	{
	    if (flag==4)
	    	websWrite(wp, "Basic_AccessPoint_SaveRestart.asp");
	    else if (flag==3)
	    	websWrite(wp, "SaveRestart.asp");
	    else if (flag==2)
		websWrite(wp, "Basic_AOperation_Content.asp");
	    else if (flag==1)	
		websWrite(wp, "Basic_AccessPoint_SaveRestart.asp");
	    else
	    {
	    	if (strcmp(urlcache2, "Advanced_WAdvanced_Content.asp")==0)
	    		websWrite(wp, "SaveRestart.asp");
	    	else
	    		websWrite(wp, "Main_Index_AccessPoint.asp");
	    }
	}
	strcpy(urlcache,"");
	strcpy(urlcache2,"");
}

/* Report sys up time */
static int
ej_uptime(int eid, webs_t wp, int argc, char_t **argv)
{

	FILE *fp;
	char buf[MAX_LINE_SIZE];
	unsigned long uptime;
	int ret;
	char *str = file2str("/proc/uptime");
	time_t tm;

	time(&tm);
	sprintf(buf, rfctime(&tm));

	if (str) {
		unsigned int up = atoi(str);
		free(str);
		sprintf(buf, "%s(%s since boot)", buf, reltime(up));
	}

	ret = websWrite(wp, buf);  
	return ret;	    
}

static int
ej_sysuptime(int eid, webs_t wp, int argc, char_t **argv)
{
	int ret=0;
	char *str = file2str("/proc/uptime");

	if (str) {
		unsigned int up = atoi(str);
		free(str);
		ret = websWrite(wp, "%s since boot", reltime(up));
	}

	return ret;	    
}

/* Dump firewall log */
static int
ej_dumplog(int eid, webs_t wp, int argc, char_t **argv)
{
	#define MAX_LOG_BUF 2048
	#define MAX_LOG_SIZE 16384
	char buf[MAX_LOG_BUF], *line;
	char *next, *s;
	int len, ret = 0;
	long filelen;
	time_t tm;
	char *verdict, *src, *dst, *proto, *spt, *dpt;
	FILE *fpOrig, *fp;

        if ((fp = fopen("/etc/linuxigd/firewall.log","a+"))==NULL)
	{	
		cprintf("Open file fail\n");
		goto fail;
	}
	
	if ((fpOrig = fopen("/etc/linuxigd/firewallOrig.log","r"))!=NULL)
	{
	   /* Find size of orignal file */
	   fseek(fpOrig, 0, SEEK_END);
	   filelen = ftell(fp);
	
	   if (filelen>MAX_LOG_SIZE)
	      fseek(fpOrig, -(filelen-MAX_LOG_SIZE), SEEK_END);
	   else
	      fseek(fpOrig, 0, SEEK_SET);     
	      
	   while((len=fread(buf, 1, MAX_LOG_BUF, fpOrig)))
	   {	    	
	       fwrite(buf, 1, len, fp);
	   }  	  							
	   fclose(fpOrig);	   
	   unlink("/etc/linuxigd/firewallOrig.log");
	}   
			
	while((len=klogctl(4, buf, MAX_LOG_BUF))>0)
	{
	    //printf("W:%s\n", buf);
	    fwrite(buf, 1, len, fp);
	}
	
	fclose(fp);
	
	if ((fp = fopen("/etc/linuxigd/firewall.log","r"))==NULL)
	{	
		cprintf("Open file fail\n");
		goto fail;
	}
	
	//fseek(fp, 0, SEEK_SET);
		
#ifdef REMOVE_WL600	
	for (next = buf; (line = strsep(&next, "\n"));) 
#else
	while(fgets(buf, MAX_LOG_BUF, fp))
#endif	
	{
#ifndef REMOVE_WL600		
		line = buf;
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
#else
		ret += websWrite(wp, "%s\n", buf);				 
		cprintf("L:%s\n",buf);
#endif				 
	}	
	fclose(fp);	
	return ret;
fail:
	ret += websWrite(wp, "");
	return ret;	
}

struct lease_t {
	unsigned char chaddr[16];
	u_int32_t yiaddr;
	u_int32_t expires;
	char hostname[64];
};

int
websWriteCh(webs_t wp, char *ch, int count)
{
   int i, ret;
   
   ret = 0;
   for(i=0; i<count; i++)
      ret+=websWrite(wp, "%s", ch);
   return(ret);   
} 

/* Dump leases in <tr><td>MAC</td><td>IP</td><td>expires</td></tr> format */
static int
#ifdef REMOVE_WL600
ej_dumpleases(int eid, webs_t wp, int argc, char_t **argv)
#else
ej_dumpleases(int eid, webs_t wp, char *lease_file)
#endif
{
	FILE *fp;
	struct lease_t lease;
	int i;
	struct in_addr addr;
	unsigned long expires;
#ifdef REMOVE_WL600	
	char *format;
	char f[]="";
	char sigusr1[] = "-XX";
#endif	
	int ret = 0;

#ifdef REMOVE_WL600
	if (ejArgs(argc, argv, "%s", &format) < 1) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	/* Write out leases file */
	sprintf(sigusr1, "-%d", SIGUSR1);
	eval("killall", sigusr1, "udhcpd");
#endif	

    ret +=websWrite(wp,"Mac Address       IP Address      Lease Time\n");	
        
	/* Parse leases file */
	if (!(fp = fopen(lease_file, "r"))) 
	{
		//websWrite(wp, "No leases\n");
		return -1;
	}
					    	           	
	while (fread(&lease, sizeof(lease), 1, fp)) {
		//ret += websWrite(wp, "%s", format);
		for (i = 0; i < 6; i++) {
			ret += websWrite(wp, "%02x", lease.chaddr[i]);
			if (i != 5) ret += websWrite(wp, ":");
		}
		addr.s_addr = lease.yiaddr;
		ret += websWrite(wp, " %s", inet_ntoa(addr));
		ret += websWriteCh(wp," ", 16-strlen(inet_ntoa(addr)));
		expires = ntohl(lease.expires);
		
		if (expires==0xffffffff)	
			ret += websWrite(wp, "manual");
		else if (!expires)
			ret += websWrite(wp, "expired");
		else {
			if (expires > 60*60*24) {
				ret += websWrite(wp, "%ld days, ", expires / (60*60*24));
				expires %= 60*60*24;
			}
			if (expires > 60*60) {
				ret += websWrite(wp, "%ld hours, ", expires / (60*60));
				expires %= 60*60;
			}
			if (expires > 60) {
				ret += websWrite(wp, "%ld minutes, ", expires / 60);
				expires %= 60;
			}
			ret += websWrite(wp, "%ld seconds", expires);
		}
		ret += websWrite(wp, "\n");
	}
	fclose(fp);

	return ret;
}

/* Dump leases in <tr><td>MAC</td><td>IP</td><td>expires</td></tr> format */
static int
ej_dumpiptable(int eid, webs_t wp, char *iptable_file)
{	
	/* Format 											    */
	/* DNAT       tcp  --  anywhere             0.0.0.0            tcp dpts:6970:32000 to:192.168.122.2 */
	char buf[MAX_LINE_SIZE], *line, *s;
	char *dst, *prot, *port, *rdst;
	int ret, i, len;
	FILE *fp;

        ret +=websWrite(wp,"Destination     Prot. Port Range  Redirect to\n");	
        
	/* Parse leases file */
	if (!(fp = fopen(iptable_file, "r"))) 
	{
		//websWrite(wp, "No leases\n");
		return -1;
	}
					    	           	
	while (fgets(buf, MAX_LINE_SIZE, fp)) 
	{
	     line = buf;	
	     /* Parse into tokens */
	     s = line;
	     len = strlen(s);
	     
	     //printf("Line: %d %s\n", len, line);
	     
	     while (strsep(&s, " "));

	     
	     
	     /* Initialize token values */	     
	     dst = rdst = "n/a";
	     prot = port = "all";

	     i = 0;
	     /* Set token values */
  	     /* destination protocol:...  proto: */
	     for (s = line; s < &line[len]; s += strlen(s) + 1)
	     {
		if (i==1) /* Destination */
	           dst = &s[0];
	     	else if (!strncmp(s, "tcp", 3)) /* Proto */
	     	   prot = &s[0];
	     	else if (!strncmp(s, "udp", 3)) /* Proto */
	     	   prot = &s[0];   
	     	else if (!strncmp(s, "all", 3)) /* Proto */
	     	   prot = &s[0];   
	     	else if (!strncmp(s, "dpts:", 5))
	           port = &s[5];	
	        else if (!strncmp(s, "dpt:", 4))
	           port = &s[4];	   		    
		else if (!strncmp(s, "to:", 3))
		   rdst = &s[3];
		else if (!strncmp(s, "protox:", 7))
		   prot = &s[7];

		if (strlen(s)!=0) i++;
		//printf("Token: %d %s\n", i, s);
	     }
	     
	     ret +=websWrite(wp, "%s", dst);
	     ret +=websWriteCh(wp, " ", 16-strlen(dst));
	     ret +=websWrite(wp, "%s", prot);
	     ret +=websWriteCh(wp, " ", 6-strlen(prot));
	     ret +=websWrite(wp, "%s", port);
	     ret +=websWriteCh(wp, " ", 12-strlen(port));
	     ret +=websWrite(wp, "%s\n", rdst);	    
	}
	fclose(fp);

	return ret;
}

static int dump_file(webs_t wp, char *filename)
{
	FILE *fp;
	char buf[MAX_LINE_SIZE];
	int ret;

	//printf("dump: %s\n", filename);
	
	fp = fopen(filename, "r");
		
	if (fp==NULL) 
	{
		ret+=websWrite(wp, "");
		return(ret);
	}

	ret = 0;
		
	while (fgets(buf, MAX_LINE_SIZE, fp)!=NULL)
	{	 	
	    //printf("Read time: %s\n", buf);
	    ret += websWrite(wp, "%s", buf);
	}

	fclose(fp);

	return(ret);
}

static int
ej_dump(int eid, webs_t wp, int argc, char_t **argv)
{	
	FILE *fp;
	char buf[MAX_LINE_SIZE];
	char filename[32];
	char *file,*script;
	int ret;
	
	if (ejArgs(argc, argv, "%s %s", &file, &script) < 2) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
	
		
	cprintf("Script : %s, File: %s\n", script, file);
	
	// run scrip first to update some status
	if(strcmp(script,"")!=0) sys_script(script); 
 
	if (strcmp(file, "wlan11b.log")==0)
	{
#ifndef NOUSB
		if (nvram_match_x("", "nobr", "1"))
			return(ej_wl_status_nobr(eid, wp, 0, NULL));
		else
#endif
			return(ej_wl_status(eid, wp, 0, NULL));
	}
	else if(strcmp(file, "leases.log")==0) 
		return(ej_lan_leases(eid, wp, 0, NULL));
	else if (strcmp(file, "iptable.log")==0) 
		return(ej_nat_table(eid, wp, 0, NULL));
	else if (strcmp(file, "route.log")==0)
		return(ej_route_table(eid, wp, 0, NULL));
	else if(strcmp(file, "apscan.log")==0)
		return(ej_ap_scan(eid, wp, 0, NULL));
	else if(strcmp(file, "apscan2.log")==0)
		return(ej_ap_scan2(eid, wp, 0, NULL));
	else if(strcmp(file, "apscan3.log")==0)
		return(ej_ap_scan3(eid, wp, 0, NULL));
	else if(strcmp(file, "apscan4.log")==0)
		return(ej_ap_scan4(eid, wp, 0, NULL));
	else if(strcmp(file, "AUTOIP.log")==0)
		return(ej_autoip_src(eid, wp, 0, NULL));
	else if(strcmp(file, "mssid.log")==0)
		return(ej_mssid(eid, wp, 0, NULL));
	else if(strcmp(file, "profile.log")==0)
		return(ej_profile(eid, wp, 0, NULL));
	else if(strcmp(file, "client_list.log")==0)
		return(ej_getclientlist(eid, wp, 0, NULL));
	else if(strcmp(file, "apselect.log")==0)
		return(ej_ap_select(eid, wp, 0, NULL));

	ret = 0;
			   
	if (strcmp(file, "syslog.log")==0)
	{
           	sprintf(filename, "/tmp/%s-1", file);
	   	ret+=dump_file(wp, filename); 
	}
	   			   
	sprintf(filename, "/tmp/%s", file);
	ret+=dump_file(wp, filename);					
	   
	return ret;	    
}	

static int
ej_load(int eid, webs_t wp, int argc, char_t **argv)
{	
	char *script;
	
	if (ejArgs(argc, argv, "%s", &script) < 1) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
	 	  
	sys_script(script);
	return(websWrite(wp,""));						    
}	

/* Return PPPoE link state */
static int
ej_ppplink(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fp = fopen("/tmp/ppp/link", "r");

	if (!fp)
		return websWrite(wp, "Down");
	else {
		fclose(fp);
		return websWrite(wp, "Up");
	}
}



static void
validate_cgi(webs_t wp, int sid, int groupFlag)
{
    struct variable *v;    
    char *value;
    const char name[64];
        
    /* Validate and set variables in table order */
    for (v = GetVariables(sid); v->name != NULL; v++) 
    {
    	 //printf("Name: %s %d\n", v->name, sid);
    	 sprintf(name, "%s", v->name);    
    	         	 
    	 if ((value = websGetVar(wp, name, NULL)))
	 { 
	      if (strcmp(v->longname, "Group")==0)
    	      {
    	      	
    	      }
    	      else 
    	      {
		 //printf("set: %s %s\n", v->name, value);
    	         nvram_set_x(GetServiceId(sid), v->name, value);
    	      }   
	 }     
	 							 	
#ifdef REMOVE_WL600				   
	 if ((!*value && v->nullok) || !v->validate)
         { 
	    nvram_set_x(GetServiceId(sid), v->name, value);
         } 
  	 else
         {
	    if (v->validate(value, v) == UPNP_E_SUCCESS)
	       nvram_set_x(FindSvcLink(sid), v->name, value);
	    else
	       websWrite(wp, "Invalid %s = %s\n", v->name, value);
         }
#endif        	
    }
}


#ifndef NOUSB
static const char * const apply_header =
"<head>"
"<title>ASUS Wireless AP/Router: Apply</title>"
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
"<input type=\"button\" name=\"action\" value=\"Continue\" OnClick=\"history.go(-1)\">"
"</form>"
"<p class=\"label\">&#169;2001 ASUSTeK COMPUTER INC. All rights reserved.</p>"
"</body>"
;
#endif

enum {
	NOTHING,
	REBOOT,
	RESTART,
};

char *svc_pop_list(char *value, char key)
{    
    char *v, *buf;
    int i;
               
    if (value==NULL || *value=='\0')
       return(NULL);      
            
    buf = value;
    v = strchr(buf, key);

    i = 0;
    
    if (v!=NULL)
    {    	
        *v = '\0';  	
        return(buf);    	   
    }    
    return(NULL);
}

#ifndef NOUSB
static int special_handler(webs_t wp, char_t *url)
{
    #define MAX_SERVICES 5	
    char *flag;
    char *svc[MAX_SERVICES] = {"80","5190","554","23","20:21"};
    char *sid="FirewallConfig";
    char name[24];	
    int i, count;
    	  	
    if ((flag = websGetVar(wp, "FirewallConfig_LanWanActiveSVCFlag", NULL)) && strcmp(flag,""))
    {   
    	count  = 0; 	        	    
        for(i=0;i<MAX_SERVICES;i++)
        { 
			//printf("Save SVC:%d %s\n", i, svc[i]);         	 	    				

    	    if (flag[i] == '1')
    	    { 	    	    		
    	        sprintf(name, "LanWanSrcIP%d", count);
    	        nvram_set_x(sid, name, "");
    	        sprintf(name, "LanWanSrcPort%d", count);
    	        nvram_set_x(sid, name, "");
    	        sprintf(name, "LanWanDstIP%d", count);
    	        nvram_set_x(sid, name, "");
    	        sprintf(name, "LanWanDstPort%d", count);
    	        nvram_set_x(sid, name, svc[i]);
    	        sprintf(name, "LanWanProFlag%d", count);
    	        nvram_set_x(sid, name, "TCP");
    	        count++;
    	    }	    		    	    
    	}  
    	sprintf(name, "%d", count);  		
    	nvram_set_x(sid, "LanWanRuleCount", name);
    }    	
}
#endif

static int
apply_cgi(webs_t wp, char_t *urlPrefix, char_t *webDir, int arg,
	  char_t *url, char_t *path, char_t *query)
{
	int sid;
	char *value;
	char *current_url;
	char *next_url;
	char *sid_list;
	char *value1;
	char *script;
	char groupId[64];
	char urlStr[64];
	char *groupName;
	char *host;

	if (!query)
		goto footer;

	urlStr[0] = 0;
	value = websGetVar(wp, "action_mode","");
	next_host = websGetVar(wp, "next_host", "");
//	cprintf("host:%s\n", next_host);
	current_url = websGetVar(wp, "current_page", "");
	memset(current_url_glocal, 0, sizeof(current_url_glocal));
	strcpy(current_url_glocal, current_url);
	next_url = websGetVar(wp, "next_page", "");
	script = websGetVar(wp, "action_script","");

//	cprintf("Apply: %s %s %s %s\n", value, current_url, next_url, websGetVar(wp, "group_id", ""));

	if (!strcmp(value," Refresh "))
	{
		strcpy(SystemCmd, websGetVar(wp,"SystemCmd",""));
		websRedirect(wp, current_url);
		return 0;
	}
	else if (!strcmp(value," Clear "))
	{
//		current only syslog implement this button
		unlink("/tmp/syslog.log-1");
		unlink("/tmp/syslog.log");
		websRedirect(wp, current_url);
		return 0;
	}
	else if (!strcmp(value,"NEXT"))
	{
		websRedirect(wp, next_url);
		return 0;
	}
	else if (!strcmp(value, " Hot_Spot "))
	{
		nvram_set("wl_hotspot", websGetVar(wp, "wl_hotspot",""));
		websRedirect(wp, current_url);
		return 0;
	}
	else if (!strcmp(value, " Select_LANG "))
	{
		nvram_set("preferred_lang", websGetVar(wp, "language_select",""));
		nvram_set("temp_lang", websGetVar(wp, "language_select",""));
		websRedirect(wp, current_url);
		return 0;
	}
	else if (!strcmp(value, "Save&Restart "))
	{
/*
		if (strcmp(nvram_get_x("", "wl_mode_ex"), "sta")==0 || strcmp(nvram_get_x("", "wl_mode_ex"), "re")==0)
			save_PROFILE_group_SP(wp, sid, NULL, "ProfileList");
*/
		/*action = ACTION_SAVE_RESTART;*/
//		websRedirect(wp, "Restarting.asp");
		websApply(wp, "Restarting.asp");
		nvram_set_f("General", "x_Setting", "1");
		sys_commit(NULL);
//		sys_script("bcm_set");
//		system("/web/script/eject-usb.sh");
		sys_reboot();
		return(0);
	}
	else if(!strcmp(value, "Restore"))
	{
		/* action = ACTION_RESET_DEFAULT;*/
//		websRedirect(wp, "Restarting.asp");
		nvram_set("restore", "1");
		websApply(wp, "Restarting.asp");
//		system("/web/script/eject-usb.sh");
//		eval("killall", "snmpd");/* Cherry Cho added in 2006/8/30. */
//		sleep(3);
		sys_default();
		sys_reboot();
		return(0);
	}
	else if (!strcmp(value, "WlanUpdate "))
	{
		/*action = ACTION_SAVE_RESTART;*/
		/*websRedirect(wp, "Restarting.asp");*/
		if (strcmp(script, "")!=0)
		{
			system(script);
		}
		websApply(wp, "Restarting.asp");
		sys_reboot();    	
		return(0);
	}
	else 
	{
		sid_list = websGetVar(wp, "sid_list", "");

		while((serviceId = svc_pop_list(sid_list, ';')))
		{
			sid = 0;
			while(GetServiceId(sid)!=0)
			{
				if (!strcmp(GetServiceId(sid), serviceId))
					break;
				sid++;
			}

			if (serviceId!=NULL)
			{
				if (!strcmp(value, " Restore "))
				{
//					sys_restore(serviceId);
				}
				else if(!strcmp(value, "  Save  ") || !strcmp(value, " Apply "))
				{
#ifdef WL550gE
					if(!strcmp(current_url, "Advanced_Password_Content.asp"))
						nvram_set("temp_lang", websGetVar(wp, "preferred_lang", ""));
#endif
					if (!strcmp("WLANConfig11b", serviceId))
					{
						if(	!strcmp(current_url, "Advanced_Wireless_Content.asp") ||
							!strcmp(current_url, "Basic_AOperation_Content.asp") ||
							!strcmp(current_url, "Basic_GOperation_Content.asp") ||
							(!strcmp(current_url, "Wizard_Gateway.asp") && !strcmp(nvram_get_x("", "wl_hotspot"), "0")) ||
							!strcmp(current_url, "Wizard_AP.asp")
						)
						{
							nvram_set("mssid_ssid_x0", websGetVar(wp, "mssid_ssid_x0", ""));
							nvram_set("mssid_ssid_2_x0", websGetVar(wp, "mssid_ssid_2_x0", ""));
						}

						if(	!strcmp(current_url, "Basic_AOperation_Content.asp") ||
							!strcmp(current_url, "Basic_GOperation_Content.asp") ||
							!strcmp(current_url, "Wizard_Adapter.asp") ||
							!strcmp(current_url, "Wizard_Repeater.asp") ||
							(!strcmp(current_url, "Wizard_Gateway.asp") && !strcmp(nvram_get_x("", "wl_hotspot"), "1"))
						)
						{
							nvram_set("bak_wl_ssid", nvram_safe_get_x("", "wl_ssid"));
							nvram_set("bak_wl_ssid2", nvram_safe_get_x("", "wl_ssid2"));
							nvram_set("bak_wl_auth_mode", nvram_safe_get_x("", "wl_auth_mode"));
							nvram_set("bak_wl_wpa_mode", nvram_safe_get_x("", "wl_wpa_mode"));
							nvram_set("bak_wl_crypto", nvram_safe_get_x("", "wl_crypto"));
							nvram_set("bak_wl_wpa_psk", nvram_safe_get_x("", "wl_wpa_psk"));
							nvram_set("bak_wl_wep_x", nvram_safe_get_x("", "wl_wep_x"));
							nvram_set("bak_wl_phrase_x", nvram_safe_get_x("", "wl_phrase_x"));
							nvram_set("bak_wl_key1", nvram_safe_get_x("", "wl_key1"));
							nvram_set("bak_wl_key2", nvram_safe_get_x("", "wl_key2"));
							nvram_set("bak_wl_key3", nvram_safe_get_x("", "wl_key3"));
							nvram_set("bak_wl_key4", nvram_safe_get_x("", "wl_key4"));
							nvram_set("bak_wl_key", nvram_safe_get_x("", "wl_key"));
						}
					}

					validate_cgi(wp, sid, TRUE);

					if (!strcmp("WLANConfig11b", serviceId))
					{
						if(	( (!strcmp(current_url, "Basic_AOperation_Content.asp") || !strcmp(current_url, "Basic_GOperation_Content.asp")) && (!strcmp(nvram_get_x("", "wl_mode_ex"), "sta") || !strcmp(nvram_get_x("", "wl_mode_ex"), "re")) ) ||
							(!strcmp(current_url, "Wizard_Adapter.asp") || !strcmp(current_url, "Wizard_Repeater.asp")) ||
							(!strcmp(current_url, "Wizard_Gateway.asp") && !strcmp(nvram_get_x("", "wl_hotspot"), "1"))
						)
						{
							if (	!strcmp(websGetVar(wp, "no_ask_password",""), "1") &&
								(!strcmp(current_url, "Wizard_Adapter.asp") || !strcmp(current_url, "Wizard_Repeater.asp") || (!strcmp(current_url, "Wizard_Gateway.asp") && !strcmp(nvram_get_x("", "wl_hotspot"), "1")))
							)
							{
								moveup_PROFILE_group_SP();
								moveup_PROFILE_group_SP();
								moveup_PROFILE_group_SP();
							}
							else
							{
								if (!save_PROFILE_group(wp, sid, NULL, "ProfileList"))
									save_PROFILE_group_SP2(wp, sid, NULL, "ProfileList");
								moveup_PROFILE_group_SP();
								moveup_PROFILE_group_SP();
								moveup_PROFILE_group_SP();
							}

							nvram_set("wl_ssid", nvram_safe_get_x("", "bak_wl_ssid"));
							nvram_set("wl_ssid2", nvram_safe_get_x("", "bak_wl_ssid2"));
							nvram_set("wl_auth_mode", nvram_safe_get_x("", "bak_wl_auth_mode"));
							nvram_set("wl_wpa_mode", nvram_safe_get_x("", "bak_wl_wpa_mode"));
							nvram_set("wl_crypto", nvram_safe_get_x("", "bak_wl_crypto"));
							nvram_set("wl_wpa_psk", nvram_safe_get_x("", "bak_wl_wpa_psk"));
							nvram_set("wl_wep_x", nvram_safe_get_x("", "bak_wl_wep_x"));
							nvram_set("wl_phrase_x", nvram_safe_get_x("", "bak_wl_phrase_x"));
							nvram_set("wl_key1", nvram_safe_get_x("", "bak_wl_key1"));
							nvram_set("wl_key2", nvram_safe_get_x("", "bak_wl_key2"));
							nvram_set("wl_key3", nvram_safe_get_x("", "bak_wl_key3"));
							nvram_set("wl_key4", nvram_safe_get_x("", "bak_wl_key4"));
							nvram_set("wl_key", nvram_safe_get_x("", "bak_wl_key"));
						}
					}
				} 
				else if(!strcmp(value, "Set") || !strcmp(value, "Unset") || !strcmp(value, "Update") || !strcmp(value, "Eject"))
				{
					validate_cgi(wp, sid, TRUE);
				}
				else if(!strcmp(value," Finish "))
				{
					if(strcmp(current_url, "Advanced_Wireless_Content.asp")==0)
					{
						nvram_set("mssid_ssid_x0", websGetVar(wp, "mssid_ssid_x0", ""));
						nvram_set("mssid_ssid_2_x0", websGetVar(wp, "mssid_ssid_2_x0", ""));
					}

					validate_cgi(wp, sid, TRUE);
				}
				else
				{
					cprintf("group ?\n");
					strcpy(groupId, websGetVar(wp, "group_id", ""));

					if ((value1 = websGetVar(wp, "action_mode", NULL)))
					{
						groupName = groupId;

//						if (!strcmp(GetServiceId(sid), groupId))
//						{
						if (!strncmp(value1, " Save ", 6))
						{
							modify_SSID_group(wp, sid, NULL, groupName);
						}
						else if (!strncmp(value1, " Delete ", 8))
						{
							apply_cgi_group(wp, sid, NULL, groupName, GROUP_FLAG_DELETE);
						}
						else if (!strncmp(value1, " Add ", 5))
						{
							apply_cgi_group(wp, sid, NULL, groupName, GROUP_FLAG_ADD);
						}
						else if (!strncmp(value1, " Del ", 5))
						{
							apply_cgi_group(wp, sid, NULL, groupName, GROUP_FLAG_REMOVE);
						}
						else if (!strncmp(value1, " Refresh ", 9))
						{
							apply_cgi_group(wp, sid, NULL, groupName, GROUP_FLAG_REFRESH);
						}
#ifndef NOUSB
						else if (!strncmp(value1, " Edit ", 6))
						{
//							apply_cgi_group(wp, sid, NULL, groupName, GROUP_FLAG_REFRESH);
//							cprintf("edit %d\n", delMap[0]);
							if (delMap[0]==-1)
								websRedirect(wp, current_url);
							else
							{
								getSharedEntry(delMap[0]);
								websRedirect(wp, "Advanced_StorageUser_Content.asp");
							}
							return 0;
						}
#endif
						else if (!strncmp(value1, " SaveProfile ", 13))
						{
							save_PROFILE_group(wp, sid, NULL, groupName);
							if (strcmp(websGetVar(wp, "exmode", ""), "sta")==0)
							{
								sprintf(urlStr, "Advanced_WirelessSTA_Content.asp#%s", groupName);
								websRedirect(wp, urlStr);
							}
							else if (strcmp(websGetVar(wp, "exmode", ""), "re")==0)
							{
								sprintf(urlStr, "Advanced_WirelessURE_Content.asp#%s", groupName);
								websRedirect(wp, urlStr);
							}
							return 0;
						}

						else if (!strncmp(value1, " DelProfile ", 12))
						{
							del_PROFILE_group(wp, sid, NULL, groupName);
							if (strcmp(websGetVar(wp, "exmode", ""), "sta")==0)
							{
								sprintf(urlStr, "Advanced_WirelessSTA_Content.asp#%s", groupName);
								websRedirect(wp, urlStr);
							}
							else if (strcmp(websGetVar(wp, "exmode", ""), "re")==0)
							{
								sprintf(urlStr, "Advanced_WirelessURE_Content.asp#%s", groupName);
								websRedirect(wp, urlStr);
							}
							return 0;
						}

						else if (!strncmp(value1, " MoveUp ", 8))
						{
							moveup_PROFILE_group(wp, sid, NULL, groupName);
							if (strcmp(websGetVar(wp, "exmode", ""), "sta")==0)
							{
								sprintf(urlStr, "Advanced_WirelessSTA_Content.asp#%s", groupName);
								websRedirect(wp, urlStr);
							}
							else if (strcmp(websGetVar(wp, "exmode", ""), "re")==0)
							{
								sprintf(urlStr, "Advanced_WirelessURE_Content.asp#%s", groupName);
								websRedirect(wp, urlStr);
							}
							return 0;
						}

						else if (!strncmp(value1, " MoveDown ", 10))
						{
							movedown_PROFILE_group(wp, sid, NULL, groupName);
							if (strcmp(websGetVar(wp, "exmode", ""), "sta")==0)
							{
								sprintf(urlStr, "Advanced_WirelessSTA_Content.asp#%s", groupName);
								websRedirect(wp, urlStr);
							}
							else if (strcmp(websGetVar(wp, "exmode", ""), "re")==0)
							{
								sprintf(urlStr, "Advanced_WirelessURE_Content.asp#%s", groupName);
								websRedirect(wp, urlStr);
							}
							return 0;
						}

						sprintf(urlStr, "%s#%s", current_url, groupName);

						validate_cgi(wp, sid, FALSE);
//						}
	    	    	     		}	// value1
				}
			}
			sid_list = sid_list + strlen(serviceId)+1;
		}

		if (!strcmp(value, "  Save  "))
		{
			if (	strcmp(current_url, "Wizard_Gateway.asp")==0 ||
		    		strcmp(current_url, "Wizard_AP.asp")==0 ||
		    		strcmp(current_url, "Wizard_Adapter.asp")==0 ||
		    		strcmp(current_url, "Wizard_Repeater.asp")==0
			)
			{
//				nvram_set_f("General", "x_Setting", "1");
				sys_commit(NULL);
				sys_reboot();
				return(0);
			}
		}

		/* Add for Internet Access Quick Setup */
//		special_handler(wp, url);

		/* Add for NVRAM mapping if necessary */
//		nvMap(current_url);

		/* Add for EMI Test page */
		if (strcmp(script, "")!=0)
		{
			sys_script(script);
		}

		if (!strcmp(value, "  Save  ") || !strcmp(value, " Apply "))
		{
			strcpy(urlcache, next_url);
			if (strcmp(current_url, "Advanced_WAdvanced_Content.asp")==0)	// for WL320g to reload treeFrame
				strcpy(urlcache2, current_url);				// for WL320g to reload treeFrame
			websRedirect(wp, next_url);
		}
		else if(!strcmp(value, " Finish "))
		{
			if (strcmp(current_url, "Advanced_WAdvanced_Content.asp")==0)	// for WL320g to reload treeFrame
			{
				strcpy(urlcache, next_url);
				strcpy(urlcache2, current_url);
				websRedirect(wp, next_url);
			}
			else
				websRedirect(wp, "SaveRestart.asp");
		}
    		else if (urlStr[0]==0)
    			websRedirect(wp, current_url);
		else
    			websRedirect(wp, urlStr);

		cprintf("apply ok\n");
		return 0;
	}

footer:
#ifndef NOUSB
	websWrite(wp, (char_t *) apply_footer);
	websFooter(wp);
	websDone(wp, 200);

#ifndef FLASH_EMULATOR
	if (action == RESTART)
		sys_restart();
	else if (action == REBOOT)
		sys_reboot();
#endif
#endif
	return 1;
}


#ifdef REMOVE_WL600
void nvram_add_group_list(webs_t wp, struct variable *v, int count)
{
    struct variable *gv;
    char *value;
    char name[64];
    int i;
    
    if (v->argv[0]==NULL) 
    {
       return;
    }
    
    i = 0;
    for (gv = (struct variable *)v->argv[0]; gv->name!=NULL; gv++)
    {    		    	
    	sprintf(name, "%s_%s_%d", serviceId, gv->name, count+1);    	
        if ((value=websGetVar(wp, name, NULL)))
        {           	
            if (groupItem[i]==0) {
    		groupItem[i]=(char*)malloc(MAX_LINE_SIZE);
    		sprintf(groupItem[i],"%s;",value);
    	    }  	
    	    else sprintf(groupItem[i],"%s%s;",groupItem[i], value);
            /* Don't check validate. Left validate to JavaScript */                        
        }   
        else
        {
            if (i==0) return;	
            if (groupItem[i]==0) {
    		groupItem[i]=(char*)malloc(MAX_LINE_SIZE);
    		sprintf(groupItem[i],";");
    	    }	
            else sprintf(groupItem[i],"%s;",groupItem[i]);
        }   
        i++;
    }   
    return;
}

void nvram_apply_group_list(webs_t wp, struct variable *v)
{
    struct variable *gv;
    int i;
        
    if (v->argv[0]==NULL) 
    {
       return;
    }

    i=0;
    for (gv = (struct variable *)v->argv[0]; gv->name!=NULL; gv++)
    {
    	if (groupItem[i]!=NULL)
    	{
    	   nvram_set_x(serviceId, gv->name, groupItem[i]);
    	   free(groupItem[i]);
    	   groupItem[i]=NULL;
    	}   
    	i++;
    }   
    return;
}
#endif

void nvram_add_group_item(webs_t wp, struct variable *v, int sid)
{
    struct variable *gv;
    char *value;
    char name[64], cstr[5];    
    int count;
    
    if (v->argv[0]==NULL) 
    {
       return;
    }
                
    count = atoi(nvram_safe_get_x(serviceId, v->argv[3]));        
    cprintf("count: %d\n", count);
    
    for (gv = (struct variable *)v->argv[0]; gv->name!=NULL; gv++)
    {    	    		    	    	
    	sprintf(name, "%s_0", gv->name);    	
    	    	    	    	    	    	
        if ((value=websGetVar(wp, name, NULL)))
        {                    
            /*SetGroupVariable(sid, v, gv->name, value, "Add");*/
            nvram_add_lists_x(serviceId, gv->name, value, count);            
            cprintf("Add: %s %s\n", gv->name, value);
        }   
        else
        {        	
            /*SetGroupVariable(sid, v, gv->name, "", "Add");  */
            nvram_add_lists_x(serviceId, gv->name, "", count);             
        }                      
    }   
    
    count++;    
    sprintf(cstr, "%d", count);       
    nvram_set_x(serviceId, v->argv[3], cstr);
    return;
}


void nvram_remove_group_item(webs_t wp, struct variable *v, int sid, int *delMap)
{
    struct variable *gv;
    char *value;
    char glist[MAX_LINE_SIZE], *itemPtr, *glistPtr, cstr[5];    
    int item, i, count;
    
    if (v->argv[0]==NULL) 
    {
       return;
    }
    
    count = atoi(nvram_safe_get_x(serviceId, v->argv[3]));    
    for (gv = (struct variable *)v->argv[0]; gv->name!=NULL; gv++)
    {    	    		    	     	    	
    	nvram_del_lists_x(serviceId, gv->name, delMap);    		    	       
    }           
    
    i=0;
    while(delMap[i]!=-1) i++;
    
    count-=i;
    sprintf(cstr, "%d\n", count);
    nvram_set_x(serviceId, v->argv[3], cstr);
    return;
}

/* Rule for table: 
 * 1. First field can not be NULL, or it is treat as empty
 * 2. 
 */

static int 
nvram_add_group_table(webs_t wp, char *serviceId, struct variable *v, int count)
{
    struct variable *gv;
    char buf[MAX_LINE_SIZE+MAX_LINE_SIZE];
    char bufs[MAX_LINE_SIZE+MAX_LINE_SIZE];    
    int i, j, fieldLen, rowLen, fieldCount, value;
    
    
    if (v->argv[0]==NULL) 
    {
       return 0;
    }
    
    bufs[0] = 0x0;
    rowLen = atoi(v->argv[2]);      
    
    if (count==-1)
    {    
       for(i=0;i<rowLen;i++)
       {     
       	   bufs[i] = ' ';
       }
       value = -1;	       	       	             	
       bufs[i] = 0x0;             	
    	
       goto ToHTML;	
    }
              
                    
    fieldCount = 0;
    
    value = count;
            
    for (gv = (struct variable *)v->argv[0]; gv->name!=NULL; gv++)
    {    		    	    	    	
    	strcpy(buf, nvram_get_list_x(serviceId, gv->name, count));    
    	fieldLen = atoi(gv->longname);
    	    	    	       	        	    
    	if (fieldLen!=0)
    	{    	
    	   if (strlen(buf)>fieldLen)
    	   {    	     
    	      buf[fieldLen] = 0x0; 	
    	   }   
    	   else
    	   {
    	      i = strlen(buf);	
    	      j = i;
    	      for(;i<fieldLen;i++)
    	      {
    	         buf[j++] = ' ';	
    	      }       	      
    	      buf[j] = 0x0;
    	   }      
    	}   
    	
    	if (fieldCount==0)
    	   sprintf(bufs,"%s", buf);
    	else
    	   sprintf(bufs,"%s%s",bufs, buf);
    	       	   
    	fieldCount++;
    }                                              
    
    if (strlen(bufs)> rowLen)
       bufs[rowLen] = 0x0;
       
ToHTML:       
    /* Replace ' ' to &nbsp; */
    buf[0] = 0;
    j = 0;
    
    for (i=0; i<strlen(bufs);i++)
    {
    	if (bufs[i] == ' ')
    	{
    	   buf[j++] = '&';
    	   buf[j++] = 'n';
    	   buf[j++] = 'b';
    	   buf[j++] = 's';
    	   buf[j++] = 'p';
    	   buf[j++] = ';';    	   	
    	} 
    	else buf[j++] = bufs[i];
    }          
    buf[j] = 0x0;
       
    return(websWrite(wp, "<option value=\"%d\">%s</option>", value, buf));
}

static int
apply_cgi_group(webs_t wp, int sid, struct variable *var, char *groupName, int flag)
{	
   struct variable *v;
   char *value, *value1;
   char item[64];
   int i, groupCount;
         
       
   cprintf("This group limit is %d %d\n", flag, sid);

   if (var!=NULL) v = var;
   else
   {         
       /* Validate and set vairables in table order */              
       for (v = GetVariables(sid); v->name != NULL; v++) 
       {
       	   //printf("Find group : %s %s\n", v->name, groupName);
       	   if(!strcmp(groupName, v->name)) break;
       }	                    
   }    
   
   if (v->name == NULL) return 0;    
   
   groupCount = atoi(v->argv[1]);

   if (flag == GROUP_FLAG_ADD)/* if (!strcmp(value, " Refresh ")) || Save condition */
   {    
   	nvram_add_group_item(wp, v, sid);
   }	  
   else if (flag == GROUP_FLAG_REMOVE)/* if (!strcmp(value, " Refresh ")) || Save condition */
   {    
   	/*nvram_remove_group_item(wp, v, sid); 	*/
   	/*sprintf(item, "%s_s", websGetVar(wp, "group_id", ""));*/
        /*value1 = websGetVar(wp, item, NULL);*/
        /* TO DO : change format from 'x=1&x=2&...' to 'x=1 2 3 ..'*/
   	nvram_remove_group_item(wp, v, sid, delMap);   	
   }	     
   return(1);
}

char *nvram_get_i3(char *name, int idx)
{
	char tmpstr1[64];

	memset(tmpstr1, 0, sizeof(tmpstr1));
	if (idx>=1 && idx<=4)
		sprintf(tmpstr1, "pf%d_%s", idx , name);

	return nvram_safe_get_x("", tmpstr1);
}

void nvram_set_i2(char *name, int idx, char *value)
{
	char tmpstr1[64];

	if (idx!=-1)
	{
		sprintf(tmpstr1, "%s%d", name, idx);
	}
	else
	{
		sprintf(tmpstr1, "%s", name); 
	}

	nvram_set(tmpstr1, value);
}

void nvram_set_i3(char *name, int idx, char *value)
{
	char tmpstr1[64];

	if (idx>=1 && idx<=4)
	{
		sprintf(tmpstr1, "pf%d_%s", idx , name);
		nvram_set(tmpstr1, value);
	}
}

static int
modify_SSID_group(webs_t wp, int sid, struct variable *var, char *groupName)
{
   struct variable *v;
   int idx;
                                                                                                                                              
   if (var!=NULL) v = var;
   else
   {
       /* Validate and set vairables in table order */
       for (v = GetVariables(sid); v->name != NULL; v++)
       {
           if(!strcmp(groupName, v->name)) break;
       }
   }
   if (v->name == NULL) return 0;

   idx=atoi(websGetVar(wp, "modify_idx", NULL));

   nvram_set("mssid_enable", websGetVar(wp, "mssid_enable", NULL));
   nvram_set("vlans_enable", websGetVar(wp, "vlans_enable", NULL));
   nvram_set_i2("mssid_ssid_x", idx, websGetVar(wp, "mssid_ssid_x_0", NULL));
   nvram_set_i2("mssid_ssid_2_x", idx, websGetVar(wp, "mssid_ssid_2_x_0", NULL));
   nvram_set_i2("mssid_vlanid_x", idx, websGetVar(wp, "mssid_vlanid_x_0", NULL));
   nvram_set_i2("mssid_auth_mode_x", idx, websGetVar(wp, "mssid_auth_mode_x_0", NULL));
   nvram_set_i2("mssid_crypto_x", idx, websGetVar(wp, "mssid_crypto_x_0", NULL));
   nvram_set_i2("mssid_wpa_psk_x", idx, websGetVar(wp, "mssid_wpa_psk_x_0", NULL));
   nvram_set_i2("mssid_wep_x", idx, websGetVar(wp, "mssid_wep_x_0", NULL));
   nvram_set_i2("mssid_phrase_x", idx, websGetVar(wp, "mssid_phrase_x_0", NULL));
   nvram_set_i2("mssid_key1_x", idx, websGetVar(wp, "mssid_key1_x_0", NULL));
   nvram_set_i2("mssid_key2_x", idx, websGetVar(wp, "mssid_key2_x_0", NULL));
   nvram_set_i2("mssid_key3_x", idx, websGetVar(wp, "mssid_key3_x_0", NULL));
   nvram_set_i2("mssid_key4_x", idx, websGetVar(wp, "mssid_key4_x_0", NULL));
   nvram_set_i2("mssid_key_x", idx, websGetVar(wp, "mssid_key_x_0", NULL));
   nvram_set_i2("mssid_wpa_gtk_rekey_x", idx, websGetVar(wp, "mssid_wpa_gtk_rekey_x_0", NULL));
   nvram_set_i2("mssid_enable_x", idx, websGetVar(wp, "mssid_enable_x_0", NULL));

   return 1;
}

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
                                                                                                               
        *(ptr+1) = '\0';
                                                                                                               
}

static void
PROFILE_compressing()
{
   	if (	strcmp(nvram_get_x("", "pf1_ssid"), "")==0 &&
   		strcmp(nvram_get_x("", "pf2_ssid"), "")==0 &&
   		strcmp(nvram_get_x("", "pf3_ssid"), "")==0 &&
   		strcmp(nvram_get_x("", "pf4_ssid"), "")==0
   	)
   		return;
	
	int i,j;
	char nvram_str1[32];
	char nvram_str2[32];
	char ssid_str[128];
	
	for (j=1; j<4; j++)
	{
		for (i=j; i<4; i++)
		{
			memset(nvram_str1, 0, sizeof(nvram_str1));
			memset(nvram_str2, 0, sizeof(nvram_str2));
			sprintf(nvram_str1, "pf%d_ssid", i);
			sprintf(nvram_str2, "pf%d_ssid", i+1);

			if (strcmp(nvram_get_x("", nvram_str1), "")!=0 && strcmp(nvram_get_x("", nvram_str2), "")==0)
			{
				nvram_set_i3("ssid", i+1, nvram_get_i3("ssid", i));
				memset(ssid_str, 0, sizeof(ssid_str));
				char_to_ascii(ssid_str, nvram_get_i3("ssid", i));
				nvram_set_i3("ssid2", i+1, ssid_str);
				nvram_set_i3("auth_mode", i+1, nvram_get_i3("auth_mode", i));
				nvram_set_i3("wpa_mode", i+1, nvram_get_i3("wpa_mode", i));
				nvram_set_i3("crypto", i+1, nvram_get_i3("crypto", i));
				nvram_set_i3("wpa_psk", i+1, nvram_get_i3("wpa_psk", i));
				nvram_set_i3("wep_x", i+1, nvram_get_i3("wep_x", i));
				nvram_set_i3("phrase", i+1, nvram_get_i3("phrase", i));
				nvram_set_i3("key1", i+1, nvram_get_i3("key1", i));
				nvram_set_i3("key2", i+1, nvram_get_i3("key2", i));
				nvram_set_i3("key3", i+1, nvram_get_i3("key3", i));
				nvram_set_i3("key4", i+1, nvram_get_i3("key4", i));
				nvram_set_i3("key", i+1, nvram_get_i3("key", i));
				
				nvram_set_i3("ssid", i, "");
				nvram_set_i3("ssid2", i, "");
				nvram_set_i3("auth_mode", i, "");
				nvram_set_i3("wpa_mode", i, "");
				nvram_set_i3("crypto", i, "");
				nvram_set_i3("wpa_psk", i, "");
				nvram_set_i3("wep_x", i, "");
				nvram_set_i3("phrase", i, "");
				nvram_set_i3("key1", i, "");
				nvram_set_i3("key2", i, "");
				nvram_set_i3("key3", i, "");
				nvram_set_i3("key4", i, "");
				nvram_set_i3("key", i, "");
			}
		}
	}
}

static int
save_PROFILE_group(webs_t wp, int sid, struct variable *var, char *groupName)
{
	if (!strcmp(current_url_glocal, "Wizard_Gateway.asp") && !strcmp(nvram_get_x("", "wl_hotspot"), "1"))
		return save_PROFILE_group_ure(wp, sid, var, groupName);

	char ssid_str[256];
	int idx=-1;

	PROFILE_compressing();

	if (strcmp(nvram_get_x("", "pf4_ssid"), websGetVar(wp, "wl_ssid", NULL))==0)
		idx=4;
	else if (strcmp(nvram_get_x("", "pf3_ssid"), websGetVar(wp, "wl_ssid", NULL))==0)
		idx=3;
	else if (strcmp(nvram_get_x("", "pf2_ssid"), websGetVar(wp, "wl_ssid", NULL))==0)
		idx=2;
	else if (strcmp(nvram_get_x("", "pf1_ssid"), websGetVar(wp, "wl_ssid", NULL))==0)
		idx=1;
	else
	{
		if (strcmp(nvram_get_x("", "pf4_ssid"), "")==0)
			idx=4;
		else if (strcmp(nvram_get_x("", "pf3_ssid"), "")==0)
			idx=3;
		else if (strcmp(nvram_get_x("", "pf2_ssid"), "")==0)
			idx=2;
		else if (strcmp(nvram_get_x("", "pf1_ssid"), "")==0)
			idx=1;
	}

	if (idx==-1)
		return 0;

	nvram_set_i3("ssid", idx, websGetVar(wp, "wl_ssid", NULL));
	memset(ssid_str, 0, sizeof(ssid_str));
	char_to_ascii(ssid_str, websGetVar(wp, "wl_ssid", NULL));
	nvram_set_i3("ssid2", idx, ssid_str);
	nvram_set_i3("auth_mode", idx, websGetVar(wp, "wl_auth_mode", NULL));
	if (strcmp(websGetVar(wp, "wl_wpa_mode", NULL), "0")==0)
   		nvram_set_i3("wpa_mode", idx, "1");
	else
		nvram_set_i3("wpa_mode", idx, websGetVar(wp, "wl_wpa_mode", NULL));
	nvram_set_i3("crypto", idx, websGetVar(wp, "wl_crypto", NULL));
	nvram_set_i3("wpa_psk", idx, websGetVar(wp, "wl_wpa_psk", NULL));
	nvram_set_i3("wep_x", idx, websGetVar(wp, "wl_wep_x", NULL));
	if (websGetVar(wp, "wl_phrase", NULL)==NULL)
		nvram_set_i3("phrase", idx, websGetVar(wp, "wl_wpa_psk", NULL));
	else
		nvram_set_i3("phrase", idx, websGetVar(wp, "wl_phrase", NULL));
	if (strcmp(websGetVar(wp, "wl_key", NULL), "1")==0)
		nvram_set_i3("key1", idx, websGetVar(wp, "wl_key1", NULL));
	else if (strcmp(websGetVar(wp, "wl_key", NULL), "2")==0)
		nvram_set_i3("key2", idx, websGetVar(wp, "wl_key2", NULL));
	else if (strcmp(websGetVar(wp, "wl_key", NULL), "3")==0)
		nvram_set_i3("key3", idx, websGetVar(wp, "wl_key3", NULL));
	else if (strcmp(websGetVar(wp, "wl_key", NULL), "4")==0)
		nvram_set_i3("key4", idx, websGetVar(wp, "wl_key4", NULL));
	nvram_set_i3("key", idx, websGetVar(wp, "wl_key", NULL));

	if (	strcmp(websGetVar(wp, "wl_auth_mode", NULL), "open")==0 ||
		strcmp(websGetVar(wp, "wl_auth_mode", NULL), "shared")==0
	)
	{
		if (strcmp(websGetVar(wp, "wl_key", NULL), "1")==0)
			nvram_set_i3("passwd", idx, websGetVar(wp, "wl_key1", NULL));
		else if (strcmp(websGetVar(wp, "wl_key", NULL), "2")==0)
			nvram_set_i3("passwd", idx, websGetVar(wp, "wl_key2", NULL));
		else if (strcmp(websGetVar(wp, "wl_key", NULL), "3")==0)
			nvram_set_i3("passwd", idx, websGetVar(wp, "wl_key3", NULL));
		else if (strcmp(websGetVar(wp, "wl_key", NULL), "4")==0)
			nvram_set_i3("passwd", idx, websGetVar(wp, "wl_key4", NULL));
	}
	else
		nvram_set_i3("passwd", idx, websGetVar(wp, "wl_wpa_psk", NULL));

	if(nvram_match_x("", "wl_ap_ssid", ""))
	{
		if (!nvram_match_x("", "pf1_ssid", ""))
			nvram_set("wl_ap_ssid", nvram_safe_get_x("", "pf1_ssid"));
		else if (!nvram_match_x("", "pf2_ssid", ""))
			nvram_set("wl_ap_ssid", nvram_safe_get_x("", "pf2_ssid"));
		else if (!nvram_match_x("", "pf3_ssid", ""))
			nvram_set("wl_ap_ssid", nvram_safe_get_x("", "pf3_ssid"));
		else if (!nvram_match_x("", "pf4_ssid", ""))
			nvram_set("wl_ap_ssid", nvram_safe_get_x("", "pf4_ssid"));
	}

	return 1;
}

static int
save_PROFILE_group_ure(webs_t wp, int sid, struct variable *var, char *groupName)
{
	char ssid_str[256];
	int idx=-1;

	PROFILE_compressing();

	if (strcmp(nvram_get_x("", "pf4_ssid"), websGetVar(wp, "wl_ssid_ure", NULL))==0)
		idx=4;
	else if (strcmp(nvram_get_x("", "pf3_ssid"), websGetVar(wp, "wl_ssid_ure", NULL))==0)
		idx=3;
	else if (strcmp(nvram_get_x("", "pf2_ssid"), websGetVar(wp, "wl_ssid_ure", NULL))==0)
		idx=2;
	else if (strcmp(nvram_get_x("", "pf1_ssid"), websGetVar(wp, "wl_ssid_ure", NULL))==0)
		idx=1;
	else
	{
		if (strcmp(nvram_get_x("", "pf4_ssid"), "")==0)
			idx=4;
		else if (strcmp(nvram_get_x("", "pf3_ssid"), "")==0)
			idx=3;
		else if (strcmp(nvram_get_x("", "pf2_ssid"), "")==0)
			idx=2;
		else if (strcmp(nvram_get_x("", "pf1_ssid"), "")==0)
			idx=1;
	}

	if (idx==-1)
		return 0;

	nvram_set_i3("ssid", idx, websGetVar(wp, "wl_ssid_ure", NULL));
	memset(ssid_str, 0, sizeof(ssid_str));
	char_to_ascii(ssid_str, websGetVar(wp, "wl_ssid_ure", NULL));
	nvram_set_i3("ssid2", idx, ssid_str);
	nvram_set_i3("auth_mode", idx, websGetVar(wp, "wl_auth_mode_ure", NULL));
	if (strcmp(websGetVar(wp, "wl_wpa_mode_ure", NULL), "0")==0)
   		nvram_set_i3("wpa_mode", idx, "1");
	else
		nvram_set_i3("wpa_mode", idx, websGetVar(wp, "wl_wpa_mode_ure", NULL));
	nvram_set_i3("crypto", idx, websGetVar(wp, "wl_crypto_ure", NULL));
	nvram_set_i3("wpa_psk", idx, websGetVar(wp, "wl_wpa_psk_ure", NULL));
	nvram_set_i3("wep_x", idx, websGetVar(wp, "wl_wep_x_ure", NULL));
	if (websGetVar(wp, "wl_phrase_ure", NULL)==NULL)
		nvram_set_i3("phrase", idx, websGetVar(wp, "wl_wpa_psk_ure", NULL));
	else
		nvram_set_i3("phrase", idx, websGetVar(wp, "wl_phrase_ure", NULL));
	if (strcmp(websGetVar(wp, "wl_key_ure", NULL), "1")==0)
		nvram_set_i3("key1", idx, websGetVar(wp, "wl_key1_ure", NULL));
	else if (strcmp(websGetVar(wp, "wl_key_ure", NULL), "2")==0)
		nvram_set_i3("key2", idx, websGetVar(wp, "wl_key2_ure", NULL));
	else if (strcmp(websGetVar(wp, "wl_key_ure", NULL), "3")==0)
		nvram_set_i3("key3", idx, websGetVar(wp, "wl_key3_ure", NULL));
	else if (strcmp(websGetVar(wp, "wl_key_ure", NULL), "4")==0)
		nvram_set_i3("key4", idx, websGetVar(wp, "wl_key4_ure", NULL));
	nvram_set_i3("key", idx, websGetVar(wp, "wl_key_ure", NULL));

	if (	strcmp(websGetVar(wp, "wl_auth_mode_ure", NULL), "open")==0 ||
		strcmp(websGetVar(wp, "wl_auth_mode_ure", NULL), "shared")==0
	)
	{
		if (strcmp(websGetVar(wp, "wl_key_ure", NULL), "1")==0)
			nvram_set_i3("passwd", idx, websGetVar(wp, "wl_key1_ure", NULL));
		else if (strcmp(websGetVar(wp, "wl_key_ure", NULL), "2")==0)
			nvram_set_i3("passwd", idx, websGetVar(wp, "wl_key2_ure", NULL));
		else if (strcmp(websGetVar(wp, "wl_key_ure", NULL), "3")==0)
			nvram_set_i3("passwd", idx, websGetVar(wp, "wl_key3_ure", NULL));
		else if (strcmp(websGetVar(wp, "wl_key_ure", NULL), "4")==0)
			nvram_set_i3("passwd", idx, websGetVar(wp, "wl_key4_ure", NULL));
	}
	else
		nvram_set_i3("passwd", idx, websGetVar(wp, "wl_wpa_psk_ure", NULL));

	if(nvram_match_x("", "wl_ap_ssid", ""))
	{
		if (!nvram_match_x("", "pf1_ssid", ""))
			nvram_set("wl_ap_ssid", nvram_safe_get_x("", "pf1_ssid"));
		else if (!nvram_match_x("", "pf2_ssid", ""))
			nvram_set("wl_ap_ssid", nvram_safe_get_x("", "pf2_ssid"));
		else if (!nvram_match_x("", "pf3_ssid", ""))
			nvram_set("wl_ap_ssid", nvram_safe_get_x("", "pf3_ssid"));
		else if (!nvram_match_x("", "pf4_ssid", ""))
			nvram_set("wl_ap_ssid", nvram_safe_get_x("", "pf4_ssid"));
	}

	return 1;
}

static int
save_PROFILE_group_SP2(webs_t wp, int sid, struct variable *var, char *groupName)
{
	if (!strcmp(current_url_glocal, "Wizard_Gateway.asp") && !strcmp(nvram_get_x("", "wl_hotspot"), "1"))
		return save_PROFILE_group_SP2_ure(wp, sid, var, groupName);

	char ssid_str[256];
	int idx=4;

	PROFILE_compressing();

	nvram_set_i3("ssid", idx, websGetVar(wp, "wl_ssid", NULL));
	memset(ssid_str, 0, sizeof(ssid_str));
	char_to_ascii(ssid_str, websGetVar(wp, "wl_ssid", NULL));
	nvram_set_i3("ssid2", idx, ssid_str);
	nvram_set_i3("auth_mode", idx, websGetVar(wp, "wl_auth_mode", NULL));
	if (strcmp(websGetVar(wp, "wl_wpa_mode", NULL), "0")==0)
   		nvram_set_i3("wpa_mode", idx, "1");
	else
		nvram_set_i3("wpa_mode", idx, websGetVar(wp, "wl_wpa_mode", NULL));
	nvram_set_i3("crypto", idx, websGetVar(wp, "wl_crypto", NULL));
	nvram_set_i3("wpa_psk", idx, websGetVar(wp, "wl_wpa_psk", NULL));
	nvram_set_i3("wep_x", idx, websGetVar(wp, "wl_wep_x", NULL));
	if (websGetVar(wp, "wl_phrase", NULL)==NULL)
		nvram_set_i3("phrase", idx, websGetVar(wp, "wl_wpa_psk", NULL));
	else
		nvram_set_i3("phrase", idx, websGetVar(wp, "wl_phrase", NULL));
	if (strcmp(websGetVar(wp, "wl_key", NULL), "1")==0)
		nvram_set_i3("key1", idx, websGetVar(wp, "wl_key1", NULL));
	else if (strcmp(websGetVar(wp, "wl_key", NULL), "2")==0)
		nvram_set_i3("key2", idx, websGetVar(wp, "wl_key2", NULL));
	else if (strcmp(websGetVar(wp, "wl_key", NULL), "3")==0)
		nvram_set_i3("key3", idx, websGetVar(wp, "wl_key3", NULL));
	else if (strcmp(websGetVar(wp, "wl_key", NULL), "4")==0)
		nvram_set_i3("key4", idx, websGetVar(wp, "wl_key4", NULL));
	nvram_set_i3("key", idx, websGetVar(wp, "wl_key", NULL));

	if (	strcmp(websGetVar(wp, "wl_auth_mode", NULL), "open")==0 ||
		strcmp(websGetVar(wp, "wl_auth_mode", NULL), "shared")==0
	)
	{
		if (strcmp(websGetVar(wp, "wl_key", NULL), "1")==0)
			nvram_set_i3("passwd", idx, websGetVar(wp, "wl_key1", NULL));
		else if (strcmp(websGetVar(wp, "wl_key", NULL), "2")==0)
			nvram_set_i3("passwd", idx, websGetVar(wp, "wl_key2", NULL));
		else if (strcmp(websGetVar(wp, "wl_key", NULL), "3")==0)
			nvram_set_i3("passwd", idx, websGetVar(wp, "wl_key3", NULL));
		else if (strcmp(websGetVar(wp, "wl_key", NULL), "4")==0)
			nvram_set_i3("passwd", idx, websGetVar(wp, "wl_key4", NULL));
	}
	else
		nvram_set_i3("passwd", idx, websGetVar(wp, "wl_wpa_psk", NULL));

	if(nvram_match_x("", "wl_ap_ssid", ""))
	{
		if (!nvram_match_x("", "pf1_ssid", ""))
			nvram_set("wl_ap_ssid", nvram_safe_get_x("", "pf1_ssid"));
		else if (!nvram_match_x("", "pf2_ssid", ""))
			nvram_set("wl_ap_ssid", nvram_safe_get_x("", "pf2_ssid"));
		else if (!nvram_match_x("", "pf3_ssid", ""))
			nvram_set("wl_ap_ssid", nvram_safe_get_x("", "pf3_ssid"));
		else if (!nvram_match_x("", "pf4_ssid", ""))
			nvram_set("wl_ap_ssid", nvram_safe_get_x("", "pf4_ssid"));
	}

	return 1;
}

static int
save_PROFILE_group_SP2_ure(webs_t wp, int sid, struct variable *var, char *groupName)
{
	char ssid_str[256];
	int idx=4;

	PROFILE_compressing();

	nvram_set_i3("ssid", idx, websGetVar(wp, "wl_ssid_ure", NULL));
	memset(ssid_str, 0, sizeof(ssid_str));
	char_to_ascii(ssid_str, websGetVar(wp, "wl_ssid_ure", NULL));
	nvram_set_i3("ssid2", idx, ssid_str);
	nvram_set_i3("auth_mode", idx, websGetVar(wp, "wl_auth_mode_ure", NULL));
	if (strcmp(websGetVar(wp, "wl_wpa_mode_ure", NULL), "0")==0)
   		nvram_set_i3("wpa_mode", idx, "1");
	else
		nvram_set_i3("wpa_mode", idx, websGetVar(wp, "wl_wpa_mode_ure", NULL));
	nvram_set_i3("crypto", idx, websGetVar(wp, "wl_crypto_ure", NULL));
	nvram_set_i3("wpa_psk", idx, websGetVar(wp, "wl_wpa_psk_ure", NULL));
	nvram_set_i3("wep_x", idx, websGetVar(wp, "wl_wep_x_ure", NULL));
	if (websGetVar(wp, "wl_phrase_ure", NULL)==NULL)
		nvram_set_i3("phrase", idx, websGetVar(wp, "wl_wpa_psk_ure", NULL));
	else
		nvram_set_i3("phrase", idx, websGetVar(wp, "wl_phrase_ure", NULL));
	if (strcmp(websGetVar(wp, "wl_key_ure", NULL), "1")==0)
		nvram_set_i3("key1", idx, websGetVar(wp, "wl_key1_ure", NULL));
	else if (strcmp(websGetVar(wp, "wl_key_ure", NULL), "2")==0)
		nvram_set_i3("key2", idx, websGetVar(wp, "wl_key2_ure", NULL));
	else if (strcmp(websGetVar(wp, "wl_key_ure", NULL), "3")==0)
		nvram_set_i3("key3", idx, websGetVar(wp, "wl_key3_ure", NULL));
	else if (strcmp(websGetVar(wp, "wl_key_ure", NULL), "4")==0)
		nvram_set_i3("key4", idx, websGetVar(wp, "wl_key4_ure", NULL));
	nvram_set_i3("key", idx, websGetVar(wp, "wl_key_ure", NULL));

	if (	strcmp(websGetVar(wp, "wl_auth_mode_ure", NULL), "open")==0 ||
		strcmp(websGetVar(wp, "wl_auth_mode_ure", NULL), "shared")==0
	)
	{
		if (strcmp(websGetVar(wp, "wl_key_ure", NULL), "1")==0)
			nvram_set_i3("passwd", idx, websGetVar(wp, "wl_key1_ure", NULL));
		else if (strcmp(websGetVar(wp, "wl_key_ure", NULL), "2")==0)
			nvram_set_i3("passwd", idx, websGetVar(wp, "wl_key2_ure", NULL));
		else if (strcmp(websGetVar(wp, "wl_key_ure", NULL), "3")==0)
			nvram_set_i3("passwd", idx, websGetVar(wp, "wl_key3_ure", NULL));
		else if (strcmp(websGetVar(wp, "wl_key_ure", NULL), "4")==0)
			nvram_set_i3("passwd", idx, websGetVar(wp, "wl_key4_ure", NULL));
	}
	else
		nvram_set_i3("passwd", idx, websGetVar(wp, "wl_wpa_psk_ure", NULL));

	if(nvram_match_x("", "wl_ap_ssid", ""))
	{
		if (!nvram_match_x("", "pf1_ssid", ""))
			nvram_set("wl_ap_ssid", nvram_safe_get_x("", "pf1_ssid"));
		else if (!nvram_match_x("", "pf2_ssid", ""))
			nvram_set("wl_ap_ssid", nvram_safe_get_x("", "pf2_ssid"));
		else if (!nvram_match_x("", "pf3_ssid", ""))
			nvram_set("wl_ap_ssid", nvram_safe_get_x("", "pf3_ssid"));
		else if (!nvram_match_x("", "pf4_ssid", ""))
			nvram_set("wl_ap_ssid", nvram_safe_get_x("", "pf4_ssid"));
	}

	return 1;
}

static int
del_PROFILE_group(webs_t wp, int sid, struct variable *var, char *groupName)
{
	char nvram_str[32];
	int ap_idx=0;

	if (websGetVar(wp, "pf1_delete", NULL)!=NULL)
	{
		nvram_set_i3("ssid", 1, "");
		nvram_set_i3("ssid2", 1, "");
		nvram_set_i3("auth_mode", 1, "open");
		nvram_set_i3("wpa_mode", 1, "1");
		nvram_set_i3("crypto", 1, "tkip");
		nvram_set_i3("wpa_psk", 1, "12345678");
		nvram_set_i3("wep_x", 1, "0");
		nvram_set_i3("phrase", 1, "");
		nvram_set_i3("key1", 1, "");
		nvram_set_i3("key2", 1, "");
		nvram_set_i3("key3", 1, "");
		nvram_set_i3("key4", 1, "");
		nvram_set_i3("key", 1, "1");
		nvram_set_i3("passwd", 1, "");
	}

	else if (websGetVar(wp, "pf2_delete", NULL)!=NULL)
	{
		nvram_set_i3("ssid", 2, "");
		nvram_set_i3("ssid2", 2, "");
		nvram_set_i3("auth_mode", 2, "open");
		nvram_set_i3("wpa_mode", 2, "1");
		nvram_set_i3("crypto", 2, "tkip");
		nvram_set_i3("wpa_psk", 2, "12345678");
		nvram_set_i3("wep_x", 2, "0");
		nvram_set_i3("phrase", 2, "");
		nvram_set_i3("key1", 2, "");
		nvram_set_i3("key2", 2, "");
		nvram_set_i3("key3", 2, "");
		nvram_set_i3("key4", 2, "");
		nvram_set_i3("key", 2, "1");
		nvram_set_i3("passwd", 2, "");
	}

	else if (websGetVar(wp, "pf3_delete", NULL)!=NULL)
	{
		nvram_set_i3("ssid", 3, "");
		nvram_set_i3("ssid2", 3, "");
		nvram_set_i3("auth_mode", 3, "open");
		nvram_set_i3("wpa_mode", 3, "1");
		nvram_set_i3("crypto", 3, "tkip");
		nvram_set_i3("wpa_psk", 3, "12345678");
		nvram_set_i3("wep_x", 3, "0");
		nvram_set_i3("phrase", 3, "");
		nvram_set_i3("key1", 3, "");
		nvram_set_i3("key2", 3, "");
		nvram_set_i3("key3", 3, "");
		nvram_set_i3("key4", 3, "");
		nvram_set_i3("key", 3, "1");
		nvram_set_i3("passwd", 3, "");
	}

	else if (websGetVar(wp, "pf4_delete", NULL)!=NULL)
	{
		nvram_set_i3("ssid", 4, "");
		nvram_set_i3("ssid2", 4, "");
		nvram_set_i3("auth_mode", 4, "open");
		nvram_set_i3("wpa_mode", 4, "1");
		nvram_set_i3("crypto", 4, "tkip");
		nvram_set_i3("wpa_psk", 4, "12345678");
		nvram_set_i3("wep_x", 4, "0");
		nvram_set_i3("phrase", 4, "");
		nvram_set_i3("key1", 4, "");
		nvram_set_i3("key2", 4, "");
		nvram_set_i3("key3", 4, "");
		nvram_set_i3("key4", 4, "");
		nvram_set_i3("key", 4, "1");
		nvram_set_i3("passwd", 4, "");
	}

	PROFILE_compressing();

	if (	nvram_match_x("", "wl_connect_selected", "1") &&
		!nvram_match_x("", "wl_ap_ssid", "")
	)
	{
		if (	strcmp(nvram_get("wl_ap_ssid"), nvram_get("pf1_ssid")) &&
			strcmp(nvram_get("wl_ap_ssid"), nvram_get("pf2_ssid")) &&
			strcmp(nvram_get("wl_ap_ssid"), nvram_get("pf3_ssid")) &&
			strcmp(nvram_get("wl_ap_ssid"), nvram_get("pf4_ssid"))
		)
		{
			if (!nvram_match_x("", "pf1_ssid", ""))
				ap_idx=1;
			else if (!nvram_match_x("", "pf2_ssid", ""))
				ap_idx=2;
			else if (!nvram_match_x("", "pf3_ssid", ""))
				ap_idx=3;
			else if (!nvram_match_x("", "pf4_ssid", ""))
				ap_idx=4;

			if (!ap_idx)
				nvram_set("wl_ap_ssid", "");
			else
			{
				memset(nvram_str, 0, sizeof(nvram_str));
				sprintf(nvram_str, "pf%d_ssid", ap_idx);
				nvram_set("wl_ap_ssid", nvram_get(nvram_str));
			}
		}
	}

	return 1;
}

static int
save_PROFILE_group_SP(webs_t wp, int sid, struct variable *var, char *groupName)
{
	if (	strcmp(nvram_get_x("", "pf4_ssid"), "") ||
		strcmp(nvram_get_x("", "pf3_ssid"), "") ||
		strcmp(nvram_get_x("", "pf2_ssid"), "") ||
		strcmp(nvram_get_x("", "pf1_ssid"), "")
	)
		return 1;

	int idx=4;

	nvram_set_i3("ssid", idx, nvram_get_x("", "wl_ssid"));
	nvram_set_i3("ssid2", idx, nvram_get_x("", "wl_ssid2"));

	nvram_set_i3("wpa_mode", idx, "1");
	nvram_set_i3("crypto", idx, "tkip");
	nvram_set_i3("wpa_psk", idx, nvram_get_x("", "wl_wpa_psk"));
	nvram_set_i3("wep_x", idx, nvram_get_x("", "wl_wep_x"));
	
	if (	!strcmp(nvram_get_x("", "wl_auth_mode"), "open") ||
		!strcmp(nvram_get_x("", "wl_auth_mode"), "shared") ||
		!strcmp(nvram_get_x("", "wl_auth_mode"), "psk")
	)
		nvram_set_i3("auth_mode", idx, nvram_get_x("", "wl_auth_mode"));
	else
	{
		nvram_set_i3("auth_mode", idx, "open");
		nvram_set_i3("wep_x", idx, "0");
	}

	nvram_set_i3("phrase", idx, nvram_get_x("", "wl_phrase"));
	if (strcmp(nvram_get_x("", "wl_key"), "1")==0)
		nvram_set_i3("key1", idx, nvram_get_x("", "wl_key1"));
	else if (strcmp(nvram_get_x("", "wl_key"), "2")==0)
		nvram_set_i3("key2", idx, nvram_get_x("", "wl_key2"));
	else if (strcmp(nvram_get_x("", "wl_key"), "3")==0)
		nvram_set_i3("key3", idx, nvram_get_x("", "wl_key3"));
	else if (strcmp(nvram_get_x("", "wl_key"), "4")==0)
		nvram_set_i3("key4", idx, nvram_get_x("", "wl_key4"));
	nvram_set_i3("key", idx, nvram_get_x("", "wl_key"));

	if (	strcmp(nvram_get_x("", "wl_auth_mode"), "open")==0 ||
		strcmp(nvram_get_x("", "wl_auth_mode"), "shared")==0
	)
	{
		if (strcmp(nvram_get_x("", "wl_key"), "1")==0)
			nvram_set_i3("passwd", idx, nvram_get_x("", "wl_key1"));
		else if (strcmp(nvram_get_x("", "wl_key"), "2")==0)
			nvram_set_i3("passwd", idx, nvram_get_x("", "wl_key2"));
		else if (strcmp(nvram_get_x("", "wl_key"), "3")==0)
			nvram_set_i3("passwd", idx, nvram_get_x("", "wl_key3"));
		else if (strcmp(nvram_get_x("", "wl_key"), "4")==0)
			nvram_set_i3("passwd", idx, nvram_get_x("", "wl_key4"));
	}
	else
		nvram_set_i3("passwd", idx, nvram_get_x("", "wl_wpa_psk"));

	nvram_set("wl_ap_ssid", nvram_get_x("", "pf4_ssid"));
	
	return 1;
}

static int
moveup_PROFILE_group(webs_t wp, int sid, struct variable *var, char *groupName)
{
	int pf_idx=0;
	char nvram_str1[32];

	char ssid_str[33];
	char ssid2_str[128];
	char auth_mode_str[32];
	char wpa_mode_str[32];
	char crypto_str[32];
	char wpa_psk_str[128];
	char wep_x_str[32];
	char phrase_str[128];
	char key1_str[128];
	char key2_str[128];
	char key3_str[128];
	char key4_str[128];
	char key_str[32];
	char passwd_str[128];
	
	if (websGetVar(wp, "pf1_delete", NULL)!=NULL)
		pf_idx=1;
	else if (websGetVar(wp, "pf2_delete", NULL)!=NULL)
		pf_idx=2;
	else if (websGetVar(wp, "pf3_delete", NULL)!=NULL)
		pf_idx=3;
	else if (websGetVar(wp, "pf4_delete", NULL)!=NULL)
		pf_idx=4;
	
	if (pf_idx==1 || pf_idx==0)
		return 1;

	memset(nvram_str1, 0, sizeof(nvram_str1));
	sprintf(nvram_str1, "pf%d_ssid", pf_idx-1);
	if (strcmp(nvram_get_x("", nvram_str1), "")!=0)
	{
		memset(ssid_str, 0, sizeof(ssid_str));
		memset(ssid2_str, 0, sizeof(ssid2_str));
		memset(auth_mode_str, 0, sizeof(auth_mode_str));
		memset(wpa_mode_str, 0, sizeof(wpa_mode_str));
		memset(crypto_str, 0, sizeof(crypto_str));
		memset(wpa_psk_str, 0, sizeof(wpa_psk_str));
		memset(wep_x_str, 0, sizeof(wep_x_str));
		memset(phrase_str, 0, sizeof(phrase_str));
		memset(key1_str, 0, sizeof(key1_str));
		memset(key2_str, 0, sizeof(key2_str));
		memset(key3_str, 0, sizeof(key3_str));
		memset(key4_str, 0, sizeof(key4_str));
		memset(key_str, 0, sizeof(key_str));
		memset(passwd_str, 0, sizeof(passwd_str));
		
		strcpy(ssid_str, nvram_get_i3("ssid", pf_idx-1));
		strcpy(ssid2_str, nvram_get_i3("ssid2", pf_idx-1));
		strcpy(auth_mode_str, nvram_get_i3("auth_mode", pf_idx-1));
		strcpy(wpa_mode_str, nvram_get_i3("wpa_mode", pf_idx-1));
		strcpy(crypto_str, nvram_get_i3("crypto", pf_idx-1));
		strcpy(wpa_psk_str, nvram_get_i3("wpa_psk", pf_idx-1));
		strcpy(wep_x_str, nvram_get_i3("wep_x", pf_idx-1));
		strcpy(phrase_str, nvram_get_i3("phrase", pf_idx-1));
		strcpy(key1_str, nvram_get_i3("key1", pf_idx-1));
		strcpy(key2_str, nvram_get_i3("key2", pf_idx-1));
		strcpy(key3_str, nvram_get_i3("key3", pf_idx-1));
		strcpy(key4_str, nvram_get_i3("key4", pf_idx-1));
		strcpy(key_str, nvram_get_i3("key", pf_idx-1));
		strcpy(passwd_str, nvram_get_i3("passwd", pf_idx-1));

		nvram_set_i3("ssid", pf_idx-1, nvram_get_i3("ssid", pf_idx));
		nvram_set_i3("ssid2", pf_idx-1, nvram_get_i3("ssid2", pf_idx));
		nvram_set_i3("auth_mode", pf_idx-1, nvram_get_i3("auth_mode", pf_idx));
		nvram_set_i3("wpa_mode", pf_idx-1, nvram_get_i3("wpa_mode", pf_idx));
		nvram_set_i3("crypto", pf_idx-1, nvram_get_i3("crypto", pf_idx));
		nvram_set_i3("wpa_psk", pf_idx-1, nvram_get_i3("wpa_psk", pf_idx));
		nvram_set_i3("wep_x", pf_idx-1, nvram_get_i3("wep_x", pf_idx));
		nvram_set_i3("phrase", pf_idx-1, nvram_get_i3("phrase", pf_idx));
		nvram_set_i3("key1", pf_idx-1, nvram_get_i3("key1", pf_idx));
		nvram_set_i3("key2", pf_idx-1, nvram_get_i3("key2", pf_idx));
		nvram_set_i3("key3", pf_idx-1, nvram_get_i3("key3", pf_idx));
		nvram_set_i3("key4", pf_idx-1, nvram_get_i3("key4", pf_idx));
		nvram_set_i3("key", pf_idx-1, nvram_get_i3("key", pf_idx));
		nvram_set_i3("passwd", pf_idx-1, nvram_get_i3("passwd", pf_idx));
		
		nvram_set_i3("ssid", pf_idx, ssid_str);
		nvram_set_i3("ssid2", pf_idx, ssid2_str);
		nvram_set_i3("auth_mode", pf_idx, auth_mode_str);
		nvram_set_i3("wpa_mode", pf_idx, wpa_mode_str);
		nvram_set_i3("crypto", pf_idx, crypto_str);
		nvram_set_i3("wpa_psk", pf_idx, wpa_psk_str);
		nvram_set_i3("wep_x", pf_idx, wep_x_str);
		nvram_set_i3("phrase", pf_idx, phrase_str);
		nvram_set_i3("key1", pf_idx, key1_str);
		nvram_set_i3("key2", pf_idx, key2_str);
		nvram_set_i3("key3", pf_idx, key3_str);
		nvram_set_i3("key4", pf_idx, key4_str);
		nvram_set_i3("key", pf_idx, key_str);
		nvram_set_i3("passwd", pf_idx, passwd_str);
	}

	return 1;
}

static int
moveup_PROFILE_group_SP(webs_t wp, int sid, struct variable *var, char *groupName)
{
	int pf_idx=0;
	char nvram_str1[32];

	char ssid_str[33];
	char ssid2_str[128];
	char auth_mode_str[32];
	char wpa_mode_str[32];
	char crypto_str[32];
	char wpa_psk_str[128];
	char wep_x_str[32];
	char phrase_str[128];
	char key1_str[128];
	char key2_str[128];
	char key3_str[128];
	char key4_str[128];
	char key_str[32];
	char passwd_str[128];

	if (!strcmp(current_url_glocal, "Wizard_Gateway.asp") && !strcmp(nvram_get_x("", "wl_hotspot"), "1"))
	{
		if (strcmp(nvram_get_x("", "pf4_ssid"), websGetVar(wp, "wl_ssid_ure", NULL))==0)
			pf_idx=4;
		else if (strcmp(nvram_get_x("", "pf3_ssid"), websGetVar(wp, "wl_ssid_ure", NULL))==0)
			pf_idx=3;
		else if (strcmp(nvram_get_x("", "pf2_ssid"), websGetVar(wp, "wl_ssid_ure", NULL))==0)
			pf_idx=2;
		else if (strcmp(nvram_get_x("", "pf1_ssid"), websGetVar(wp, "wl_ssid_ure", NULL))==0)
			pf_idx=1;
	}
	else
	{
		if (strcmp(nvram_get_x("", "pf4_ssid"), websGetVar(wp, "wl_ssid", NULL))==0)
			pf_idx=4;
		else if (strcmp(nvram_get_x("", "pf3_ssid"), websGetVar(wp, "wl_ssid", NULL))==0)
			pf_idx=3;
		else if (strcmp(nvram_get_x("", "pf2_ssid"), websGetVar(wp, "wl_ssid", NULL))==0)
			pf_idx=2;
		else if (strcmp(nvram_get_x("", "pf1_ssid"), websGetVar(wp, "wl_ssid", NULL))==0)
			pf_idx=1;
	}
	
	if (pf_idx==1 || pf_idx==0)
		return 1;

	if (!strcmp(current_url_glocal, "Wizard_Gateway.asp") && !strcmp(nvram_get_x("", "wl_mode_ex"), "re"))
		nvram_set_i3("crypto", pf_idx, websGetVar(wp, "wl_crypto_ure", NULL));
	else
		nvram_set_i3("crypto", pf_idx, websGetVar(wp, "wl_crypto", NULL));

	memset(nvram_str1, 0, sizeof(nvram_str1));
	sprintf(nvram_str1, "pf%d_ssid", pf_idx-1);
	if (strcmp(nvram_get_x("", nvram_str1), "")!=0)
	{
		memset(ssid_str, 0, sizeof(ssid_str));
		memset(ssid2_str, 0, sizeof(ssid2_str));
		memset(auth_mode_str, 0, sizeof(auth_mode_str));
		memset(wpa_mode_str, 0, sizeof(wpa_mode_str));
		memset(crypto_str, 0, sizeof(crypto_str));
		memset(wpa_psk_str, 0, sizeof(wpa_psk_str));
		memset(wep_x_str, 0, sizeof(wep_x_str));
		memset(phrase_str, 0, sizeof(phrase_str));
		memset(key1_str, 0, sizeof(key1_str));
		memset(key2_str, 0, sizeof(key2_str));
		memset(key3_str, 0, sizeof(key3_str));
		memset(key4_str, 0, sizeof(key4_str));
		memset(key_str, 0, sizeof(key_str));
		memset(passwd_str, 0, sizeof(passwd_str));
		
		strcpy(ssid_str, nvram_get_i3("ssid", pf_idx-1));
		strcpy(ssid2_str, nvram_get_i3("ssid2", pf_idx-1));
		strcpy(auth_mode_str, nvram_get_i3("auth_mode", pf_idx-1));
		strcpy(wpa_mode_str, nvram_get_i3("wpa_mode", pf_idx-1));
		strcpy(crypto_str, nvram_get_i3("crypto", pf_idx-1));
		strcpy(wpa_psk_str, nvram_get_i3("wpa_psk", pf_idx-1));
		strcpy(wep_x_str, nvram_get_i3("wep_x", pf_idx-1));
		strcpy(phrase_str, nvram_get_i3("phrase", pf_idx-1));
		strcpy(key1_str, nvram_get_i3("key1", pf_idx-1));
		strcpy(key2_str, nvram_get_i3("key2", pf_idx-1));
		strcpy(key3_str, nvram_get_i3("key3", pf_idx-1));
		strcpy(key4_str, nvram_get_i3("key4", pf_idx-1));
		strcpy(key_str, nvram_get_i3("key", pf_idx-1));
		strcpy(passwd_str, nvram_get_i3("passwd", pf_idx-1));

		nvram_set_i3("ssid", pf_idx-1, nvram_get_i3("ssid", pf_idx));
		nvram_set_i3("ssid2", pf_idx-1, nvram_get_i3("ssid2", pf_idx));
		nvram_set_i3("auth_mode", pf_idx-1, nvram_get_i3("auth_mode", pf_idx));
		nvram_set_i3("wpa_mode", pf_idx-1, nvram_get_i3("wpa_mode", pf_idx));
		nvram_set_i3("crypto", pf_idx-1, nvram_get_i3("crypto", pf_idx));
		nvram_set_i3("wpa_psk", pf_idx-1, nvram_get_i3("wpa_psk", pf_idx));
		nvram_set_i3("wep_x", pf_idx-1, nvram_get_i3("wep_x", pf_idx));
		nvram_set_i3("phrase", pf_idx-1, nvram_get_i3("phrase", pf_idx));
		nvram_set_i3("key1", pf_idx-1, nvram_get_i3("key1", pf_idx));
		nvram_set_i3("key2", pf_idx-1, nvram_get_i3("key2", pf_idx));
		nvram_set_i3("key3", pf_idx-1, nvram_get_i3("key3", pf_idx));
		nvram_set_i3("key4", pf_idx-1, nvram_get_i3("key4", pf_idx));
		nvram_set_i3("key", pf_idx-1, nvram_get_i3("key", pf_idx));
		nvram_set_i3("passwd", pf_idx-1, nvram_get_i3("passwd", pf_idx));
		
		nvram_set_i3("ssid", pf_idx, ssid_str);
		nvram_set_i3("ssid2", pf_idx, ssid2_str);
		nvram_set_i3("auth_mode", pf_idx, auth_mode_str);
		nvram_set_i3("wpa_mode", pf_idx, wpa_mode_str);
		nvram_set_i3("crypto", pf_idx, crypto_str);
		nvram_set_i3("wpa_psk", pf_idx, wpa_psk_str);
		nvram_set_i3("wep_x", pf_idx, wep_x_str);
		nvram_set_i3("phrase", pf_idx, phrase_str);
		nvram_set_i3("key1", pf_idx, key1_str);
		nvram_set_i3("key2", pf_idx, key2_str);
		nvram_set_i3("key3", pf_idx, key3_str);
		nvram_set_i3("key4", pf_idx, key4_str);
		nvram_set_i3("key", pf_idx, key_str);
		nvram_set_i3("passwd", pf_idx, passwd_str);
	}

	return 1;
}

static int
movedown_PROFILE_group(webs_t wp, int sid, struct variable *var, char *groupName)
{
	int pf_idx=0;
	char nvram_str1[32];

	char ssid_str[33];
	char ssid2_str[128];
	char auth_mode_str[32];
	char wpa_mode_str[32];
	char crypto_str[32];
	char wpa_psk_str[128];
	char wep_x_str[32];
	char phrase_str[128];
	char key1_str[128];
	char key2_str[128];
	char key3_str[128];
	char key4_str[128];
	char key_str[32];
	char passwd_str[128];
	
	if (websGetVar(wp, "pf1_delete", NULL)!=NULL)
		pf_idx=1;
	else if (websGetVar(wp, "pf2_delete", NULL)!=NULL)
		pf_idx=2;
	else if (websGetVar(wp, "pf3_delete", NULL)!=NULL)
		pf_idx=3;
	else if (websGetVar(wp, "pf4_delete", NULL)!=NULL)
		pf_idx=4;
	
	if (pf_idx==4 || pf_idx==0)
		return 1;

	memset(nvram_str1, 0, sizeof(nvram_str1));
	sprintf(nvram_str1, "pf%d_ssid", pf_idx+1);
	if (strcmp(nvram_get_x("", nvram_str1), "")!=0)
	{
		memset(ssid_str, 0, sizeof(ssid_str));
		memset(ssid2_str, 0, sizeof(ssid2_str));
		memset(auth_mode_str, 0, sizeof(auth_mode_str));
		memset(wpa_mode_str, 0, sizeof(wpa_mode_str));
		memset(crypto_str, 0, sizeof(crypto_str));
		memset(wpa_psk_str, 0, sizeof(wpa_psk_str));
		memset(wep_x_str, 0, sizeof(wep_x_str));
		memset(phrase_str, 0, sizeof(phrase_str));
		memset(key1_str, 0, sizeof(key1_str));
		memset(key2_str, 0, sizeof(key2_str));
		memset(key3_str, 0, sizeof(key3_str));
		memset(key4_str, 0, sizeof(key4_str));
		memset(key_str, 0, sizeof(key_str));
		memset(passwd_str, 0, sizeof(passwd_str));
		
		strcpy(ssid_str, nvram_get_i3("ssid", pf_idx+1));
		strcpy(ssid2_str, nvram_get_i3("ssid2", pf_idx+1));
		strcpy(auth_mode_str, nvram_get_i3("auth_mode", pf_idx+1));
		strcpy(wpa_mode_str, nvram_get_i3("wpa_mode", pf_idx+1));
		strcpy(crypto_str, nvram_get_i3("crypto", pf_idx+1));
		strcpy(wpa_psk_str, nvram_get_i3("wpa_psk", pf_idx+1));
		strcpy(wep_x_str, nvram_get_i3("wep_x", pf_idx+1));
		strcpy(phrase_str, nvram_get_i3("phrase", pf_idx+1));
		strcpy(key1_str, nvram_get_i3("key1", pf_idx+1));
		strcpy(key2_str, nvram_get_i3("key2", pf_idx+1));
		strcpy(key3_str, nvram_get_i3("key3", pf_idx+1));
		strcpy(key4_str, nvram_get_i3("key4", pf_idx+1));
		strcpy(key_str, nvram_get_i3("key", pf_idx+1));
		strcpy(passwd_str, nvram_get_i3("passwd", pf_idx+1));

		nvram_set_i3("ssid", pf_idx+1, nvram_get_i3("ssid", pf_idx));
		nvram_set_i3("ssid2", pf_idx+1, nvram_get_i3("ssid2", pf_idx));
		nvram_set_i3("auth_mode", pf_idx+1, nvram_get_i3("auth_mode", pf_idx));
		nvram_set_i3("wpa_mode", pf_idx+1, nvram_get_i3("wpa_mode", pf_idx));
		nvram_set_i3("crypto", pf_idx+1, nvram_get_i3("crypto", pf_idx));
		nvram_set_i3("wpa_psk", pf_idx+1, nvram_get_i3("wpa_psk", pf_idx));
		nvram_set_i3("wep_x", pf_idx+1, nvram_get_i3("wep_x", pf_idx));
		nvram_set_i3("phrase", pf_idx+1, nvram_get_i3("phrase", pf_idx));
		nvram_set_i3("key1", pf_idx+1, nvram_get_i3("key1", pf_idx));
		nvram_set_i3("key2", pf_idx+1, nvram_get_i3("key2", pf_idx));
		nvram_set_i3("key3", pf_idx+1, nvram_get_i3("key3", pf_idx));
		nvram_set_i3("key4", pf_idx+1, nvram_get_i3("key4", pf_idx));
		nvram_set_i3("key", pf_idx+1, nvram_get_i3("key", pf_idx));
		nvram_set_i3("passwd", pf_idx+1, nvram_get_i3("passwd", pf_idx));
		
		nvram_set_i3("ssid", pf_idx, ssid_str);
		nvram_set_i3("ssid2", pf_idx, ssid2_str);
		nvram_set_i3("auth_mode", pf_idx, auth_mode_str);
		nvram_set_i3("wpa_mode", pf_idx, wpa_mode_str);
		nvram_set_i3("crypto", pf_idx, crypto_str);
		nvram_set_i3("wpa_psk", pf_idx, wpa_psk_str);
		nvram_set_i3("wep_x", pf_idx, wep_x_str);
		nvram_set_i3("phrase", pf_idx, phrase_str);
		nvram_set_i3("key1", pf_idx, key1_str);
		nvram_set_i3("key2", pf_idx, key2_str);
		nvram_set_i3("key3", pf_idx, key3_str);
		nvram_set_i3("key4", pf_idx, key4_str);
		nvram_set_i3("key", pf_idx, key_str);
		nvram_set_i3("passwd", pf_idx, passwd_str);
	}

	return 1;
}

static int
nvram_generate_table(webs_t wp, char *serviceId, char *groupName)
{	
   struct variable *v;
   int i, groupCount, ret, r, sid;
     
   
   sid = LookupServiceId(serviceId);
         
   if (sid==-1) return 0;
    
   /* Validate and set vairables in table order */              
   for (v = GetVariables(sid); v->name != NULL; v++) 
   {
      /* printf("Find group : %s %s\n", v->name, groupName);*/
      if(!strcmp(groupName, v->name)) break;                       
   }    
   
   if (v->name == NULL) return 0;    
            
   groupCount = atoi(nvram_safe_get_x(serviceId, v->argv[3]));               
            
   if (groupCount==0) ret = nvram_add_group_table(wp, serviceId, v, -1);
   else
   {
   
      ret = 0;
   
      for (i=0; i<groupCount; i++)
      {       	            	
        r = nvram_add_group_table(wp, serviceId, v, i);
        if (r!=0)
           ret += r;
        else break;
      }    
   }   
   
   return(ret);
}


#ifdef WEBS
mySecurityHandler(webs_t wp, char_t *urlPrefix, char_t *webDir, int arg, char_t *url, char_t *path, char_t *query)
{
	char_t *user, *passwd, *digestCalc;
	int flags, nRet;


	user = websGetRequestUserName(wp);
	passwd = websGetRequestPassword(wp);
	flags = websGetRequestFlags(wp);


	nRet = 0;

	if (user && *user)
	{
		if (strcmp(user, "admin")!=0)
		{
			websError(wp, 401, T("Wrong User Name"));
			nRet = 1;
		}
		else if (passwd && *passwd)
		{
			if (strcmp(passwd, websGetPassword())!=0)
			{
				websError(wp, 401, T("Wrong Password"));
				nRet = 1;
			}
		}
#ifdef DIGEST_ACCESS_SUPPORT		
		else if (flags & WEBS_AUTH_DIGEST)
		{
			wp->password = websGetPassword();

			a_assert(wp->digest);
			a_assert(wp->nonce);
			a_assert(wp->password);

			digestCalc = websCalcDigest(wp);
			a_assert(digestCalc);		

			if (gstrcmp(wp->digest, digestCalc)!=0)
			{
				websError(wp, 401, T("Wrong Password"));
				nRet = 1;
			}
			bfree(B_L, digestCalc);
		}
#endif	
	}
	else 
	{
#ifdef DIGEST_ACCESS_SUPPORT
		wp->flags |= WEBS_AUTH_DIGEST;
#endif
		websError(wp, 401, T(""));
		nRet = 1;
	}
	return nRet;

}
#else
static void
do_auth(char *userid, char *passwd, char *realm)
{	
	time_t tm;
					
	if (strcmp(ProductID,"")==0)
	{
	        strncpy(ProductID, nvram_safe_get_x("general.log", "productid"), 32);
	}
	if (strcmp(UserID,"")==0)
	{	   
	   	strncpy(UserID, nvram_safe_get_x("General","http_username"), 32);
	}
   	if (strcmp(UserPass,"")==0)
   	{
		strncpy(UserPass, nvram_safe_get_x("General","http_passwd"), 32);
	}	
	//time(&tm);
	//printf("do_auth %s\n", rfctime(&tm));
	//printf("User: %s:%s%s\n", ProductID, UserID, UserPass);
	strncpy(userid, UserID, AUTH_MAX);

	if (redirect || !is_auth())
	{
		//printf("Disable password checking!!!\n");
		strcpy(passwd, "");
	}
	else
	{
		strncpy(passwd, UserPass, AUTH_MAX);
	}
	strncpy(realm, ProductID, AUTH_MAX);	
}
#endif


static void
do_apply_cgi(char *url, FILE *stream)
{
	char *path, *query;
        
        cprintf(" Before Apply : %s\n", url);
        
        websScan(url);        
	query = url;
	path = strsep(&query, "?") ? : url;	
#ifndef HANDLE_POST	
	init_cgi(query);
#endif	
	apply_cgi(stream, NULL, NULL, 0, url, path, query);
#ifndef HANDLE_POST	
	init_cgi(NULL);
#endif	
}

#ifdef HANDLE_POST
static char post_buf[10000] = { 0 };
#endif

static void
do_apply_cgi_post(char *url, FILE *stream, int len, char *boundary)
{
	//printf("In : %s %s\n", url, boundary);
#ifdef HANDLE_POST	
	if (!fgets(post_buf, MIN(len+1, sizeof(post_buf)), stream)) return;
	len -= strlen(post_buf);
	init_cgi(post_buf);
	while(len--) (void) fgetc(stream);
#endif	
}


#ifdef REMOVE_WL600
static void
do_internal_cgi(char *url, FILE *stream)
{
	char *path, *query;

	query = url;
	path = strsep(&query, "?") ? : url;
	init_cgi(query);
	internal_cgi(stream, NULL, NULL, 0, url, path, query);
}
#endif

#if defined(linux)


static void
do_webcam_cgi(char *url, FILE *stream)
{
	#define MAX_RECORD_IMG 12
	char *query, *path, *last;
	char pic[32];
	int query_idx, last_idx;
	        
	query = url;
	path = strsep(&query, "?") ? : url;
	cprintf("WebCam CGI\n");
	//ret = fcntl(fileno(stream), F_GETOWN, 0);
	query_idx = atoi(query);
	last_idx = atoi(nvram_get_f("webcam.log", "WebPic"));
	//pic = nvram_get_f("webcam.log","WebPic");
			
	if (query_idx==0) sprintf(pic, "../var/tmp/display.jpg");
	else sprintf(pic, "../var/tmp/record%d.jpg", (query_idx>last_idx+1) ? (last_idx+1+MAX_RECORD_IMG-query_idx):(last_idx+1-query_idx));
	
	cprintf("\nWebCam: %s\n", pic);
	do_file(pic, stream);
}

static void
do_upgrade_post(char *url, FILE *stream, int len, char *boundary)
{
	#define MAX_VERSION_LEN 64
	char upload_fifo[] = "/tmp/linux.trx";
	FILE *fifo = NULL;
	FILE *fp = NULL;
	char dataPtr[4];
	char verPtr[64];
	unsigned long *imagelen_fw;
	/*char *write_argv[] = { "write", upload_fifo, "linux", NULL };*/
	pid_t pid;
	char buf[1024];
	int count, ret = EINVAL, ch;
	int eno, cnt;
	long filelen, *filelenptr;
	int hwmajor=0, hwminor=0;
	char version[MAX_VERSION_LEN], cmpHeader;
	char *hwver;
	long imagelen;			// 330gE
	long count2=0;			// 330gE debug
	unsigned char profile[20];	// 330gE
	int i;				// 330gE
	int hasProfile;			// 330gE
	unsigned long *logolen;		// 330gE
	unsigned long *settinglen;	// 330gE
	char odm_productid[13];		// 330gE

	cprintf("Start upgrade!!!\n");
	eval("stopservice", "99");
	nvram_set("reboot", "0");
	nvram_set("uploading", "1");
	
	/* Look for our part */
	while (len > 0) 
	{
	     if (!fgets(buf, MIN(len + 1, sizeof(buf)), stream))
	     {			
			goto err;
	     }			
		
	     len -= strlen(buf);
		
	     if (!strncasecmp(buf, "Content-Disposition:", 20) &&
		    strstr(buf, "name=\"file\""))
			break;			
	}
	
	/* Skip boundary and headers */
	while (len > 0) {
		if (!fgets(buf, MIN(len + 1, sizeof(buf)), stream))
		{
			goto err;
		}		
		len -= strlen(buf);
		if (!strcmp(buf, "\n") || !strcmp(buf, "\r\n"))
		{
			break;
		}
	}

	if (!(fifo = fopen(upload_fifo, "a+"))) goto err;
				
        filelen = len;
        cnt = 0;

	printf("remaining len:        %d\n", len);	// 330gE debug

	/* Pipe the rest to the FIFO */
	//printf("Upgrading %d\n", len);
	cmpHeader = 0;
	
	while (len>0 && filelen>0) 
	{				
		if (waitfor(fileno(stream), 10) <= 0)
		{
			/*printf("Break while len=%x filelen=%x\n", len, filelen);*/
			break;
		}

//		count = fread(buf, 1, MIN(len, sizeof(buf)), stream);	// 330gE
		count = fread(buf, 1, MIN(filelen, sizeof(buf)), stream);
		
		if (cnt == 0 && count>8) 
		{		  
		    if (strncmp(buf, IMAGE_HEADER, 4)!=0) 
		    {
		       /*printf("Header %x %x %x %x\n", buf[0], buf[1], buf[2], buf[3]);*/
		       goto err;
		    }
		    else cmpHeader = 1;	   

		    filelenptr = (buf+4);
		    imagelen = *filelenptr;
		    filelen = *filelenptr;					// for WL-330gE ODM profile data
		    ("image:                %d\n", filelen);	// 330gE debug
		    filelen+=20;						// for WL-330gE ODM profile data, 20 is the length of profile header.
		    printf("image+profile header: %d\n", filelen);	// 330gE debug
		    /*printf("Filelen: %x %x %x %x %x %x\n", filelen, count, (unsigned long)(buf+4), (unsigned long)(buf+7), buf[5], buf[4]);*/
		    cnt ++;
		}

		filelen-=count;
		len-=count;
#ifndef WRITE2FLASH
		count2+=fwrite(buf, 1, count, fifo);
#else
		if (!flashwrite(buf, count, filelen)) goto err;
#endif
		/*printf(".");*/								   		
	}

	if (!cmpHeader) goto err;

	while (len > 0 && filelen>0)
	{
		ch = fgetc(stream);
		len--;
#ifndef WRITE2FLASH
		   count2+=fwrite(&ch, 1, 1, fifo);
		   filelen--;
#else
		   flashwrite(buf, 1, --filelen);
#endif
	}

	printf("remaining len:        %d\n", len);	// 330gE debug
	printf("image+profile header: %d\n", filelen);	// 330gE debug
	printf("written len:          %d\n", count2);	// 330gE debug

#ifndef	WRITE2FLASH
	for(cnt=0;cnt<MAX_VERSION_LEN;cnt++) version[cnt] = 0;
	   		
//	fseek(fifo, -MAX_VERSION_LEN, SEEK_END);
	fseek(fifo, imagelen-64 , SEEK_SET);	// change for WL-330gE ODM profile
	
	fread(version, 1, MAX_VERSION_LEN, fifo);
	fread(profile, 1, 20, fifo);	// 330gE
	
	sscanf(nvram_get_x("general.log", "HardwareVer"), "%d.%d", &hwmajor, &hwminor);
//	cprintf("Hardware : %d.%d %s", hwmajor, hwminor, version+4);
//	printf("file hardware ver: %d.%d.%d.%d\n", version[16], version[17], version[18], version[19]);

	if (strncmp(nvram_safe_get_x("", "productid_own"), version+4, strlen(nvram_safe_get_x("", "productid_odm"))))
		goto err;

	if ((fp = fopen("/dev/mtd/1", "rb"))!=NULL)	// check hardware version
	{
		if (fseek(fp, 4, SEEK_SET)!=0) goto err;

		fread(dataPtr, 1, 4, fp);
		imagelen_fw = (unsigned long *)dataPtr;
//		dprintf("image len %x\n", *imagelen_fw);

		if (fseek(fp, *imagelen_fw - 64, SEEK_SET)!=0) goto err;
//		dprintf("seek\n");

		if (!fread(verPtr, 1, 64, fp)) goto err;

//		printf("cur hardware ver: %d.%d.%d.%d\n", verPtr[16], verPtr[17], verPtr[18], verPtr[19]);
		if (version[16] != verPtr[16])
		{
			printf("hardware version mismatch!\n");
			goto err;
		}

		if (fp)
		{
			fclose(fp);
			fp=NULL;
		}
	}

#else
	ret=0;
#endif
	hasProfile=0;
	for(i=0;i<20;i++)
	{
		printf("%u ", *(profile+i));	// 330gE debug
		if (*(profile+i)!=0)
		{
			hasProfile=1;
			break;
		}
	}
	printf("\n");	// 330gE debug
	if (hasProfile==1)
	{	// ODM productid check
		strncpy(odm_productid, profile, 12);
		odm_productid[12] = 0;
		printf("%s\n", nvram_safe_get_x("", "productid_odm"));		// 330gE debug
		printf("%s\n", odm_productid);					// 330gE debug
		printf("%d\n", strlen(nvram_safe_get_x("", "productid_odm")));	// 330gE debug
		if (strlen(nvram_safe_get_x("", "productid_odm"))!=0 &&
		    strncmp(nvram_safe_get_x("", "productid_odm"), odm_productid, strlen(nvram_safe_get_x("", "productid_odm")))==0
		)
		{	// ODMa->ODMa
			printf("cp 2\n");	// 330gE debug
			ret=0;
		}
		else
		{	// ODMa->ODMb OWN->ODM
			ret=EINVAL;
			goto err;
		}

		logolen = (unsigned long *) (profile+12);
		settinglen = (unsigned long *) (profile+16);
		printf("logolen:              %d\n", *logolen);	// 330gE debug
		printf("settinglen:           %d\n", *settinglen);	// 330gE debug
		filelen += *logolen;
		filelen += *settinglen;
	}
	else
	{
		printf("no profile!!\n");	// 330gE debug
		if (strlen(nvram_safe_get_x("", "productid_odm"))==0)
		{	// OWN->OWN
			printf("cp 3\n");	// 330gE debug
			ret=0;
		}
		else
		{	// ODM->OWN
			ret=EINVAL;
			goto err;
		}
	}

	/* Slurp anything remaining in the request */
	while (len>0 && filelen>0)
	{
		ch = fgetc(stream);
		len--;
#ifndef WRITE2FLASH
		count2+=fwrite(&ch, 1, 1, fifo);
		filelen--;
#else
		flashwrite(buf, 1, --filelen);
#endif
	}	

	fseek(fifo, 0, SEEK_END);
	fclose(fifo);
	fifo = NULL;
	printf("written totally:      %d\n", count2);	// 330gE debug
	/*printf("done\n");*/
 	
 err:
	if (fifo)
	   fclose(fifo);
	if (fp)
		fclose(fp);
		
	/* Slurp anything remaining in the request */
	while (len > 0)
	{
		ch = fgetc(stream);
		len--;
	}
		
#ifdef REMOVE_WL600
	unlink(upload_fifo);
#endif	
	//printf("Error : %d\n", ret);	
	fcntl(fileno(stream), F_SETOWN, -ret);
}

static void
do_upgrade_cgi(char *url, FILE *stream)
{
	int ret;
	
	//printf("Upgrade CGI\n");

	ret = fcntl(fileno(stream), F_GETOWN, 0);
	
	/* Reboot if successful */

	if (ret == 0)
	{
#ifndef WRITE2FLASH
                websApply(stream, "Updating.asp");
		sys_upgrade("/tmp/linux.trx");
#else
		websApply(stream, "Restarting.asp");
#endif
		sys_reboot();
	}	
	else    
	{
	   	websApply(stream, "UpdateError.asp");
	   	unlink("/tmp/linux.trx");
	   	sys_reboot();
	}
}

static void
do_upload_post(char *url, FILE *stream, int len, char *boundary)
{
	#define MAX_VERSION_LEN 64
	char upload_fifo[] = "/tmp/settings_u.prf";
	FILE *fifo = NULL;
	/*char *write_argv[] = { "write", upload_fifo, "linux", NULL };*/
	pid_t pid;
	char buf[1024];
	int count, ret = EINVAL, ch;
	int eno, cnt;
	long filelen, *filelenptr;
	int hwmajor=0, hwminor=0;
	char version[MAX_VERSION_LEN], cmpHeader;
	char *hwver;
	
	cprintf("Start upload\n");
	eval("stopservice", "99");
	nvram_set("uploading", "1");
	
	/* Look for our part */
	while (len > 0) 
	{
	     if (!fgets(buf, MIN(len + 1, sizeof(buf)), stream))
	     {			
			goto err;
	     }			
		
	     len -= strlen(buf);
		
	     if (!strncasecmp(buf, "Content-Disposition:", 20) &&
		    strstr(buf, "name=\"file\""))
			break;			
	}
	
	/* Skip boundary and headers */
	while (len > 0) {
		if (!fgets(buf, MIN(len + 1, sizeof(buf)), stream))
		{
			goto err;
		}		
		len -= strlen(buf);
		if (!strcmp(buf, "\n") || !strcmp(buf, "\r\n"))
		{
			break;
		}
	}

	if (!(fifo = fopen(upload_fifo, "a+"))) goto err;
				
        filelen = len;    
        cnt = 0;
        
	/* Pipe the rest to the FIFO */
	cprintf("Upgrading %d\n", len);
	cmpHeader = 0;
	
	while (len > 0 && filelen>0) 
	{				
		if (waitfor(fileno(stream), 10) <= 0)
		{
			//printf("Break while len=%x filelen=%x\n", len, filelen);
			break;
		}
                				
		count = fread(buf, 1, MIN(len, sizeof(buf)), stream);
		
		if (cnt == 0 && count>8) 
		{		  
		    if (strncmp(buf, PROFILE_HEADER, 4)!=0) 
		    {
		       //printf("Header %x %x %x %x\n", buf[0], buf[1], buf[2], buf[3]);
		       len-=count;
		       goto err;
		    }
		    else cmpHeader = 1;	   
		    		   
		    filelenptr = (buf+4);		   
		    filelen = *filelenptr;		  
		    //printf("Filelen: %x %x %x %x %x %x\n", filelen, count, (unsigned long)(buf+4), (unsigned long)(buf+7), buf[5], buf[4]);
		    cnt ++;
		}
		
		filelen-=count; 
		len-=count;	
										
		fwrite(buf, 1, count, fifo);
		//printf(".");								   
	}		 		
	
	if (!cmpHeader) goto err;		

	/* Slurp anything remaining in the request */
	while (len-- > 0)
	{
		ch = fgetc(stream);
		
		if (filelen>0)
		{
		   fwrite(&ch, 1, 1, fifo);
		   filelen--;
		}   
	}	
	
#ifdef REMOVE_WL600	
	for(cnt=0;cnt<MAX_VERSION_LEN;cnt++) version[cnt] = 0;
	   		
	fseek(fifo, -MAX_VERSION_LEN, SEEK_END);
	fread(version, 1, MAX_VERSION_LEN, fifo);
	
	sscanf(nvram_get_x("general.log", "HardwareVer"), "%d.%d", &hwmajor, &hwminor);
	cprintf("Hardware : %d.%d", hwmajor, hwminor);
	
	if (checkVersion(version,hwmajor,hwminor)) ret = 0;
#else
	ret = 0;	
#endif
		
	fseek(fifo, 0, SEEK_END);
	fclose(fifo);
	fifo = NULL;
	/*printf("done\n");*/
 	
 err:
	if (fifo)
	   fclose(fifo);
		
	/* Slurp anything remaining in the request */
	while (len-- > 0)	
		ch = fgetc(stream);
		
#ifdef REMOVE_WL600
	unlink(upload_fifo);
#endif	
	//printf("Error : %d\n", ret);
	fcntl(fileno(stream), F_SETOWN, -ret);
}

static void
do_upload_cgi(char *url, FILE *stream)
{
	int ret;
	
	//printf("Upgrade CGI\n");

	ret = fcntl(fileno(stream), F_GETOWN, 0);
	
	/* Reboot if successful */
	if (ret == 0)
	{
                websApply(stream, "Uploading.asp"); 
		sys_upload("/tmp/settings_u.prf");
		sys_commit();
		sys_reboot();
	}	
	else    
	{
	   	websApply(stream, "UploadError.asp");
	   	//unlink("/tmp/settings_u.prf");
	   	sys_reboot();
	}   	
	  
}


static void
do_prf_file(char *url, FILE *stream)
{
	sys_commit(NULL);
	sys_download("/tmp/settings");
	do_file(url, stream);
	//unlink("/tmp/settings");
}


#elif defined(vxworks)

static void
do_upgrade_post(char *url, FILE *stream, int len, char *boundary)
{
}

static void
do_upgrade_cgi(char *url, FILE *stream)
{
}

#endif

static char no_cache[] =
"Cache-Control: no-cache\r\n"
"Pragma: no-cache\r\n"
"Expires: 0"
;

static char log_headers[] = 
"Content-Disposition: attachment; filename=\"SYSLOG.DAT\"\r\n"
"Cache-Control: no-cache\r\n"
"Pragma: no-cache\r\n"
"Expires: 0"
;

static void 
do_log_cgi(char *path, FILE *stream)
{
	dump_file(stream, "/tmp/syslog.log-1");
	dump_file(stream, "/tmp/syslog.log");
	fputs("\r\n", stream); /* terminator */
	fputs("\r\n", stream); /* terminator */
}

#ifdef WEBS
void
initHandlers(void)
{                
	websAspDefine("nvram_get_x", ej_nvram_get_x);
	websAspDefine("nvram_get_f", ej_nvram_get_f);
	websAspDefine("nvram_get_list_x", ej_nvram_get_list_x);	
	websAspDefine("nvram_get_buf_x", ej_nvram_get_buf_x);	
	websAspDefine("nvram_get_table_x", ej_nvram_get_table_x);	
	websAspDefine("nvram_match_x", ej_nvram_match_x);
	websAspDefine("nvram_double_match_x", ej_nvram_double_match_x);
    	websAspDefine("nvram_match_both_x", ej_nvram_match_both_x);	
	websAspDefine("nvram_match_list_x", ej_nvram_match_list_x);
	websAspDefine("select_channel", ej_select_channel);
	websAspDefine("select_country", ej_select_country);
    	websAspDefine("urlcache", ej_urlcache);	
	websAspDefine("uptime", ej_uptime);
	websAspDefine("sysuptime", ej_sysuptime);
	websAspDefine("nvram_dump", ej_dump);	
    	websAspDefine("load_script", ej_load);
    	websAspDefine("select_profile", ej_select_profile);
	websSecurityDelete();
	websUrlHandlerDefine("", NULL, 0, mySecurityHandler, WEBS_HANDLER_FIRST);	
	websUrlHandlerDefine("/apply.cgi", NULL, 0, apply_cgi, 0);	
	websSetPassword(nvram_safe_get_x("General", "x_Password"));
	websSetRealm(nvram_safe_get_x("general.log", "ProductID"));
	websAspDefine("select_profile", ej_select_profile);
	websAspDefine("nvram_char_to_ascii", ej_nvram_char_to_ascii);
}


#else
struct mime_handler mime_handlers[] = {
	{ "**.asp", "text/html", no_cache, NULL, do_ej, do_auth },
	{ "**.css", "text/css", NULL, NULL, do_file, do_auth },
	{ "**.gif", "image/gif", NULL, NULL, do_file, do_auth },
	{ "**.jpg", "image/jpeg", NULL, NULL, do_file, do_auth },
	{ "**.js",  "text/javascript", NULL, NULL, do_file, do_auth },
	{ "**.cab", "text/txt", NULL, NULL, do_file, do_auth },
	{ "**.CFG", "text/txt", NULL, NULL, do_prf_file, do_auth },
	{ "apply.cgi*", "text/html", no_cache, do_apply_cgi_post, do_apply_cgi, do_auth },	
	{ "upgrade.cgi*", "text/html", no_cache, do_upgrade_post, do_upgrade_cgi, do_auth},
	{ "upload.cgi*", "text/html", no_cache, do_upload_post, do_upload_cgi, do_auth },
 	{ "syslog.cgi*", "text/txt", no_cache, NULL, do_log_cgi, do_auth },
	{ "webcam.cgi*", "text/html", no_cache, NULL, do_webcam_cgi, do_auth },
	{ NULL, NULL, NULL, NULL, NULL, NULL }
};


static int
ej_select_folder(int eid, webs_t wp, int argc, char_t **argv)
{
	char *sid;
	int ret = 0;	
	char buf[8], out[64];	
	int i, channel;
	FILE *fp;
	DIR *dir;
	struct dirent *entry;

	if (ejArgs(argc, argv, "%s", &sid) < 1) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
         
        i=0;  
              
	/* display folder in first level */
	dir = opendir("/");
	if (dir!=NULL)
	{
		i=0;

		while((entry=readdir(dir))!=NULL)
		{	
             	 	sprintf(out, "<option value=\"%s\"", entry->d_name);
                 	sprintf(out,"%s>%s</option>", out, entry->d_name);                 
                 	websWrite(wp, out);
       		} 
        	closedir(dir);
	}     
	return ret;
}


static int
ej_select_list(int eid, webs_t wp, int argc, char_t **argv)
{
	char *id;
	int ret = 0;	
	char buf[8], out[64], idxstr[12], tmpstr[64], tmpstr1[64];
	int i, channel, curr, hit, noneFlag;
	char *ref1, *ref2, *refnum;

	if (ejArgs(argc, argv, "%s", &id) < 1) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}

	//printf("id: %s\n", id);

        if (strcmp(id, "Storage_x_SharedPath")==0)
        {
		ref1 = "sh_path_x";
		ref2 = "sh_path";
		refnum = "sh_num";

		curr = atoi(nvram_get(ref1));
		noneFlag = 0;
        }
	else if (strncmp(id, "Storage_x_AccUser", 17)==0)
	{
		sprintf(tmpstr, "sh_accuser_x%s", id + 17);
		ref2 = "acc_username";
		refnum = "acc_num";

		curr = atoi(nvram_get(tmpstr));
		noneFlag =1;
	}
	else if (strcmp(id, "Storage_x_AccAny")==0)
	{
		
                 ret = websWrite(wp, "<option value=\"Guest\">Guest</option>");
		 return ret;
	}
	else 
	{
		 return ret;     
	}
        

	//printf("out %s\n", curr);
	hit = 0;
 
	for(i=0;i<atoi(nvram_get(refnum));i++)
	{     
 		 sprintf(idxstr, "%d", i);
		 strcpy(tmpstr1, nvram_get(strcat_r(ref2, idxstr, tmpstr)));
             	 sprintf(out, "<option value=\"%d\"", i);

                 if (i==curr) 
		 {
			hit = 1;
			sprintf(out, "%s selected", out);
		 }
                 sprintf(out,"%s>%s</option>", out, tmpstr1);       
 
		 //printf("out: %s\n", out);

                 ret += websWrite(wp, out);
	}     

	if (noneFlag)
	{
		cprintf("hit : %d\n", hit);
		if (!hit) sprintf(out, "<option value=\"99\" selected>None</option>");
		else sprintf(out, "<option value=\"99\">None</option>");

		ret += websWrite(wp, out);
	}	
	return ret;
}


#ifndef NOUSB
static int nvMap(char_t *url)
{
	cprintf("MAP:%s\n", url);
	if (strcmp(url, "Advanced_StorageUser_Content.asp")==0)
	{
		setSharedEntry(atoi(nvram_get("sh_path_x")));	
	}
}
#endif

struct ej_handler ej_handlers[] = {
	{ "nvram_get_x", ej_nvram_get_x},
	{ "nvram_get_f", ej_nvram_get_f},
	{ "nvram_get_list_x", ej_nvram_get_list_x},
	{ "nvram_get_buf_x", ej_nvram_get_buf_x},
	{ "nvram_get_table_x", ej_nvram_get_table_x},
	{ "nvram_match_x", ej_nvram_match_x},
	{ "nvram_double_match_x", ej_nvram_double_match_x},
        { "nvram_match_both_x", ej_nvram_match_both_x},
	{ "nvram_match_list_x", ej_nvram_match_list_x},
	{ "select_channel", ej_select_channel},
	{ "select_country", ej_select_country},
	{ "urlcache", ej_urlcache},     
	{ "uptime", ej_uptime},
	{ "sysuptime", ej_sysuptime},
	{ "nvram_dump", ej_dump},
	{ "load_script", ej_load},
	{ "select_folder", ej_select_folder},
	{ "select_list", ej_select_list},
	{ "select_profile", ej_select_profile},
	{ "nvram_char_to_ascii", ej_nvram_char_to_ascii},
	{ NULL, NULL }
};

#endif /* !WEBS */

void websSetVer(void)
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

		cprintf("image len %x\n", *imagelen);
		if (fseek(fp, *imagelen - 64, SEEK_SET)!=0) goto write_ver;
		cprintf("seek\n");
		if (!fread(verPtr, 1, 64, fp)) goto write_ver;
		cprintf("ver %x %x", verPtr[0], verPtr[1]);
		strncpy(productid, verPtr + 4, 12);
		productid[12] = 0;
		sprintf(fwver, "%d.%d.%d.%d", verPtr[0], verPtr[1], verPtr[2], verPtr[3]);

		cprintf("get fw ver: %s\n", productid);
		fclose(fp);
	}
write_ver:
	nvram_set_f("general.log", "productid", productid);
	nvram_set_f("general.log", "firmver", fwver);	
}

/*
 * Country names and abbreviations from ISO 3166
 */
typedef struct {
	char *name;     /* Long name */
	char *abbrev;   /* Abbreviation */
} country_name_t;
country_name_t country_names[];     /* At end of this file */


/*
 * Country names and abbreviations from ISO 3166
 */
country_name_t country_names[] = {

{"AFGHANISTAN",		 "AF"},
{"ALBANIA",		 "AL"},
{"ALGERIA",		 "DZ"},
{"AMERICAN SAMOA", 	 "AS"},
{"ANDORRA",		 "AD"},
{"ANGOLA",		 "AO"},
{"ANGUILLA",		 "AI"},
{"ANTARCTICA",		 "AQ"},
{"ANTIGUA AND BARBUDA",	 "AG"},
{"ARGENTINA",		 "AR"},
{"ARMENIA",		 "AM"},
{"ARUBA",		 "AW"},
{"AUSTRALIA",		 "AU"},
{"AUSTRIA",		 "AT"},
{"AZERBAIJAN",		 "AZ"},
{"BAHAMAS",		 "BS"},
{"BAHRAIN",		 "BH"},
{"BANGLADESH",		 "BD"},
{"BARBADOS",		 "BB"},
{"BELARUS",		 "BY"},
{"BELGIUM",		 "BE"},
{"BELIZE",		 "BZ"},
{"BENIN",		 "BJ"},
{"BERMUDA",		 "BM"},
{"BHUTAN",		 "BT"},
{"BOLIVIA",		 "BO"},
{"BOSNIA AND HERZEGOVINA","BA"},
{"BOTSWANA",		 "BW"},
{"BOUVET ISLAND",	 "BV"},
{"BRAZIL",		 "BR"},
{"BRITISH INDIAN OCEAN TERRITORY", 	"IO"},
{"BRUNEI DARUSSALAM",	 "BN"},
{"BULGARIA",		 "BG"},
{"BURKINA FASO",	 "BF"},
{"BURUNDI",		 "BI"},
{"CAMBODIA",		 "KH"},
{"CAMEROON",		 "CM"},
{"CANADA",		 "CA"},
{"CAPE VERDE",		 "CV"},
{"CAYMAN ISLANDS",	 "KY"},
{"CENTRAL AFRICAN REPUBLIC","CF"},
{"CHAD",		 "TD"},
{"CHILE",		 "CL"},
{"CHINA",		 "CN"},
{"CHRISTMAS ISLAND",	 "CX"},
{"COCOS (KEELING) ISLANDS","CC"},
{"COLOMBIA",		 "CO"},
{"COMOROS",		 "KM"},
{"CONGO",		 "CG"},
{"CONGO, THE DEMOCRATIC REPUBLIC OF THE", "CD"},
{"COOK ISLANDS",	 "CK"},
{"COSTA RICA",		 "CR"},
{"COTE D'IVOIRE",	 "CI"},
{"CROATIA",		 "HR"},
{"CUBA",		 "CU"},
{"CYPRUS",		 "CY"},
{"CZECH REPUBLIC",	 "CZ"},
{"DENMARK",		 "DK"},
{"DJIBOUTI",		 "DJ"},
{"DOMINICA",		 "DM"},
{"DOMINICAN REPUBLIC", 	 "DO"},
{"ECUADOR",		 "EC"},
{"EGYPT",		 "EG"},
{"EL SALVADOR",		 "SV"},
{"EQUATORIAL GUINEA",	 "GQ"},
{"ERITREA",		 "ER"},
{"ESTONIA",		 "EE"},
{"ETHIOPIA",		 "ET"},
{"FALKLAND ISLANDS (MALVINAS)",	"FK"},
{"FAROE ISLANDS",	 "FO"},
{"FIJI",		 "FJ"},
{"FINLAND",		 "FI"},
{"FRANCE",		 "FR"},
{"FRENCH GUIANA",	 "GF"},
{"FRENCH POLYNESIA",	 "PF"},
{"FRENCH SOUTHERN TERRITORIES",	 "TF"},
{"GABON",		 "GA"},
{"GAMBIA",		 "GM"},
{"GEORGIA",		 "GE"},
{"GERMANY",		 "DE"},
{"GHANA",		 "GH"},
{"GIBRALTAR",		 "GI"},
{"GREECE",		 "GR"},
{"GREENLAND",		 "GL"},
{"GRENADA",		 "GD"},
{"GUADELOUPE",		 "GP"},
{"GUAM",		 "GU"},
{"GUATEMALA",		 "GT"},
{"GUINEA",		 "GN"},
{"GUINEA-BISSAU",	 "GW"},
{"GUYANA",		 "GY"},
{"HAITI",		 "HT"},
{"HEARD ISLAND AND MCDONALD ISLANDS",	"HM"},
{"HOLY SEE (VATICAN CITY STATE)", 	"VA"},
{"HONDURAS",		 "HN"},
{"HONG KONG",		 "HK"},
{"HUNGARY",		 "HU"},
{"ICELAND",		 "IS"},
{"INDIA",		 "IN"},
{"INDONESIA",		 "ID"},
{"IRAN, ISLAMIC REPUBLIC OF",		"IR"},
{"IRAQ",		 "IQ"},
{"IRELAND",		 "IE"},
{"ISRAEL",		 "IL"},
{"ITALY",		 "IT"},
{"JAMAICA",		 "JM"},
{"JAPAN",		 "JP"},
{"JORDAN",		 "JO"},
{"KAZAKHSTAN",		 "KZ"},
{"KENYA",		 "KE"},
{"KIRIBATI",		 "KI"},
{"KOREA, DEMOCRATIC PEOPLE'S REPUBLIC OF", "KP"},
{"KOREA, REPUBLIC OF",	 "KR"},
{"KUWAIT",		 "KW"},
{"KYRGYZSTAN",		 "KG"},
{"LAO PEOPLE'S DEMOCRATIC REPUBLIC", 	"LA"},
{"LATVIA",		 "LV"},
{"LEBANON",		 "LB"},
{"LESOTHO",		 "LS"},
{"LIBERIA",		 "LR"},
{"LIBYAN ARAB JAMAHIRIYA","LY"},
{"LIECHTENSTEIN",	 "LI"},
{"LITHUANIA",		 "LT"},
{"LUXEMBOURG",		 "LU"},
{"MACAO",		 "MO"},
{"MACEDONIA",	 "MK"},
{"MADAGASCAR",		 "MG"},
{"MALAWI",		 "MW"},
{"MALAYSIA",		 "MY"},
{"MALDIVES",		 "MV"},
{"MALI",		 "ML"},
{"MALTA",		 "MT"},
{"MARSHALL ISLANDS",	 "MH"},
{"MARTINIQUE",		 "MQ"},
{"MAURITANIA",		 "MR"},
{"MAURITIUS",		 "MU"},
{"MAYOTTE",		 "YT"},
{"MEXICO",		 "MX"},
{"MICRONESIA", 		 "FM"},
{"MOLDOVA, REPUBLIC OF", "MD"},
{"MONACO",		 "MC"},
{"MONGOLIA",		 "MN"},
{"MONTSERRAT",		 "MS"},
{"MOROCCO",		 "MA"},
{"MOZAMBIQUE",		 "MZ"},
{"MYANMAR",		 "MM"},
{"NAMIBIA",		 "NA"},
{"NAURU",		 "NR"},
{"NEPAL",		 "NP"},
{"NETHERLANDS",		 "NL"},
{"NETHERLANDS ANTILLES", "AN"},
{"NEW CALEDONIA",	 "NC"},
{"NEW ZEALAND",		 "NZ"},
{"NICARAGUA",		 "NI"},
{"NIGER",		 "NE"},
{"NIGERIA",		 "NG"},
{"NIUE",		 "NU"},
{"NORFOLK ISLAND",	 "NF"},
{"NORTHERN MARIANA ISLANDS","MP"},
{"NORWAY",		 "NO"},
{"OMAN",		 "OM"},
{"PAKISTAN",		 "PK"},
{"PALAU",		 "PW"},
{"PALESTINIAN TERRITORY, OCCUPIED", 	"PS"},
{"PANAMA",		 "PA"},
{"PAPUA NEW GUINEA",	 "PG"},
{"PARAGUAY",		 "PY"},
{"PERU",		 "PE"},
{"PHILIPPINES",		 "PH"},
{"PITCAIRN",		 "PN"},
{"POLAND",		 "PL"},
{"PORTUGAL",		 "PT"},
{"PUERTO RICO",		 "PR"},
{"QATAR",		 "QA"},
{"REUNION",		 "RE"},
{"ROMANIA",		 "RO"},
{"RUSSIAN FEDERATION",	 "RU"},
{"RWANDA",		 "RW"},
{"SAINT HELENA",	 "SH"},
{"SAINT KITTS AND NEVIS","KN"},
{"SAINT LUCIA",		 "LC"},
{"SAINT PIERRE AND MIQUELON",	 	"PM"},
{"SAINT VINCENT AND THE GRENADINES", 	"VC"},
{"SAMOA",		 "WS"},
{"SAN MARINO",		 "SM"},
{"SAO TOME AND PRINCIPE","ST"},
{"SAUDI ARABIA",	 "SA"},
{"SENEGAL",		 "SN"},
{"SEYCHELLES",		 "SC"},
{"SIERRA LEONE",	 "SL"},
{"SINGAPORE",		 "SG"},
{"SLOVAKIA",		 "SK"},
{"SLOVENIA",		 "SI"},
{"SOLOMON ISLANDS",	 "SB"},
{"SOMALIA",		 "SO"},
{"SOUTH AFRICA",	 "ZA"},
{"SOUTH GEORGIA", "GS"},
{"SPAIN",		 "ES"},
{"SRI LANKA",		 "LK"},
{"SUDAN",		 "SD"},
{"SURINAME",		 "SR"},
{"SVALBARD AND JAN MAYEN","SJ"},
{"SWAZILAND",		 "SZ"},
{"SWEDEN",		 "SE"},
{"SWITZERLAND",		 "CH"},
{"SYRIAN ARAB REPUBLIC", "SY"},
{"TAIWAN, PROVINCE OF CHINA", 		"TW"},
{"TAJIKISTAN",		 "TJ"},
{"TANZANIA",		"TZ"},
{"THAILAND",		 "TH"},
{"TIMOR-LESTE",		 "TL"},
{"TOGO",		 "TG"},
{"TOKELAU",		 "TK"},
{"TONGA",		 "TO"},
{"TRINIDAD AND TOBAGO",	 "TT"},
{"TUNISIA",		 "TN"},
{"TURKEY",		 "TR"},
{"TURKMENISTAN",	 "TM"},
{"TURKS AND CAICOS ISLANDS",		"TC"},
{"TUVALU",		 "TV"},
{"UGANDA",		 "UG"},
{"UKRAINE",		 "UA"},
{"UNITED ARAB EMIRATES", "AE"},
{"UNITED KINGDOM",	 "GB"},
{"UNITED STATES",	 "US"},
{"UNITED STATES MINOR OUTLYING ISLANDS","UM"},
{"URUGUAY",		 "UY"},
{"UZBEKISTAN",		 "UZ"},
{"VANUATU",		 "VU"},
{"VENEZUELA",		 "VE"},
{"VIET NAM",		 "VN"},
{"VIRGIN ISLANDS, BRITISH", "VG"},
{"VIRGIN ISLANDS, U.S.", "VI"},
{"WALLIS AND FUTUNA",	 "WF"},
{"WESTERN SAHARA", 	 "EH"},
{"YEMEN",		 "YE"},
{"YUGOSLAVIA",		 "YU"},
{"ZAMBIA",		 "ZM"},
{"ZIMBABWE",		 "ZW"},
{"ALL",		 	 "ALL"},
{NULL, 			 NULL}
};

static int
ej_select_country(int eid, webs_t wp, int argc, char_t **argv)
{
	char *country, *sid;	
	country_name_t *cntry;
	int ret=0;

	if (ejArgs(argc, argv, "%s", &sid) < 1) {
		websError(wp, 400, "Insufficient args\n");
		return -1;
	}
	
        country=nvram_safe_get_x("", "wl_country_code");
                                
	for(cntry = country_names; cntry->name; cntry++)
	{ 
		if (strcmp(country, cntry->abbrev)!=0)
                   ret += websWrite(wp, "<option value=\"%s\">%s</option>", cntry->abbrev, cntry->name);
                else
                   ret += websWrite(wp, "<option value=\"%s\" selected>%s</option>", cntry->abbrev, cntry->name);
        }                                             
	return ret;
}

static int
wl_channels_in_country_asus(char *abbrev, int channels[])
{
	int i, j;
	wl_channels_in_country_t *cic = (wl_channels_in_country_t *)ibuf2;
	char var[256], *next;
	int log_level, log_drop, log_accept;

	i=0;
	
	if (nvram_match_x("", "wl_chan_list", "")) {
		for(i=0;i<14;i++)
		{
			channels[i] = i+1;
			channels[i+1] = -1;
		}	
	}
	else {
		foreach(var, nvram_safe_get_x("", "wl_chan_list"), next) {
			channels[i]=atoi(var);
			channels[i+1]=-1;
			i++;
		}
	}
	return i;
}

static int
wl_channels_in_country(char *abbrev, int channels[])
{
	int i, j, unit;
	wl_channels_in_country_t *cic = (wl_channels_in_country_t *)ibuf2;
	char tmp[100], prefix[] = "wlXXXXXXXXXX_";
	char *name;
	channel_info_t ci;

	if ((unit = atoi(nvram_safe_get_x("", "wl_unit"))) < 0)
		return -1;

	snprintf(prefix, sizeof(prefix), "wl%d_", unit);
	name = nvram_safe_get_x("", strcat_r(prefix, "ifname", tmp));		
	

	i = 0;
	channels[i++] = 0;
	channels[i] = -1;

	if (strlen(abbrev)==0) return 0;

	cic->buflen = sizeof(ibuf2);
	strcpy(cic->country_abbrev, abbrev);
	cic->band = WLC_BAND_2G;
	cic->count = 0;


	if (wl_ioctl(name, WLC_GET_CHANNELS_IN_COUNTRY, cic, cic->buflen) == 0) 	{
		if (cic->count == 0)
			return 0;

		for(j = 0; j < cic->count; j++)
		{
			channels[i++] = cic->channel[j];
		}

		channels[i] = -1;
	}
	return cic->count;

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
