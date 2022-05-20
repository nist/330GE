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
/*****************************************************************************
* sdladrv.h	SDLA Support Module.  Kernel API Definitions.
*
* Author: 	Gideon Hack	
*
* Copyright:	(c) 1995-2000 Sangoma Technologies Inc.
*
*		This program is free software; you can redistribute it and/or
*		modify it under the terms of the GNU General Public License
*		as published by the Free Software Foundation; either version
*		2 of the License, or (at your option) any later version.
* ============================================================================
* Jun 02, 1999 	Gideon Hack	Added support for the S514 PCI adapter.
* Dec 11, 1996	Gene Kozin	Complete overhaul.
* Oct 17, 1996	Gene Kozin	Minor bug fixes.
* Jun 12, 1996	Gene Kozin 	Added support for S503 card.
* Dec 06, 1995	Gene Kozin	Initial version.
*****************************************************************************/
#ifndef	_SDLADRV_H
#define	_SDLADRV_H

#include <linux/version.h>

#ifndef KERNEL_VERSION
  #define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,3,0)
#define LINUX_2_4	
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2,1,0)
#define LINUX_2_1
#else
#define LINUX_2_0
#endif

#define	SDLA_MAXIORANGE	4	/* maximum I/O port range */
#define	SDLA_WINDOWSIZE	0x2000	/* default dual-port memory window size */
/****** Data Structures *****************************************************/

/*----------------------------------------------------------------------------
 * Adapter hardware configuration. Pointer to this structure is passed to all
 * APIs.
 */
typedef struct sdlahw
{
	unsigned type;			/* adapter type */
	unsigned fwid;			/* firmware ID */
	unsigned port;			/* adapter I/O port base */
	int irq;			/* interrupt request level */
	char S514_cpu_no[1];		/* PCI CPU Number */
	unsigned char S514_slot_no;	/* PCI Slot Number */
	char auto_pci_cfg;		/* Autodetect PCI Slot */
#if defined(LINUX_2_1) || defined(LINUX_2_4)
	struct pci_dev *pci_dev;	/* PCI device */
#else
	unsigned char pci_bus;		/* PCI bus number */
	unsigned char pci_dev_func;	/* PCI device/function number */
#endif
	void * dpmbase;			/* dual-port memory base */
	unsigned dpmsize;		/* dual-port memory size */
	unsigned pclk;			/* CPU clock rate, kHz */
	unsigned long memory;		/* memory size */
	unsigned long vector;		/* local offset of the DPM window */
	unsigned io_range;		/* I/O port range */
	unsigned char regs[SDLA_MAXIORANGE]; /* was written to registers */
	unsigned reserved[5];
} sdlahw_t;

/****** Function Prototypes *************************************************/

extern int sdla_setup	(sdlahw_t* hw, void* sfm, unsigned len);
extern int sdla_down	(sdlahw_t* hw);
extern int sdla_inten	(sdlahw_t* hw);
extern int sdla_intde	(sdlahw_t* hw);
extern int sdla_intack	(sdlahw_t* hw);
extern void S514_intack  (sdlahw_t* hw, u32 int_status);
extern void read_S514_int_stat (sdlahw_t* hw, u32* int_status);
extern int sdla_intr	(sdlahw_t* hw);
extern int sdla_mapmem	(sdlahw_t* hw, unsigned long addr);
extern int sdla_peek	(sdlahw_t* hw, unsigned long addr, void* buf,
			 unsigned len);
extern int sdla_poke	(sdlahw_t* hw, unsigned long addr, void* buf,
			 unsigned len);
extern int sdla_exec	(void* opflag);

extern unsigned wanpipe_hw_probe(void);

#endif	/* _SDLADRV_H */