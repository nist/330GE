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
 * Carsten Langgaard, carstenl@mips.com
 * Copyright (C) 2000 MIPS Technologies, Inc.  All rights reserved.
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 */
#ifndef _ASM_GT64120_H
#define _ASM_GT64120_H

#define MSK(n)                    ((1 << (n)) - 1)

/*
 *  Register offset addresses
 */
#define GT_CPU_OFS		0x000

/*
 * Interrupt Registers
 */
#define GT_SCS10LD_OFS		0x008
#define GT_SCS10HD_OFS		0x010
#define GT_SCS32LD_OFS		0x018
#define GT_SCS32HD_OFS		0x020
#define GT_CS20LD_OFS		0x028
#define GT_CS20HD_OFS		0x030
#define GT_CS3BOOTLD_OFS	0x038
#define GT_CS3BOOTHD_OFS	0x040
#define GT_PCI0IOLD_OFS		0x048
#define GT_PCI0IOHD_OFS		0x050
#define GT_PCI0M0LD_OFS		0x058
#define GT_PCI0M0HD_OFS		0x060
#define GT_ISD_OFS		0x068
#define GT_PCI0M1LD_OFS		0x080
#define GT_PCI0M1HD_OFS		0x088
#define GT_PCI1IOLD_OFS		0x090
#define GT_PCI1IOHD_OFS		0x098
#define GT_PCI1M0LD_OFS		0x0a0
#define GT_PCI1M0HD_OFS		0x0a8
#define GT_PCI1M1LD_OFS		0x0b0
#define GT_PCI1M1HD_OFS		0x0b8

/*
 * GT64120A only
 */
#define GT_PCI0IOREMAP_OFS	0x0f0
#define GT_PCI0M0REMAP_OFS	0x0f8
#define GT_PCI0M1REMAP_OFS	0x100
#define GT_PCI1IOREMAP_OFS	0x108
#define GT_PCI1M0REMAP_OFS	0x110
#define GT_PCI1M1REMAP_OFS	0x118

#define GT_SCS0LD_OFS		0x400
#define GT_SCS0HD_OFS		0x404
#define GT_SCS1LD_OFS		0x408
#define GT_SCS1HD_OFS		0x40c
#define GT_SCS2LD_OFS		0x410
#define GT_SCS2HD_OFS		0x414
#define GT_SCS3LD_OFS		0x418
#define GT_SCS3HD_OFS		0x41c
#define GT_CS0LD_OFS		0x420
#define GT_CS0HD_OFS		0x424
#define GT_CS1LD_OFS		0x428
#define GT_CS1HD_OFS		0x42c
#define GT_CS2LD_OFS		0x430
#define GT_CS2HD_OFS		0x434
#define GT_CS3LD_OFS		0x438
#define GT_CS3HD_OFS		0x43c
#define GT_BOOTLD_OFS		0x440
#define GT_BOOTHD_OFS		0x444

#define GT_SDRAM_B0_OFS	    	0x44c
#define GT_SDRAM_CFG_OFS	0x448
#define GT_SDRAM_B2_OFS		0x454
#define GT_SDRAM_OPMODE_OFS	0x474
#define GT_SDRAM_BM_OFS		0x478
#define GT_SDRAM_ADDRDECODE_OFS	0x47c

#define GT_PCI0_CMD_OFS		0xc00	/* GT64120A only */
#define GT_PCI0_TOR_OFS		0xc04
#define GT_PCI0_BS_SCS10_OFS    0xc08
#define GT_PCI0_BS_SCS32_OFS    0xc0c
#define GT_INTRCAUSE_OFS	0xc18
#define GT_INTRMASK_OFS		0xc1c	/* GT64120A only */
#define GT_PCI0_IACK_OFS	0xc34
#define GT_PCI0_BARE_OFS	0xc3c
#define GT_HINTRCAUSE_OFS	0xc98	/* GT64120A only */
#define GT_HINTRMASK_OFS	0xc9c	/* GT64120A only */
#define GT_PCI1_CFGADDR_OFS	0xcf0	/* GT64120A only */
#define GT_PCI1_CFGDATA_OFS	0xcf4	/* GT64120A only */
#define GT_PCI0_CFGADDR_OFS	0xcf8
#define GT_PCI0_CFGDATA_OFS	0xcfc


