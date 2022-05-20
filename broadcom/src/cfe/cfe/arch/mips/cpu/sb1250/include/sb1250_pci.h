/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
/*  *********************************************************************
    *  SB1250 Board Support Package
    *  
    *  PCI constants				File: sb1250_pci.h
    *  
    *  This module contains constants and macros to describe 
    *  the PCI interface on the SB1250.  
    *  
    *  SB1250 specification level:  User's manual 1/02/02
    *  
    *  Author:  Mitch Lichtenberg (mpl@broadcom.com)
    *  
    *********************************************************************  
    *
    *  Copyright 2000,2001,2002,2003
    *  ASUSTek Inc.. All rights reserved.
    *  
    *  This software is furnished under license and may be used and 
    *  copied only in accordance with the following terms and 
    *  conditions.  Subject to these conditions, you may download, 
    *  copy, install, use, modify and distribute modified or unmodified 
    *  copies of this software in source and/or binary form.  No title 
    *  or ownership is transferred hereby.
    *  
    *  1) Any source code used, modified or distributed must reproduce 
    *     and retain this copyright notice and list of conditions 
    *     as they appear in the source file.
    *  
    *  2) No right is granted to use any trade name, trademark, or 
    *     logo of ASUSTek Inc..  The "Broadcom Corporation" 
    *     name may not be used to endorse or promote products derived 
    *     from this software without the prior written permission of 
    *     ASUSTek Inc..
    *  
    *  3) THIS SOFTWARE IS PROVIDED "AS-IS" AND ANY EXPRESS OR
    *     IMPLIED WARRANTIES, INCLUDING BUT NOT LIMITED TO, ANY IMPLIED
    *     WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
    *     PURPOSE, OR NON-INFRINGEMENT ARE DISCLAIMED. IN NO EVENT 
    *     SHALL BROADCOM BE LIABLE FOR ANY DAMAGES WHATSOEVER, AND IN 
    *     PARTICULAR, BROADCOM SHALL NOT BE LIABLE FOR DIRECT, INDIRECT,
    *     INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
    *     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
    *     GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
    *     BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
    *     OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
    *     TORT (INCLUDING NEGLIGENCE OR OTHERWISE), EVEN IF ADVISED OF 
    *     THE POSSIBILITY OF SUCH DAMAGE.
    ********************************************************************* */


#ifndef _SB1250_PCI_H
#define _SB1250_PCI_H

#include "sb1250_defs.h"

#define K_PCI_VENDOR_SIBYTE	0x166D
#define K_PCI_DEVICE_SB1250	0x0001

/*
 * PCI Interface Type 0 configuration header
 */

#define R_PCI_TYPE0_DEVICEID	0x0000
#define R_PCI_TYPE0_CMDSTATUS	0x0004
#define R_PCI_TYPE0_CLASSREV	0x0008
#define R_PCI_TYPE0_DEVHDR	0x000C
#define R_PCI_TYPE0_BAR0	0x0010	/* translated via mapping table */
#define R_PCI_TYPE0_BAR1	0x0014	/* reserved */
#define R_PCI_TYPE0_BAR2	0x0018	/* mbox 0 */
#define R_PCI_TYPE0_BAR3	0x001C	/* mbox 1 */
#define R_PCI_TYPE0_BAR4	0x0020	/* low memory */
#define R_PCI_TYPE0_BAR5	0x0024	/* high memory */
#define R_PCI_TYPE0_CARDBUSCIS	0x0028
#define R_PCI_TYPE0_SUBSYSID	0x002C
#define R_PCI_TYPE0_ROMBASE	0x0030
#define R_PCI_TYPE0_CAPPTR	0x0034	/* not used */
#define R_PCI_TYPE0_RESERVED1	0x0038
#define R_PCI_TYPE0_INTGRANT	0x003C	/* interrupt pin and grant latency */
#define R_PCI_TYPE0_TIMEOUT	0x0040	/* FControl, Timeout */
#define R_PCI_TYPE0_FCONTROL	0x0040	/* FControl, Timeout */
#define R_PCI_TYPE0_MAPBASE	0x0044	/* 0x44 through 0x80 - map table */
#define PCI_TYPE0_MAPENTRIES	32	/* 64 bytes, 32 entries */
#define R_PCI_TYPE0_ERRORADDR	0x0084
#define R_PCI_TYPE0_ADDSTATUS	0x0088
#define R_PCI_TYPE0_SUBSYSSET	0x008C	/* only accessible from ZBBus */
#if SIBYTE_HDR_FEATURE(1250, PASS2) || SIBYTE_HDR_FEATURE(112x, PASS1)
#define R_PCI_TYPE0_READHOST	0x0094	/* Read Host register */
#define R_PCI_TYPE0_ADXTEND	0x0098	/* Adaptive Extend register */
#endif /* 1250 PASS2 || 112x PASS1 */

