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
/******************************************************************************/
/*                                                                            */
/* Broadcom BCM5700 Linux Network Driver, Copyright (c) 2000 - 2004 Broadcom  */
/* Corporation.                                                               */
/* All rights reserved.                                                       */
/*                                                                            */
/* This program is free software; you can redistribute it and/or modify       */
/* it under the terms of the GNU General Public License as published by       */
/* the Free Software Foundation, located in the file LICENSE.                 */
/*                                                                            */
/* ethtool -t selftest code.                                                  */
/*                                                                            */
/******************************************************************************/

#include "mm.h"

#ifdef NICE_SUPPORT
#include "nicext.h"
#endif

#ifdef ETHTOOL_TEST

typedef struct reg_entry
{
	LM_UINT16   offset;
	LM_UINT16   flags;
#define BCM5705_ONLY		1
#define NOT_FOR_BCM5705		2
#define NOT_FOR_BCM5788		4
	LM_UINT32   read_mask;
	LM_UINT32   write_mask;
} reg_entry_t;

typedef struct mem_entry
{
	LM_UINT32   offset;
	LM_UINT32   len;
} mem_entry_t;

/* Returns 1 on success, 0 on failure */
int
b57_test_registers(UM_DEVICE_BLOCK *pUmDevice)
{
	LM_DEVICE_BLOCK *pDevice = &pUmDevice->lm_dev;
	int ret;
	int i, bcm5705;
	LM_UINT32 offset, read_mask, write_mask, val, save_val, read_val;
	static reg_entry_t reg_tbl[] = {
		/* MAC block */
		{ 0x0400, 0x0002, 0x00000000, 0x00ef6f8c },
		{ 0x0400, 0x0001, 0x00000000, 0x01ef6b8c },	/* 5705 */
		{ 0x0404, 0x0002, 0x03800107, 0x00000000 },
		{ 0x0404, 0x0001, 0x03800100, 0x00000000 },	/* 5705 */
		{ 0x0408, 0x0002, 0x00000000, 0x07c01400 },
		{ 0x0408, 0x0001, 0x00000000, 0x07c01000 },	/* 5705 */
		{ 0x040c, 0x0000, 0x00000000, 0xfff8007f },
		{ 0x0410, 0x0000, 0x00000000, 0x0000ffff },
		{ 0x0414, 0x0000, 0x00000000, 0xffffffff },
		{ 0x0418, 0x0000, 0x00000000, 0x0000ffff },
		{ 0x041c, 0x0000, 0x00000000, 0xffffffff },
		{ 0x0420, 0x0000, 0x00000000, 0x0000ffff },
		{ 0x0424, 0x0000, 0x00000000, 0xffffffff },
		{ 0x0428, 0x0000, 0x00000000, 0x0000ffff },
		{ 0x042c, 0x0000, 0x00000000, 0xffffffff },
		{ 0x0430, 0x0002, 0x00000000, 0xffffffff },
		{ 0x0430, 0x0001, 0x00000000, 0x0fff03ff },	/* 5705 */
		{ 0x0434, 0x0002, 0x00000000, 0x0fffffff },
		{ 0x0434, 0x0001, 0x00000000, 0x000001ff },	/* 5705 */
		{ 0x043c, 0x0000, 0x00000000, 0x0000ffff },
		{ 0x0454, 0x0000, 0x00000000, 0x00000010 },
		{ 0x045c, 0x0000, 0x00000000, 0x00000070 },
		{ 0x0464, 0x0000, 0x00000000, 0x00003fff },
		{ 0x0468, 0x0002, 0x00000000, 0x000007fc },
		{ 0x0468, 0x0001, 0x00000000, 0x000007dc },	/* 5705 */
		{ 0x0470, 0x0000, 0x00000000, 0xffffffff },
		{ 0x0474, 0x0000, 0x00000000, 0xffffffff },
		{ 0x0478, 0x0000, 0x00000000, 0xffffffff },
		{ 0x047c, 0x0000, 0x00000000, 0xffffffff },
		{ 0x0480, 0x0002, 0x00000000, 0xffffffff },
		{ 0x0480, 0x0001, 0x00000000, 0xe7ffffff },	/* 5705 */
		{ 0x0484, 0x0000, 0x00000000, 0xffffffff },
		{ 0x0488, 0x0002, 0x00000000, 0xffffffff },
		{ 0x0488, 0x0001, 0x00000000, 0xe7ffffff },	/* 5705 */
		{ 0x048c, 0x0000, 0x00000000, 0xffffffff },
		{ 0x0490, 0x0002, 0x00000000, 0xffffffff },
		{ 0x0490, 0x0001, 0x00000000, 0xe7ffffff },	/* 5705 */
		{ 0x0494, 0x0000, 0x00000000, 0xffffffff },
		{ 0x0498, 0x0002, 0x00000000, 0xffffffff },
		{ 0x0498, 0x0001, 0x00000000, 0xe7ffffff },	/* 5705 */
		{ 0x049c, 0x0000, 0x00000000, 0xffffffff },
		{ 0x04a0, 0x0002, 0x00000000, 0xffffffff },
		{ 0x04a0, 0x0001, 0x00000000, 0xe7ffffff },	/* 5705 */
		{ 0x04a4, 0x0000, 0x00000000, 0xffffffff },
		{ 0x04a8, 0x0002, 0x00000000, 0xffffffff },
		{ 0x04a8, 0x0001, 0x00000000, 0xe7ffffff },	/* 5705 */
		{ 0x04ac, 0x0000, 0x00000000, 0xffffffff },
		{ 0x04b0, 0x0002, 0x00000000, 0xffffffff },
		{ 0x04b0, 0x0001, 0x00000000, 0xe7ffffff },	/* 5705 */
		{ 0x04b4, 0x0000, 0x00000000, 0xffffffff },
		{ 0x04b8, 0x0002, 0x00000000, 0xffffffff },
		{ 0x04b8, 0x0001, 0x00000000, 0xe7ffffff },	/* 5705 */
		{ 0x04bc, 0x0000, 0x00000000, 0xffffffff },
		{ 0x04c0, 0x0002, 0x00000000, 0xffffffff },
		{ 0x04c4, 0x0002, 0x00000000, 0xffffffff },
		{ 0x04c8, 0x0002, 0x00000000, 0xffffffff },
		{ 0x04cc, 0x0002, 0x00000000, 0xffffffff },
		{ 0x04d0, 0x0002, 0x00000000, 0xffffffff },
		{ 0x04d4, 0x0002, 0x00000000, 0xffffffff },
		{ 0x04d8, 0x0002, 0x00000000, 0xffffffff },
		{ 0x04dc, 0x0002, 0x00000000, 0xffffffff },
		{ 0x04e0, 0x0002, 0x00000000, 0xffffffff },
		{ 0x04e4, 0x0002, 0x00000000, 0xffffffff },
		{ 0x04e8, 0x0002, 0x00000000, 0xffffffff },
		{ 0x04ec, 0x0002, 0x00000000, 0xffffffff },
		{ 0x04f0, 0x0002, 0x00000000, 0xffffffff },
		{ 0x04f4, 0x0002, 0x00000000, 0xffffffff },
		{ 0x04f8, 0x0002, 0x00000000, 0xffffffff },
		{ 0x04fc, 0x0002, 0x00000000, 0xffffffff },
		{ 0x0500, 0x0002, 0x00000000, 0x000000f8 },
		{ 0x0500, 0x0001, 0x00000000, 0x00000008 },	/* 5705 */

		/* Send Data Initiator Control Registers */
		{ 0x0c00, 0x0000, 0x00000000, 0x00000006 },
		{ 0x0c04, 0x0000, 0x00000004, 0x00000000 },
		{ 0x0c08, 0x0000, 0x00000000, 0x0000001b },
		{ 0x0c0c, 0x0002, 0x00000000, 0x00ffffff },
		{ 0x0c0c, 0x0001, 0x00000000, 0x00000001 },
		{ 0x0c80, 0x0000, 0x000003ff, 0x00000000 },
		{ 0x0c84, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x0c88, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x0c8c, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x0c90, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x0c94, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x0c98, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x0c9c, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x0ca0, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x0ca4, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x0ca8, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x0cac, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x0cb0, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x0cb4, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x0cb8, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x0cbc, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x0cc0, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x0cc4, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x0cc8, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x0ccc, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x0cd0, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x0cd4, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x0cd8, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x0cdc, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x0ce0, 0x0001, 0x00000000, 0xffffffff },	/* 5705 */
		{ 0x0ce4, 0x0001, 0x00000000, 0xffffffff },	/* 5705 */
		{ 0x0ce8, 0x0001, 0x00000000, 0x00ffffff },	/* 5705 */
		{ 0x0cec, 0x0001, 0x00000000, 0x000efcf7 },	/* 5705 */
		{ 0x0cf0, 0x0001, 0x00000000, 0x0000ffff },	/* 5705 */
		{ 0x0cf4, 0x0001, 0x00000000, 0x20000000 },	/* 5705 */

		/* SDC Control Registers */
		{ 0x1000, 0x0000, 0x00000000, 0x00000002 },
		{ 0x1008, 0x0001, 0x00000000, 0x40000000 },	/* 5705 */

		/* Send BD Ring Selector Control Registers. */
		{ 0x1400, 0x0000, 0x00000000, 0x00000006 },
		{ 0x1404, 0x0000, 0x00000004, 0x00000000 },
		{ 0x1408, 0x0000, 0x0000ffff, 0x00000000 },
		{ 0x1440, 0x0000, 0x0000000f, 0x00000000 },
		{ 0x1444, 0x0002, 0x0000000f, 0x00000000 },
		{ 0x1448, 0x0002, 0x0000000f, 0x00000000 },
		{ 0x144c, 0x0002, 0x0000000f, 0x00000000 },
		{ 0x1450, 0x0002, 0x0000000f, 0x00000000 },
		{ 0x1454, 0x0002, 0x0000000f, 0x00000000 },
		{ 0x1458, 0x0002, 0x0000000f, 0x00000000 },
		{ 0x145c, 0x0002, 0x0000000f, 0x00000000 },
		{ 0x1460, 0x0002, 0x0000000f, 0x00000000 },
		{ 0x1464, 0x0002, 0x0000000f, 0x00000000 },
		{ 0x1468, 0x0002, 0x0000000f, 0x00000000 },
		{ 0x146c, 0x0002, 0x0000000f, 0x00000000 },
		{ 0x1470, 0x0002, 0x0000000f, 0x00000000 },
		{ 0x1474, 0x0002, 0x0000000f, 0x00000000 },
		{ 0x1478, 0x0002, 0x0000000f, 0x00000000 },
		{ 0x147c, 0x0002, 0x0000000f, 0x00000000 },

		/* Send BD Inititor Control Registers.*/
		{ 0x1800, 0x0000, 0x00000000, 0x00000006 },
		{ 0x1804, 0x0000, 0x00000004, 0x00000000 },
		{ 0x1808, 0x0000, 0xffffffff, 0x00000000 },
		{ 0x180c, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x1810, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x1814, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x1818, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x181c, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x1820, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x1824, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x1828, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x182c, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x1830, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x1834, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x1838, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x183c, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x1840, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x1844, 0x0002, 0xffffffff, 0x00000000 },

		/* Send BD Completion Control Registers */
		{ 0x1c00, 0x0000, 0x00000000, 0x00000002 },

		/* Receive List Placement Control Registers. */
		{ 0x2000, 0x0000, 0x00000000, 0x0000001e },
		{ 0x2004, 0x0000, 0x0000001c, 0x00000000 },
		{ 0x2010, 0x0002, 0x00000000, 0x00007fff },
		{ 0x2010, 0x0001, 0x00000000, 0x000060ff },	/* 5705 */
		{ 0x2014, 0x0000, 0x00000000, 0x00000001 },
		{ 0x2200, 0x0000, 0x000003ff, 0x00000000 },
		{ 0x2204, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x2208, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x220c, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x2210, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x2214, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x2218, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x221c, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x2220, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x2224, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x2228, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x222c, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x2230, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x2234, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x2238, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x223c, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x2240, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x2244, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x2248, 0x0002, 0x000003ff, 0x00000000 },
		{ 0x224c, 0x0000, 0x000003ff, 0x00000000 },
		{ 0x2250, 0x0000, 0x000003ff, 0x00000000 },
		{ 0x2254, 0x0000, 0x000003ff, 0x00000000 },
		{ 0x2258, 0x0002, 0x000003ff, 0x00000000 },

		/* Receive Data and Receive BD Initiator Control Registers. */
		{ 0x2400, 0x0002, 0x00000000, 0x0000001e },
		{ 0x2400, 0x0001, 0x00000000, 0x0000001a },	/* 5705 */
		{ 0x2404, 0x0000, 0x0000001c, 0x00000000 },
		{ 0x2408, 0x0002, 0x00000000, 0x0000ffff },
		{ 0x2440, 0x0002, 0x00000000, 0xffffffff },
		{ 0x2444, 0x0002, 0x00000000, 0xffffffff },
		{ 0x2448, 0x0002, 0x00000000, 0x00000003 },
		{ 0x244c, 0x0002, 0x00000000, 0xffffffff },
		{ 0x2450, 0x0000, 0x00000000, 0xffffffff },
		{ 0x2454, 0x0000, 0x00000000, 0xffffffff },
		{ 0x2458, 0x0000, 0x00000000, 0xffff0002 },
		{ 0x245c, 0x0000, 0x00000000, 0xffffffff },
		{ 0x2470, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x2474, 0x0000, 0xffffffff, 0x00000000 },
		{ 0x2478, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x2480, 0x0000, 0xffffffff, 0x00000000 },
		{ 0x2484, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x2488, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x248c, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x2490, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x2494, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x2498, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x249c, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x24a0, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x24a4, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x24a8, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x24ac, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x24b0, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x24b4, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x24b8, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x24bc, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x24c0, 0x0000, 0xffffffff, 0x00000000 },
	
		/* Receive Data Completion Control Registers */
		{ 0x2800, 0x0000, 0x00000000, 0x00000002 },

		/* Receive BD Initiator Control Registers. */
		{ 0x2c00, 0x0000, 0x00000000, 0x00000006 },
		{ 0x2c04, 0x0000, 0x00000004, 0x00000000 },
		{ 0x2c18, 0x0002, 0x00000000, 0xffffffff },
		{ 0x2c18, 0x0001, 0x00000000, 0x000003ff },	/* 5705 */
		{ 0x2c1c, 0x0002, 0x00000000, 0xffffffff },
	
		/* Receive BD Completion Control Registers. */
		{ 0x3000, 0x0000, 0x00000000, 0x00000006 },
		{ 0x3004, 0x0000, 0x00000004, 0x00000000 },
		{ 0x3008, 0x0002, 0x00000000, 0x000000ff },
		{ 0x300c, 0x0000, 0x00000000, 0x000001ff },

		/* Host Coalescing Control Registers. */
		{ 0x3c00, 0x0002, 0x00000000, 0x00000004 },
		{ 0x3c00, 0x0001, 0x00000000, 0x000000f6 },	/* 5705 */
		{ 0x3c04, 0x0000, 0x00000004, 0x00000000 },
		{ 0x3c08, 0x0002, 0x00000000, 0xffffffff },
		{ 0x3c08, 0x0001, 0x00000000, 0x000003ff },	/* 5705 */
		{ 0x3c0c, 0x0002, 0x00000000, 0xffffffff },
		{ 0x3c0c, 0x0001, 0x00000000, 0x000003ff },	/* 5705 */
		{ 0x3c10, 0x0002, 0x00000000, 0xffffffff },
		{ 0x3c10, 0x0005, 0x00000000, 0x000000ff },	/* 5705 */
		{ 0x3c14, 0x0002, 0x00000000, 0xffffffff },
		{ 0x3c14, 0x0005, 0x00000000, 0x000000ff },	/* 5705 */
		{ 0x3c18, 0x0002, 0x00000000, 0xffffffff },
		{ 0x3c1c, 0x0002, 0x00000000, 0xffffffff },
		{ 0x3c20, 0x0002, 0x00000000, 0xffffffff },
		{ 0x3c20, 0x0005, 0x00000000, 0x000000ff },	/* 5705 */
		{ 0x3c24, 0x0002, 0x00000000, 0xffffffff },
		{ 0x3c24, 0x0005, 0x00000000, 0x000000ff },	/* 5705 */
		{ 0x3c28, 0x0002, 0x00000000, 0xffffffff },
		{ 0x3c30, 0x0002, 0x00000000, 0xffffffff },
		{ 0x3c34, 0x0002, 0x00000000, 0xffffffff },
		{ 0x3c38, 0x0000, 0x00000000, 0xffffffff },
		{ 0x3c3c, 0x0000, 0x00000000, 0xffffffff },
		{ 0x3c40, 0x0000, 0xffffffff, 0x00000000 },
		{ 0x3c44, 0x0000, 0xffffffff, 0x00000000 },
		{ 0x3c50, 0x0002, 0x00000000, 0x000000ff },
		{ 0x3c54, 0x0000, 0x00000000, 0x000000ff },
		{ 0x3c80, 0x0002, 0x00000000, 0x000007ff },
		{ 0x3c80, 0x0001, 0x00000000, 0x000001ff },	/* 5705 */
		{ 0x3c84, 0x0002, 0x00000000, 0x000007ff },
		{ 0x3c88, 0x0002, 0x00000000, 0x000007ff },
		{ 0x3c8c, 0x0002, 0x00000000, 0x000007ff },
		{ 0x3c90, 0x0002, 0x00000000, 0x000007ff },
		{ 0x3c94, 0x0002, 0x00000000, 0x000007ff },
		{ 0x3c98, 0x0002, 0x00000000, 0x000007ff },
		{ 0x3c9c, 0x0002, 0x00000000, 0x000007ff },
		{ 0x3ca0, 0x0002, 0x00000000, 0x000007ff },
		{ 0x3ca4, 0x0002, 0x00000000, 0x000007ff },
		{ 0x3ca8, 0x0002, 0x00000000, 0x000007ff },
		{ 0x3cac, 0x0002, 0x00000000, 0x000007ff },
		{ 0x3cb0, 0x0002, 0x00000000, 0x000007ff },
		{ 0x3cb4, 0x0002, 0x00000000, 0x000007ff },
		{ 0x3cb8, 0x0002, 0x00000000, 0x000007ff },
		{ 0x3cbc, 0x0002, 0x00000000, 0x000007ff },
		{ 0x3cc0, 0x0000, 0x00000000, 0x000001ff },
		{ 0x3cc4, 0x0002, 0x00000000, 0x000001ff },
		{ 0x3cc8, 0x0002, 0x00000000, 0x000001ff },
		{ 0x3ccc, 0x0002, 0x00000000, 0x000001ff },
		{ 0x3cd0, 0x0002, 0x00000000, 0x000001ff },
		{ 0x3cd4, 0x0002, 0x00000000, 0x000001ff },
		{ 0x3cd8, 0x0002, 0x00000000, 0x000001ff },
		{ 0x3cdc, 0x0002, 0x00000000, 0x000001ff },
		{ 0x3ce0, 0x0002, 0x00000000, 0x000001ff },
		{ 0x3ce4, 0x0002, 0x00000000, 0x000001ff },
		{ 0x3ce8, 0x0002, 0x00000000, 0x000001ff },
		{ 0x3cec, 0x0002, 0x00000000, 0x000001ff },
		{ 0x3cf0, 0x0002, 0x00000000, 0x000001ff },
		{ 0x3cf4, 0x0002, 0x00000000, 0x000001ff },
		{ 0x3cf8, 0x0002, 0x00000000, 0x000001ff },
		{ 0x3cfc, 0x0002, 0x00000000, 0x000001ff },

		/* Memory Arbiter Registers */
		{ 0x4000, 0x0002, 0x00000000, 0x001ffffe },
		{ 0x4000, 0x0001, 0x00000000, 0x38111e7e },
		{ 0x4004, 0x0002, 0x001ffffc, 0x00000000 },
		{ 0x4004, 0x0002, 0x00111dfc, 0x00000000 },
		{ 0x4008, 0x0000, 0x00000000, 0x001fffff },
		{ 0x400c, 0x0000, 0x00000000, 0x001fffff },

		/* Buffer Manager Control Registers. */
		{ 0x4400, 0x0000, 0x00000000, 0x0000001c },
		{ 0x4404, 0x0000, 0x00000014, 0x00000000 },
		{ 0x4408, 0x0000, 0x00000000, 0x007fff80 },
		{ 0x440c, 0x0000, 0x00000000, 0x007fffff },
		{ 0x4410, 0x0000, 0x00000000, 0x0000003f },
		{ 0x4414, 0x0000, 0x00000000, 0x000001ff },
		{ 0x4418, 0x0000, 0x00000000, 0x000001ff },
		{ 0x4420, 0x0000, 0xffffffff, 0x00000000 },
		{ 0x4428, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x442c, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x4430, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x4440, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x4448, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x444c, 0x0000, 0xffffffff, 0x00000000 },
		{ 0x4450, 0x0000, 0xffffffff, 0x00000000 },
		{ 0x4454, 0x0000, 0xffffffff, 0x00000000 },
		{ 0x4458, 0x0001, 0x00000000, 0x000001ff },	/* 5705 */
	
		{ 0x4800, 0x0002, 0x00000000, 0x000003fe },
		{ 0x4800, 0x0001, 0x00000000, 0xc00003fe },	/* 5705 */
		{ 0x4804, 0x0000, 0x000003fc, 0x00000000 },
		{ 0x4c00, 0x0002, 0x00000000, 0x000003fc },
		{ 0x4c00, 0x0001, 0x00000000, 0x000007fc },	/* 5705 */
		{ 0x4c04, 0x0000, 0x000003fc, 0x00000000 },

		/* Mailbox Registers */
		{ 0x5804, 0x0000, 0x00000000, 0xffffffff },
		{ 0x586c, 0x0000, 0x00000000, 0x000001ff },
		{ 0x5874, 0x0002, 0x00000000, 0x000001ff },
		{ 0x5884, 0x0000, 0x00000000, 0x000007ff },
		{ 0x5904, 0x0000, 0x00000000, 0x000001ff },
		{ 0x5984, 0x0002, 0x00000000, 0x000001ff },
		{ 0x5a04, 0x0000, 0x00000000, 0xffffffff },
		{ 0x5a0c, 0x0000, 0x00000000, 0xffffffff },

		/* Flow Through Queues. */
		{ 0x5c14, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x5c24, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x5c34, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x5c44, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x5c54, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x5c64, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x5c74, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x5c84, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x5c94, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x5ca4, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x5cb4, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x5cc4, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x5cd4, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x5ce4, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x5cf4, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x5d04, 0x0002, 0xffffffff, 0x00000000 },
		{ 0x5d14, 0x0002, 0xffffffff, 0x00000000 },
		{ 0xffff, 0x0000, 0x00000000, 0x00000000 },
	};

    if (T3_ASIC_IS_5705_BEYOND(pDevice->ChipRevId)){
		bcm5705 = 1;
	}
	else {
		bcm5705 = 0;
	}

	ret = 1;
	for (i = 0; reg_tbl[i].offset != 0xffff; i++) {
		if (bcm5705 && (reg_tbl[i].flags & NOT_FOR_BCM5705))
			continue;
		if (!bcm5705 && (reg_tbl[i].flags & BCM5705_ONLY))
			continue;
		if ((pDevice->Flags & BCM5788_FLAG) &&
			(reg_tbl[i].flags & NOT_FOR_BCM5788))
			continue;
		offset = (LM_UINT32) reg_tbl[i].offset;
		read_mask = reg_tbl[i].read_mask;
		write_mask = reg_tbl[i].write_mask;

		/* Save the original register content */
		save_val = LM_RegRd(pDevice, offset);

		/* Determine the read-only value. */
		read_val = save_val & read_mask;

		/* Write zero to the register, then make sure the read-only bits
		   are not changed and the read/write bits are all zeros. */
		LM_RegWr(pDevice, offset, 0, FALSE);

		val = LM_RegRd(pDevice, offset);

		/* Test the read-only and read/write bits. */
		if (((val & read_mask) != read_val) ||
			(val & write_mask)) {

	                ret = 0;
			LM_RegWr(pDevice, offset, save_val, FALSE);
			break;
		}


		/* Write ones to all the bits defined by RdMask and WrMask, then
		   make sure the read-only bits are not changed and the
		   read/write bits are all ones. */
		LM_RegWr(pDevice, offset, read_mask | write_mask, FALSE);

		val = LM_RegRd(pDevice, offset);

		/* Test the read-only bits. */
		if ((val & read_mask) != read_val) {
	                ret = 0;
			LM_RegWr(pDevice, offset, save_val, FALSE);
			break;
		}

		/* Test the read/write bits. */
		if ((val & write_mask) != write_mask) {
	                ret = 0;
			LM_RegWr(pDevice, offset, save_val, FALSE);
			break;
		}

		LM_RegWr(pDevice, offset, save_val, FALSE);
	}

	return ret;
}