/*
 * Timer/Counter.  GT64120A only.
 */
#define GT_TC0_OFS		0x850
#define GT_TC1_OFS		0x854
#define GT_TC2_OFS		0x858
#define GT_TC3_OFS		0x85C
#define GT_TC_CONTROL_OFS	0x864

/*
 * I2O Support Registers
 */
#define INBOUND_MESSAGE_REGISTER0_PCI_SIDE		0x010
#define INBOUND_MESSAGE_REGISTER1_PCI_SIDE		0x014
#define OUTBOUND_MESSAGE_REGISTER0_PCI_SIDE		0x018
#define OUTBOUND_MESSAGE_REGISTER1_PCI_SIDE		0x01c
#define INBOUND_DOORBELL_REGISTER_PCI_SIDE		0x020
#define INBOUND_INTERRUPT_CAUSE_REGISTER_PCI_SIDE	0x024
#define INBOUND_INTERRUPT_MASK_REGISTER_PCI_SIDE	0x028
#define OUTBOUND_DOORBELL_REGISTER_PCI_SIDE		0x02c
#define OUTBOUND_INTERRUPT_CAUSE_REGISTER_PCI_SIDE	0x030
#define OUTBOUND_INTERRUPT_MASK_REGISTER_PCI_SIDE	0x034
#define INBOUND_QUEUE_PORT_VIRTUAL_REGISTER_PCI_SIDE	0x040
#define OUTBOUND_QUEUE_PORT_VIRTUAL_REGISTER_PCI_SIDE	0x044
#define QUEUE_CONTROL_REGISTER_PCI_SIDE			0x050
#define QUEUE_BASE_ADDRESS_REGISTER_PCI_SIDE		0x054
#define INBOUND_FREE_HEAD_POINTER_REGISTER_PCI_SIDE	0x060
#define INBOUND_FREE_TAIL_POINTER_REGISTER_PCI_SIDE	0x064
#define INBOUND_POST_HEAD_POINTER_REGISTER_PCI_SIDE	0x068
#define INBOUND_POST_TAIL_POINTER_REGISTER_PCI_SIDE	0x06c
#define OUTBOUND_FREE_HEAD_POINTER_REGISTER_PCI_SIDE	0x070
#define OUTBOUND_FREE_TAIL_POINTER_REGISTER_PCI_SIDE	0x074
#define OUTBOUND_POST_HEAD_POINTER_REGISTER_PCI_SIDE	0x078
#define OUTBOUND_POST_TAIL_POINTER_REGISTER_PCI_SIDE	0x07c