/*
 * PCI Device ID register
 */

#define S_PCI_DEVICEID_VENDOR		0
#define M_PCI_DEVICEID_VENDOR		_SB_MAKEMASK_32(16,S_PCI_DEVICEID_VENDOR)
#define V_PCI_DEVICEID_VENDOR(x)	_SB_MAKEVALUE_32(x,S_PCI_DEVICEID_VENDOR)
#define G_PCI_DEVICEID_VENDOR(x)	_SB_GETVALUE_32(x,S_PCI_DEVICEID_VENDOR,M_PCI_DEVICEID_VENDOR)

#define S_PCI_DEVICEID_DEVICEID		16
#define M_PCI_DEVICEID_DEVICEID		_SB_MAKEMASK_32(16,S_PCI_DEVICEID_DEVICEID)
#define V_PCI_DEVICEID_DEVICEID(x)	_SB_MAKEVALUE_32(x,S_PCI_DEVICEID_DEVICEID)
#define G_PCI_DEVICEID_DEVICEID(x)	_SB_GETVALUE_32(x,S_PCI_DEVICEID_DEVICEID,M_PCI_DEVICEID_DEVICEID)


/*
 * PCI Command Register (Table 8-4)
 */

#define M_PCI_CMD_IOSPACE_EN		_SB_MAKEMASK1_32(0)
#define M_PCI_CMD_MEMSPACE_EN		_SB_MAKEMASK1_32(1)
#define M_PCI_CMD_MASTER_EN		_SB_MAKEMASK1_32(2)
#define M_PCI_CMD_SPECCYC_EN		_SB_MAKEMASK1_32(3)
#define M_PCI_CMD_MEMWRINV_EN		_SB_MAKEMASK1_32(4)
#define M_PCI_CMD_VGAPALSNP_EN		_SB_MAKEMASK1_32(5)
#define M_PCI_CMD_PARERRRESP		_SB_MAKEMASK1_32(6)
#define M_PCI_CMD_STEPCTRL		_SB_MAKEMASK1_32(7)
#define M_PCI_CMD_SERR_EN		_SB_MAKEMASK1_32(8)
#define M_PCI_CMD_FASTB2B_EN		_SB_MAKEMASK1_32(9)

/*
 * PCI class and revision registers
 */

#define S_PCI_CLASSREV_REV		0
#define M_PCI_CLASSREV_REV		_SB_MAKEMASK_32(8,S_PCI_CLASSREV_REV)
#define V_PCI_CLASSREV_REV(x)		_SB_MAKEVALUE_32(x,S_PCI_CLASSREV_REV)
#define G_PCI_CLASSREV_REV(x)		_SB_GETVALUE_32(x,S_PCI_CLASSREV_REV,M_PCI_CLASSREV_REV)

#define S_PCI_CLASSREV_CLASS		8
#define M_PCI_CLASSREV_CLASS		_SB_MAKEMASK_32(24,S_PCI_CLASSREV_CLASS)
#define V_PCI_CLASSREV_CLASS(x)		_SB_MAKEVALUE_32(x,S_PCI_CLASSREV_CLASS)
#define G_PCI_CLASSREV_CLASS(x)		_SB_GETVALUE_32(x,S_PCI_CLASSREV_CLASS,M_PCI_CLASSREV_CLASS)

#define K_PCI_REV			0x01
#define K_PCI_CLASS			0x060000

/*
 * Device Header (offset 0x0C)
 */

#define S_PCI_DEVHDR_CLINESZ		0
#define M_PCI_DEVHDR_CLINESZ		_SB_MAKEMASK_32(8,S_PCI_DEVHDR_CLINESZ)
#define V_PCI_DEVHDR_CLINESZ(x)		_SB_MAKEVALUE_32(x,S_PCI_DEVHDR_CLINESZ)
#define G_PCI_DEVHDR_CLINESZ(x)		_SB_GETVALUE_32(x,S_PCI_DEVHDR_CLINESZ,M_PCI_DEVHDR_CLINESZ)

