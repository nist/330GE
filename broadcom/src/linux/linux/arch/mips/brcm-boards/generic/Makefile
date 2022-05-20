#
# Makefile for generic Broadcom MIPS boards
#
# Copyright 2006, ASUSTek Inc.
# All Rights Reserved.
# 
# THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
# KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
# SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
#
# $Id: Makefile,v 1.1.1.1 2007/01/25 12:51:50 jiahao_jhou Exp $
#

.S.s:
	$(CPP) $(AFLAGS) $< -o $*.s
.S.o:
	$(CC) $(AFLAGS) -c $< -o $*.o

O_TARGET	:= brcm.o

obj-y		:= int-handler.o
ifneq ($(strip $(CONFIG_BCM4704)),y)
obj-y		+= irq.o
endif

obj-$(CONFIG_REMOTE_DEBUG)	+= gdb_hook.o

include $(TOPDIR)/Rules.make