#define INBOUND_MESSAGE_REGISTER0_CPU_SIDE		0x1c10
#define INBOUND_MESSAGE_REGISTER1_CPU_SIDE		0x1c14
#define OUTBOUND_MESSAGE_REGISTER0_CPU_SIDE		0x1c18
#define OUTBOUND_MESSAGE_REGISTER1_CPU_SIDE		0x1c1c
#define INBOUND_DOORBELL_REGISTER_CPU_SIDE		0x1c20
#define INBOUND_INTERRUPT_CAUSE_REGISTER_CPU_SIDE	0x1c24
#define INBOUND_INTERRUPT_MASK_REGISTER_CPU_SIDE	0x1c28
#define OUTBOUND_DOORBELL_REGISTER_CPU_SIDE		0x1c2c
#define OUTBOUND_INTERRUPT_CAUSE_REGISTER_CPU_SIDE	0x1c30
#define OUTBOUND_INTERRUPT_MASK_REGISTER_CPU_SIDE	0x1c34
#define INBOUND_QUEUE_PORT_VIRTUAL_REGISTER_CPU_SIDE	0x1c40
#define OUTBOUND_QUEUE_PORT_VIRTUAL_REGISTER_CPU_SIDE	0x1c44
#define QUEUE_CONTROL_REGISTER_CPU_SIDE			0x1c50
#define QUEUE_BASE_ADDRESS_REGISTER_CPU_SIDE		0x1c54
#define INBOUND_FREE_HEAD_POINTER_REGISTER_CPU_SIDE	0x1c60
#define INBOUND_FREE_TAIL_POINTER_REGISTER_CPU_SIDE	0x1c64
#define INBOUND_POST_HEAD_POINTER_REGISTER_CPU_SIDE	0x1c68
#define INBOUND_POST_TAIL_POINTER_REGISTER_CPU_SIDE	0x1c6c
#define OUTBOUND_FREE_HEAD_POINTER_REGISTER_CPU_SIDE	0x1c70
#define OUTBOUND_FREE_TAIL_POINTER_REGISTER_CPU_SIDE	0x1c74
#define OUTBOUND_POST_HEAD_POINTER_REGISTER_CPU_SIDE	0x1c78
#define OUTBOUND_POST_TAIL_POINTER_REGISTER_CPU_SIDE	0x1c7c

/*
 *  Register encodings
 */
#define GT_CPU_ENDIAN_SHF       12
#define GT_CPU_ENDIAN_MSK       (MSK(1) << GT_CPU_ENDIAN_SHF)
#define GT_CPU_ENDIAN_BIT       GT_CPU_ENDIAN_MSK
#define GT_CPU_WR_SHF		16
#define GT_CPU_WR_MSK		(MSK(1) << GT_CPU_WR_SHF)
#define GT_CPU_WR_BIT		GT_CPU_WR_MSK
#define GT_CPU_WR_DXDXDXDX	0
#define GT_CPU_WR_DDDD		1


#define GT_CFGADDR_CFGEN_SHF	31
#define GT_CFGADDR_CFGEN_MSK	(MSK(1) << GT_CFGADDR_CFGEN_SHF)
#define GT_CFGADDR_CFGEN_BIT	GT_CFGADDR_CFGEN_MSK

#define GT_CFGADDR_BUSNUM_SHF	16
#define GT_CFGADDR_BUSNUM_MSK	(MSK(8) << GT_CFGADDR_BUSNUM_SHF)

#define GT_CFGADDR_DEVNUM_SHF	11
#define GT_CFGADDR_DEVNUM_MSK	(MSK(5) << GT_CFGADDR_DEVNUM_SHF)

#define GT_CFGADDR_FUNCNUM_SHF	8
#define GT_CFGADDR_FUNCNUM_MSK	(MSK(3) << GT_CFGADDR_FUNCNUM_SHF)

#define GT_CFGADDR_REGNUM_SHF	2
#define GT_CFGADDR_REGNUM_MSK	(MSK(6) << GT_CFGADDR_REGNUM_SHF)


#define GT_SDRAM_BM_ORDER_SHF	2
#define GT_SDRAM_BM_ORDER_MSK	(MSK(1) << GT_SDRAM_BM_ORDER_SHF)
#define GT_SDRAM_BM_ORDER_BIT	GT_SDRAM_BM_ORDER_MSK
#define GT_SDRAM_BM_ORDER_SUB	1
#define GT_SDRAM_BM_ORDER_LIN	0

#define GT_SDRAM_BM_RSVD_ALL1	0xffb