#define S_PCI_DEVHDR_LATTMR		8
#define M_PCI_DEVHDR_LATTMR		_SB_MAKEMASK_32(8,S_PCI_DEVHDR_LATTMR)
#define V_PCI_DEVHDR_LATTMR(x)		_SB_MAKEVALUE_32(x,S_PCI_DEVHDR_LATTMR)
#define G_PCI_DEVHDR_LATTMR(x)		_SB_GETVALUE_32(x,S_PCI_DEVHDR_LATTMR,M_PCI_DEVHDR_LATTMR)

#define S_PCI_DEVHDR_HDRTYPE		16
#define M_PCI_DEVHDR_HDRTYPE		_SB_MAKEMASK_32(8,S_PCI_DEVHDR_HDRTYPE)
#define V_PCI_DEVHDR_HDRTYPE(x)		_SB_MAKEVALUE_32(x,S_PCI_DEVHDR_HDRTYPE)
#define G_PCI_DEVHDR_HDRTYPE(x)		_SB_GETVALUE_32(x,S_PCI_DEVHDR_HDRTYPE,M_PCI_DEVHDR_HDRTYPE)

#define K_PCI_DEVHDR_HDRTYPE_TYPE0	0

#define S_PCI_DEVHDR_BIST		24
#define M_PCI_DEVHDR_BIST		_SB_MAKEMASK_32(8,S_PCI_DEVHDR_BIST)
#define V_PCI_DEVHDR_BIST(x)		_SB_MAKEVALUE_32(x,S_PCI_DEVHDR_BIST)
#define G_PCI_DEVHDR_BIST(x)		_SB_GETVALUE_32(x,S_PCI_DEVHDR_BIST,M_PCI_DEVHDR_BIST)

/*
 * PCI Status Register (Table 8-5).  Note that these constants
 * assume you've read the command and status register 
 * together (32-bit read at offset 0x04)
 */

#define M_PCI_STATUS_CAPLIST		_SB_MAKEMASK1_32(20)
#define M_PCI_STATUS_66MHZCAP		_SB_MAKEMASK1_32(21)
#define M_PCI_STATUS_RESERVED2		_SB_MAKEMASK1_32(22)
#define M_PCI_STATUS_FASTB2BCAP		_SB_MAKEMASK1_32(23)
#define M_PCI_STATUS_MSTRDPARERR	_SB_MAKEMASK1_32(24)

#define S_PCI_STATUS_DEVSELTIMING	25
#define M_PCI_STATUS_DEVSELTIMING	_SB_MAKEMASK_32(2,S_PCI_STATUS_DEVSELTIMING)
#define V_PCI_STATUS_DEVSELTIMING(x)	_SB_MAKEVALUE_32(x,S_PCI_STATUS_DEVSELTIMING)
#define G_PCI_STATUS_DEVSELTIMING(x)	_SB_GETVALUE_32(x,S_PCI_STATUS_DEVSELTIMING,M_PCI_STATUS_DEVSELTIMING)

#define M_PCI_STATUS_SIGDTGTABORT	_SB_MAKEMASK1_32(27)
#define M_PCI_STATUS_RCVDTGTABORT	_SB_MAKEMASK1_32(28)
#define M_PCI_STATUS_RCVDMSTRABORT	_SB_MAKEMASK1_32(29)
#define M_PCI_STATUS_SIGDSERR		_SB_MAKEMASK1_32(30)
#define M_PCI_STATUS_DETPARERR		_SB_MAKEMASK1_32(31)

/*
 * Device Header Register (Table 8-6, Table 8-7)
 */

#define S_PCI_DEVHDR_CLINESZ		0
#define M_PCI_DEVHDR_CLINESZ		_SB_MAKEMASK_32(8,S_PCI_DEVHDR_CLINESZ)
#define V_PCI_DEVHDR_CLINESZ(x)		_SB_MAKEVALUE_32(x,S_PCI_DEVHDR_CLINESZ)
#define G_PCI_DEVHDR_CLINESZ(x)		_SB_GETVALUE_32(x,S_PCI_DEVHDR_CLINESZ,M_PCI_DEVHDR_CLINESZ)

