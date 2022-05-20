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
 * mcs01_pci.h
 *
 * Carsten Langgaard, carstenl@mips.com
 * Copyright (C) 2002 MIPS Technologies, Inc.  All rights reserved.
 *
 * ########################################################################
 *
 * PCI Register definitions for the MIPS System Controller.
 */
#ifndef MSC01_PCI_H
#define MSC01_PCI_H

/*****************************************************************************
 * Register offset addresses
 ****************************************************************************/

#define MSC01_PCI_ID_OFS		0x0000
#define MSC01_PCI_SC2PMBASL_OFS		0x0208
#define MSC01_PCI_SC2PMMSKL_OFS		0x0218
#define MSC01_PCI_SC2PMMAPL_OFS		0x0228
#define MSC01_PCI_SC2PIOBASL_OFS	0x0248
#define MSC01_PCI_SC2PIOMSKL_OFS	0x0258
#define MSC01_PCI_SC2PIOMAPL_OFS	0x0268
#define MSC01_PCI_P2SCMSKL_OFS		0x0308
#define MSC01_PCI_P2SCMAPL_OFS		0x0318
#define MSC01_PCI_INTCFG_OFS		0x0600
#define MSC01_PCI_INTSTAT_OFS		0x0608
#define MSC01_PCI_CFGADDR_OFS		0x0610
#define MSC01_PCI_CFGDATA_OFS		0x0618
#define MSC01_PCI_IACK_OFS		0x0620
#define MSC01_PCI_HEAD0_OFS		0x2000  /* DevID, VendorID */
#define MSC01_PCI_HEAD1_OFS		0x2008  /* Status, Command */
#define MSC01_PCI_HEAD2_OFS		0x2010  /* Class code, RevID */
#define MSC01_PCI_HEAD3_OFS		0x2018  /* bist, header, latency */
#define MSC01_PCI_HEAD4_OFS		0x2020  /* BAR 0 */
#define MSC01_PCI_HEAD5_OFS		0x2028  /* BAR 1 */
#define MSC01_PCI_HEAD6_OFS		0x2030  /* BAR 2 */
#define MSC01_PCI_HEAD7_OFS		0x2038  /* BAR 3 */
#define MSC01_PCI_HEAD8_OFS		0x2040  /* BAR 4 */
#define MSC01_PCI_HEAD9_OFS		0x2048  /* BAR 5 */
#define MSC01_PCI_HEAD10_OFS		0x2050  /* CardBus CIS Ptr */
#define MSC01_PCI_HEAD11_OFS		0x2058  /* SubSystem ID, -VendorID */
#define MSC01_PCI_HEAD12_OFS		0x2060  /* ROM BAR */
#define MSC01_PCI_HEAD13_OFS		0x2068  /* Capabilities ptr */
#define MSC01_PCI_HEAD14_OFS		0x2070  /* reserved */
#define MSC01_PCI_HEAD15_OFS		0x2078  /* Maxl, ming, intpin, int */
#define MSC01_PCI_BAR0_OFS		0x2220
#define MSC01_PCI_CFG_OFS		0x2380
#define MSC01_PCI_SWAP_OFS		0x2388


/*****************************************************************************
 * Register encodings
 ****************************************************************************/

#define MSC01_PCI_ID_ID_SHF		16
#define MSC01_PCI_ID_ID_MSK		0x00ff0000
#define MSC01_PCI_ID_ID_HOSTBRIDGE	82
#define MSC01_PCI_ID_MAR_SHF		8
#define MSC01_PCI_ID_MAR_MSK		0x0000ff00
#define MSC01_PCI_ID_MIR_SHF		0
#define MSC01_PCI_ID_MIR_MSK		0x000000ff

#define MSC01_PCI_SC2PMBASL_BAS_SHF	24
#define MSC01_PCI_SC2PMBASL_BAS_MSK	0xff000000

#define MSC01_PCI_SC2PMMSKL_MSK_SHF	24
#define MSC01_PCI_SC2PMMSKL_MSK_MSK	0xff000000

#define MSC01_PCI_SC2PMMAPL_MAP_SHF	24
#define MSC01_PCI_SC2PMMAPL_MAP_MSK	0xff000000

#define MSC01_PCI_SC2PIOBASL_BAS_SHF	24
#define MSC01_PCI_SC2PIOBASL_BAS_MSK	0xff000000

#define MSC01_PCI_SC2PIOMSKL_MSK_SHF	24
#define MSC01_PCI_SC2PIOMSKL_MSK_MSK	0xff000000