#define GT_SDRAM_ADDRDECODE_ADDR_SHF	0
#define GT_SDRAM_ADDRDECODE_ADDR_MSK	(MSK(3) << GT_SDRAM_ADDRDECODE_ADDR_SHF)
#define GT_SDRAM_ADDRDECODE_ADDR_0	0
#define GT_SDRAM_ADDRDECODE_ADDR_1	1
#define GT_SDRAM_ADDRDECODE_ADDR_2	2
#define GT_SDRAM_ADDRDECODE_ADDR_3	3
#define GT_SDRAM_ADDRDECODE_ADDR_4	4
#define GT_SDRAM_ADDRDECODE_ADDR_5	5
#define GT_SDRAM_ADDRDECODE_ADDR_6	6
#define GT_SDRAM_ADDRDECODE_ADDR_7	7


#define GT_SDRAM_B0_CASLAT_SHF		0
#define GT_SDRAM_B0_CASLAT_MSK		(MSK(2) << GT_SDRAM_B0__SHF)
#define GT_SDRAM_B0_CASLAT_2		1
#define GT_SDRAM_B0_CASLAT_3		2

#define GT_SDRAM_B0_FTDIS_SHF		2
#define GT_SDRAM_B0_FTDIS_MSK		(MSK(1) << GT_SDRAM_B0_FTDIS_SHF)
#define GT_SDRAM_B0_FTDIS_BIT		GT_SDRAM_B0_FTDIS_MSK

#define GT_SDRAM_B0_SRASPRCHG_SHF	3
#define GT_SDRAM_B0_SRASPRCHG_MSK	(MSK(1) << GT_SDRAM_B0_SRASPRCHG_SHF)
#define GT_SDRAM_B0_SRASPRCHG_BIT	GT_SDRAM_B0_SRASPRCHG_MSK
#define GT_SDRAM_B0_SRASPRCHG_2		0
#define GT_SDRAM_B0_SRASPRCHG_3		1

#define GT_SDRAM_B0_B0COMPAB_SHF	4
#define GT_SDRAM_B0_B0COMPAB_MSK	(MSK(1) << GT_SDRAM_B0_B0COMPAB_SHF)
#define GT_SDRAM_B0_B0COMPAB_BIT	GT_SDRAM_B0_B0COMPAB_MSK

#define GT_SDRAM_B0_64BITINT_SHF	5
#define GT_SDRAM_B0_64BITINT_MSK	(MSK(1) << GT_SDRAM_B0_64BITINT_SHF)
#define GT_SDRAM_B0_64BITINT_BIT	GT_SDRAM_B0_64BITINT_MSK
#define GT_SDRAM_B0_64BITINT_2		0
#define GT_SDRAM_B0_64BITINT_4		1

#define GT_SDRAM_B0_BW_SHF		6
#define GT_SDRAM_B0_BW_MSK		(MSK(1) << GT_SDRAM_B0_BW_SHF)
#define GT_SDRAM_B0_BW_BIT		GT_SDRAM_B0_BW_MSK
#define GT_SDRAM_B0_BW_32		0
#define GT_SDRAM_B0_BW_64		1

#define GT_SDRAM_B0_BLODD_SHF		7
#define GT_SDRAM_B0_BLODD_MSK		(MSK(1) << GT_SDRAM_B0_BLODD_SHF)
#define GT_SDRAM_B0_BLODD_BIT		GT_SDRAM_B0_BLODD_MSK

#define GT_SDRAM_B0_PAR_SHF		8
#define GT_SDRAM_B0_PAR_MSK		(MSK(1) << GT_SDRAM_B0_PAR_SHF)
#define GT_SDRAM_B0_PAR_BIT		GT_SDRAM_B0_PAR_MSK

#define GT_SDRAM_B0_BYPASS_SHF		9
#define GT_SDRAM_B0_BYPASS_MSK		(MSK(1) << GT_SDRAM_B0_BYPASS_SHF)
#define GT_SDRAM_B0_BYPASS_BIT		GT_SDRAM_B0_BYPASS_MSK

