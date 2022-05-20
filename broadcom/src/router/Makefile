#
# Broadcom Linux Router Makefile
# 
# Copyright 2004, ASUSTek Inc.
# All Rights Reserved.
# 
# THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
# KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
# SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
#
# $Id: Makefile,v 1.1.1.1 2007/01/25 12:52:13 jiahao_jhou Exp $
#

include .config
include .config.asus

export CONFIG_MULTICONFIG := y

#
# Paths
#

# Source bases
export PLATFORM LINUXDIR LIBDIR USRLIBDIR
export TOP := $(shell pwd)
export SRCBASE := $(shell (cd $(TOP)/.. && pwd -P))

#
# Cross-compile environment variables
#

# Build platform
export BUILD := i386-pc-linux-gnu
export HOSTCC := gcc

# uClibc wrapper
ifeq ($(CONFIG_UCLIBC),y)
export PLATFORM := $(PLATFORM)-uclibc
endif

ifeq ($(PLATFORM),mipsel)
export CROSS_COMPILE := mipsel-linux-
export CONFIGURE := ./configure mipsel-linux --build=$(BUILD)
export TOOLCHAIN := $(shell cd $(dir $(shell which $(CROSS_COMPILE)gcc))/../mipsel-linux && pwd -P)
endif

ifeq ($(PLATFORM),mipsel-uclibc)
export CROSS_COMPILE := mipsel-uclibc-
export CONFIGURE := ./configure mipsel-linux --build=$(BUILD)
export TOOLCHAIN := $(shell cd $(dir $(shell which $(CROSS_COMPILE)gcc))/.. && pwd -P)
endif

ifeq ($(CONFIG_BCMWPA2),y)
export CFLAGS += -DBCMWPA2 
endif

export CC := $(CROSS_COMPILE)gcc
export AR := $(CROSS_COMPILE)ar
export AS := $(CROSS_COMPILE)as
export LD := $(CROSS_COMPILE)ld
export NM := $(CROSS_COMPILE)nm
export RANLIB := $(CROSS_COMPILE)ranlib
export STRIP := $(CROSS_COMPILE)strip
export SIZE := $(CROSS_COMPILE)size

#
# Install and target directories
#

export PLATFORMDIR := $(TOP)/$(PLATFORM)
export INSTALLDIR := $(PLATFORMDIR)/install
export TARGETDIR := $(PLATFORMDIR)/target

# always build libbcmcrypto
obj-y += libbcmcrypto 

#
# Configuration
#

obj-$(CONFIG_SHARED) += shared
obj-$(CONFIG_NVRAM) += nvram
obj-$(CONFIG_HTTPD) += httpd
obj-$(CONFIG_RC) += rc
obj-$(CONFIG_LIBBCM) += libbcm
obj-$(CONFIG_WWW) += www
ifeq ($(CNFIG_SENTRY5),y)
include Makefiles5.cnf
endif
obj-$(CONFIG_GLIBC) += lib
obj-$(CONFIG_UCLIBC) += lib
obj-$(CONFIG_WLCONF) += wlconf
obj-$(CONFIG_BRIDGE) += bridge
obj-$(CONFIG_BUSYBOX) += busybox
obj-$(CONFIG_DNSMASQ) += dproxy
obj-$(CONFIG_IPTABLES) += iptables
obj-$(CONFIG_LIBIPT) += iptables
obj-$(CONFIG_NAS) += nas
obj-$(CONFIG_NETCONF) += netconf
obj-$(CONFIG_NTP) += ntpclient
obj-$(CONFIG_PPP) += ppp
obj-$(CONFIG_UDHCPD) += udhcpd
obj-$(CONFIG_UPNP) += upnp
obj-$(CONFIG_UTILS) += utils
obj-$(CONFIG_ETC) += etc
obj-$(CONFIG_VLAN) += vlan

