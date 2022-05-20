#
# Router rc control scripts Makefile
#
# Copyright 2004, ASUSTek Inc.
# All Rights Reserved.
# 
# THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
# KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
# SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
#
# $Id: Makefile,v 1.1.1.1 2007/01/25 12:52:21 jiahao_jhou Exp $
#

include $(TOP)/.config

CFLAGS	+= -I. -I$(TOP)/shared -I$(SRCBASE)/include -Wall -DASUS_EXT  -DGPQOS $(GLOBAL_OPTIONS)
#CFLAGS	+= -g -DDEBUG
CFLAGS	+= -s -O2
LDFLAGS	+= -L$(TOP)/nvram -L$(INSTALLDIR)/nvram/usr/lib -lnvram -L$(TOP)/shared -L$(INSTALLDIR)/shared/usr/lib -lshared -lcrypt

ifeq ($(LANGUAGE),EN)
CFLAGS += -DLANGUAGE_EN
endif

ifeq ($(LANGUAGE),TW)
CFLAGS += -DLANGUAGE_TW
endif

ifeq ($(LANGUAGE),CN)
CFLAGS += -DLANGUAGE_CN
endif

ifeq ($(LANGUAGE),ML)
CFLAGS += -DLANGUAGE_CN
endif

ifeq ($(LANGUAGE),KR)
CFLAGS += -DLANGUAGE_KR
endif

ifeq ($(LANGUAGE),JP)
CFLAGS += -DLANGUAGE_JP
endif

OBJS := rc.o init.o interface.o network.o services.o udhcpc.o ppp.o http.o stats.o crc.o mtd.o common_ex.o network_ex.o services_ex.o manu_ex.o http_ex.o watchdog.o udhcpc_ex.o ntp.o bigpond.o# qosutils.o qos.o speedtest.o

ifeq ($(CONFIG_NETCONF),y)
ifeq ($(CONFIG_NOIPTABLES), y)
OBJS += firewall_noipt.o firewall_ex_noipt.o
LDFLAGS += -L$(TOP)/netconf -L$(INSTALLDIR)/netconf/usr/lib -lnetconf
else
OBJS += firewall.o firewall_ex.o
#OBJS += rcamd.o waveserver.o sendalarm.o dns.o dns_decode.o
LDFLAGS += -L$(TOP)/netconf -L$(INSTALLDIR)/netconf/usr/lib -lnetconf
endif
endif

ifeq ($(CONFIG_BTNSETUP),y)
#OBJS += btnsetup.o
#LDFLAGS += -L$(TOP)/libbcmcrypto -L$(INSTALLDIR)/libbcmcrypto/usr/lib -lbcmcrypto
endif

ifeq ($(CONFIG_SENTRY5),y)
include $(TOP)/rc/Makefiles5.mk
endif

#OBJS += ILibParsers.o

vpath %.c $(TOP)/shared $(SRCBASE)/rts/src

all: rc

clean:
	rm -f *.o rc

install: all
	install -d $(INSTALLDIR)/sbin
	install rc $(INSTALLDIR)/sbin
	$(STRIP) $(INSTALLDIR)/sbin/rc
	cd $(INSTALLDIR)/sbin && ln -sf rc init
	cd $(INSTALLDIR)/sbin && ln -sf rc erase
	cd $(INSTALLDIR)/sbin && ln -sf rc write
	cd $(INSTALLDIR)/sbin && ln -sf rc stats
	cd $(INSTALLDIR)/sbin && ln -sf rc hotplug

	# Added by ASUS
	cd $(INSTALLDIR)/sbin && ln -sf rc stopservice
	cd $(INSTALLDIR)/sbin && ln -sf rc ddns_updated
	cd $(INSTALLDIR)/sbin && ln -sf rc watchdog
	#cd $(INSTALLDIR)/sbin && ln -sf rc sendalarm
	#cd $(INSTALLDIR)/sbin && ln -sf rc rmwebcam
	#cd $(INSTALLDIR)/sbin && ln -sf rc rmstorage
	cd $(INSTALLDIR)/sbin && ln -sf rc wsrom
	cd $(INSTALLDIR)/sbin && ln -sf rc rsrom
	cd $(INSTALLDIR)/sbin && ln -sf rc wmac
	cd $(INSTALLDIR)/sbin && ln -sf rc ntp	
	#cd $(INSTALLDIR)/sbin && ln -sf rc rcamdmain
	#cd $(INSTALLDIR)/sbin && ln -sf rc waveservermain
	#cd $(INSTALLDIR)/sbin && ln -sf rc bpa_connect	
	#cd $(INSTALLDIR)/sbin && ln -sf rc bpa_disconnect
	cd $(INSTALLDIR)/sbin && ln -sf rc restore
	cd $(INSTALLDIR)/sbin && ln -sf rc start_ddns
	cd $(INSTALLDIR)/sbin && ln -sf rc wlan_update
	cd $(INSTALLDIR)/sbin && ln -sf rc ots
	cd $(INSTALLDIR)/sbin && ln -sf rc gpio
	cd $(INSTALLDIR)/sbin && ln -sf rc radioctrl
	cd $(INSTALLDIR)/sbin && ln -sf rc early_convert
	#cd $(INSTALLDIR)/sbin && ln -sf rc wsc
ifeq ($(CONFIG_DLM), y)
	cd $(INSTALLDIR)/sbin && ln -sf rc write_disc_status
	cd $(INSTALLDIR)/sbin && ln -sf rc hotplug_usb_mass
	cd $(INSTALLDIR)/sbin && ln -sf rc run_apps
	cd $(INSTALLDIR)/sbin && ln -sf rc run_ftpsamba
	cd $(INSTALLDIR)/sbin && ln -sf rc eject_usb1
	cd $(INSTALLDIR)/sbin && ln -sf rc eject_usb2
endif

rc: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