#define MSC01_PCI_SC2PIOMAPL_MAP_SHF	24
#define MSC01_PCI_SC2PIOMAPL_MAP_MSK	0xff000000

#define MSC01_PCI_P2SCMSKL_MSK_SHF	24
#define MSC01_PCI_P2SCMSKL_MSK_MSK	0xff000000

#define MSC01_PCI_P2SCMAPL_MAP_SHF	24
#define MSC01_PCI_P2SCMAPL_MAP_MSK	0xff000000

#define MSC01_PCI_INTCFG_RST_SHF        10
#define MSC01_PCI_INTCFG_RST_MSK        0x00000400
#define MSC01_PCI_INTCFG_RST_BIT        0x00000400
#define MSC01_PCI_INTCFG_MWE_SHF        9
#define MSC01_PCI_INTCFG_MWE_MSK        0x00000200
#define MSC01_PCI_INTCFG_MWE_BIT        0x00000200
#define MSC01_PCI_INTCFG_DTO_SHF        8
#define MSC01_PCI_INTCFG_DTO_MSK        0x00000100
#define MSC01_PCI_INTCFG_DTO_BIT        0x00000100
#define MSC01_PCI_INTCFG_MA_SHF         7
#define MSC01_PCI_INTCFG_MA_MSK         0x00000080
#define MSC01_PCI_INTCFG_MA_BIT         0x00000080
#define MSC01_PCI_INTCFG_TA_SHF         6
#define MSC01_PCI_INTCFG_TA_MSK         0x00000040
#define MSC01_PCI_INTCFG_TA_BIT         0x00000040
#define MSC01_PCI_INTCFG_RTY_SHF        5
#define MSC01_PCI_INTCFG_RTY_MSK        0x00000020
#define MSC01_PCI_INTCFG_RTY_BIT        0x00000020
#define MSC01_PCI_INTCFG_MWP_SHF        4
#define MSC01_PCI_INTCFG_MWP_MSK        0x00000010
#define MSC01_PCI_INTCFG_MWP_BIT        0x00000010
#define MSC01_PCI_INTCFG_MRP_SHF        3
#define MSC01_PCI_INTCFG_MRP_MSK        0x00000008
#define MSC01_PCI_INTCFG_MRP_BIT        0x00000008
#define MSC01_PCI_INTCFG_SWP_SHF        2
#define MSC01_PCI_INTCFG_SWP_MSK        0x00000004
#define MSC01_PCI_INTCFG_SWP_BIT        0x00000004
#define MSC01_PCI_INTCFG_SRP_SHF        1
#define MSC01_PCI_INTCFG_SRP_MSK        0x00000002
#define MSC01_PCI_INTCFG_SRP_BIT        0x00000002
#define MSC01_PCI_INTCFG_SE_SHF         0
#define MSC01_PCI_INTCFG_SE_MSK         0x00000001
#define MSC01_PCI_INTCFG_SE_BIT         0x00000001

#define MSC01_PCI_INTSTAT_RST_SHF       10
#define MSC01_PCI_INTSTAT_RST_MSK       0x00000400
#define MSC01_PCI_INTSTAT_RST_BIT       0x00000400
#define MSC01_PCI_INTSTAT_MWE_SHF       9
#define MSC01_PCI_INTSTAT_MWE_MSK       0x00000200
#define MSC01_PCI_INTSTAT_MWE_BIT       0x00000200
#define MSC01_PCI_INTSTAT_DTO_SHF       8
#define MSC01_PCI_INTSTAT_DTO_MSK       0x00000100
#define MSC01_PCI_INTSTAT_DTO_BIT       0x00000100
#define MSC01_PCI_INTSTAT_MA_SHF        7
#define MSC01_PCI_INTSTAT_MA_MSK        0x00000080
#define MSC01_PCI_INTSTAT_MA_BIT        0x00000080
#define MSC01_PCI_INTSTAT_TA_SHF        6
#define MSC01_PCI_INTSTAT_TA_MSK        0x00000040
#define MSC01_PCI_INTSTAT_TA_BIT        0x00000040
#define MSC01_PCI_INTSTAT_RTY_SHF       5
#define MSC01_PCI_INTSTAT_RTY_MSK       0x00000020
#define MSC01_PCI_INTSTAT_RTY_BIT       0x00000020
#define MSC01_PCI_INTSTAT_MWP_SHF       4
#define MSC01_PCI_INTSTAT_MWP_MSK       0x00000010
#define MSC01_PCI_INTSTAT_MWP_BIT       0x00000010
#define MSC01_PCI_INTSTAT_MRP_SHF       3
#define MSC01_PCI_INTSTAT_MRP_MSK       0x00000008
#define MSC01_PCI_INTSTAT_MRP_BIT       0x00000008
#define MSC01_PCI_INTSTAT_SWP_SHF       2
#define MSC01_PCI_INTSTAT_SWP_MSK       0x00000004
#define MSC01_PCI_INTSTAT_SWP_BIT       0x00000004
#define MSC01_PCI_INTSTAT_SRP_SHF       1
#define MSC01_PCI_INTSTAT_SRP_MSK       0x00000002
#define MSC01_PCI_INTSTAT_SRP_BIT       0x00000002
#define MSC01_PCI_INTSTAT_SE_SHF        0
#define MSC01_PCI_INTSTAT_SE_MSK        0x00000001
#define MSC01_PCI_INTSTAT_SE_BIT        0x00000001

