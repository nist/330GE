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
/* $Id: idprom.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $
 * idprom.h: Macros and defines for idprom routines
 *
 * Copyright (C) 1996 David S. Miller (davem@caip.rutgers.edu)
 */

#ifndef _SPARC64_IDPROM_H
#define _SPARC64_IDPROM_H

#include <linux/types.h>

/* Offset into the EEPROM where the id PROM is located on the 4c */
#define IDPROM_OFFSET  0x7d8

/* On sun4m; physical. */
/* MicroSPARC(-II) does not decode 31rd bit, but it works. */
#define IDPROM_OFFSET_M  0xfd8

struct idprom
{
	u8		id_format;	/* Format identifier (always 0x01) */
	u8		id_machtype;	/* Machine type */
	u8		id_ethaddr[6];	/* Hardware ethernet address */
	s32		id_date;	/* Date of manufacture */
	u32		id_sernum:24;	/* Unique serial number */
	u8		id_cksum;	/* Checksum - xor of the data bytes */
	u8		reserved[16];
};

extern struct idprom *idprom;
extern void idprom_init(void);

#define IDPROM_SIZE  (sizeof(struct idprom))

#endif /* !(_SPARC_IDPROM_H) */
