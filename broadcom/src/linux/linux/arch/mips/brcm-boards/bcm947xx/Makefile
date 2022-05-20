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
# $Id: Makefile,v 1.1.1.1 2007/01/25 12:51:49 jiahao_jhou Exp $
#

O_TARGET	:= bcm947xx.o

export-objs	:= nvram_linux.o setup.o
obj-y		:= prom.o setup.o time.o irq.o
obj-y		+= hndmips.o hndchipc.o perfcntr.o linux_gpio.o
obj-y		+= sflash.o nvram.o nvram_linux.o
obj-$(CONFIG_PCI) += hndpci.o pcibios.o

vpath %.c $(SRCBASE)/shared $(SRCBASE)/shared/nvram

include $(TOPDIR)/Rules.make