#define MSC01_PCI_CFGADDR_BNUM_SHF	16
#define MSC01_PCI_CFGADDR_BNUM_MSK	0x00ff0000
#define MSC01_PCI_CFGADDR_DNUM_SHF	11
#define MSC01_PCI_CFGADDR_DNUM_MSK	0x0000f800
#define MSC01_PCI_CFGADDR_FNUM_SHF	8
#define MSC01_PCI_CFGADDR_FNUM_MSK	0x00000700
#define MSC01_PCI_CFGADDR_RNUM_SHF	2
#define MSC01_PCI_CFGADDR_RNUM_MSK	0x000000fc

#define MSC01_PCI_CFGDATA_DATA_SHF	0
#define MSC01_PCI_CFGDATA_DATA_MSK	0xffffffff

/* The defines below are ONLY valid for a MEM bar! */
#define MSC01_PCI_BAR0_SIZE_SHF	        4
#define MSC01_PCI_BAR0_SIZE_MSK	        0xfffffff0
#define MSC01_PCI_BAR0_P_SHF	        3
#define MSC01_PCI_BAR0_P_MSK	        0x00000008
#define MSC01_PCI_BAR0_P_BIT	        MSC01_PCI_BAR0_P_MSK
#define MSC01_PCI_BAR0_D_SHF	        1
#define MSC01_PCI_BAR0_D_MSK	        0x00000006
#define MSC01_PCI_BAR0_T_SHF	        0
#define MSC01_PCI_BAR0_T_MSK	        0x00000001
#define MSC01_PCI_BAR0_T_BIT	        MSC01_PCI_BAR0_T_MSK


#define MSC01_PCI_CFG_RA_SHF	        17
#define MSC01_PCI_CFG_RA_MSK	        0x00020000
#define MSC01_PCI_CFG_RA_BIT	        MSC01_PCI_CFG_RA_MSK
#define MSC01_PCI_CFG_G_SHF	        16
#define MSC01_PCI_CFG_G_MSK	        0x00010000
#define MSC01_PCI_CFG_G_BIT	        MSC01_PCI_CFG_G_MSK
#define MSC01_PCI_CFG_EN_SHF	        15
#define MSC01_PCI_CFG_EN_MSK	        0x00008000
#define MSC01_PCI_CFG_EN_BIT	        MSC01_PCI_CFG_EN_MSK
#define MSC01_PCI_CFG_MAXRTRY_SHF       0
#define MSC01_PCI_CFG_MAXRTRY_MSK       0x000000ff

#define MSC01_PCI_SWAP_IO_SHF		18
#define MSC01_PCI_SWAP_IO_MSK		0x000c0000
#define MSC01_PCI_SWAP_MEM_SHF		16
#define MSC01_PCI_SWAP_MEM_MSK		0x00030000
#define MSC01_PCI_SWAP_BAR0_SHF		0
#define MSC01_PCI_SWAP_BAR0_MSK		0x00000003
#define MSC01_PCI_SWAP_NOSWAP		0
#define MSC01_PCI_SWAP_BYTESWAP		1

/*****************************************************************************
 * Registers absolute addresses
 ****************************************************************************/

