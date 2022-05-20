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
/*
 * HND SiliconBackplane Gigabit Ethernet core registers
 *
 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: sbgige.h,v 1.1.1.1 2007/01/25 12:52:12 jiahao_jhou Exp $
 */

#ifndef	_sbgige_h_
#define	_sbgige_h_

#include <typedefs.h>
#include <sbconfig.h>
#include <pcicfg.h>

/* cpp contortions to concatenate w/arg prescan */
#ifndef PAD
#define	_PADLINE(line)	pad ## line
#define	_XSTR(line)	_PADLINE(line)
#define	PAD		_XSTR(__LINE__)
#endif	/* PAD */

/* PCI to OCP shim registers */
typedef volatile struct {
	uint32 FlushStatusControl;
	uint32 FlushReadAddr;
	uint32 FlushTimeoutCntr;
	uint32 BarrierReg;
	uint32 MaocpSIControl;
	uint32 SiocpMaControl;
	uint8 PAD[0x02E8];
} sbgige_pcishim_t;

/* SB core registers */
typedef volatile struct {
	/* PCI I/O Read/Write registers */
	uint8 pciio[0x0400];

	/* Reserved */
	uint8 reserved[0x0400];

	/* PCI configuration registers */
	pci_config_regs pcicfg;
	uint8 PAD[0x0300];

	/* PCI to OCP shim registers */
	sbgige_pcishim_t pcishim;

	/* Sonics SiliconBackplane registers */
	sbconfig_t sbconfig;
} sbgige_t;

#endif	/* _sbgige_h_ */
