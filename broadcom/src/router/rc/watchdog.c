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
 * $Id: watchdog.c,v 1.1.1.1 2007/01/25 12:52:21 jiahao_jhou Exp $
 */


#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <shutils.h>
#include <rc.h>
#include <stdarg.h>
#include <wlioctl.h>
#include <syslog.h>
#include <bcmnvram.h>
#include <fcntl.h>
#include <linux_gpio.h>
#include <sys/stat.h>
#include <math.h>
#include <string.h>

#include <bcmutils.h>

#define BCM47XX_SOFTWARE_RESET  0x40		/* GPIO 6 */
#define RESET_WAIT		3		/* seconds */
#define RESET_WAIT_COUNT	RESET_WAIT * 10 /* 10 times a second */

#define NORMAL_PERIOD		1		/* second */
#define URGENT_PERIOD		100 * 1000	/* microsecond */	
#define RUSHURGENT_PERIOD	50 * 1000	/* microsecond */	
						/* 1/10 second */
//#define STACHECK_PERIOD_CONNECT 30/5		/* 30 seconds */
//#define STACHECK_PERIOD_DISCONNECT 5/5		/* 5 seconds */

#ifdef WL330GE
#define QUICK_PERIOD		250 * 1000	/* microsecond */
#endif

#define GPIO0 0x0001
#define GPIO1 0x0002
#define GPIO2 0x0004
#define GPIO3 0x0008
#define GPIO4 0x0010
#define GPIO5 0x0020
#define GPIO6 0x0040
#define GPIO7 0x0080
#define GPIO15 0x8000

#define LED_ON 	0
#define LED_OFF 1

#define LED_POWER	GPIO0
#define BTN_RESET	GPIO2 
//#define BTN_SETUP	GPIO3

#ifdef BTN_SETUP
#define SETUP_WAIT		3		/* seconds */
#define SETUP_WAIT_COUNT	SETUP_WAIT * 10 /* 10 times a second */
#define SETUP_TIMEOUT		60 		/* seconds */
#define SETUP_TIMEOUT_COUNT	SETUP_TIMEOUT * 10 /* 60 times a second */
#endif //BTN_SETUP

#ifdef WL330GE
#define WIF "eth1"
char buf[WLC_IOCTL_MAXLEN];
#define GPIO11		0x0800
#define LED_AIR2	GPIO11
int RSSI_TH_A=0;
int RSSI_TH_B=0;
int RSSI_TH_C=0;
int RSSI_TH_D=0;
int RSSI_TH_E=0;
static int wlcheck_period=0;
static int wl_led=0;
int wl_on=0;
int wl_timer=0;
int wl_led_state=0;
int wl_led_state_old=0;
int assoc_after_boot=-1;
static int rssi_g=-32768;
static int rssi_g_old=-32768;
static int wl_on_old=0;
static int wl_reset_timer=0;
static int wl_reset_timer_ure=0;
static int ddns_timer=0;
static int stacheck_timer=0;
char AIR_LED_OFF[]={0x67, 0x70, 0x69, 0x6f, 0x74, 0x69, 0x6d, 0x65, 0x72, 0x76, 0x61, 0x6c, 0x0, 0x0, 0x10,  0x0, 0x0};
char AIR_LED_ON[]= {0x67, 0x70, 0x69, 0x6f, 0x74, 0x69, 0x6d, 0x65, 0x72, 0x76, 0x61, 0x6c, 0x0, 0x0, 0x00, 0x64, 0x0};
#endif

static int int_nas_enable=0;
static int int_wl_mode_EX=0;
static int int_wl0_mode=0;
static int int_wl0_akm=0;
static int int_ure_disable=0;
static int int_wan_nat_X=0;
static int int_wl0_auth=0;
static int int_no_profile=1;
static int int_router_disable=0;
static int int_ddns_enable_x=0;

/* Global containing the file descriptor of the GPIO kernel drivers */
static int bcmgpio_fd;
                                                                                                                             
/* Global containing information about each GPIO pin */
                                                                                                                             
/* Static function prototypes */
                                                                                                                             
/* Generic functions to read/write Chip Common core's GPIO registers on the AP */
int tgpio_drvinit ();
void tgpio_drvcleanup ();
                                                                                                                             
int tgpio_ioctl(int gpioreg, unsigned int mask , unsigned int val);
                                                                                                                             
/* GPIO registers */
#define BCMGPIO_REG_IN          0
#define BCMGPIO_REG_OUT         1
#define BCMGPIO_REG_OUTEN       2
#define BCMGPIO_REG_RESERVE     3
#define BCMGPIO_REG_RELEASE     4


#define LED_CONTROL(led,flag) gpio_write("/dev/gpio/out", led, flag)


struct itimerval itv; 
int watchdog_period=0;
static int btn_pressed=0;
static int btn_count = 0;
long sync_interval=-1; // every 30 seconds a unit
int sync_flag=0;
long timestamp_g=0;
int stacheck_interval=-1;
#ifdef BTN_SETUP
int btn_pressed_setup=0;
int btn_pressed_flag=0;
int btn_count_setup=0;
int btn_count_timeout=0;
int btn_stage=0;
#endif

#ifdef CDMA
int cdma_down=0;
int cdma_connect=0;
#endif

int reboot_count=0;

int tgpio_fd_init ();
void tgpio_fd_cleanup ();
int tgpio_ioctl(int gpioreg, unsigned int mask , unsigned int val);
void gpio_write(char *dev, int gpio, int val);


int
tgpio_fd_init ()
{
        bcmgpio_fd = open("/dev/gpio", O_RDWR);
        if (bcmgpio_fd == -1) {
                printf ("Failed to open /dev/gpio\n");
                return -1;
        }
        return 0;
}
                                                                                                                             
void
tgpio_fd_cleanup ()
{
        if (bcmgpio_fd!= -1) {
                close (bcmgpio_fd);
                bcmgpio_fd = -1;
        }
}
                                                                                                                             
int
tgpio_ioctl(int gpioreg, unsigned int mask , unsigned int val)
{
        struct gpio_ioctl gpio;
        int type;
                                                                                                                             
        gpio.val = val;
        gpio.mask = mask;
                                                                                                                             
        switch (gpioreg) {
                case BCMGPIO_REG_IN:
                        type = GPIO_IOC_IN;
                        break;
                case BCMGPIO_REG_OUT:
                        type = GPIO_IOC_OUT;
                        break;
                case BCMGPIO_REG_OUTEN:
                        type = GPIO_IOC_OUTEN;
                        break;
                case BCMGPIO_REG_RESERVE:
                        type = GPIO_IOC_RESERVE;
                        break;
                case BCMGPIO_REG_RELEASE:
                        type = GPIO_IOC_RELEASE;
                        break;
                default:
                        printf ("invalid gpioreg %d\n", gpioreg);
                        return -1;
        }
        if (ioctl(bcmgpio_fd, type, &gpio) < 0) {
                printf ("invalid gpioreg %d\n", gpioreg);
                return -1;
        }
        return (gpio.val);
}                                                                                                                             

