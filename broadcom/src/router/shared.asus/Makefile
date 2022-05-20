#
# Linux router shared code Makefile
#
# Copyright 2004, ASUSTek Inc.
# All Rights Reserved.
# 
# THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
# KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
# SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
#
# $Id: Makefile,v 1.1.1.1 2007/01/25 12:53:02 jiahao_jhou Exp $
#


CFLAGS	= -I. -I../httpd.asus -I$(SRCBASE)/include.asus -Wall
#CFLAGS	+= -g -DDEBUG
CFLAGS	+= -s -O2
LDFLAGS += -L.

OBJS := nvparse.o shutils.o wl.o wl_linux.o linux_timer.o defaults.o

ifeq ($(CONFIG_SENTRY5),y)
include $(TOP)/shared/Makefiles5.mk
endif

ifeq ($(CONFIG_NOUSB),y)
CFLAGS += -DNOUSB
endif

all: libshared.so

install: all
	install -d $(INSTALLDIR)/usr/lib
	install -m 755 libshared.so $(INSTALLDIR)/usr/lib/libshared.asus.so
	$(STRIP) $(INSTALLDIR)/usr/lib/libshared.asus.so

clean:
	rm -f *.o *.so

libshared.so: $(OBJS)
	$(LD) -shared -o $@ $^
	cp $@ libshared.asus.so
