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
 *	include/asm-mips/dec/kn05.h
 *
 *	DECstation 5000/260 (4max+ or KN05) and DECsystem 5900-260
 *	definitions.
 *
 *	Copyright (C) 2002  Maciej W. Rozycki
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 *
 *	WARNING!  All this information is pure guesswork based on the
 *	ROM.  It is provided here in hope it will give someone some
 *	food for thought.  No documentation for the KN05 module has
 *	been located so far.
 */
#ifndef __ASM_MIPS_DEC_KN05_H
#define __ASM_MIPS_DEC_KN05_H

#include <asm/dec/ioasic_addrs.h>

/*
 * The oncard MB (Memory Buffer) ASIC provides an additional address
 * decoder.  Certain address ranges within the "high" 16 slots are
 * passed to the I/O ASIC's decoder like with the KN03.  Others are
 * handled locally.  "Low" slots are always passed.
 */
#define KN05_MB_ROM	(16*IOASIC_SLOT_SIZE)	/* KN05 card ROM */
#define KN05_IOCTL	(17*IOASIC_SLOT_SIZE)	/* I/O ASIC */
#define KN05_ESAR	(18*IOASIC_SLOT_SIZE)	/* LANCE MAC address chip */
#define KN05_LANCE	(19*IOASIC_SLOT_SIZE)	/* LANCE Ethernet */
#define KN05_MB_INT	(20*IOASIC_SLOT_SIZE)	/* MB interrupt register? */
#define KN05_MB_UNKN_0	(21*IOASIC_SLOT_SIZE)	/* MB unknown register */
#define KN05_MB_UNKN_1	(22*IOASIC_SLOT_SIZE)	/* MB unknown register */
#define KN05_MB_CSR	(23*IOASIC_SLOT_SIZE)	/* MB control & status */
#define KN05_RESERVED_0	(24*IOASIC_SLOT_SIZE)	/* unused? */
#define KN05_RESERVED_1	(25*IOASIC_SLOT_SIZE)	/* unused? */
#define KN05_RESERVED_2	(26*IOASIC_SLOT_SIZE)	/* unused? */
#define KN05_RESERVED_3	(27*IOASIC_SLOT_SIZE)	/* unused? */
#define KN05_SCSI	(28*IOASIC_SLOT_SIZE)	/* ASC SCSI */
#define KN05_RESERVED_4	(29*IOASIC_SLOT_SIZE)	/* unused? */
#define KN05_RESERVED_5	(30*IOASIC_SLOT_SIZE)	/* unused? */
#define KN05_RESERVED_6	(31*IOASIC_SLOT_SIZE)	/* unused? */

/*
 * Bits for the MB interrupt (?) register.
 * The register appears read-only.
 */
#define KN05_MB_INT_TC		(1<<0)		/* TURBOchannel? */
#define KN05_MB_INT_RTC		(1<<1)		/* RTC? */

/*
 * Bits for the MB control & status register.
 * Set to 0x00bf8001 on my system by the ROM.
 */
#define KN05_MB_CSR_PF		(1<<0)		/* ??? */
#define KN05_MB_CSR_F		(1<<1)		/* ??? */
#define KN05_MB_CSR_ECC		(0xff<<2)	/* ??? */
#define KN05_MB_CSR_OD		(1<<10)		/* ??? */
#define KN05_MB_CSR_CP		(1<<11)		/* ??? */
#define KN05_MB_CSR_UNC		(1<<12)		/* ??? */
#define KN05_MB_CSR_IM		(1<<13)		/* ??? */
#define KN05_MB_CSR_NC		(1<<14)		/* ??? */
#define KN05_MB_CSR_EE		(1<<15)		/* (bus) Exception Enable? */
#define KN05_MB_CSR_MSK		(0x1f<<16)	/* ??? */
#define KN05_MB_CSR_FW		(1<<21)		/* ??? */

#endif /* __ASM_MIPS_DEC_KN05_H */
