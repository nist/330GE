#
# Linux router shared code Makefile
#
# Copyright 2005, ASUSTek Inc.
# All Rights Reserved.
# 
# THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
# KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
# SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
#
# $Id: Makefile,v 1.1.1.1 2007/01/25 12:52:22 jiahao_jhou Exp $
#


CFLAGS	+= -I. -I$(SRCBASE)/include -I../httpd -Wall
#CFLAGS	+= -g -DDEBUG
CFLAGS	+= -s -O2
LDFLAGS += -L.

OBJS := nvparse.o shutils.o wl.o wl_linux.o linux_timer.o defaults.o


all: libshared.so# crc_gen

install: all
	install -d $(INSTALLDIR)/usr/lib
	install -m 755 libshared.so $(INSTALLDIR)/usr/lib
	$(STRIP) $(INSTALLDIR)/usr/lib/libshared.so

clean:
	rm -f *.o *.so

libshared.so: $(OBJS)
	$(LD) -shared -o $@ $^

crc_gen: crc_gen.o
	gcc -o crc_gen crc_gen.o

crc_gen.o:
	gcc -c -o crc_gen.o crc_gen.c
