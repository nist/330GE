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
 * BK Id: SCCS/s.nvram.h 1.5 05/17/01 18:14:25 cort
 */
/*
 * PreP compliant NVRAM access
 */

#ifdef __KERNEL__
#ifndef _PPC_NVRAM_H
#define _PPC_NVRAM_H

#define NVRAM_AS0  0x74
#define NVRAM_AS1  0x75
#define NVRAM_DATA 0x77


/* RTC Offsets */

#define MOTO_RTC_SECONDS		0x1FF9
#define MOTO_RTC_MINUTES		0x1FFA
#define MOTO_RTC_HOURS		0x1FFB
#define MOTO_RTC_DAY_OF_WEEK		0x1FFC
#define MOTO_RTC_DAY_OF_MONTH	0x1FFD
#define MOTO_RTC_MONTH		0x1FFE
#define MOTO_RTC_YEAR		0x1FFF
#define MOTO_RTC_CONTROLA            0x1FF8
#define MOTO_RTC_CONTROLB            0x1FF9

#ifndef BCD_TO_BIN
#define BCD_TO_BIN(val) ((val)=((val)&15) + ((val)>>4)*10)
#endif

#ifndef BIN_TO_BCD
#define BIN_TO_BCD(val) ((val)=(((val)/10)<<4) + (val)%10)
#endif

/* PowerMac specific nvram stuffs */

enum {
	pmac_nvram_OF,		/* Open Firmware partition */
	pmac_nvram_XPRAM,	/* MacOS XPRAM partition */
	pmac_nvram_NR		/* MacOS Name Registry partition */
};

#ifdef __KERNEL__

/* Return partition offset in nvram */
extern int	pmac_get_partition(int partition);

/* Direct access to XPRAM */
extern u8	pmac_xpram_read(int xpaddr);
extern void	pmac_xpram_write(int xpaddr, u8 data);

#endif /* __KERNEL__ */

/* Some offsets in XPRAM */
#define PMAC_XPRAM_MACHINE_LOC	0xe4
#define PMAC_XPRAM_SOUND_VOLUME	0x08

/* Machine location structure in XPRAM */
struct pmac_machine_location {
	unsigned int	latitude;	/* 2+30 bit Fractional number */
	unsigned int	longitude;	/* 2+30 bit Fractional number */
	unsigned int	delta;		/* mix of GMT delta and DLS */
};

/* /dev/nvram ioctls */
#define PMAC_NVRAM_GET_OFFSET	_IOWR('p', 0x40, int) /* Get NVRAM partition offset */

#endif
#endif /* __KERNEL__ */
