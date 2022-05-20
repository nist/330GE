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
 * BK Id: SCCS/s.ohare.h 1.8 12/01/01 20:09:11 benh
 */
/*
 * ohare.h: definitions for using the "O'Hare" I/O controller chip.
 *
 * Copyright (C) 1997 Paul Mackerras.
 * 
 * BenH: Changed to match those of heathrow (but not all of them). Please
 *       check if I didn't break anything (especially the media bay).
 */

/* offset from ohare base for feature control register */
#define OHARE_MBCR	0x34
#define OHARE_FCR	0x38

/*
 * Bits in feature control register.
 * These were mostly derived by experiment on a powerbook 3400
 * and may differ for other machines.
 */
#define OH_SCC_RESET		1
#define OH_BAY_POWER_N		2	/* a guess */
#define OH_BAY_PCI_ENABLE	4	/* a guess */
#define OH_BAY_IDE_ENABLE	8
#define OH_BAY_FLOPPY_ENABLE	0x10
#define OH_IDE0_ENABLE		0x20
#define OH_IDE0_RESET_N		0x40	/* a guess */
#define OH_BAY_DEV_MASK		0x1c
#define OH_BAY_RESET_N		0x80
#define OH_IOBUS_ENABLE		0x100	/* IOBUS seems to be IDE */
#define OH_SCC_ENABLE		0x200
#define OH_MESH_ENABLE		0x400
#define OH_FLOPPY_ENABLE	0x800
#define OH_SCCA_IO		0x4000
#define OH_SCCB_IO		0x8000
#define OH_VIA_ENABLE		0x10000	/* Is apparently wrong, to be verified */
#define OH_IDE1_RESET_N		0x800000

/*
 * Bits to set in the feature control register on PowerBooks.
 */
#define PBOOK_FEATURES		(OH_IDE_ENABLE | OH_SCC_ENABLE | \
				 OH_MESH_ENABLE | OH_SCCA_IO | OH_SCCB_IO)

/*
 * A magic value to put into the feature control register of the
 * "ohare" I/O controller on Starmaxes to enable the IDE CD interface.
 * Contributed by Harry Eaton.
 */
#define STARMAX_FEATURES	0xbeff7a
