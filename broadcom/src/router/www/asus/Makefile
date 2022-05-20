#
# Broadcom web pages
#
# Copyright 2003, ASUSTek Inc.
# All Rights Reserved.                
#                                     
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of ASUSTek Inc.;   
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior      
# written permission of ASUSTek Inc..                            
#
# $Id: Makefile,v 1.1.1.1 2006/04/25 06:10:24 jiahao_jhou Exp $
#

PAGES := *
PAGES_DIR := graph/*

all: pages.mk

pages.mk:
	echo "PAGES := $(PAGES)" > $@

install:
	./mkweb WL500gx "EN"	
	install -d $(INSTALLDIR)/www
	install -m 644 web_WL500gx/*.asp $(INSTALLDIR)/www
	install -m 644 web_WL500gx/*.js $(INSTALLDIR)/www
	install -m 644 web_WL500gx/*.css $(INSTALLDIR)/www
	install -d $(INSTALLDIR)/www/graph
	install -m 644 web_WL500gx/$(PAGES_DIR).gif $(INSTALLDIR)/www/graph
	install -m 644 web_WL500gx/$(PAGES_DIR).jpg $(INSTALLDIR)/www/graph
	rm -rf web
	rm -rf web_WL500gx

install-%:
	echo $(LANGUAGE)
	./mkweb $* "$(LANGUAGE)"
	rm -rf www_$*	
	install -d www_$*
	install -m 644 web_$*/*.asp www_$*
	install -m 644 web_$*/*.js www_$*
	install -m 644 web_$*/*.css www_$*
	( if [ "$*" != "WL520g" ] && [ "$*" != "WL550gE" ] && [ "$*" != "WL320gE" ] && [ "$*" != "WL320gP" ] && [ "$*" != "WL500bl" ] && [ "$*" != "WL331gl" ] && [ "$*" != "WL330gE" ]; then \
		install -m 644 web_$*/*.cab www_$*;\
	fi )
	( if [ "$*" = "WL320gE" ] || [ "$*" = "WL320gP" ]; then \
                install -d www_$*/slider; \
                install -m 644 web_$*/slider/*.js www_$*/slider; \
                install -m 644 web_$*/slider/*.gif www_$*/slider; \
        fi )
	install -d www_$*/graph
	install -m 644 web_$*/$(PAGES_DIR).gif www_$*/graph
	install -m 644 web_$*/$(PAGES_DIR).jpg www_$*/graph

#	ln -sf /tmp/settings www_$*/WL500g.Deluxe.CFG
#	ln -sf /tmp/settings www_$*/WL500g.CFG
#	ln -sf /tmp/settings www_$*/WL500gl.CFG
#	ln -sf /tmp/settings www_$*/WL500bl.CFG
#	ln -sf /tmp/settings www_$*/WL500gv2.CFG
#	ln -sf /tmp/settings www_$*/WL500g-X.CFG
#	ln -sf /tmp/settings www_$*/WL500b.CFG
#	ln -sf /tmp/settings www_$*/WL500bv2.CFG
#	ln -sf /tmp/settings www_$*/WL520g.CFG
#	ln -sf /tmp/settings www_$*/WL550gE.CFG
#	ln -sf /tmp/settings www_$*/WL320gE.CFG
#	ln -sf /tmp/settings www_$*/WL320gP.CFG
	ln -sf /tmp/settings www_$*/WL-330gE.CFG
	ln -sf /tmp/settings www_$*/setting.CFG
	ln -sf /tmp/model.jpg www_$*/model.jpg
#	ln -sf /tmp/settings www_$*/WL300g.CFG
#	ln -sf /tmp/settings www_$*/WL331g.CFG
#	ln -sf /tmp/settings www_$*/WL331gl.CFG
	ln -sf /var/tmp/display.jpg www_$*/display.jpg
	cp ../../httpd/*.dict www_$*/ -f
	rm -f www_$*/KR.dict
	rm -f www_$*/select_KR.asp

	rm -rf web
	rm -rf web_$*

clean:
	rm -f pages.mk