#define MSC01_PCI_ID            (MSC01_PCI_REG_BASE + MSC01_PCI_ID_OFS)
#define MSC01_PCI_SC2PMBASL     (MSC01_PCI_REG_BASE + MSC01_PCI_SC2PMBASL_OFS)
#define MSC01_PCI_SC2PMMSKL     (MSC01_PCI_REG_BASE + MSC01_PCI_SC2PMMSKL_OFS)
#define MSC01_PCI_SC2PMMAPL     (MSC01_PCI_REG_BASE + MSC01_PCI_SC2PMMAPL_OFS)
#define MSC01_PCI_SC2PIOBASL    (MSC01_PCI_REG_BASE + MSC01_PCI_SC2PIOBASL_OFS)
#define MSC01_PCI_SC2PIOMSKL    (MSC01_PCI_REG_BASE + MSC01_PCI_SC2PIOMSKL_OFS)
#define MSC01_PCI_SC2PIOMAPL    (MSC01_PCI_REG_BASE + MSC01_PCI_SC2PIOMAPL_OFS)
#define MSC01_PCI_P2SCMSKL      (MSC01_PCI_REG_BASE + MSC01_PCI_P2SCMSKL_OFS)
#define MSC01_PCI_P2SCMAPL      (MSC01_PCI_REG_BASE + MSC01_PCI_P2SCMAPL_OFS)
#define MSC01_PCI_INTCFG        (MSC01_PCI_REG_BASE + MSC01_PCI_INTCFG_OFS)
#define MSC01_PCI_INTSTAT       (MSC01_PCI_REG_BASE + MSC01_PCI_INTSTAT_OFS)
#define MSC01_PCI_CFGADDR       (MSC01_PCI_REG_BASE + MSC01_PCI_CFGADDR_OFS)
#define MSC01_PCI_CFGDATA       (MSC01_PCI_REG_BASE + MSC01_PCI_CFGDATA_OFS)
#define MSC01_PCI_IACK		(MSC01_PCI_REG_BASE + MSC01_PCI_IACK_OFS)
#define MSC01_PCI_HEAD0		(MSC01_PCI_REG_BASE + MSC01_PCI_HEAD0_OFS)
#define MSC01_PCI_HEAD1		(MSC01_PCI_REG_BASE + MSC01_PCI_HEAD1_OFS)
#define MSC01_PCI_HEAD2		(MSC01_PCI_REG_BASE + MSC01_PCI_HEAD2_OFS)
#define MSC01_PCI_HEAD3		(MSC01_PCI_REG_BASE + MSC01_PCI_HEAD3_OFS)
#define MSC01_PCI_HEAD4		(MSC01_PCI_REG_BASE + MSC01_PCI_HEAD4_OFS)
#define MSC01_PCI_HEAD5		(MSC01_PCI_REG_BASE + MSC01_PCI_HEAD5_OFS)
#define MSC01_PCI_HEAD6		(MSC01_PCI_REG_BASE + MSC01_PCI_HEAD6_OFS)
#define MSC01_PCI_HEAD7		(MSC01_PCI_REG_BASE + MSC01_PCI_HEAD7_OFS)
#define MSC01_PCI_HEAD8		(MSC01_PCI_REG_BASE + MSC01_PCI_HEAD8_OFS)
#define MSC01_PCI_HEAD9		(MSC01_PCI_REG_BASE + MSC01_PCI_HEAD9_OFS)
#define MSC01_PCI_HEAD10	(MSC01_PCI_REG_BASE + MSC01_PCI_HEAD10_OFS)
#define MSC01_PCI_HEAD11	(MSC01_PCI_REG_BASE + MSC01_PCI_HEAD11_OFS)
#define MSC01_PCI_HEAD12	(MSC01_PCI_REG_BASE + MSC01_PCI_HEAD11_OFS)
#define MSC01_PCI_HEAD13	(MSC01_PCI_REG_BASE + MSC01_PCI_HEAD11_OFS)
#define MSC01_PCI_HEAD14	(MSC01_PCI_REG_BASE + MSC01_PCI_HEAD11_OFS)
#define MSC01_PCI_HEAD15        (MSC01_PCI_REG_BASE + MSC01_PCI_HEAD11_OFS)
#define MSC01_PCI_BAR0		(MSC01_PCI_REG_BASE + MSC01_PCI_BAR0_OFS)
#define MSC01_PCI_CFG		(MSC01_PCI_REG_BASE + MSC01_PCI_CFG_OFS)
#define MSC01_PCI_SWAP		(MSC01_PCI_REG_BASE + MSC01_PCI_SWAP_OFS)

#endif
/*****************************************************************************
 *  End of msc01_pci.h
 *****************************************************************************/
