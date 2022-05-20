#
# milli_httpd Makefile
#
# Copyright (C) 2001 ASUSTek Inc.
#
# $Id: Makefile,v 1.1.1.1 2007/01/25 12:52:52 jiahao_jhou Exp $
#

CFLAGS	+= -I. -I$(TOP)/shared -I$(SRCBASE)/include -Wall -DFLASH_EMULATOR -DLinux -DMIPS
#CFLAGS	+= -g -DDEBUG
CFLAGS	+= -s -O2
LDFLAGS	+= -L$(TOP)/nvram -L$(TOP)/shared -L$(TOP)/netconf

vpath %.c $(TOP)/shared
vpath %.c $(TOP)/shared_common

all: test
	$(STRIP) test

clean:
	rm -f *.o *~ test

install: all
	install -D test $(INSTALLDIR)/usr/sbin/test
	$(STRIP) $(INSTALLDIR)/usr/sbin/test

httpd: test.o 
	$(CC) -o $@ $^ $(LDFLAGS) -lnvram -lshared -lnetconf
