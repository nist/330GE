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
#ifndef __ASM_SH_HD64461
#define __ASM_SH_HD64461
/*
 *	$Id: hd64461.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $
 *	Copyright (C) 2000 YAEGASHI Takeshi
 *	Hitachi HD64461 companion chip support
 */
#include <linux/config.h>

#define HD64461_STBCR	0x10000
#define HD64461_SYSCR	0x10002
#define HD64461_SCPUCR	0x10004

#define HD64461_LCDCBAR		0x11000
#define HD64461_LCDCLOR		0x11002
#define HD64461_LCDCCRR		0x11004
#define	HD64461_LDR1		0x11010
#define	HD64461_LDR2		0x11012
#define	HD64461_LDHNCR		0x11014
#define	HD64461_LDHNSR		0x11016
#define HD64461_LDVNTR		0x11018
#define HD64461_LDVNDR		0x1101a
#define HD64461_LDVSPR		0x1101c
#define HD64461_LDR3		0x1101e

#define HD64461_CPTWAR		0x11030	
#define HD64461_CPTWDR		0x11032
#define HD64461_CPTRAR		0x11034	
#define HD64461_CPTRDR		0x11036

#define HD64461_PCC0ISR         0x12000
#define HD64461_PCC0GCR         0x12002
#define HD64461_PCC0CSCR        0x12004
#define HD64461_PCC0CSCIER      0x12006
#define HD64461_PCC0SCR         0x12008
#define HD64461_PCC1ISR         0x12010
#define HD64461_PCC1GCR         0x12012
#define HD64461_PCC1CSCR        0x12014
#define HD64461_PCC1CSCIER      0x12016
#define HD64461_PCC1SCR         0x12018
#define HD64461_P0OCR           0x1202a
#define HD64461_P1OCR           0x1202c
#define HD64461_PGCR            0x1202e

#define HD64461_GPACR		0x14000
#define HD64461_GPBCR		0x14002
#define HD64461_GPCCR		0x14004
#define HD64461_GPDCR		0x14006
#define HD64461_GPADR		0x14010
#define HD64461_GPBDR		0x14012
#define HD64461_GPCDR		0x14014
#define HD64461_GPDDR		0x14016
#define HD64461_GPAICR		0x14020
#define HD64461_GPBICR		0x14022
#define HD64461_GPCICR		0x14024
#define HD64461_GPDICR		0x14026
#define HD64461_GPAISR		0x14040
#define HD64461_GPBISR		0x14042
#define HD64461_GPCISR		0x14044
#define HD64461_GPDISR		0x14046

#define HD64461_NIRR		0x15000
#define HD64461_NIMR		0x15002

#ifndef CONFIG_HD64461_IOBASE
#define CONFIG_HD64461_IOBASE	0xb0000000
#endif
#ifndef CONFIG_HD64461_IRQ
#define CONFIG_HD64461_IRQ	36
#endif

#define HD64461_IRQBASE		OFFCHIP_IRQ_BASE
#define HD64461_IRQ_NUM 	16

#endif
