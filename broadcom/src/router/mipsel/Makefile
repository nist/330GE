
#*
# ****************************************************************
# *   Copyright 2004, ASUSTek COMPUTER Inc.  All Rights Reserved *
# ****************************************************************
#
# THIS SOFTWARE IS OFFERED "AS IS", AND ASUSTek GRANTS NO WARRANTIES OF ANY
# KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE.  ASUSTek
# SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE
#
#
# $Id: Makefile,v 1.1.1.1 2007/01/25 12:52:22 jiahao_jhou Exp $
#

include ../.config
#export CHMOD := $(shell (chmod +x mkweb))

# Source bases
export PLATFORM LINUXDIR LIBDIR USRLIBDIR
export TOP := $(shell pwd)/..
export SRCBASE := $(shell (cd $(TOP)/.. && pwd -P))
RELEASEDIR := $(shell (cd $(SRCBASE)/.. && pwd -P))
PATH := $(RELEASEDIR)/tools:$(PATH)
#
# Install and target directories
#

export PLATFORMDIR := $(TOP)/$(PLATFORM)
export INSTALLDIR := $(PLATFORMDIR)/install
export TARGETDIR := $(PLATFORMDIR)/target.br

# Addeb by Joey to include version parameters
include version.conf

WL500gx:
	rm -rf www	
	mv target/www .
	cp -rf ../www/asus/www_WL500gx target/www
	mkcramfs target target.cramfs
	addver target.cramfs $(KERNEL_VER) $(FS_VER) WL500gx $(HWL1) $(HWH1) $(HWL2) $(HWH2) $(HWL3) $(HWH3) $(HWL4) $(HWH4) vmlinuz
	( if [ "$(USB_WAN)" = "CDMA" ]; then \
		trx -o WL500gx_$(KERNEL_VER).$(FS_VER)_$(LANGUAGE)_CDMA.trx vmlinuz target.cramfs; \
	else \
		trx -o WL500gx_$(KERNEL_VER).$(FS_VER)_$(LANGUAGE).trx vmlinuz target.cramfs; \
	fi )

WL500gp:
	rm -rf `find | grep CVS`
	rm -rf www	
	mv target/www .
	cp -rf ../www/asus/www_WL500gp target/www
	cd target
	rm -rf `find | grep CVS`
	cd ..
	mkcramfs target target.cramfs
	addver target.cramfs $(KERNEL_VER) $(FS_VER) WL520gu $(HWL1) $(HWH1) $(HWL2) $(HWH2) $(HWL3) $(HWH3) $(HWL4) $(HWH4) vmlinuz
	trx -o WL520gu_$(KERNEL_VER).$(FS_VER)_$(LANGUAGE).trx vmlinuz target.cramfs

WL330gE:
	rm -rf `find | grep CVS`
#	rm -rf www	
#	mv target/www .
	rm -rf target/www
	cp -rf ../www/asus/www_WL330gE target/www
	cd target
	rm -rf `find | grep CVS`
	cd ..
#	mkcramfs target target.cramfs
	$(MAKE) -C $(LINUXDIR)/scripts/squashfs mksquashfs CC=gcc CXX=g++
	$(LINUXDIR)/scripts/squashfs/mksquashfs target target.cramfs -all-root -noappend -nopad
	addver target.cramfs $(KERNEL_VER) $(FS_VER) WL-330gE $(HWL1) $(HWH1) $(HWL2) $(HWH2) $(HWL3) $(HWH3) $(HWL4) $(HWH4) vmlinuz
	trx -o WL-330gE_$(KERNEL_VER).$(FS_VER)_$(LANGUAGE).trx vmlinuz target.cramfs
	addtail WL-330gE_$(KERNEL_VER).$(FS_VER)_$(LANGUAGE).trx

WL500g:
	rm -rf www	
	mv target/www .
	cp -rf ../www/asus/www_WL500g target/www
	mkcramfs target target.cramfs
	addver target.cramfs $(KERNEL_VER) $(FS_VER) WL500g $(HWL1) $(HWH1) $(HWL2) $(HWH2) $(HWL3) $(HWH3) $(HWL4) $(HWH4) vmlinuz
	( if [ "$(USB_WAN)" = "CDMA" ]; then \
		trx -o WL500g_$(KERNEL_VER).$(FS_VER)_$(LANGUAGE)_CDMA.trx vmlinuz target.cramfs; \
	else \
		trx -o WL500g_$(KERNEL_VER).$(FS_VER)_$(LANGUAGE).trx vmlinuz target.cramfs; \
	fi )
	rm -rf target/www
	mv www target/.