void gpio_write(char *dev, int gpio, int val)
{
	unsigned int gpio_type;
	unsigned long bitmask;
	unsigned long gpio_val;
//	int gpio_pin;

	if( strcmp (dev, "/dev/gpio/in") == 0)
		gpio_type = BCMGPIO_REG_IN;
	else if ( strcmp (dev, "/dev/gpio/out") == 0)
		gpio_type = BCMGPIO_REG_OUT;
	else if ( strcmp (dev, "/dev/gpio/outen") == 0)
		gpio_type = BCMGPIO_REG_OUTEN;
	else
		printf("ERROR GPIO NAME %s\n", dev);
                                                                                                                             
	//sscanf(argv[3], "%x", &val);
	tgpio_fd_init();
	tgpio_ioctl(BCMGPIO_REG_RESERVE, gpio, gpio);
	if (val > 0)
	tgpio_ioctl(gpio_type, gpio, gpio);
	else 
	tgpio_ioctl(gpio_type, gpio, 0);
	tgpio_fd_cleanup();
}

int gpio_read (char *dev, int gpio)
{
	unsigned int gpio_type;
	int ret;
	unsigned long bitmask;
        if( strcmp (dev, "/dev/gpio/in") == 0)
                gpio_type = BCMGPIO_REG_IN;
        else if ( strcmp (dev, "/dev/gpio/out") == 0)
                gpio_type = BCMGPIO_REG_OUT;
        else if ( strcmp (dev, "/dev/gpio/outen") == 0)
                gpio_type = BCMGPIO_REG_OUTEN;
        else
                printf("ERROR GPIO NAME %s\n", dev);
	
	tgpio_fd_init();

	/* read the value of GPIO*/
	ret = (tgpio_ioctl(gpio_type, gpio, 0));
	tgpio_fd_cleanup();
	
	return (ret&gpio);
}

/* Functions used to control led and button */
gpio_init()
{
	gpio_write("/dev/gpio/outen", LED_POWER, 1);
	gpio_write("/dev/gpio/outen", BTN_RESET, 0);
#ifdef BTN_SETUP
//	gpio_write("/dev/gpio/outen", BTN_SETUP, 0);
#endif
	gpio_write("/dev/gpio/out", LED_POWER, 0);

#ifdef WL330GE
//	gpio_write("/dev/gpio/outen", LED_AIR2, 1);
//	gpio_write("/dev/gpio/out", LED_AIR2, 0);
#endif
}

static void
alarmtimer(unsigned long sec,unsigned long usec)
{
	itv.it_value.tv_sec  = sec;
	itv.it_value.tv_usec = usec;
	itv.it_interval = itv.it_value;
	setitimer(ITIMER_REAL, &itv, NULL);
}

void btn_check(void)
{
char buf[17];
#ifdef BTN_SETUP
	if (btn_pressed_flag)
	{
		btn_pressed_flag++;

		if(btn_pressed_flag==8)
		{
//			start_ots();/* Cherry Cho unmarked for using EZsetup in 2007/2/12. */
		}
		else if(btn_pressed_flag==10)
		{		
			btn_pressed_flag=0;
			btn_pressed_setup=BTNSETUP_START;
			btn_count_setup=0;
			alarmtimer(0, RUSHURGENT_PERIOD);
		}
	}

	if (btn_pressed_setup==BTNSETUP_NONE)
	{
#endif

	//printf("btn :  %d %d\n", btn_pressed, btn_count);	
	if (!gpio_read("/dev/gpio/in", BTN_RESET))
	{
	/*--------------- Add BTN_RST MFG test ------------------------*/
/*
	   if (nvram_invmatch("asus_mfg", "")){
		nvram_set("btn_rst", "1");
	   }
	   else{
*/
		if (nvram_get("RESET_PRESSED")==NULL || nvram_match("RESET_PRESSED", "0"))
			nvram_set("RESET_PRESSED", "1");

		if (!btn_pressed)
		{
			btn_pressed=1;
			btn_count=0;
			alarmtimer(0, URGENT_PERIOD);
		}
		else {	/* Whenever it is pushed steady */
			if( ++btn_count > RESET_WAIT_COUNT )
			{
				btn_pressed=2;
			}
			if (btn_pressed==2)
			{
				/* 0123456789 */
				/* 0011100111 */
				if ((btn_count%10)<1 ||
				    ((btn_count%10)>4 && (btn_count%10)<7)) LED_CONTROL(LED_POWER, LED_OFF);
				else LED_CONTROL(LED_POWER, LED_ON);
			}
		}
//	   } //end BTN_RST MFG test
	}
	else
	{
//		if (	!nvram_match("wl0_mode", "wds") &&
		if (	int_wl0_mode!=1 &&
//			(wl_on!=wl_on_old || (nvram_match("wl_mode_EX", "sta") || nvram_match("wl_mode_EX", "re")))
			(wl_on!=wl_on_old || int_wl_mode_EX!=0)
		)
		{
			if(wl_on==1)
			{
				wl_led++;
	
				if(rssi_g<RSSI_TH_E)
					wl_timer=6;
				else if ((rssi_g>=RSSI_TH_E) && (rssi_g<RSSI_TH_D))
					wl_timer=5;
				else if ((rssi_g>=RSSI_TH_D) && (rssi_g<RSSI_TH_C))
					wl_timer=4;
				else if ((rssi_g>=RSSI_TH_C) && (rssi_g<RSSI_TH_B))
					wl_timer=3;
				else if ((rssi_g>=RSSI_TH_B) && (rssi_g<RSSI_TH_A))
					wl_timer=2;
				else
					wl_timer=1;
	
				wl_led%=wl_timer;
				
				wl_led_state_old=wl_led_state;
	
				if(wl_timer==1)
					wl_led_state=1;
				else
					wl_led_state=(wl_led==0?1:0);
	
				if(wl_led_state!=wl_led_state_old)
				{
					if(wl_led_state)
					{
//						LED_CONTROL(LED_AIR2, LED_ON);
//						eval("wl", "gpiotimerval", "0x640000");
						wl_ioctl(WIF, WLC_SET_VAR, AIR_LED_ON, sizeof(AIR_LED_ON));
					}
					else
					{
//						LED_CONTROL(LED_AIR2, LED_OFF);
//						eval("wl", "gpiotimerval", "0x001000");
						wl_ioctl(WIF, WLC_SET_VAR, AIR_LED_OFF, sizeof(AIR_LED_OFF));
					}
				}
			}
			else if(wl_on==2 || wl_on==3)	// 3 for URE mode
			{
//				LED_CONTROL(LED_AIR2, LED_ON);
//				eval("wl", "gpiotimerval", "0x640000");
				wl_ioctl(WIF, WLC_SET_VAR, AIR_LED_ON, sizeof(AIR_LED_ON));
			}
			else
			{
				wl_led=0;
				wl_led_state_old=wl_led_state;
				wl_led_state=0;
//				LED_CONTROL(LED_AIR2, LED_OFF);
//				eval("wl", "gpiotimerval", "0x001000");
				wl_ioctl(WIF, WLC_SET_VAR, AIR_LED_OFF, sizeof(AIR_LED_OFF));
			}
		}

		if(btn_pressed==1)
		{
			btn_count = 0;
			btn_pressed = 0;			
			LED_CONTROL(LED_POWER, LED_ON);
#ifndef WL330GE
			alarmtimer(NORMAL_PERIOD, 0);
#else
			alarmtimer(0, QUICK_PERIOD);
#endif
		}
		else if(btn_pressed==2)
		{
			LED_CONTROL(LED_POWER, LED_OFF);
			alarmtimer(0, 0);
			eval("erase", "/dev/mtd/3");
			kill(1, SIGTERM);
		}
	}
#ifdef BTN_SETUP
	}
	
	if (btn_pressed!=0) return;
	
	if (btn_pressed_setup<BTNSETUP_START)
	{
/*
		if (!gpio_read("/dev/gpio/in", BTN_SETUP)) 
		{
		   // Add BTN_EZ MFG test
//
//		   if (nvram_invmatch("asus_mfg", "")){
//			nvram_set("btn_ez", "1");
//		   }
//		   else{
//
			if (btn_pressed_setup==BTNSETUP_NONE)
			{
				btn_pressed_setup=BTNSETUP_DETECT;
				btn_count_setup=0;
				alarmtimer(0, RUSHURGENT_PERIOD);
			}
			else {	// Whenever it is pushed steady
				if( ++btn_count_setup > SETUP_WAIT_COUNT )
				{
					btn_pressed_setup=BTNSETUP_START;
					btn_count_setup=0;
//					start_ots();
				}
			}
//		   } //end BTN_EZ MFG test
		}
		else 
*/
		if(btn_pressed_setup==BTNSETUP_DETECT)
		{
			btn_pressed_setup = BTNSETUP_NONE;
			btn_count_setup = 0;
			LED_CONTROL(LED_POWER, LED_ON);
#ifndef WL330GE
			alarmtimer(NORMAL_PERIOD, 0);
#else
			alarmtimer(0, QUICK_PERIOD);
#endif
		}
	}
	else 
	{		
		++btn_count_setup;
		btn_count_setup = (btn_count_setup%20);

		/* 0123456789 */
		/* 1010101010 */
		if(btn_count_setup==0 && nvram_match("bs_mode", "1"))
		{
			btn_pressed_setup = BTNSETUP_NONE;
			btn_count_setup = 0;
			LED_CONTROL(LED_POWER, LED_ON);
#ifndef WL330GE
			alarmtimer(NORMAL_PERIOD, 0);
#else
			alarmtimer(0, QUICK_PERIOD);
#endif
			nvram_set("bs_mode", "");	
			btn_stage=0;
		}
		else 
		{
			if (btn_stage) // second stage with different led
			{
				if ((btn_count_setup%2)==0) LED_CONTROL(LED_POWER, LED_ON);
				else LED_CONTROL(LED_POWER, LED_OFF);	
			}
			else
			{
				if ((btn_count_setup%2)==0&&(btn_count_setup>10)) LED_CONTROL(LED_POWER, LED_ON);
				else LED_CONTROL(LED_POWER, LED_OFF);	
			}
		}
	}
#endif
}