/* Returns 1 on success, 0 on failure */
int
b57_do_memory_test(LM_DEVICE_BLOCK *pDevice, LM_UINT32 start, LM_UINT32 size)
{
	const LM_UINT32 test_pattern[] = { 0x00000000, 0xffffffff, 0x55555555,
		0xaaaaaaaa , 0xaa55aa55, 0x55aa55aa };
	LM_UINT32 offset;
	int i;

	for (i = 0; i < sizeof(test_pattern)/sizeof(LM_UINT32); i++) {
		for (offset = 0; offset < size; offset += 4) {

			LM_MemWrInd(pDevice, start + offset, test_pattern[i]);

			if (LM_MemRdInd(pDevice, start + offset) !=
				test_pattern[i]) {
				return 0;
			}
		}
	}
	return 1;
}

/* Returns 1 on success, 0 on failure */
int
b57_test_memory(UM_DEVICE_BLOCK *pUmDevice)
{
	LM_DEVICE_BLOCK *pDevice = &pUmDevice->lm_dev;
	int ret = 0;
	int i;
	mem_entry_t *mem_tbl;

	static mem_entry_t mem_tbl_570x[] = {
		{ 0x00000000, 0x01000},
		{ 0x00002000, 0x1c000},
		{ 0xffffffff, 0x00000}
	};
	static mem_entry_t mem_tbl_5705[] = {
		{ 0x00000100, 0x0000c},
		{ 0x00000200, 0x00008},
		{ 0x00000b50, 0x00400},
		{ 0x00004000, 0x00800},
		{ 0x00006000, 0x01000},
		{ 0x00008000, 0x02000},
		{ 0x00010000, 0x0e000},
		{ 0xffffffff, 0x00000}
	};

    if (T3_ASIC_IS_5705_BEYOND(pDevice->ChipRevId)){
		mem_tbl = mem_tbl_5705;
	}
	else {
		mem_tbl = mem_tbl_570x;
	}
	for (i = 0; mem_tbl[i].offset != 0xffffffff; i++) {
		if ((ret = b57_do_memory_test(pDevice, mem_tbl[i].offset,
			mem_tbl[i].len)) == 0) {
			return ret;
		}
	}
	
	return ret;
}