# Added by Joey for ASUS application
obj-$(CONFIG_PPPD) += pppd
obj-$(CONFIG_PPTP) += pptp-client
obj-$(CONFIG_PPPOE) += rp-pppoe
obj-$(CONFIG_PPPOERELAY) += pppoe-relay
obj-$(CONFIG_INFOSVR) += infosvr
obj-$(CONFIG_EZIPUPDATE) += ez-ipupdate
#obj-$(CONFIG_JPEG6B) += jpeg-6b
#obj-$(CONFIG_RCAMD) += rcamdmips
#obj-$(CONFIG_SENDMAIL) += sendmail
#obj-$(CONFIG_LPRNG) += LPRng
obj-$(CONFIG_OTHERS) += others
#obj-$(CONFIG_OV51X) += ov51x
obj-$(CONFIG_TEST) += test
obj-$(CONFIG_BPALOGIN) += bpalogin
#obj-$(CONFIG_IPROUTE2) += iproute2
obj-$(CONFIG_BPLAY) += bplay

#obj-$(CONFIG_ATTR) += attr
#obj-$(CONFIG_ACL) += acl
obj-$(CONFIG_LIBCRYPTO) += libcrypto
#obj-$(CONFIG_DOSFSTOOLS) += dosfstools
#obj-$(CONFIG_VSFTPD) += vsftpd
#obj-y += utelnetd

obj-clean := $(foreach obj,$(obj-y) $(obj-n),$(obj)-clean)
obj-install := $(foreach obj,$(obj-y),$(obj)-install)

export LANGUAGE := "EN"
export USB_WAN := "NONE"

#
# Basic rules
#

all: $(obj-y) $(LINUXDIR)/.config
	# Also build kernel
	#( . $(LINUXDIR)/.config ; \
	#if [ "$${CONFIG_EMBEDDED_RAMDISK}" = "" ] ; then \
	#    $(MAKE) -C $(LINUXDIR) zImage ; \
	#fi ; \
	#if [ "$${CONFIG_MODULES}" = "y" ] ; then \
	#    $(MAKE) -C $(LINUXDIR) modules ; \
	#fi )
	. $(LINUXDIR)/.config ; \
	$(MAKE) -C $(LINUXDIR) zImage ; \
	$(MAKE) -C $(LINUXDIR) modules ; \

clean: $(obj-clean) config-clean
	rm -rf $(TARGETDIR)
	rm -f $(PLATFORMDIR)/linux.trx $(PLATFORMDIR)/vmlinuz $(PLATFORMDIR)/target.cramfs
	rm -f full usb20 lite middle high 330gE
	$(MAKE) -C $(LINUXDIR) clean

distclean mrproper: clean
	rm -f .config $(LINUXDIR)/.config

install package: $(filter-out lib-install,$(obj-install)) $(LINUXDIR)/.config
	# Install binaries into target directory
	install -d $(TARGETDIR)
	for dir in $(wildcard $(patsubst %,$(INSTALLDIR)/%,$(obj-y))) ; do \
            (cd $${dir} && tar cpf - .) | (cd $(TARGETDIR) && tar xpf -) \
        done
	# optimize the crypto library by removing unneeded symbols
	[ ! -d libbcmcrypto ] || $(MAKE) -C libbcmcrypto optimize
	# Install (and possibly optimize) C library
	$(MAKE) lib-install
	# Install modules into filesystem
	if grep -q "CONFIG_MODULES=y" $(LINUXDIR)/.config ; then \
            $(MAKE) -C $(LINUXDIR) modules_install DEPMOD=/bin/true INSTALL_MOD_PATH=$(TARGETDIR) ; \
        fi
	find $(TARGETDIR) -name "wl_*.o"  | xargs rm -rf
	# Prepare filesystem
	cd $(TARGETDIR) && $(TOP)/misc/rootprep.sh
	# Make sure mkcramfs-2.0 is used
	$(MAKE) -C $(LINUXDIR)/scripts/cramfs mkcramfs
	# '-u 0 -g 0' will set the uid and gid of all the files to 0 (root)
	# These options are currently only available on our version of mkcramfs
	#$(LINUXDIR)/scripts/cramfs/mkcramfs -u 0 -g 0 $(TARGETDIR) $(PLATFORMDIR)/target.cramfs
	# Package kernel and filesystem