WL500gv2:
	rm -rf www	
	mv target/www .
	cp -rf ../www/asus/www_WL500g target/www
	mkcramfs target target.cramfs
	addver target.cramfs $(KERNEL_VER) $(FS_VER) WL500gv2 $(HWL1) $(HWH1) $(HWL2) $(HWH2) $(HWL3) $(HWH3) $(HWL4) $(HWH4) vmlinuz
	trx -o WL500gv2_$(KERNEL_VER).$(FS_VER)_$(LANGUAGE).trx vmlinuz target.cramfs
	rm -rf target/www
	mv www target/.

WL520g:
	rm -rf www	
	mv target/www .
	cp -rf ../www/asus/www_WL520g target/www
	mkcramfs target target.cramfs


	( if [ "$(LANGUAGE)" = "CN" ] || [ "$(LANGUAGE)" = "ML" ]; then \
		addver target.cramfs $(KERNEL_VER) $(FS_VER) WL500g-X $(HWL1) $(HWH1) $(HWL2) $(HWH2) $(HWL3) $(HWH3) $(HWL4) $(HWH4) vmlinuz; \
		trx -o WL500g-X_$(KERNEL_VER).$(FS_VER)_$(LANGUAGE).trx vmlinuz target.cramfs; \
	  else \
		addver target.cramfs $(KERNEL_VER) $(FS_VER) WL520g $(HWL1) $(HWH1) $(HWL2) $(HWH2) $(HWL3) $(HWH3) $(HWL4) $(HWH4) vmlinuz; \
		trx -o WL520g_$(KERNEL_VER).$(FS_VER)_$(LANGUAGE).trx vmlinuz target.cramfs; \
	fi )

	rm -rf target/www
	mv www target/.

WL550gE:
	rm -rf www	
	mv target/www .
	cp -rf ../www/asus/www_WL550gE target/www
	#cp -f wl.o_131 target/lib/modules/2.4.20/kernel/drivers/net/wl/wl.o
	#cp -f et.o_131 target/lib/modules/2.4.20/kernel/drivers/net/et/et.o
	mkcramfs target target.cramfs


	( if [ "$(LANGUAGE)" = "CN" ] || [ "$(LANGUAGE)" = "ML" ]; then \
		addver target.cramfs $(KERNEL_VER) $(FS_VER) WL500gE $(HWL1) $(HWH1) $(HWL2) $(HWH2) $(HWL3) $(HWH3) $(HWL4) $(HWH4) vmlinuz; \
		trx -o WL500gE_$(KERNEL_VER).$(FS_VER)_$(LANGUAGE).trx vmlinuz target.cramfs; \
	  else \
		addver target.cramfs $(KERNEL_VER) $(FS_VER) WL550gE $(HWL1) $(HWH1) $(HWL2) $(HWH2) $(HWL3) $(HWH3) $(HWL4) $(HWH4) vmlinuz; \
		trx -o WL550gE_$(KERNEL_VER).$(FS_VER)_$(LANGUAGE).trx vmlinuz target.cramfs; \
	fi )

	rm -rf target/www
	mv www target/.

WL500b:
	rm -rf www	
	mv target/www .
	cp -rf ../www/asus/www_WL500b target/www
	mkcramfs target target.cramfs
	addver target.cramfs $(KERNEL_VER) $(FS_VER) WL500b $(HWL1) $(HWH1) $(HWL2) $(HWH2) $(HWL3) $(HWH3) $(HWL4) $(HWH4) vmlinuz
	trx -o WL500b_$(KERNEL_VER).$(FS_VER)_$(LANGUAGE).trx vmlinuz target.cramfs
	rm -rf target/www
	mv www target/.

WL500bv2:
	rm -rf www	
	mv target/www .
	cp -rf ../www/asus/www_WL500bv2 target/www
	mkcramfs target target.cramfs
	addver target.cramfs $(KERNEL_VER) $(FS_VER) WL500bv2 $(HWL1) $(HWH1) $(HWL2) $(HWH2) $(HWL3) $(HWH3) $(HWL4) $(HWH4) vmlinuz
	trx -o WL500bv2_$(KERNEL_VER).$(FS_VER)_$(LANGUAGE).trx vmlinuz target.cramfs
	rm -rf target/www
	mv www target/.