#define GT_SDRAM_B0_SRAS2SCAS_SHF	10
#define GT_SDRAM_B0_SRAS2SCAS_MSK	(MSK(1) << GT_SDRAM_B0_SRAS2SCAS_SHF)
#define GT_SDRAM_B0_SRAS2SCAS_BIT	GT_SDRAM_B0_SRAS2SCAS_MSK
#define GT_SDRAM_B0_SRAS2SCAS_2		0
#define GT_SDRAM_B0_SRAS2SCAS_3		1

#define GT_SDRAM_B0_SIZE_SHF		11
#define GT_SDRAM_B0_SIZE_MSK		(MSK(1) << GT_SDRAM_B0_SIZE_SHF)
#define GT_SDRAM_B0_SIZE_BIT		GT_SDRAM_B0_SIZE_MSK
#define GT_SDRAM_B0_SIZE_16M		0
#define GT_SDRAM_B0_SIZE_64M		1

#define GT_SDRAM_B0_EXTPAR_SHF		12
#define GT_SDRAM_B0_EXTPAR_MSK		(MSK(1) << GT_SDRAM_B0_EXTPAR_SHF)
#define GT_SDRAM_B0_EXTPAR_BIT		GT_SDRAM_B0_EXTPAR_MSK

#define GT_SDRAM_B0_BLEN_SHF		13
#define GT_SDRAM_B0_BLEN_MSK		(MSK(1) << GT_SDRAM_B0_BLEN_SHF)
#define GT_SDRAM_B0_BLEN_BIT		GT_SDRAM_B0_BLEN_MSK
#define GT_SDRAM_B0_BLEN_8		0
#define GT_SDRAM_B0_BLEN_4		1


#define GT_SDRAM_CFG_REFINT_SHF		0
#define GT_SDRAM_CFG_REFINT_MSK		(MSK(14) << GT_SDRAM_CFG_REFINT_SHF)

#define GT_SDRAM_CFG_NINTERLEAVE_SHF	14
#define GT_SDRAM_CFG_NINTERLEAVE_MSK    (MSK(1) << GT_SDRAM_CFG_NINTERLEAVE_SHF)
#define GT_SDRAM_CFG_NINTERLEAVE_BIT	GT_SDRAM_CFG_NINTERLEAVE_MSK

#define GT_SDRAM_CFG_RMW_SHF		15
#define GT_SDRAM_CFG_RMW_MSK		(MSK(1) << GT_SDRAM_CFG_RMW_SHF)
#define GT_SDRAM_CFG_RMW_BIT		GT_SDRAM_CFG_RMW_MSK

#define GT_SDRAM_CFG_NONSTAGREF_SHF	16
#define GT_SDRAM_CFG_NONSTAGREF_MSK	(MSK(1) << GT_SDRAM_CFG_NONSTAGREF_SHF)
#define GT_SDRAM_CFG_NONSTAGREF_BIT	GT_SDRAM_CFG_NONSTAGREF_MSK

#define GT_SDRAM_CFG_DUPCNTL_SHF	19
#define GT_SDRAM_CFG_DUPCNTL_MSK	(MSK(1) << GT_SDRAM_CFG_DUPCNTL_SHF)
#define GT_SDRAM_CFG_DUPCNTL_BIT	GT_SDRAM_CFG_DUPCNTL_MSK

#define GT_SDRAM_CFG_DUPBA_SHF		20
#define GT_SDRAM_CFG_DUPBA_MSK		(MSK(1) << GT_SDRAM_CFG_DUPBA_SHF)
#define GT_SDRAM_CFG_DUPBA_BIT		GT_SDRAM_CFG_DUPBA_MSK

#define GT_SDRAM_CFG_DUPEOT0_SHF	21
#define GT_SDRAM_CFG_DUPEOT0_MSK	(MSK(1) << GT_SDRAM_CFG_DUPEOT0_SHF)
#define GT_SDRAM_CFG_DUPEOT0_BIT	GT_SDRAM_CFG_DUPEOT0_MSK