#define S_PCI_DEVHDR_LATTIME		8
#define M_PCI_DEVHDR_LATTIME		_SB_MAKEMASK_32(8,S_PCI_DEVHDR_LATTIME)
#define V_PCI_DEVHDR_LATTIME(x)		_SB_MAKEVALUE_32(x,S_PCI_DEVHDR_LATTIME)
#define G_PCI_DEVHDR_LATTIME(x)		_SB_GETVALUE_32(x,S_PCI_DEVHDR_LATTIME,M_PCI_DEVHDR_LATTIME)

#define S_PCI_DEVHDR_HDRTYPE		16
#define M_PCI_DEVHDR_HDRTYPE		_SB_MAKEMASK_32(8,S_PCI_DEVHDR_HDRTYPE)
#define V_PCI_DEVHDR_HDRTYPE(x)		_SB_MAKEVALUE_32(x,S_PCI_DEVHDR_HDRTYPE)
#define G_PCI_DEVHDR_HDRTYPE(x)		_SB_GETVALUE_32(x,S_PCI_DEVHDR_HDRTYPE,M_PCI_DEVHDR_HDRTYPE)

#define S_PCI_DEVHDR_BIST		24
#define M_PCI_DEVHDR_BIST		_SB_MAKEMASK_32(8,S_PCI_DEVHDR_BIST)
#define V_PCI_DEVHDR_BIST(x)		_SB_MAKEVALUE_32(x,S_PCI_DEVHDR_BIST)
#define G_PCI_DEVHDR_BIST(x)		_SB_GETVALUE_32(x,S_PCI_DEVHDR_BIST,M_PCI_DEVHDR_BIST)

/*
 * Timeout and feature control Register (Table 8-8) (Table 8-9)
 * Note that these constants assume you've read the timeout/fcontrol register
 * together (32-bit read at offset 0x40)
 */

#define S_PCI_TIMEOUT_TRDY		0
#define M_PCI_TIMEOUT_TRDY		_SB_MAKEMASK_32(8,S_PCI_TIMEOUT_TRDY)
#define V_PCI_TIMEOUT_TRDY(x)		_SB_MAKEVALUE_32(x,S_PCI_TIMEOUT_TRDY)
#define G_PCI_TIMEOUT_TRDY(x)		_SB_GETVALUE_32(x,S_PCI_TIMEOUT_TRDY,M_PCI_TIMEOUT_TRDY)

#define S_PCI_TIMEOUT_RETRY		8
#define M_PCI_TIMEOUT_RETRY		_SB_MAKEMASK_32(8,S_PCI_TIMEOUT_RETRY)
#define V_PCI_TIMEOUT_RETRY(x)		_SB_MAKEVALUE_32(x,S_PCI_TIMEOUT_RETRY)
#define G_PCI_TIMEOUT_RETRY(x)		_SB_GETVALUE_32(x,S_PCI_TIMEOUT_RETRY,M_PCI_TIMEOUT_RETRY)

#define M_PCI_FCONTROL_BAR4_EN		_SB_MAKEMASK1_32(16)
#define M_PCI_FCONTROL_BAR5_EN		_SB_MAKEMASK1_32(17)
#define M_PCI_FCONTROL_PTP_EN		_SB_MAKEMASK1_32(18)
#define M_PCI_FCONTROL_ADAPT_RETRY_EN	_SB_MAKEMASK1_32(19)

#define S_PCI_FCONTROL_MIN_TAR_RETRY	20
#define M_PCI_FCONTROL_MIN_TAR_RETRY	_SB_MAKEMASK_32(3,S_PCI_FCONTROL_MIN_TAR_RETRY)
#define V_PCI_FCONTROL_MIN_TAR_RETRY(x)	_SB_MAKEVALUE_32(x,S_PCI_FCONTROL_MIN_TAR_RETRY)
#define G_PCI_FCONTROL_MIN_TAR_RETRY(x)	_SB_GETVALUE_32(x,S_PCI_FCONTROL_MIN_TAR_RETRY,M_PCI_FCONTROL_MIN_TAR_RETRY)

#define S_PCI_FCONTROL_NOM_TAR_RETRY	23
#define M_PCI_FCONTROL_NOM_TAR_RETRY	_SB_MAKEMASK_32(4,S_PCI_FCONTROL_NOM_TAR_RETRY)
#define V_PCI_FCONTROL_NOM_TAR_RETRY(x)	_SB_MAKEVALUE_32(x,S_PCI_FCONTROL_NOM_TAR_RETRY)
#define G_PCI_FCONTROL_NOM_TAR_RETRY(x)	_SB_GETVALUE_32(x,S_PCI_FCONTROL_NOM_TAR_RETRY,M_PCI_FCONTROL_NOM_TAR_RETRY)