WL500gl:
	rm -rf www	
	mv target/www .
	cp -rf ../www/asus/www_WL500gl target/www
	mkcramfs target target.cramfs
	addver target.cramfs $(KERNEL_VER) $(FS_VER) WL520g $(HWL1) $(HWH1) $(HWL2) $(HWH2) $(HWL3) $(HWH3) $(HWL4) $(HWH4) vmlinuz
	trx -o WL520g_$(KERNEL_VER).$(FS_VER)_$(LANGUAGE).trx vmlinuz target.cramfs
	rm -rf target/www
	mv www target/.

WL500bl:
	rm -rf www	
	mv target/www .
	cp -rf ../www/asus/www_WL500bl target/www
	mkcramfs target target.cramfs
	addver target.cramfs $(KERNEL_VER) $(FS_VER) WL500bl $(HWL1) $(HWH1) $(HWL2) $(HWH2) $(HWL3) $(HWH3) $(HWL4) $(HWH4) vmlinuz
	trx -o WL500bl_$(KERNEL_VER).$(FS_VER)_$(LANGUAGE).trx vmlinuz target.cramfs
	rm -rf target/www
	mv www target/.

WL300g:
	#change some product dependent objects	
	rm -rf www	
	mv target/www .
	cp -rf ../www/asus/www_WL300g target/www
	mkcramfs target target.cramfs
	addver target.cramfs $(KERNEL_VER) $(FS_VER) WL300g $(HWL1) $(HWH1) $(HWL2) $(HWH2) $(HWL3) $(HWH3) $(HWL4) $(HWH4) vmlinuz
	trx -o WL300g_$(KERNEL_VER).$(FS_VER)_$(LANGUAGE).trx vmlinuz target.cramfs
	rm -rf target/www
	mv www target/.

WL300g2:
	rm -rf www	
	mv target/www .
	cp -rf ../www/asus/www_WL300g2 target/www
	mkcramfs target target.cramfs
	addver target.cramfs $(KERNEL_VER) $(FS_VER) WL300g2 $(HWL1) $(HWH1) $(HWL2) $(HWH2) $(HWL3) $(HWH3) $(HWL4) $(HWH4) vmlinuz
	trx -o WL300g2_$(KERNEL_VER).$(FS_VER)_$(LANGUAGE).trx vmlinuz target.cramfs
	rm -rf target/www
	mv www target/.

WL331g:
	rm -rf www	
	mv target/www .
	cp -rf ../www/asus/www_WL331g target/www
	mkcramfs target target.cramfs
	addver target.cramfs $(KERNEL_VER) $(FS_VER) WL331g $(HWL1) $(HWH1) $(HWL2) $(HWH2) $(HWL3) $(HWH3) $(HWL4) $(HWH4) vmlinuz
	trx -o WL331g_$(KERNEL_VER).$(FS_VER)_$(LANGUAGE).trx vmlinuz target.cramfs
	rm -rf target/www
	mv www target/.

WL331gl:
	rm -rf www	
	mv target/www .
	cp -rf ../www/asus/www_WL331gl target/www
	mkcramfs target target.cramfs
	addver target.cramfs $(KERNEL_VER) $(FS_VER) WL331gl $(HWL1) $(HWH1) $(HWL2) $(HWH2) $(HWL3) $(HWH3) $(HWL4) $(HWH4) vmlinuz
	trx -o WL331gl_$(KERNEL_VER).$(FS_VER)_$(LANGUAGE).trx vmlinuz target.cramfs
	rm -rf target/www
	mv www target/.

R100:
	rm -rf www	
	mv target/www .
	cp -rf ../www/mea/www_R100 target/www
	mkcramfs target target.cramfs
	addver target.cramfs $(KERNEL_VER) $(FS_VER) R100 $(HWL1) $(HWH1) $(HWL2) $(HWH2) $(HWL3) $(HWH3) $(HWL4) $(HWH4) vmlinuz
	trx -o R100_$(KERNEL_VER).$(FS_VER)_$(LANGUAGE).trx vmlinuz target.cramfs
	rm -rf target/www
	mv www target/.

R101:
	rm -rf www	
	mv target/www .
	cp -rf ../www/mea/www_R101 target/www
	mkcramfs target target.cramfs
	addver target.cramfs $(KERNEL_VER) $(FS_VER) R100x $(HWL1) $(HWH1) $(HWL2) $(HWH2) $(HWL3) $(HWH3) $(HWL4) $(HWH4) vmlinuz
	trx -o R100x_$(KERNEL_VER).$(FS_VER)_$(LANGUAGE).trx vmlinuz target.cramfs
	rm -rf target/www
	mv www target/.

clean:
	cd $(PLATFORMDIR) && \
	rm -rf *.trx && \
	rm -rf target target.cramfs && \
	$(MAKE) -C $(LINUXDIR) clean