#define GT_SDRAM_CFG_DUPEOT1_SHF	22
#define GT_SDRAM_CFG_DUPEOT1_MSK	(MSK(1) << GT_SDRAM_CFG_DUPEOT1_SHF)
#define GT_SDRAM_CFG_DUPEOT1_BIT	GT_SDRAM_CFG_DUPEOT1_MSK

#define GT_SDRAM_OPMODE_OP_SHF		0
#define GT_SDRAM_OPMODE_OP_MSK		(MSK(3) << GT_SDRAM_OPMODE_OP_SHF)
#define GT_SDRAM_OPMODE_OP_NORMAL	0
#define GT_SDRAM_OPMODE_OP_NOP		1
#define GT_SDRAM_OPMODE_OP_PRCHG	2
#define GT_SDRAM_OPMODE_OP_MODE		3
#define GT_SDRAM_OPMODE_OP_CBR		4


#define GT_PCI0_BARE_SWSCS3BOOTDIS_SHF	0
#define GT_PCI0_BARE_SWSCS3BOOTDIS_MSK	(MSK(1) << GT_PCI0_BARE_SWSCS3BOOTDIS_SHF)
#define GT_PCI0_BARE_SWSCS3BOOTDIS_BIT	GT_PCI0_BARE_SWSCS3BOOTDIS_MSK

#define GT_PCI0_BARE_SWSCS32DIS_SHF	1
#define GT_PCI0_BARE_SWSCS32DIS_MSK	(MSK(1) << GT_PCI0_BARE_SWSCS32DIS_SHF)
#define GT_PCI0_BARE_SWSCS32DIS_BIT	GT_PCI0_BARE_SWSCS32DIS_MSK

#define GT_PCI0_BARE_SWSCS10DIS_SHF	2
#define GT_PCI0_BARE_SWSCS10DIS_MSK	(MSK(1) << GT_PCI0_BARE_SWSCS10DIS_SHF)
#define GT_PCI0_BARE_SWSCS10DIS_BIT	GT_PCI0_BARE_SWSCS10DIS_MSK

#define GT_PCI0_BARE_INTIODIS_SHF	3
#define GT_PCI0_BARE_INTIODIS_MSK	(MSK(1) << GT_PCI0_BARE_INTIODIS_SHF)
#define GT_PCI0_BARE_INTIODIS_BIT	GT_PCI0_BARE_INTIODIS_MSK

#define GT_PCI0_BARE_INTMEMDIS_SHF	4
#define GT_PCI0_BARE_INTMEMDIS_MSK	(MSK(1) << GT_PCI0_BARE_INTMEMDIS_SHF)
#define GT_PCI0_BARE_INTMEMDIS_BIT	GT_PCI0_BARE_INTMEMDIS_MSK

#define GT_PCI0_BARE_CS3BOOTDIS_SHF	5
#define GT_PCI0_BARE_CS3BOOTDIS_MSK	(MSK(1) << GT_PCI0_BARE_CS3BOOTDIS_SHF)
#define GT_PCI0_BARE_CS3BOOTDIS_BIT	GT_PCI0_BARE_CS3BOOTDIS_MSK

#define GT_PCI0_BARE_CS20DIS_SHF	6
#define GT_PCI0_BARE_CS20DIS_MSK	(MSK(1) << GT_PCI0_BARE_CS20DIS_SHF)
#define GT_PCI0_BARE_CS20DIS_BIT	GT_PCI0_BARE_CS20DIS_MSK

#define GT_PCI0_BARE_SCS32DIS_SHF	7
#define GT_PCI0_BARE_SCS32DIS_MSK	(MSK(1) << GT_PCI0_BARE_SCS32DIS_SHF)
#define GT_PCI0_BARE_SCS32DIS_BIT	GT_PCI0_BARE_SCS32DIS_MSK

#define GT_PCI0_BARE_SCS10DIS_SHF	8
#define GT_PCI0_BARE_SCS10DIS_MSK	(MSK(1) << GT_PCI0_BARE_SCS10DIS_SHF)
#define GT_PCI0_BARE_SCS10DIS_BIT	GT_PCI0_BARE_SCS10DIS_MSK


