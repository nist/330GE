#
# Toplevel Makefile for the BCM947xx Linux Router release
#
# Copyright 2006, ASUSTek Inc.
# All Rights Reserved.
# 
# THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
# KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
# SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
#
# $Id: Makefile,v 1.1.1.1 2007/01/25 12:51:46 jiahao_jhou Exp $
#

SRCBASE := $(shell pwd)
RELEASEDIR := $(shell (cd $(SRCBASE)/.. && pwd -P))

install: all
	install -d $(RELEASEDIR)/image
	$(MAKE) -C router install
	-cp router/mipsel/linux.trx $(RELEASEDIR)/image/linux-glibc.trx
	-cp router/mipsel-uclibc/linux.trx $(RELEASEDIR)/image/linux.trx
	-cp router/mipsel-uclibc/linux.bin $(RELEASEDIR)/image/linux.bin
ifneq ($(wildcard cfe),)
	cp cfe/build/broadcom/bcm947xx/cfe.bin $(RELEASEDIR)/image/
	cp cfe/build/broadcom/bcm947xx/compressed/cfez.bin $(RELEASEDIR)/image/
	# Clean and make one more time with GigE
	$(MAKE) -C cfe clean
	$(MAKE) -C cfe CFG_PCI=1
	cp cfe/build/broadcom/bcm947xx/cfe.bin $(RELEASEDIR)/image/cfe-gige.bin
	cp cfe/build/broadcom/bcm947xx/compressed/cfez.bin $(RELEASEDIR)/image/cfez-gige.bin
endif
	cp shared/nvram/*.txt $(RELEASEDIR)/image/

all clean:
	$(MAKE) -C router $@
ifneq ($(wildcard cfe),)
	$(MAKE) -C cfe $@
endif

.PHONY: all clean install