#	if grep -q "CONFIG_EMBEDDED_RAMDISK=y" $(LINUXDIR)/.config ; then \
		cp $(PLATFORMDIR)/target.cramfs $(LINUXDIR)/arch/mips/ramdisk/$${CONFIG_EMBEDDED_RAMDISK_IMAGE} ; \
		$(MAKE) -C $(LINUXDIR) zImage ; \
	else \
		cp $(LINUXDIR)/arch/mips/brcm-boards/bcm947xx/compressed/vmlinuz $(PLATFORMDIR)/ ; \
		trx -o $(PLATFORMDIR)/linux.trx $(PLATFORMDIR)/vmlinuz $(PLATFORMDIR)/target.cramfs ; \
	fi
	# Pad self-booting Linux to a 64 KB boundary
	#cp $(LINUXDIR)/arch/mips/brcm-boards/bcm947xx/compressed/zImage $(PLATFORMDIR)/
	#dd conv=sync bs=64k < $(PLATFORMDIR)/zImage > $(PLATFORMDIR)/linux.bin
	# Append filesystem to self-booting Linux
	#cat $(PLATFORMDIR)/target.cramfs >> $(PLATFORMDIR)/linux.bin
	cp $(LINUXDIR)/arch/mips/brcm-boards/bcm947xx/compressed/vmlinuz $(PLATFORMDIR)/

#
# Configuration rules
#

conf mconf:
	$(MAKE) -C config
	@./config/$@ ./config/Config
	# Also configure kernel
	$(MAKE) k$@

oldconf: .config
	$(MAKE) -C config
	@./config/conf -o ./config/Config
	# Also configure kernel
	$(MAKE) k$@

kconf:
	$(MAKE) -C $(LINUXDIR) config

kmconf:
	$(MAKE) -C $(LINUXDIR) menuconfig

koldconf: $(LINUXDIR)/.config
	$(MAKE) -C $(LINUXDIR) oldconfig

# Convenience
config: conf

menuconfig: mconf

oldconfig: oldconf

# Default configurations
.config:
	cp config/defconfig $@
	$(MAKE) oldconfig

$(LINUXDIR)/.config:
	cp $(LINUXDIR)/arch/mips/defconfig-bcm947xx $@

#
# Overrides
#

httpd: shared nvram $(EXTRA_LIBS)

rc: shared nvram netconf $(EXTRA_LIBS)

www www-%:
	$(MAKE) -C www/$(CONFIG_VENDOR) $* INSTALLDIR=$(INSTALLDIR)/www

wwwmt wwwmt-%:
	$(MAKE) -C www/mea $* INSTALLDIR=$(INSTALLDIR)/www

ifeq ($(CONFIG_SENTRY5),y)
include $(TOP)/Makefiles5.mk
endif

bridge:
	$(MAKE) -C bridge brctl/brctl

bridge-install: bridge
	install -D bridge/brctl/brctl $(INSTALLDIR)/bridge/usr/sbin/brctl
	$(STRIP) $(INSTALLDIR)/bridge/usr/sbin/brctl

busybox:
	cd busybox && rm -f Config.h && ln -sf configs/$(CONFIG_BUSYBOX_CONFIG).h Config.h
	$(MAKE) -C busybox STRIPTOOL=$(STRIP)

busybox-install: busybox
	$(MAKE) -C busybox STRIPTOOL=$(STRIP) PREFIX=$(INSTALLDIR)/busybox install

dproxy:
	$(MAKE) -C dproxy

dproxy-install: dproxy
	install -D dproxy/dproxy $(INSTALLDIR)/dproxy/usr/sbin/dproxy
	$(STRIP) $(INSTALLDIR)/dproxy/usr/sbin/dproxy

iptables:
	$(MAKE) -C iptables BINDIR=/usr/sbin LIBDIR=/usr/lib KERNEL_DIR=$(LINUXDIR)