#define S_PCI_FCONTROL_MAX_TAR_RETRY	27
#define M_PCI_FCONTROL_MAX_TAR_RETRY	_SB_MAKEMASK_32(5,S_PCI_FCONTROL_MAX_TAR_RETRY)
#define V_PCI_FCONTROL_MAX_TAR_RETRY(x)	_SB_MAKEVALUE_32(x,S_PCI_FCONTROL_MAX_TAR_RETRY)
#define G_PCI_FCONTROL_MAX_TAR_RETRY(x)	_SB_GETVALUE_32(x,S_PCI_FCONTROL_MAX_TAR_RETRY,M_PCI_FCONTROL_MAX_TAR_RETRY)

/*
 * BAR0 Map Table Entry (Offsets 0x40-0x80) (Table 8-10)
 */

#define M_PCI_BAR0MAP_ENABLE		_SB_MAKEMASK1_32(0)
#define M_PCI_BAR0MAP_SENDLDT		_SB_MAKEMASK1_32(1)
#define S_PCI_BAR0MAP_ADDR		12
#define M_PCI_BAR0MAP_ADDR		_SB_MAKEMASK_32(20,S_PCI_BAR0MAP_ADDR)

/*
 * Additional Status Register (Table 8-11)
 */

#define M_PCI_ASTATUS_HOTPLUG_EN	_SB_MAKEMASK1_32(0)
#define M_PCI_ASTATUS_SERR_DET		_SB_MAKEMASK1_32(1)
#define M_PCI_ASTATUS_TRDYERR		_SB_MAKEMASK1_32(2)
#define M_PCI_ASTATUS_RETRTYERR		_SB_MAKEMASK1_32(3)
#define M_PCI_ASTATUS_TRDYINTMASK	_SB_MAKEMASK1_32(4)
#define M_PCI_ASTATUS_RETRYINTMASK	_SB_MAKEMASK1_32(5)
#define M_PCI_ASTATUS_SIGNALINTA	_SB_MAKEMASK1_32(6)

#if SIBYTE_HDR_FEATURE(1250, PASS2) || SIBYTE_HDR_FEATURE(112x, PASS1)
/*
 * Read Host Register
 */

#define M_PCI_READHOST_RDHOST	_SB_MAKEMASK1_32(0)

/*
 * Adaptive Extend Register
 */

#define S_PCI_ADXTEND_NOM_TAR_RETRY	1
#define M_PCI_ADXTEND_NOM_TAR_RETRY	_SB_MAKEMASK_32(3,S_PCI_ADXTEND_NOM_TAR_RETRY)
#define V_PCI_ADXTEND_NOM_TAR_RETRY(x)	_SB_MAKEVALUE_32(x,S_PCI_ADXTEND_NOM_TAR_RETRY)
#define G_PCI_ADXTEND_NOM_TAR_RETRY(x)	_SB_GETVALUE_32(x,S_PCI_ADXTEND_NOM_TAR_RETRY,M_PCI_ADXTEND_NOM_TAR_RETRY)

#define S_PCI_ADXTEND_MAX_TAR_RETRY	4
#define M_PCI_ADXTEND_MAX_TAR_RETRY	_SB_MAKEMASK_32(2,S_PCI_ADXTEND_MAX_TAR_RETRY)
#define V_PCI_ADXTEND_MAX_TAR_RETRY(x)	_SB_MAKEVALUE_32(x,S_PCI_ADXTEND_MAX_TAR_RETRY)
#define G_PCI_ADXTEND_MAX_TAR_RETRY(x)	_SB_GETVALUE_32(x,S_PCI_ADXTEND_MAX_TAR_RETRY,M_PCI_ADXTEND_MAX_TAR_RETRY)

#define M_PCI_ADXTEND_DIS_DMAR_IOW_DEP  _SB_MAKEMASK1_32(6)
#define M_PCI_ADXTEND_DIS_MEMRD_BE      _SB_MAKEMASK1_32(6)
#endif /* 1250 PASS2 || 112x PASS1 */


#endif


