#
# milli_httpd Makefile
#
# Copyright (C) 2001 ASUSTek Inc.
#
# $Id: Makefile,v 1.1.1.1 2006/04/25 06:09:26 jiahao_jhou Exp $
#

CFLAGS	+= -I. -I$(TOP)/shared -I$(SRCBASE)/include -Wall -DFLASH_EMULATOR -DLinux -DMIPS $(GLOBAL_OPTIONS)
#CFLAGS	+= -g -DDEBUG
CFLAGS	+= -s -O2 -DWL550gE
LDFLAGS	+= -L$(TOP)/nvram -L$(TOP)/shared -L$(TOP)/netconf

vpath %.c $(TOP)/shared
vpath %.c $(TOP)/shared_common

ifeq ($(CONFIG_NOUSB),y)
CFLAGS += -DNOUSB
endif

LANG_FILE = EN.dict TW.dict CN.dict DE.dict JP.dict TR.dict# KR.dict 
WWWDIR  = ../www/asus/web_asus_en
vpath translate_%.txt $(WWWDIR)/

all: dep httpd $(LANG_FILE)
	$(STRIP) httpd

dep:
	@echo "Generate depend file..."
	@for i in `find . -name "*.c"`; do ${CC} -MM ${CFLAGS} "$${i}"; done > $@

clean:
	rm -f *.o *~ httpd
	for f in $(LANG_FILE) ; do rm -f $$f; done

%.dict: translate_%.txt
	egrep '[^=]*=[^=]*' $^ | sed -e 's,<#ZVMODELVZ#>,<#ZVMODELVZ#>,g' -e 's,\r,,' > $@
#	cp $^ $@
#	egrep '[^=]*=[^=]*' $^ | sed -e 's,<#ZVMODELVZ#>,WL-330gE,g' -e 's,\r,,' > $@
#	egrep '[^=]*=[^=]*' $^ | sed -e 's,<#ZVMODELVZ#>,$(PID),g' -e 's,\r,,' > $@
#	egrep '[^=]*=[^=]*' $^ | sed -e 's,<#ZVMODELVZ#>,WL320g,g' -e 's,\r,,' > $@

install: all
	install -D httpd $(INSTALLDIR)/usr/sbin/httpd
	$(STRIP) $(INSTALLDIR)/usr/sbin/httpd

httpd: httpd.o ej.o cgi.o web_ex.o common.o nvram_f.o nvmap.o broadcom.o# ralink.o
	$(CC) -o $@ $^ $(LDFLAGS) -lnvram -lshared -lnetconf