void refresh_ntpc(void)
{
	eval("killall","ntpclient");
	kill_pidfile_s("/var/run/ntp.pid", SIGTSTP);
	//printf("Sync time %dn", sync_interval);
}

static int ntp_first_refresh = 0;

int ntp_timesync(void)
{
	time_t now;
	struct tm tm;	
	struct tm gm, local;
	struct timezone tz;

	if (sync_interval != -1)
	{
		sync_interval--;
//		fprintf(stderr, "sync interval is %d\n", sync_interval);

		if (!ntp_first_refresh && nvram_match("ntp_ready", "1"))
		{
			ntp_first_refresh = 1;

			sync_interval = 4320;
			logmessage("ntp client", "time is synchronized to %s", nvram_safe_get("ntp_servers"));

			if (!int_router_disable && int_ddns_enable_x && nvram_match("ddns_updated", "0"))
				start_ddns();
		}
		else if (sync_interval == 0)
		{
			/* Update kernel timezone */
			setenv("TZ", nvram_safe_get("time_zone_x"), 1);
			time(&now);
			gmtime_r(&now, &gm);
			localtime_r(&now, &local);
			tz.tz_minuteswest = (mktime(&gm) - mktime(&local)) / 60;
			settimeofday(NULL, &tz);
			memcpy(&tm, localtime(&now), sizeof(struct tm));

		   	if (tm.tm_year > 100) // More than 2000 
			{	
				sync_interval = 4320;
				logmessage("ntp client", "time is synchronized to %s", nvram_safe_get("ntp_servers"));
				system("date");

				stop_upnp();
				start_upnp();
			}
			else
			{
//				sync_interval = 6;
				sync_interval = 3;
			}

			refresh_ntpc();	
		}
	}
}

enum 
{
	URLACTIVE=0,
	WEBACTIVE,
	RADIOACTIVE,
	ACTIVEITEMS
} ACTIVE;

int svcStatus[ACTIVEITEMS] = { -1, -1, -1};
int extStatus[ACTIVEITEMS] = { 0, 0, 0};
char svcDate[ACTIVEITEMS][10];
char *svcTime[ACTIVEITEMS][20];

int timecheck_item(char *activeDate, char *activeTime)
{
	#define DAYSTART (0)
	#define DAYEND (60*60*23+60*59+59) //86399
	int current, active, activeTimeStart, activeTimeEnd, i;
	time_t now;
	struct tm *tm;

	setenv("TZ", nvram_safe_get("time_zone_x"), 1);

	time(&now);
	tm = localtime(&now);
	current = tm->tm_hour*60 + tm->tm_min;

	active=0;

	//printf("active: %s %s\n", activeDate, activeTime);

	activeTimeStart=((activeTime[0]-'0')*10+(activeTime[1]-'0'))*60 + (activeTime[2]-'0')*10 + (activeTime[3]-'0');
		
	activeTimeEnd=((activeTime[4]-'0')*10+(activeTime[5]-'0'))*60 + (activeTime[6]-'0')*10 + (activeTime[7]-'0');
				
	if (activeDate[tm->tm_wday] == '1')
	{
		if (activeTimeEnd<activeTimeStart)
		{
			if ((current>=activeTimeStart && current<=DAYEND) ||
			   (current>=DAYSTART && current<=activeTimeEnd))
			{
				active = 1;
			}
			else
			{
				active = 0;
			}
		}
		else
		{
			if (current>=activeTimeStart &&
		     	current<=activeTimeEnd)
			{	
				active = 1;
			}	
			else 
			{
				active = 0;
			}
		}			
	}
	return(active);
}

/* Check for time-dependent service 	*/
/* 1. URL filter 			*/
/* 2. WEB Camera Security filter 	*/

