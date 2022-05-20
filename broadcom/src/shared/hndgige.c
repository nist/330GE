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
 * HND SiliconBackplane Gigabit Ethernet core software interface
 *
 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: hndgige.c,v 1.1.1.1 2007/01/25 12:52:12 jiahao_jhou Exp $
 */

#include <typedefs.h>
#include <osl.h>
#include <pcicfg.h>
#include <sbconfig.h>
#include <bcmutils.h>
#include <sbutils.h>
#include <sbgige.h>
#include <hndpci.h>
#include <hndgige.h>

/*
 * Setup the gige core.
 * Resetting the core will lose all settings.
 */
void
sb_gige_init(sb_t *sbh, uint32 unit, bool *rgmii)
{
	volatile pci_config_regs *pci;
	sbgige_pcishim_t *ocp;
	sbconfig_t *sb;
	osl_t *osh;
	uint32 statelow;
	uint32 statehigh;
	uint32 base;
	uint32 idx;
	void *regs;

	/* Sanity checks */
	ASSERT(sbh);
	ASSERT(rgmii);

	idx = sb_coreidx(sbh);

	/* point to the gige core registers */
	regs = sb_setcore(sbh, SB_GIGETH, unit);
	ASSERT(regs);

	osh = sb_osh(sbh);

	pci = &((sbgige_t *)regs)->pcicfg;
	ocp = &((sbgige_t *)regs)->pcishim;
	sb = &((sbgige_t *)regs)->sbconfig;

	/* Enable the core clock and memory access */
	if (!sb_iscoreup(sbh))
		sb_core_reset(sbh, 0, 0);

	/*
	 * Setup the 64K memory-mapped region base address through BAR0.
	 * Leave the other BAR values alone.
	 */
	base = sb_base(R_REG(osh, &sb->sbadmatch1));
	W_REG(osh, &pci->base[0], base);
	W_REG(osh, &pci->base[1], 0);

	/*
	 * Enable the PCI memory access anyway. Any PCI config commands
	 * issued before the core is enabled will go to the emulation
	 * only and will not go to the real PCI config registers.
	 */
	OR_REG(osh, &pci->command, 2);

	/*
	 * Enable the posted write flush scheme as follows:
	 *
	 * - Enable flush on any core register read
	 * - Enable timeout on the flush
	 * - Disable the interrupt mask when flushing
	 *
	 * This differs from the default setting only in that interrupts are
	 * not masked.  Since posted writes are not flushed on interrupt, the
	 * driver must explicitly request a flush in its interrupt handling
	 * by reading a core register.
	 */
	W_REG(osh, &ocp->FlushStatusControl, 0x68);

	/*
	 * Determine whether the GbE is in GMII or RGMII mode.  This is
	 * indicated in bit 16 of the SBTMStateHigh register, which is
	 * part of the core-specific flags field.
	 *
	 * For GMII, bypass the Rx/Tx DLLs, i.e. add no delay to RXC/GTXC
	 * within the core.  For RGMII, do not bypass the DLLs, resulting
	 * in added delay for RXC/GTXC.  The SBTMStateLow register contains
	 * the controls for doing this in the core-specific flags field:
	 *
	 *   bit 24 - Enable DLL controls
	 *   bit 20 - Bypass Rx DLL
	 *   bit 19 - Bypass Tx DLL
	 */
	statelow = R_REG(osh, &sb->sbtmstatelow);	/* DLL controls */
	statehigh = R_REG(osh, &sb->sbtmstatehigh);	/* GMII/RGMII mode */
	if ((statehigh & (1 << 16)) != 0)	/* RGMII */
	{
		statelow &= ~(1 << 20);		/* no Rx bypass (delay) */
		statelow &= ~(1 << 19);		/* no Tx bypass (delay) */
		*rgmii = TRUE;
	}
	else					/* GMII */
	{
		statelow |= (1 << 20);		/* Rx bypass (no delay) */
		statelow |= (1 << 19);		/* Tx bypass (no delay) */
		*rgmii = FALSE;
	}
	statelow |= (1 << 24);			/* enable DLL controls */
	W_REG(osh, &sb->sbtmstatelow, statelow);

	sb_setcoreidx(sbh, idx);
}