iptables-install: iptables
ifeq ($(CONFIG_IPTABLES),y)
	install -d $(INSTALLDIR)/iptables/usr/lib/iptables
	install iptables/extensions/*.so $(INSTALLDIR)/iptables/usr/lib/iptables
	$(STRIP) $(INSTALLDIR)/iptables/usr/lib/iptables/*.so
	install -D iptables/iptables $(INSTALLDIR)/iptables/usr/sbin/iptables
	install -D iptables/iptables-restore $(INSTALLDIR)/iptables/usr/sbin/iptables-restore
	$(STRIP) $(INSTALLDIR)/iptables/usr/sbin/iptables-restore
else
	# So that generic rule does not take precedence
	@true
endif

iptables-clean:
	-$(MAKE) -C iptables KERNEL_DIR=$(LINUXDIR) clean

netconf: iptables

ntpclient-install: ntpclient
	install -D ntpclient/ntpclient $(INSTALLDIR)/ntpclient/usr/sbin/ntpclient
	$(STRIP) $(INSTALLDIR)/ntpclient/usr/sbin/ntpclient

ppp ppp-%:
	$(MAKE) -C ppp/pppoecd $* INSTALLDIR=$(INSTALLDIR)/ppp

udhcpd-install: udhcpd
	install -D udhcpd/udhcpd $(INSTALLDIR)/udhcpd/usr/sbin/udhcpd
	$(STRIP) $(INSTALLDIR)/udhcpd/usr/sbin/udhcpd
	cd $(INSTALLDIR)/udhcpd/usr/sbin && ln -sf udhcpd udhcpc

upnp: shared nvram netconf

wlconf: shared nvram

vlan:
	$(MAKE) -C vlan CROSS=$(CROSS_COMPILE) STRIPTOOL=$(STRIP)

vlan-install:
	$(MAKE) -C vlan CROSS=$(CROSS_COMPILE) STRIPTOOL=$(STRIP) INSTALLDIR=$(INSTALLDIR) install

vlan-clean:
	$(MAKE) -C vlan clean

pptp-client-install: pptp-client
	install -D pptp-client/pptp $(INSTALLDIR)/pptp-client/usr/sbin/pptp
	$(STRIP) $(INSTALLDIR)/pptp-client/usr/sbin/pptp

pppd-install: pppd pppd/pppd/plugins
	$(MAKE) -C pppd/pppd/plugins
	install -D pppd/pppd/pppd $(INSTALLDIR)/pppd/usr/sbin/pppd
	$(STRIP) $(INSTALLDIR)/pppd/usr/sbin/pppd
#	install -D pppd/chat/chat $(INSTALLDIR)/pppd/usr/sbin/chat
#	$(STRIP) $(INSTALLDIR)/pppd/usr/sbin/chat
	install -D pppd/pppd/plugins/rp-pppoe/rp-pppoe.so $(INSTALLDIR)/pppd/usr/lib/pppd/rp-pppoe.so
	$(STRIP) $(INSTALLDIR)/pppd/usr/lib/pppd/*.so

pppd:
	$(MAKE) -C pppd/pppd

pppd-clean:
	$(MAKE) -C pppd/pppd clean

acl: attr

checkout-%: 
	# Get up-to-date source from CVS
	cvs -d:pserver:$*@192.168.39.9:/WL500g login
	cd $(LINUXDIR) && cvs co 
	cd $(TOP) 
	for dir in $(obj-y) ; do \
	    cvs co $${dir} \
	done

image-%:
ifeq ($(CONFIG_MULTICONFIG),y)
	( if [ "$*" = "WL500gp" ]; then \
                ( if [ -f ./500gp ]; then \
                        echo "the same configuration"; \
                else \
		        cp -f ./config.asus/linux.config.500gp ../linux/linux/.config; \
                        cp -f ./config.asus/router.config.500gp ./.config.asus; \
                        cp -f ./config.asus/busybox.router.h.500gp ./busybox/configs/router.h; \
                        cd ../linux/linux && make dep && cd ../../router; \
                        make menuconfig; \
                        cp -f ./config.asus/router.config.500gp ./.config.asus; \
                        make clean; \
                        make netconf; \
                        make all; \
                        rm -f usb20; \
                        rm -f 500gp; \
                        touch 500gp; \
                fi ) \
	elif [ "$*" = "WL330gE" ]; then \
                ( if [ -f ./330gE ]; then \
                        echo "the same configuration"; \
			rm -rf www/asus/www_WL330gE/; \
			rm -rf mipsel/target/www/; \
                else \
		        cp -f ./config.asus/linux.config.330gE ../linux/linux/.config; \
                        cp -f ./config.asus/router.config.330gE ./.config.asus; \
                        cp -f ./config.asus/busybox.router.h.330gE ./busybox/configs/router.h; \
                        cd ../linux/linux && make dep && cd ../../router; \
                        make menuconfig; \
                        cp -f ./config.asus/router.config.330gE ./.config.asus; \
                        make clean; \
                        make netconf; \
                        make all; \
                        rm -f usb20; \
                        rm -f 330gE; \
                        touch 330gE; \
                fi ) \
	else \
		( if [ -f ./full ]; then \
			echo "the same configuration"; \
		else \
			cp -f ./config.asus/linux.config.full ../linux/linux/.config; \
			cp -f ./config.asus/router.config.full ./.config.asus; \
			cp -f ./config.asus/busybox.router.h.full ./busybox/configs/router.h; \
			cd ../linux/linux && make dep && cd ../../router; \
			make menuconfig; \
			cp -f ./config.asus/router.config.full ./.config.asus; \
			make clean; \
			make netconf; \
			make all; \
			rm -f lite; \
			rm -f usb20; \
			touch full; \
		fi ) \
	fi )
endif
	( if [ "$*" = "WL520g" ]; then \
		rm -rf pppd; \
                rm -rf pptp-client; \
                cp -rf pppd.orig pppd; \
                cp -rf pptp-client.orig pptp-client; \
	else \
                rm -rf pppd; \
                rm -rf pptp-client; \
                cp -rf pppd.mppe pppd; \
                cp -rf pptp-client.mppe pptp-client; \
	fi )

	# clean folders for multiple compile
	make rc-clean
	make infosvr-clean
#	make LPRng-clean
	make vlan-clean
#	make shared-clean
	make httpd-clean

	$(MAKE) -C $(LINUXDIR) zImage ;
	$(MAKE) -C $(LINUXDIR) modules ;

	( if [ "$*" = "WL500gp" ] && [ "$(USB_WAN)" = "CDMA" ]; then \
		make rc GLOBAL_OPTIONS="-DWPA2_WMM -DWOB -DRPPPPOE -DBTN_SETUP -DQOS -DGUEST_ACCOUNT -DURE -DUSB_SUPPORT -DWL500GP -DDHCP_PPTP -DCDMA" CONFIG_BTNSETUP=y; \
		make infosvr GLOBAL_OPTIONS="-DPRNINFO -DNO_PARALLEL"; \
		make LPRng GLOBAL_OPTIONS="-DUSBONLY"; \
		make httpd GLOBAL_OPTIONS="-DTRANSLATE_ON_FLY -DCDMA" CONFIG_NOUSB=y; \
	elif [ "$*" = "WL500gp" ]; then \
		make rc GLOBAL_OPTIONS="-DWPA2_WMM -DWOB -DRPPPPOE -DBTN_SETUP -DURE -DUSB_SUPPORT -DWL500GP -DDHCP_PPTP -DWSC -DWCN -DQOS" CONFIG_BTNSETUP=y; \
		make infosvr GLOBAL_OPTIONS="-DPRNINFO -DNO_PARALLEL"; \
		make LPRng GLOBAL_OPTIONS="-DUSBONLY"; \
		make httpd GLOBAL_OPTIONS="-DTRANSLATE_ON_FLY " CONFIG_NOUSB=y; \
	elif [ "$*" = "WL520gu" ]; then \
		make rc GLOBAL_OPTIONS="-DWPA2_WMM -DWOB -DRPPPPOE -DBTN_SETUP -DURE -DUSB_SUPPORT -DWL500GP -DDHCP_PPTP -DWSC -DWCN -DQOS" CONFIG_BTNSETUP=y; \
		make infosvr GLOBAL_OPTIONS="-DPRNINFO -DNO_PARALLEL"; \
		make LPRng GLOBAL_OPTIONS="-DUSBONLY"; \
		make httpd GLOBAL_OPTIONS="-DTRANSLATE_ON_FLY" CONFIG_NOUSB=y; \
	elif [ "$*" = "WL330gE" ]; then \
                make rc GLOBAL_OPTIONS="-DWPA2_WMM -DWOB -DRPPPPOE -DBTN_SETUP -DGUEST_ACCOUNT -DWIRELESS_WAN -DURE -DRANGE_EXTENDER -DWL500GP -DWL330GE -DDHCP_PPTP" CONFIG_BTNSETUP=y; \
                make infosvr GLOBAL_OPTIONS="-DNO_PARALLEL"; \
                make httpd GLOBAL_OPTIONS="-DSHOWALL -DTRANSLATE_ON_FLY -DGUEST_ACCOUNT -DNOUSB -DNOQOS" CONFIG_NOUSB=y; \
	else \
		echo "no such model name"; \
	fi )
	echo $(LANGUAGE)

	make www-install-$*
 
	rm $(PLATFORMDIR)/target/usr/sbin/waveserver -f

	make install 
	
ifeq ($(CONFIG_BPLAY),y)
	cp others/waveserver $(PLATFORMDIR)/target/usr/sbin/. -f
endif

	rm -f $(PLATFORMDIR)/target/lib/modules/2.4.20/kernel/fs/nls/nls_cp932.o; \
	rm -f $(PLATFORMDIR)/target/lib/modules/2.4.20/kernel/fs/nls/nls_cp936.o; \
	rm -f $(PLATFORMDIR)/target/lib/modules/2.4.20/kernel/fs/nls/nls_cp949.o; \
	rm -f $(PLATFORMDIR)/target/lib/modules/2.4.20/kernel/fs/nls/nls_cp950.o; \

	( if [ "$*" = "WL330gE" ]; then \
		rm $(PLATFORMDIR)/target/lib/modules/2.4.20/parport/* -f; \
		rm $(PLATFORMDIR)/target/usr/sbin/waveserver -f; \
		rm $(PLATFORMDIR)/target/lib/modules/2.4.20/kernel/drivers/media -rf; \
		rm $(PLATFORMDIR)/target/lib/modules/2.4.20/kernel/drivers/usb/serial -rf; \
		rm $(PLATFORMDIR)/target/usr/sbin/pppoecd -f; \
		rm $(PLATFORMDIR)/target/usr/sbin/rcamd -f; \
		rm $(PLATFORMDIR)/target/usr/sbin/et -f; \
		rm $(PLATFORMDIR)/target/usr/sbin/mini_sendmail -f; \
		rm $(PLATFORMDIR)/target/lib/modules/2.4.20/kernel/drivers/usb/hcd/* -f; \
		rm $(PLATFORMDIR)/target/lib/modules/2.4.20/kernel/fs -rf; \
		rm $(PLATFORMDIR)/target/usr/sbin/test -f; \
		rm $(PLATFORMDIR)/target/lib/modules/2.4.20/pcmcia -rf; \
		rm $(PLATFORMDIR)/target/bin/wsc -f; \
	fi )

#	rm $(PLATFORMDIR)/target/usr/sbin/iptables;

	cd $(PLATFORMDIR) && make $* && cd ..

images-%:
	make image-$* LANGUAGE="CN"
	make image-$* LANGUAGE="EN"
#	make image-$* LANGUAGE="KR"
	make image-$* LANGUAGE="TW"
	make image-$* LANGUAGE="DE"

images:
	make images-WL500gx
	make images-WL500g

# Generic rules
#

%:
	[ ! -d $* ] || $(MAKE) -C $*

%-clean:
	[ ! -d $* ] || $(MAKE) -C $* clean

%-install:
	[ ! -d $* ] || $(MAKE) -C $* install INSTALLDIR=$(INSTALLDIR)/$*

$(obj-y) $(obj-n) $(obj-clean) $(obj-install): dummy

.PHONY: all clean distclean mrproper install package
.PHONY: conf mconf oldconf kconf kmconf config menuconfig oldconfig
.PHONY: dummy