int svc_timecheck(void)
{
	int activeFlag, activeNow;

	activeFlag = 0;

	int val;

	/* Initialize */
	if (svcStatus[URLACTIVE]==-1 && nvram_invmatch("url_enable_x", "0"))
	{
		strcpy(svcDate[URLACTIVE], nvram_safe_get("url_date_x"));
		strcpy(svcTime[URLACTIVE], nvram_safe_get("url_time_x"));
		svcStatus[URLACTIVE] = -2;
	}	

	if (svcStatus[URLACTIVE]!=-1)
	{
		activeNow = timecheck_item(svcDate[URLACTIVE], svcTime[URLACTIVE]);	
		if (activeNow!=svcStatus[URLACTIVE])
		{
			//printf("url time change: %d\n", activeNow);
			svcStatus[URLACTIVE] = activeNow;
			stop_dns();
			start_dns();
		}
	}

	if (svcStatus[WEBACTIVE]==-1 && 
		nvram_invmatch("usb_webenable_x", "0") &&
		nvram_invmatch("usb_websecurity_x", "0"))
	{	
		strcpy(svcDate[WEBACTIVE], nvram_safe_get("usb_websecurity_date_x"));
		strcpy(svcTime[WEBACTIVE], nvram_safe_get("usb_websecurity_time_x"));
		svcStatus[WEBACTIVE] = -2;
	}

	if (svcStatus[WEBACTIVE]!=-1)
	{
		activeNow = timecheck_item(svcDate[WEBACTIVE], svcTime[WEBACTIVE]);
		if (activeNow!=svcStatus[WEBACTIVE])
		{
			svcStatus[WEBACTIVE] = activeNow;

			if (!notice_rcamd(svcStatus[WEBACTIVE])) svcStatus[WEBACTIVE]=-1;
		}	
	}

	if (svcStatus[RADIOACTIVE]==-1 && nvram_invmatch("wl_radio_x", "0"))
	{	
		strcpy(svcDate[RADIOACTIVE], nvram_safe_get("wl_radio_date_x"));
		strcpy(svcTime[RADIOACTIVE], nvram_safe_get("wl_radio_time_x"));
		svcStatus[RADIOACTIVE] = -2;
	}


	if (svcStatus[RADIOACTIVE]!=-1)
	{
		activeNow = timecheck_item(svcDate[RADIOACTIVE], svcTime[RADIOACTIVE]);
		if (activeNow!=svcStatus[RADIOACTIVE])
		{
			svcStatus[RADIOACTIVE] = activeNow;

			if (activeNow) 
			{
//				eval("wl", "radio", "on");
				val=0x10000;
				wl_ioctl(WIF, WLC_SET_RADIO, &val, sizeof(val));
				wl_led_ctrl(1);
			}
			else 
			{
//				eval("wl", "radio", "off");
				val=0x10001;
				wl_ioctl(WIF, WLC_SET_RADIO, &val, sizeof(val));
				wl_led_ctrl(0);
			}
		}
	}

	//printf("svc : %d %d %d\n", svcStatus[0], svcStatus[1], svcStatus[2]);
	return 0;
}	
	
/* Sometimes, httpd becomes inaccessible, try to re-run it */
int http_processcheck(void)
{
	char http_cmd[32];
	char buf[256];

	//printf("http check\n");
	sprintf(http_cmd, "http://127.0.0.1/");
	if (	!http_check(http_cmd, buf, sizeof(buf), 0) &&
		nvram_invmatch("ap_scanning", "1") &&
		nvram_invmatch("updating", "1") &&
		nvram_invmatch("ap_selecting", "1") &&
		nvram_invmatch("uploading", "1")
	)
	{
		dprintf("http rerun\n");
		kill_pidfile("/var/run/httpd.pid");
		if(nvram_match("httpd_die_reboot", "1")){
			nvram_set("httpd_die_reboot", "");
			eval("reboot");
		}
		//stop_httpd();
		start_httpd();
	}

#ifdef USB_SUPPORT
	if (nvram_invmatch("usb_webdriver_x", "") &&
	    nvram_invmatch("usb_webdriver_x", "0"))
	{						
		sprintf(http_cmd, "http://127.0.0.1:%s/", nvram_safe_get("usb_webhttpport_x"));
		//logmessage("webcam", "webcam httpd die checking %s\n", http_cmd);

		if (	!http_check(http_cmd, buf, sizeof(buf), 0) &&
			nvram_invmatch("ap_scanning", "1") &&
			nvram_invmatch("updating", "1") &&
			nvram_invmatch("ap_selecting", "1") &&
			nvram_invmatch("uploading", "1")
		)
		{
			dprintf("http rerun\n");
			sprintf(buf, "/var/run/httpd-%s.pid", nvram_safe_get("usb_webhttpport_x"));
			kill_pidfile(buf);
			//logmessage("webcam", "webcam httpd rerun\n");

			chdir("/tmp/webcam");
			eval("httpd", nvram_safe_get("wan0_ifname"), nvram_safe_get("usb_webhttpport_x"));
			chdir("/");
		}
	}
#endif
	return 0;
}

/* While radius auth fails, wireless is down. We need to retry the auth */
int nas_processcheck()
{
	FILE *fp;
	char cfgfile[64];
	char pidfile[64];

        snprintf(cfgfile, sizeof(cfgfile), "/tmp/nas.%s.conf", "lan");
        snprintf(pidfile, sizeof(pidfile), "/tmp/nas.%s.pid", "lan");

	fp = fopen (pidfile, "r");

	if (fp  == NULL) {

		char *argv[] = {"nas", cfgfile, pidfile, "lan", NULL};
		pid_t pid;
		syslog(LOG_NOTICE, "Radius server authentication failed.");
		syslog(LOG_NOTICE, "Please make sure the settings of the radius server are correct and the radius server is available.");
		syslog(LOG_NOTICE, "AP/Router would try to authenticate again in seconds.");
		sleep(5);
		_eval(argv, NULL, 0, &pid);
	}
	return 0;
}

#ifdef USB_SUPPORT
char usbinterrupt[128];

int rcamd_processcheck()
{
	FILE *fp;
	char buf[128];
	int flag=1;

	fp = fopen("/proc/interrupts", "r");

	if (fp!=NULL)
	{
		while(fgets(buf, sizeof(buf), fp))
		{
#ifdef WL500GX
			if (strstr(buf, "ehci"))
#else
			if (strstr(buf, "ohci"))
#endif
			{
				//logmessage("web camera", buf);

//				if (strcmp(usbinterrupt, buf)==0) flag=0;
//				strcpy(usbinterrupt, buf);
//				break;
			}				
		}
		fclose(fp);
	}
	return flag;		
}
#endif

int notice_rcamd(int flag)
{
	 int rcamdpid=-1;
	 //printf("Send signal : %d %d\n", rcamdpid, flag);
	 if (rcamdpid==-1)
	 {
			FILE *fp;

			if ((fp=fopen("/var/run/rcamd.pid","r"))!=NULL)
			{
				fscanf(fp,"%d", &rcamdpid);
				fclose(fp);
			}			
	 }
	 if (rcamdpid!=-1) 
	 {
		 if (flag)
			kill(rcamdpid, SIGUSR1);
		 else
			kill(rcamdpid, SIGUSR2);	

		 return 1;
	 }
	 return 0;
}

int refresh_rcamd(void)
{
	FILE *fp;
	int rcamdpid=-1;

	if ((fp=fopen("/var/run/rcamd.pid","r"))!=NULL)
	{
		fscanf(fp,"%d", &rcamdpid);
		fclose(fp);
		unlink("/var/run/rcamd.pid");
		kill(rcamdpid, SIGUSR1);
	}			
	else 
	{	
		eval("killall", "rcamd");
	}

	if ((fp=fopen("/var/run/rcamdmain.pid","r"))!=NULL)
	{
		fscanf(fp,"%d", &rcamdpid);
		fclose(fp);
		kill(rcamdpid, SIGUSR1);
	}		
	return 0;
}

int refresh_wave(void)
{
	FILE *fp;
	int wavepid=-1;

	eval("killall", "waveserver");

	if ((fp=fopen("/var/run/waveservermain.pid","r"))!=NULL)
	{
		fscanf(fp,"%d", &wavepid);
		fclose(fp);
		kill(wavepid, SIGUSR1);
	}			
	return 0;
}

