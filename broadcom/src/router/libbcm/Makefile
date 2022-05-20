#
# BCM shared library Makefile for Linux router
#
# Copyright 2006, ASUSTek Inc.
# All Rights Reserved.                
#                                     
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of ASUSTek Inc.;   
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior      
# written permission of ASUSTek Inc..                            
#
# $Id: Makefile,v 1.1.1.1 2007/01/25 12:52:20 jiahao_jhou Exp $
#

include $(TOP)/.config


CFLAGS	+= -I. -I$(SRCBASE)/include -I$(SRCBASE)/router/shared -Wall
#CFLAGS	+= -g -DDEBUG
CFLAGS	+= -s -O2
LDFLAGS += -L.

OBJS := bcmgpio.o


all: libbcm.so

install: all
	install -d $(INSTALLDIR)/usr/lib
	install -m 755 libbcm.so $(INSTALLDIR)/usr/lib
	$(STRIP) $(INSTALLDIR)/usr/lib/libbcm.so

clean:
	rm -f *.o *.so

libbcm.so: $(OBJS)
	$(LD) -shared -o $@ $^
