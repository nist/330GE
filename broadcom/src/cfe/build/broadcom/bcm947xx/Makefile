#
# Makefile for Broadcom BCM947XX boards
#
# Copyright 2006, ASUSTek Inc.
# All Rights Reserved.
# 
# THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
# KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
# SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
#
# $Id$
#

# Standard options
CFG_MLONG64 := 0
CFG_LITTLE ?= 1
CFG_RELOC := 0
CFG_UNCACHED := 0
CFG_VAPI := 0 
CFG_BOARDNAME := "BCM947XX"
CFG_PCI ?= 0
CFG_ZLIB := 1
CFG_MINIMAL_SIZE := 1
CFG_CFLASH := 1
CFG_XIP := 0
CFG_ATE := 0

# BCM947XX options
CFG_EMBEDDED_NVRAM := 1
CFG_FLASH := 1   # if CFG_XIP is set then CFG_FLASH must be set to 0
CFG_SFLASH := 1
CFG_ET ?= 1
CFG_WL := 0
CFG_WLU := 0
CFG_SIM := 0	# build for Quickturn/Simulator
CFG_SIM_CONSOLE := 0	# with console
ifeq ($(strip $(CFG_PCI)),1)
CFG_BCM57XX := 1
endif
CFG_BUFLOG := 0

#ASUStek's applications (unlock the file size of cfez)
CFE_ASUS := 1


# HND source base
SRCBASE := $(shell cd ../../../.. && pwd)
TOP := $(SRCBASE)/cfe/cfe

ifeq ($(strip $(CFG_RELOC)),0)

# Link binary in RAM
CFG_BOOTRAM := 1
ifeq ($(strip $(CFG_XIP)),0)
CFG_TEXT_START := 0x80800000
else
CFG_DATA_START := 0x80001000
endif

# To generating ATE traces
ifeq ($(strip $(CFG_ATE)),1)
CFLAGS += -DBCM_ATE
endif

# Toolchain prefix 
ifeq ($(CFG_LITTLE), 1)
TOOLPREFIX := mipsel-linux-
else
TOOLPREFIX := mips-wrs-vxworks-
endif

#PATH := $(PATH):/projects/hnd/tools/linux/bin:/projects/hnd/tools/linux/hndtools-mips-wrs-vxworks-3.2.3/bin

CFLAGS += -Ulinux -pipe

# Disable 4710A0 compiler workarounds if any
check_gcc = $(shell if $(GCC) $(1) -c -o /dev/null -xc /dev/null > /dev/null 2>&1; then echo "$(1)"; else echo "$(2)";  fi)
CFLAGS += $(call check_gcc, -m4710a0kern, )

# Disable PIC
CFLAGS += -G 0 -mno-abicalls -fno-pic

# Discard unused sections if supported
CFLAGS += -ffunction-sections $(call check_gcc, -fvtable-gc, )
LDFLAGS += -static --gc-sections

else

# Locate data in RAM
CFG_BOOTRAM := 0
CFG_DATA_START := 0x80001000

endif

ARCH := mips
CPU := bcmcore
BOARD := bcm947xx


# define WL_CONFFILE for arch/mips/board/bcm947xx/src/Makefile
ifeq ($(strip $(CFG_WL)),1)
WLCONFFILE ?= wlconfig_cfe_apsta
WLTUNEFILE ?= wltunable_cfe.h
WLCFGDIR   := $(SRCBASE)/wl/config
UPDATESH   := $(WLCFGDIR)/diffupdate.sh
endif

include $(TOP)/main/cfe.mk

#
# Main target for normal builds
#
ifneq ($(strip $(CFG_WL)),1)
ALL: cfe.bin
	$(MAKE)  -C compressed
#	nvserial -i compressed/cfez.bin -o WL500GU_boot_1.6_32.bin -s 45 WL-500GU_boot_1.0_32.txt
#	nvserial -i compressed/cfez.bin -o WL520GU_boot_1.6_16.bin -s 45 WL-520GU_boot_1.0_16.txt
#	nvserial -i compressed/cfez.bin -o WL520GC_boot_1.6_8.bin -s 45 WL-520GC_boot_1.0_8.txt
	nvserial -i compressed/cfez.bin -o WL330GE_boot_1.6_8.bin -s 45 WL-330GE_boot_1.0_8.txt
#	nvserial -i cfe.bin -o tmp_WL500GU_boot_tmp_32.bin -s 45 WL-500GU_boot_1.0_32.txt
#	nvserial -i cfe.bin -o tmp_WL520GU_boot_tmp_16.bin -s 45 WL-520GU_boot_1.0_16.txt
#	nvserial -i cfe.bin -o tmp_WL520GC_boot_tmp_8.bin -s 45 WL-520GC_boot_1.0_8.txt
#       nvserial -i cfe.bin -o WL800G_boot_1.0_8.bin -s 45 WL-800GC_boot_1.0_8.txt
#       nvserial -i compressed/cfez.bin -o WL520GC_boot_1.2z_16.bin -s 45 WL-520GC_boot_1.0_8.txt


else # CFG_WL = 1

CLEANOBJS += wlconf.h

ALL: wlconf.h cfe.bin
	$(MAKE) -C compressed

wlconf.h: $(WLCFGDIR)/$(WLTUNEFILE) FORCE
	[ ! -f $@ ] || chmod +w $@
	@echo "check and update config file"
	cp $< wltemp
	$(UPDATESH) wltemp $@

FORCE:

endif

cfe1M.bin:	cfe.bin
	dd conv=sync if=cfe.bin of=cfe1M.bin bs=1M

CLEANOBJS += cfe1M.bin

CLEAN:
	$(MAKE) -C compressed clean

clean: CLEAN


.EXPORT_ALL_VARIABLES:

#
# Linker script
#

include $(TOP)/main/cfe_link.mk