static void catch_sig(int sig)
{
	if (sig == SIGUSR1)
	{
		dprintf("Catch Reset to Default Signal\n");
		//sys_default();
		//set_pid(getpid());	
	}
	else if (sig == SIGUSR2)
	{
		FILE *fp;
		char command[256], *cmd_ptr;

		dprintf("Get Signal: %d %d %d\n", svcStatus[WEBACTIVE], extStatus[WEBACTIVE], sig);

		if (!svcStatus[WEBACTIVE]) return;

		if (extStatus[WEBACTIVE]==0)
		{
			fp = fopen("/var/tmp/runshell", "r+");
			if (fp!=NULL)
			{
				cmd_ptr = fgets(command, 256, fp);

				if (cmd_ptr!=NULL) system(command);
			}
			fclose(fp);
			unlink("/tmp/runshell");
			notice_rcamd(0);
			extStatus[WEBACTIVE]=1;
		}
		else if (svcStatus[WEBACTIVE]==1)
		{
			notice_rcamd(1);
			extStatus[WEBACTIVE] = 0;
		}
	}	
}

void sta_check(void)
{
	int ret;
	struct maclist *auth;
	int maclist_size;
	int max_sta_count = 128;
	char uppermac[18];
	char sbuf[256];
	int setauth=0;
	char ssid[33];
	int ssid_len=0;

/*
	if (stacheck_interval==-1)
		stacheck_interval=STACHECK_PERIOD_DISCONNECT;
	
	stacheck_interval--;
	if (stacheck_interval) return;
*/

	if (nvram_invmatch("wl_sta_mac", ""))
	{	
#ifdef WL330GE
//		if (nvram_match("wl0_akm", "psk") || nvram_match("wl0_akm", "psk2"))
		if (int_wl0_akm==1 || int_wl0_akm==2)
		{
			maclist_size = sizeof(auth->count) + max_sta_count * sizeof(struct ether_addr);
			auth = malloc(maclist_size);
			// query wl for authenticated sta list
			strcpy((char*)auth, "autho_sta_list");
			if (!wl_ioctl(WIF, WLC_GET_VAR, auth, maclist_size))
			{
				if (auth->count>0)
				{
					nvram_set("wl_sta_state", "authorized");
					stacheck_timer=0;
//					stacheck_interval=STACHECK_PERIOD_CONNECT;
				}
				else
				{
					nvram_set("wl_sta_state", "associated");
//					stacheck_interval=STACHECK_PERIOD_DISCONNECT;
				}					
			}
			if (auth) free(auth);
		}
		else
//			nvram_set("wl_sta_state", "associated");
#endif
			stacheck_timer=0;
//			stacheck_interval=STACHECK_PERIOD_CONNECT;
	}
	else 
	{
		if(assoc_after_boot==1)
			system("wl disassoc");

//		memset(sbuf, 0, sizeof(sbuf));
//		sprintf(sbuf, "wl join \"%s\"", nvram_safe_get("wl0_ssid"));
//		system(sbuf);

		if(assoc_after_boot!=-1 && nvram_invmatch("wl0_ssid", ""))
		{
			wlc_ssid_t wst={0, ""};
			memset(ssid, 0, sizeof(ssid));
			strcpy(ssid, nvram_safe_get("wl0_ssid"));
			ssid_len=strlen(ssid);
			if (ssid && ssid_len > 0 && ssid_len <= sizeof(wst.SSID)) {
				wst.SSID_len = ssid_len;
				memcpy(wst.SSID, ssid, ssid_len);
				wl_ioctl(WIF, WLC_SET_SSID, &wst, sizeof(wst));
			}
		}

//		if (assoc_after_boot==1 && nvram_match("wl0_auth", "1"))
		if (assoc_after_boot==1 && int_wl0_auth==1)
		{
//			system("wl auth 1");
			setauth=1;
			wl_ioctl(WIF, WLC_SET_AUTH, &setauth, sizeof(setauth));
		}

//		stacheck_interval=Stacheck_Period_Disconnect;
	}
	return;
}

/* wathchdog is runned in NORMAL_PERIOD, 1 seconds
 * check in each NORMAL_PERIOD
 *	1. button
 *
 * check in each NORAML_PERIOD*5
 *
 *      1. ntptime, 
 *      2. time-dependent service
 *      3. http-process
 *      4. usb hotplug status
 */
