#
# Linux NVRAM Makefile
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

CFLAGS	+= -I. -I$(SRCBASE)/include -Wall
#CFLAGS	+= -g -DDEBUG
CFLAGS	+= -s -O2 -DASUS
LDFLAGS += -L.
LDFLAGS	+= -L$(TOP)/shared -L$(INSTALLDIR)/shared/usr/lib -lshared


vpath %.c $(SRCBASE)/shared/nvram

all: libnvram.so nvram# jj_iconv

install: all
	install -D libnvram.so $(INSTALLDIR)/usr/lib/libnvram.so
	$(STRIP) $(INSTALLDIR)/usr/lib/libnvram.so
	install -D nvram $(INSTALLDIR)/usr/sbin/nvram
	$(STRIP) $(INSTALLDIR)/usr/sbin/nvram
#	cp jj_iconv $(TOP)/500gp/exinstall/bin
#	$(STRIP) $(TOP)/500gp/exinstall/bin/jj_iconv

clean:
	rm -f *.o *.a *.so nvram

libnvram.so: nvram_linux.o
	$(LD) -shared -o $@ $^

nvram: main.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) -lnvram

#jj_iconv: jj_iconv.o
#	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) -lnvram