#define GT_INTRCAUSE_MASABORT0_SHF	18
#define GT_INTRCAUSE_MASABORT0_MSK	(MSK(1) << GT_INTRCAUSE_MASABORT0_SHF)
#define GT_INTRCAUSE_MASABORT0_BIT	GT_INTRCAUSE_MASABORT0_MSK

#define GT_INTRCAUSE_TARABORT0_SHF	19
#define GT_INTRCAUSE_TARABORT0_MSK	(MSK(1) << GT_INTRCAUSE_TARABORT0_SHF)
#define GT_INTRCAUSE_TARABORT0_BIT	GT_INTRCAUSE_TARABORT0_MSK


#define GT_PCI0_CFGADDR_REGNUM_SHF	2
#define GT_PCI0_CFGADDR_REGNUM_MSK	(MSK(6) << GT_PCI0_CFGADDR_REGNUM_SHF)
#define GT_PCI0_CFGADDR_FUNCTNUM_SHF	8
#define GT_PCI0_CFGADDR_FUNCTNUM_MSK    (MSK(3) << GT_PCI0_CFGADDR_FUNCTNUM_SHF)
#define GT_PCI0_CFGADDR_DEVNUM_SHF	11
#define GT_PCI0_CFGADDR_DEVNUM_MSK	(MSK(5) << GT_PCI0_CFGADDR_DEVNUM_SHF)
#define GT_PCI0_CFGADDR_BUSNUM_SHF	16
#define GT_PCI0_CFGADDR_BUSNUM_MSK	(MSK(8) << GT_PCI0_CFGADDR_BUSNUM_SHF)
#define GT_PCI0_CFGADDR_CONFIGEN_SHF	31
#define GT_PCI0_CFGADDR_CONFIGEN_MSK	(MSK(1) << GT_PCI0_CFGADDR_CONFIGEN_SHF)
#define GT_PCI0_CFGADDR_CONFIGEN_BIT	GT_PCI0_CFGADDR_CONFIGEN_MSK

#define GT_PCI0_CMD_MBYTESWAP_SHF       0
#define GT_PCI0_CMD_MBYTESWAP_MSK       (MSK(1) << GT_PCI0_CMD_MBYTESWAP_SHF)
#define GT_PCI0_CMD_MBYTESWAP_BIT       GT_PCI0_CMD_MBYTESWAP_MSK
#define GT_PCI0_CMD_MWORDSWAP_SHF       10
#define GT_PCI0_CMD_MWORDSWAP_MSK       (MSK(1) << GT_PCI0_CMD_MWORDSWAP_SHF)
#define GT_PCI0_CMD_MWORDSWAP_BIT       GT_PCI0_CMD_MWORDSWAP_MSK
#define GT_PCI0_CMD_SBYTESWAP_SHF       16
#define GT_PCI0_CMD_SBYTESWAP_MSK       (MSK(1) << GT_PCI0_CMD_SBYTESWAP_SHF)
#define GT_PCI0_CMD_SBYTESWAP_BIT       GT_PCI0_CMD_SBYTESWAP_MSK
#define GT_PCI0_CMD_SWORDSWAP_SHF       11
#define GT_PCI0_CMD_SWORDSWAP_MSK       (MSK(1) << GT_PCI0_CMD_SWORDSWAP_SHF)
#define GT_PCI0_CMD_SWORDSWAP_BIT       GT_PCI0_CMD_SWORDSWAP_MSK

/*
 *  Misc
 */
#define GT_DEF_BASE		0x14000000
#define GT_DEF_PCI0_MEM0_BASE	0x12000000
#define GT_MAX_BANKSIZE		(256 * 1024 * 1024)   /* Max 256MB bank */
#define GT_LATTIM_MIN    	6		      /* Minimum lat	*/

#endif /* _ASM_GT64120_H */