void watchdog(void)
{
	time_t now;

#ifdef WL330GE
	wlcheck_period = (watchdog_period+39) % 40;
	if((int_wl_mode_EX!=1 || btn_pressed==0) && (wlcheck_period==0 || wlcheck_period==10 || wlcheck_period==20 || wlcheck_period==30))
	{
		wl_on_old=wl_on;	// for LED AIR2 control on URE mode
		wl_on=wl_status(&rssi_g);

		if (wl_on==0)
		{
//			if (nvram_match("wl_mode_EX", "sta") || nvram_match("wl_mode_EX", "re"))
			if (int_wl_mode_EX!=0)
			{
				nvram_set("wl_sta_mac", "");
				nvram_set("wl_sta_state", "scanning");
			}
		}
	}
#endif

	/* if timer is set to less than 1 sec, then bypass the following */
#ifndef WL330GE
	if (itv.it_value.tv_sec==0) return;
#endif

	/* handle button */
	btn_check();
	
	if(nvram_match("more_mem", "1"))	// save memory for ATE function
	{
		eval("killall", "ntp");
		eval("killall", "ntpclient");
		eval("killall", "udhcpc");
//		stop_httpd();
		stop_logger();
		stop_nas();
		stop_upnp();
		stop_dhcpd();
		stop_dns();
		nvram_set("more_mem", "2");
	}

	// reboot signal checking
	if(nvram_match("reboot", "1"))
	{
		reboot_count++;
		if(reboot_count>=2) kill(1, SIGTERM);
	}
	else if(nvram_match("reboot", "0")) return;


/* station or ethernet bridge handler */
#ifdef WL330GE
//	if(nvram_match("no_profile", "0"))
	if(int_no_profile==0)
	{
//	if(	(nvram_match("wl_mode_EX", "sta") || nvram_match("wl_mode_EX", "re")) &&
	if(	(int_wl_mode_EX!=0) &&
		(	(wlcheck_period==0 && stacheck_timer==0) ||
//			((nvram_match("wl0_akm", "psk") || nvram_match("wl0_akm", "psk2")) && nvram_invmatch("wl_sta_mac", "") && nvram_invmatch("wl_sta_state", "authorized") && (wlcheck_period==0 || wlcheck_period==10 || wlcheck_period==20 || wlcheck_period==30)) ||
			((int_wl0_akm==1 || int_wl0_akm==2) && nvram_invmatch("wl_sta_mac", "") && nvram_invmatch("wl_sta_state", "authorized") && (wlcheck_period==0 || wlcheck_period==10 || wlcheck_period==20 || wlcheck_period==30)) ||
			(nvram_match("wl_sta_mac", "") && (wlcheck_period==0 || wlcheck_period==13 || wlcheck_period==27))
		)
	)
		sta_check();
	}
#endif

#ifdef WL330GE
	watchdog_period = (watchdog_period+1) % 40;
#else
	watchdog_period = (watchdog_period+1) %10;
#endif

	if (watchdog_period) return;

	time(&now);
	//printf("now: %d\n", (long )now);

#ifdef WL330GE
	wl_reset_timer = (wl_reset_timer+1) % 60;
	ddns_timer = (ddns_timer+1) % 360*12;
	stacheck_timer = (stacheck_timer+1) % 3;


//	if(	nvram_match("no_profile", "0") &&
	if(	int_no_profile==0 &&
//		nvram_match("wl_mode_EX", "re") &&
		int_wl_mode_EX==2 &&
		nvram_match("wl_sta_mac", "")
	)
	{
		if (++wl_reset_timer_ure>=3)
		{	
			wlcheck_period=0;
			wl_led=0;
			wl_on=0;
			wl_timer=0;
			wl_led_state=0;
			wl_led_state_old=0;
			assoc_after_boot=-1;
			rssi_g=-32768;
			rssi_g_old=-32768;
			wl_on_old=0;
			wl_reset_timer=0;
			wl_reset_timer_ure=0;

			wl_ioctl(WIF, WLC_DOWN, NULL, 0);
			wl_ioctl(WIF, WLC_UP, NULL, 0);

			return;			
		}
	}
	else
		wl_reset_timer_ure=0;

//	if (nvram_match("wl_mode_EX", "ap"))
	if (int_wl_mode_EX==0)
	{
		int mf=0;
		wl_mssid(&mf);
		if (wl_reset_timer==0 && mf==1)
		{
			wl_ioctl(WIF, WLC_DOWN, NULL, 0);
			wl_ioctl(WIF, WLC_UP, NULL, 0);
		}
	}
	else if(assoc_after_boot==-1)
		assoc_after_boot=0;

#endif

#ifdef BTN_SETUP
	if (btn_pressed_setup>=BTNSETUP_START) return;
#endif

	if(nvram_invmatch("more_mem", "0"))
		return;

//	if(nvram_match("wan_nat_X", "1"))
	if(int_wan_nat_X==1)
	{
		/* sync time to ntp server if necessary */
		if (!nvram_match("wan_dns_t", "") && !nvram_match("wan_gateway_t", ""))
		{
			ntp_timesync();
		}

//		if (nvram_invmatch("router_disable", "1") &&
		if (	int_router_disable==0 &&
//			nvram_match("ddns_enable_x", "1") &&
			int_ddns_enable_x==1 &&
			(nvram_get("wan_ipaddr_t")!=NULL && nvram_invmatch("wan_ipaddr_t", "")) &&
			(nvram_match("ddns_updated", "0")||ddns_timer==0))
		{
			start_ddns();
			ddns_timer=0;
		}

		/* check for time-dependent services */
		svc_timecheck();
	}
	else
	{
		if (!nvram_match("lan_dns_t", "") && !nvram_match("lan_gateway_t", ""))
			ntp_timesync();
	}

	/* http server check */
#ifdef WL330GE
	if (	nvram_invmatch("ap_scanning", "1") &&
		nvram_invmatch("updating", "1") &&
		nvram_invmatch("ap_selecting", "1") &&
		nvram_invmatch("uploading", "1")
	)
#endif
		http_processcheck();

	/* nas check for radius fail auth */
	if (int_nas_enable)
		nas_processcheck();

#ifdef USB_SUPPORT

	/* web cam process */
	if (nvram_invmatch("usb_web_device", ""))
	{	
		if (nvram_invmatch("usb_webdriver_x", ""))
		{
			if (!rcamd_processcheck())
			{
				refresh_rcamd();
			}
		}				
		else
		{	
			//hotplug_usb_webcam(nvram_safe_get("usb_web_device"), 
			//	   atoi(nvram_safe_get("usb_web_flag")));
			//nvram_set("usb_web_device", "");
			//nvram_set("usb_web_flag", "");
			// reset WEBCAM status	
			refresh_rcamd();
			svcStatus[WEBACTIVE] = -1;
		}
	}

	/* storage process */
	if (nvram_invmatch("usb_storage_device", ""))
	{
		hotplug_usb_mass(nvram_safe_get("usb_storage_device"));
		nvram_set("usb_storage_device", "");
	}
#endif	

#ifdef CDMA
	/* CDMA_DOWN = 99, none
	 * CDMA_DOWN = 1, currently down
	 * CDMA_DOWN = 2, currently up
	 * CDMA_DOWN = 0, currently trying to connect
         */
	if (nvram_match("cdma_down", "1"))
	{
		logmessage("CDMA client", "cdma is down(%d)!", cdma_down);

		cdma_down++;
		cdma_connect=0;
		
		if(cdma_down==2)
		{
			stop_wan();
			start_wan();
		}
		else if(cdma_down>=12) /* 2 minutes timeout for retry */
		{
			cdma_down=0;
		}
	}
	else if(nvram_match("cdma_down", "0"))
	{
		logmessage("CDMA client", "cdma try connect(%d)!", cdma_connect);
		cdma_down=0;
		cdma_connect++;

		if(cdma_connect>12) /* 2 minitues timeout for connecting */
		{
			nvram_set("cdma_down", "1");
		}
	}
	else
	{
		cdma_down=0;
		cdma_connect=0;
	}
#endif
}

int
gpio_main(int ledin)
{
//#ifdef BTN_SETUP
//	printf("BTN:%d,%d", gpio_read("/dev/gpio/in", BTN_RESET), gpio_read("/dev/gpio/in", BTN_SETUP));
//#else
	printf("BTN:%d,0", gpio_read("/dev/gpio/in", BTN_RESET));
//#endif
}

int 
watchdog_main(int argc, char *argv[])
{
	FILE *fp;

#ifdef REMOVE
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
		/* parent process should just die */
		_exit(0);
	}
#endif

	/* write pid */
	if ((fp=fopen("/var/run/watchdog.pid", "w"))!=NULL)
	{
		fprintf(fp, "%d", getpid());
		fclose(fp);
	}

	/* set the signal handler */
	signal(SIGUSR1, catch_sig);
	signal(SIGUSR2, catch_sig);
	signal(SIGALRM, watchdog);

	/* Start GPIO function */
	gpio_init();

	/* Start POWER LED */
	LED_CONTROL(LED_POWER, LED_ON);

 	if (	nvram_match("wl0_akm", "wpa" ) ||	// WPA-Enterprise
		nvram_match("wl0_akm", "wpa2" ) ||	// WPA2-Enterprise
		nvram_match("wl0_auth_mode", "radius" )	// Radius with 802.1x
	)
		int_nas_enable=1;

	if (nvram_match("wl_mode_EX", "ap"))
		int_wl_mode_EX=0;
	else if (nvram_match("wl_mode_EX", "sta"))
		int_wl_mode_EX=1;
	else if (nvram_match("wl_mode_EX", "re"))
		int_wl_mode_EX=2;

	if (nvram_match("wl0_mode", "ap"))
		int_wl0_mode=0;
	else if (nvram_match("wl0_mode", "wds"))
		int_wl0_mode=1;
	else if (nvram_match("wl0_mode", "sta"))
		int_wl0_mode=2;
	else if (nvram_match("wl0_mode", "wet"))
		int_wl0_mode=3;

	if (nvram_match("wl0_akm", "psk"))
		int_wl0_akm=1;
	else if (nvram_match("wl0_akm", "psk2"))
		int_wl0_akm=2;
	else
		int_wl0_akm=3;

	if (nvram_match("ure_disable", "1"))
		int_ure_disable=1;

	if (nvram_match("wan_nat_X", "1"))
		int_wan_nat_X=1;

	if (nvram_match("wl0_auth", "1"))
		int_wl0_auth=1;

	if(nvram_match("no_profile", "0"))
		int_no_profile=0;
	else
		int_no_profile=1;

	if (nvram_match("router_disable", "1"))
		int_router_disable=1;

	if (nvram_match("ddns_enable_x", "1"))
		int_ddns_enable_x=1;