#define EEPROM_SIZE 0x100

/* Returns 1 on success, 0 on failure */
int
b57_test_nvram(UM_DEVICE_BLOCK *pUmDevice)
{
	LM_DEVICE_BLOCK *pDevice = &pUmDevice->lm_dev;
	LM_UINT32 buf[EEPROM_SIZE/4];
	LM_UINT8 *pdata = (LM_UINT8 *) buf;
	int i;
	LM_UINT32 magic, csum;

	for (i = 0; i < EEPROM_SIZE; i += 4) {
		if (LM_NvramRead(pDevice, i, (LM_UINT32 *) (pdata + i)) !=
			LM_STATUS_SUCCESS) {
			break;
		}
	}
	if (i < EEPROM_SIZE) {
		return 0;
	}

        magic = MM_SWAP_BE32(buf[0]);
	if (magic != 0x669955aa) {
		return 0;
	}

	csum = ComputeCrc32(pdata, 16);
	if(csum != MM_SWAP_LE32(buf[0x10/4])) {
		return 0;
	}

	csum = ComputeCrc32(&pdata[0x74], 136);
	if (csum != MM_SWAP_LE32(buf[0xfc/4])) {
		return 0;
	}

	return 1;
}

/* Returns 1 on success, 0 on failure */
int
b57_test_link(UM_DEVICE_BLOCK *pUmDevice)
{
	LM_DEVICE_BLOCK *pDevice = &pUmDevice->lm_dev;
	LM_UINT32 phy_reg;

	if (pDevice->TbiFlags & ENABLE_TBI_FLAG) {
		if (REG_RD(pDevice, MacCtrl.Status) &
			(MAC_STATUS_PCS_SYNCED | MAC_STATUS_SIGNAL_DETECTED)) {
			return 1;
		}
		return 0;
	}
	LM_ReadPhy(pDevice, PHY_STATUS_REG, &phy_reg);
	LM_ReadPhy(pDevice, PHY_STATUS_REG, &phy_reg);
	if (phy_reg & PHY_STATUS_LINK_PASS)
		return 1;
	return 0;
}