#ifdef WL330GE
//	LED_CONTROL(LED_AIR2, LED_OFF);
//	eval("wl", "gpiotimerval", "0x001000");

//	if (nvram_match("wl0_mode", "wds"))
	if (int_wl0_mode==1)
		wl_ioctl(WIF, WLC_SET_VAR, AIR_LED_ON, sizeof(AIR_LED_ON));
	else
		wl_ioctl(WIF, WLC_SET_VAR, AIR_LED_OFF, sizeof(AIR_LED_OFF));
#endif
// MFG test LED off for MFG USB test
//	LED_CONTROL(LED_POWER, LED_OFF);
//end MFG

	/* Start sync time */
	sync_interval=1;

#ifdef BTN_SETUP
/*
	if (nvram_invmatch("sharedkeystr", ""))
	{
		printf("Debug::sharedkeystr=%s\n", nvram_get("sharedkeystr"));
		btn_pressed_flag++;
		btn_stage=1;
	}
	nvram_set("bs_mode", "");
*/	
#endif

#ifdef WL330GE
	RSSI_TH_A=atoi(nvram_safe_get("threshold_a"));
	RSSI_TH_B=atoi(nvram_safe_get("threshold_b"));
	RSSI_TH_C=atoi(nvram_safe_get("threshold_c"));
	RSSI_TH_D=atoi(nvram_safe_get("threshold_d"));
	RSSI_TH_E=atoi(nvram_safe_get("threshold_e"));
#endif

	/* set timer */
#ifdef WL330GE
	alarmtimer(0, QUICK_PERIOD);
#else
	alarmtimer(NORMAL_PERIOD, 0);
#endif

#ifdef WL330GE
	nvram_set("ap_scanning", "0");
	nvram_set("ap_selecting", "0");
//	nvram_set("scan_get_flag", "0");
	nvram_set("updating", "0");
	wl_on=wl_on_old=0;
	rssi_g=rssi_g_old=-32768;
	wl_reset_timer=0;
	ddns_timer=0;
	nvram_set("RESET_PRESSED", "0");
	nvram_set("wl_sta_mac", "");
	nvram_set("wl_sta_state", "disconnected");
	nvram_set("more_mem", "0");	// save memory for ATE function
	nvram_set("uploading", "0");
#endif

//	if (	nvram_match("wan_nat_X", "1") &&
	if (	int_wan_nat_X==1 &&
		(nvram_match("wan0_proto", "pppoe") || nvram_match("wan0_proto", "pptp"))
	)
	{
		char *ping_argv[] = { "ping", "140.113.1.1", NULL};
		int pid;
		_eval(ping_argv, NULL, 0, &pid);
	}

	/* Most of time it goes to sleep */
	while(1)
	{
		pause();
	}
	
	return 0;
}

#define LED_RADIO GPIO0

void wl_led_ctrl(int ctrl)
{
	int led;
	if (!ctrl)
	{
		gpio_write("/dev/gpio/out", LED_RADIO, !ctrl);	
		led=0;
		wl_ioctl("eth2", WLC_SET_LED, &led, sizeof(led));
	}
	else
	{
		tgpio_fd_init();
		tgpio_ioctl(BCMGPIO_REG_RELEASE, LED_RADIO, LED_RADIO);
		tgpio_fd_cleanup();
	} 
}

int radio_main(int ctrl)
{
	int val;
	if (ctrl)
	{
//		eval("wl", "radio", "on");
		val=0x10000;
		wl_ioctl(WIF, WLC_SET_RADIO, &val, sizeof(val));
		wl_led_ctrl(1);
	}
	else
	{
//		eval("wl", "radio", "off");
		val=0x10001;
		wl_ioctl(WIF, WLC_SET_RADIO, &val, sizeof(val));
		wl_led_ctrl(0);
	}
}

typedef struct wl_scan_params2 {
	wlc_ssid_t ssid;	// default is {0, ""}
	struct ether_addr bssid;// default is bcast
	int8 bss_type;		// default is any, DOT11_BSSTYPE_ANY/INFRASTRUCTURE/INDEPENDENT
	int8 scan_type;		// -1 use default, DOT11_SCANTYPE_ACTIVE/PASSIVE
	int32 nprobes;		// -1 use default, number of probes per channel
	int32 active_time;	// -1 use default, dwell time per channel for active scanning
	int32 passive_time;	// -1 use default, dwell time per channel for passive scanning
	int32 home_time;	// -1 use default, dwell time for the home channel between channel scans
	int32 channel_num;	// 0 use default (all available channels), count of channels in channel_list
	uint16 channel_list[1];// list of chanspecs
} wl_scan_params_asus;