#endif

#if defined(ETHTOOL_TEST) || defined(NICE_SUPPORT)

#if (LINUX_VERSION_CODE < 0x020605)
#define pci_dma_sync_single_for_cpu pci_dma_sync_single
#endif

/* Returns 1 on success, 0 on failure */
int
b57_test_loopback(UM_DEVICE_BLOCK *pUmDevice, int testtype, int linespeed)
{
	LM_DEVICE_BLOCK *pDevice = &pUmDevice->lm_dev;
	struct sk_buff *skb, *rx_skb;
	unsigned char *packet;
	dma_addr_t map;
	LM_UINT32 value32;
	LM_UINT32 send_idx, rx_start_idx, rx_idx;
	int num_pkts, pkt_size, i, ret;
	LM_PACKET *pPacket;
	UM_PACKET *pUmPacket;
	T3_SND_BD *pSendBd;
	T3_RCV_BD *pRcvBd;

	ret = 0;
	if (!pUmDevice->opened)
		return ret;
	LM_ResetAdapter(pDevice);
	LM_HaltCpu(pDevice,T3_RX_CPU_ID | T3_TX_CPU_ID);
	switch (testtype) {
		case NICE_LOOPBACK_TESTTYPE_MAC:
			LM_EnableMacLoopBack(pDevice);
			break;
		case NICE_LOOPBACK_TESTTYPE_PHY:
			LM_EnablePhyLoopBack(pDevice);
			break;
		case NICE_LOOPBACK_TESTTYPE_EXT:
			LM_EnableExtLoopBack(pDevice, linespeed);

			/* Wait 4 seconds for link to come up. */
			for (i = 0; i < 4; i++) {
				LM_ReadPhy(pDevice, PHY_STATUS_REG, &value32);
				LM_ReadPhy(pDevice, PHY_STATUS_REG, &value32);
				if (value32 & PHY_STATUS_LINK_PASS) {
					LM_SetupPhy(pDevice);
					break;
				}
				MM_Sleep(pDevice,1000);
			}
			if (!(value32 & PHY_STATUS_LINK_PASS))
				return ret;
	}
	pkt_size = 1514;
	skb = dev_alloc_skb(pkt_size);
	packet = skb_put(skb, pkt_size);
	memcpy(packet, pDevice->NodeAddress, 6);
	memset(packet + 6, 0x0, 8);

	for (i = 14; i < pkt_size; i++)
		packet[i] = (unsigned char) (i & 0xff);

	map = pci_map_single(pUmDevice->pdev, skb->data, pkt_size,
		PCI_DMA_TODEVICE);

	REG_WR(pDevice, HostCoalesce.Mode,
		pDevice->CoalesceMode | HOST_COALESCE_ENABLE |
			HOST_COALESCE_NOW);
	MM_Wait(10);
	rx_start_idx = pDevice->pStatusBlkVirt->Idx[0].RcvProdIdx;

	send_idx = 0;
	num_pkts = 0;
	pSendBd = &pDevice->pSendBdVirt[send_idx];
	if (pDevice->Flags & NIC_SEND_BD_FLAG) {
		T3_64BIT_HOST_ADDR HostAddr;

		MM_SetT3Addr(&HostAddr, map);
		MM_MEMWRITEL(&(pSendBd->HostAddr.High), HostAddr.High);
		MM_MEMWRITEL(&(pSendBd->HostAddr.Low), HostAddr.Low);
		MM_MEMWRITEL(&(pSendBd->u1.Len_Flags),
		             (pkt_size << 16) | SND_BD_FLAG_END);
		send_idx++;
		num_pkts++;
		MB_REG_WR(pDevice, Mailbox.SendNicProdIdx[0].Low, send_idx);
		if (T3_CHIP_REV(pDevice->ChipRevId) == T3_CHIP_REV_5700_BX) {
			MB_REG_WR(pDevice, Mailbox.SendNicProdIdx[0].Low,
				send_idx);
		}
		MB_REG_RD(pDevice, Mailbox.SendNicProdIdx[0].Low);
	}
	else {
		MM_SetT3Addr(&pSendBd->HostAddr, map);
                pSendBd->u1.Len_Flags = (pkt_size << 16) | SND_BD_FLAG_END;
		MM_WMB();
		send_idx++;
		num_pkts++;
	        MB_REG_WR(pDevice, Mailbox.SendHostProdIdx[0].Low, send_idx);
		if (T3_CHIP_REV(pDevice->ChipRevId) == T3_CHIP_REV_5700_BX) {
			MB_REG_WR(pDevice, Mailbox.SendHostProdIdx[0].Low,
				send_idx);
		}
	        MB_REG_RD(pDevice, Mailbox.SendHostProdIdx[0].Low);
	}

	MM_Wait(100);

	REG_WR(pDevice, HostCoalesce.Mode,
		pDevice->CoalesceMode | HOST_COALESCE_ENABLE |
			HOST_COALESCE_NOW);

	MM_Sleep(pDevice, 1000);

	pci_unmap_single(pUmDevice->pdev, map, pkt_size, PCI_DMA_TODEVICE);
	dev_kfree_skb_irq(skb);

	if (pDevice->pStatusBlkVirt->Idx[0].SendConIdx != send_idx) {
		goto loopback_test_done;
	}

	rx_idx = pDevice->pStatusBlkVirt->Idx[0].RcvProdIdx;
	if (rx_idx != rx_start_idx + num_pkts) {
		goto loopback_test_done;
	}

	pRcvBd = &pDevice->pRcvRetBdVirt[rx_start_idx];
	pPacket = (PLM_PACKET) (MM_UINT_PTR(pDevice->pPacketDescBase) +
		MM_UINT_PTR(pRcvBd->Opaque));

	pUmPacket = (UM_PACKET *) pPacket;

	if (pRcvBd->ErrorFlag &&
		pRcvBd->ErrorFlag != RCV_BD_ERR_ODD_NIBBLED_RCVD_MII) {
		goto loopback_test_done;
	}
	if ((pRcvBd->Len - 4) != pkt_size) {
		goto loopback_test_done;
	}
	rx_skb = pUmPacket->skbuff;

	pci_dma_sync_single_for_cpu(pUmDevice->pdev,
		pci_unmap_addr(pUmPacket, map[0]),
		pPacket->u.Rx.RxBufferSize,
		PCI_DMA_FROMDEVICE);

	for (i = 14; i < pkt_size; i++) {
		if (*(rx_skb->data + i) != (unsigned char) (i & 0xff)) {
			goto loopback_test_done;
		}
	}
	ret = 1;

loopback_test_done:
	switch (testtype) {
		case NICE_LOOPBACK_TESTTYPE_MAC:
			LM_DisableMacLoopBack(pDevice);
			break;
		case NICE_LOOPBACK_TESTTYPE_PHY:
			LM_DisablePhyLoopBack(pDevice);
			break;
		case NICE_LOOPBACK_TESTTYPE_EXT:
			LM_DisableExtLoopBack(pDevice);
			break;
	}
	return ret;
}
#endif