int wl_sta_status(int* rssin)
{
	int ret;
	char bssid[6];
//	wl_scan_results_t *result;
	wl_bss_info_t *info;
	char *info_b;
	unsigned char *bssidp;
//	unsigned char *bssidp2;
	int count=0;
	int rssi=-32768;
	char ssid[32];
	int ssid_len=0;
	int i;
	char uppermac[18];
	char uppermac2[18];

/*
	struct maclist *auth;
	int maclist_size;
	int max_sta_count = 128;
*/

	ret=wl_ioctl(WIF, WLC_GET_BSSID, bssid, sizeof(bssid));
	if (ret)
		return 0;
	else
	{
		ret=wl_ioctl(WIF, WLC_GET_BSS_INFO, buf, WLC_IOCTL_MAXLEN);
		if (ret)
			return 0;
		info = (wl_bss_info_t *)(buf+4);
		bssidp = (unsigned char *)&info->BSSID;
		if (!(bssidp[0]==0&&bssidp[1]==0&&bssidp[2]==0&&bssidp[3]==0&&bssidp[4]==0&&bssidp[5]==0))
		{
			memset(uppermac2, 0, sizeof(uppermac2));
			sprintf(uppermac2, "%02X:%02X:%02X:%02X:%02X:%02X", (unsigned char)bssidp[0], (unsigned char)bssidp[1], (unsigned char)bssidp[2], (unsigned char)bssidp[3], (unsigned char)bssidp[4], (unsigned char)bssidp[5]);
			nvram_set("wl_sta_mac", uppermac2);
			nvram_set("wl_sta_state", "associated");
			if (assoc_after_boot==0 || assoc_after_boot==-1)
				assoc_after_boot=1;
			wl_led=0;
			wl_led_state_old=0;
			wl_led_state=1;
			wl_ioctl(WIF, WLC_SET_VAR, AIR_LED_ON, sizeof(AIR_LED_ON));

/*
			if (nvram_match("wl0_akm", "psk") || nvram_match("wl0_akm", "psk2"))
			{
				maclist_size = sizeof(auth->count) + max_sta_count * sizeof(struct ether_addr);
				auth = malloc(maclist_size);
				// query wl for authenticated sta list
				strcpy((char*)auth, "autho_sta_list");
				if (!wl_ioctl(WIF, WLC_GET_VAR, auth, maclist_size))
				{
					if (auth->count>0)
					{
						stacheck_interval=STACHECK_PERIOD_CONNECT;
						nvram_set("wl_sta_state", "authorized");
					}
					else
					{
						stacheck_interval=STACHECK_PERIOD_DISCONNECT;
					}
				}
				if (auth) free(auth);
			}
*/
//			if (nvram_match("ure_disable", "1"))
			if (int_ure_disable==1)
			{
//				if (nvram_match("wan_nat_X", "1") && nvram_match("wl_mode_EX", "sta"))
				if (int_wan_nat_X==1 && int_wl_mode_EX==1)
					nvram_set("wan_status_t", "Connected");

				ret=wl_ioctl(WIF, WLC_GET_RSSI, &rssi, sizeof(rssi));
				if (ret==0)
					*rssin=rssi;
				return 1;
			}
			else	// do WLC_SCAN and WLC_SCAN_RESULTS alternately for URE mode
			{
//				if (nvram_match("wan_nat_X", "1"))
				if (int_wan_nat_X==1)
					nvram_set("wan_status_t", "Connected");
				return 3;
/*
				if (nvram_match("scan_get_flag", "0"))
				{
					nvram_set("scan_get_flag", "1");

					wlc_ssid_t wst={0, ""};
					strcpy(ssid, nvram_safe_get("wl_ssid"));
					ssid_len=strlen(ssid);
					if (ssid && ssid_len > 0 && ssid_len <= sizeof(wst.SSID)) {
						wst.SSID_len = ssid_len;
						memcpy(wst.SSID, ssid, ssid_len);
					}

					wl_scan_params_asus wsp;
					wsp.ssid=wst;
					wsp.bssid.octet[0]=bssidp[0];
					wsp.bssid.octet[1]=bssidp[1];
					wsp.bssid.octet[2]=bssidp[2];
					wsp.bssid.octet[3]=bssidp[3];
					wsp.bssid.octet[4]=bssidp[4];
					wsp.bssid.octet[5]=bssidp[5];
					wsp.bss_type=DOT11_BSSTYPE_ANY;
					wsp.scan_type=DOT11_SCANTYPE_PASSIVE;
					wsp.nprobes=wsp.active_time=wsp.passive_time=wsp.home_time=-1;
					wsp.channel_num=0;
					wsp.channel_list[0]=(uint16)info->channel;

					ret=wl_ioctl(WIF, WLC_SCAN, &wsp, sizeof(wsp));
					if (ret)
						return 0;
					if (wl_on_old)
						*rssin=rssi_g_old;
					return wl_on_old;
				}
				else
				{
					nvram_set("scan_get_flag", "0");

					result = (wl_scan_results_t *)buf;
					result->buflen=WLC_IOCTL_MAXLEN - sizeof(result);
					result->version=WL_BSS_INFO_VERSION;
					result->count=0;
					ret=wl_ioctl(WIF, WLC_SCAN_RESULTS, result, WLC_IOCTL_MAXLEN);
					if (ret)
						return 0;

					info = &(result->bss_info[0]);
					info_b = (unsigned char *)info;		
					for(i=0;i<result->count;i++)
					{
						bssidp2 = (unsigned char *)&info->BSSID;
						sprintf(uppermac, "%02X:%02X:%02X:%02X:%02X:%02X", (unsigned char)bssidp2[0], (unsigned char)bssidp2[1], (unsigned char)bssidp2[2], (unsigned char)bssidp2[3], (unsigned char)bssidp2[4], (unsigned char)bssidp2[5]);
					
						if (strcmp(uppermac, uppermac2)==0)
						{
							*rssin=info->RSSI;
							rssi_g_old=info->RSSI;
							return 1;
						}

						info_b += (sizeof(wl_bss_info_t));
						if ((info->ie_length%4)) 
						{				
							info_b+=info->ie_length+4-(info->ie_length%4);
						}
						else info_b+=info->ie_length;
						info = (wl_bss_info_t *)info_b;
					}
				}
*/
			}
		}
		else
		{
/*
			if (nvram_match("wan_nat_X", "1") && nvram_match("wl_mode_EX", "re"))
				nvram_set("wan_status_t", "Disconnected");
			else if (nvram_match("wan_nat_X", "1") && nvram_match("wl_mode_EX", "sta"))
				nvram_set("wan_status_t", "Disconnected");
*/
			if (int_wl_mode_EX!=0 && int_wan_nat_X==1)
				nvram_set("wan_status_t", "Disconnected");
		}
	}
	return 0;
}

int
wl_status(int* rssin)
{
//	int unit;
//	char tmp[100], prefix[] = "wlXXXXXXXXXX_";
//	char *name;
	struct maclist *auth, *assoc;
	int max_sta_count, maclist_size;
	int ret=0;
	int val;	

//	if ((unit = atoi(nvram_safe_get("wl_unit"))) < 0)
//		return -1;

//	snprintf(prefix, sizeof(prefix), "wl%d_", unit);
//	name = nvram_safe_get(strcat_r(prefix, "ifname", tmp));		
	
//	wl_ioctl(name, WLC_GET_RADIO, &val, sizeof(val));
	wl_ioctl(WIF, WLC_GET_RADIO, &val, sizeof(val));

	if (val==1) 
		return -1;	// Radio is disabled

/*
//	if (nvram_match(strcat_r(prefix, "mode", tmp), "ap"))
	if (int_wl0_mode==0)
	{
//		if (nvram_match("wl_lazywds", "1") || nvram_match("wl_wdsapply_x", "1"))
//			ret+=websWrite(wp, "Mode	: Hybrid\n");
//		else
//			ret+=websWrite(wp, "Mode	: AP Only\n");
	}
//	else if (nvram_match(strcat_r(prefix, "mode", tmp), "wds"))
	else if (int_wl0_mode==1)
	{
//		ret+=websWrite(wp, "Mode	: WDS Only\n");
	}
//	else if (nvram_match(strcat_r(prefix, "mode", tmp), "sta"))
	else
*/
	if (int_wl0_mode==2)
	{
//		ret+=websWrite(wp, "Mode	: Stations\n");
		return wl_sta_status(rssin);
	}
//	else if (nvram_match(strcat_r(prefix, "mode", tmp), "wet"))
	else if (int_wl0_mode==3)
	{
//		ret+=websWrite(wp, "Mode	: Ethernet Bridge\n");
		return wl_sta_status(rssin);
	}

	/* buffers and length */
	max_sta_count = 128;
	maclist_size = sizeof(auth->count) + max_sta_count * sizeof(struct ether_addr);

	auth = malloc(maclist_size);
	assoc = malloc(maclist_size);

	if (!auth || !assoc)
		goto exit;

	/* query wl for authenticated sta list */
	strcpy((char*)auth, "authe_sta_list");
//	if (wl_ioctl(name, WLC_GET_VAR, auth, maclist_size))
	if (wl_ioctl(WIF, WLC_GET_VAR, auth, maclist_size))
		goto exit;

	/* query wl for associated sta list */
	assoc->count = max_sta_count;
//	if (wl_ioctl(name, WLC_GET_ASSOCLIST, assoc, maclist_size))
	if (wl_ioctl(WIF, WLC_GET_ASSOCLIST, assoc, maclist_size))
		goto exit;

	if (assoc->count > 0)
		ret=2;

exit:
	if (auth) free(auth);
	if (assoc) free(assoc);
	return ret;
}

int wl_mssid(int *mssid_flag)
{
	int err;
	char buf[WLC_IOCTL_MAXLEN];
	
	memset(buf, 0, WLC_IOCTL_MAXLEN);
	strcpy(buf, "mssid");
	if ( (err=wl_ioctl(WIF, WLC_GET_VAR, buf, WLC_IOCTL_MAXLEN, FALSE))<0 )
		return err;
	*mssid_flag=*buf;
	return 0;
}
