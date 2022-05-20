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
/* $Id: msi.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $
 * msi.h:  Defines specific to the MBus - Sbus - Interface.
 *
 * Copyright (C) 1996 David S. Miller (davem@caip.rutgers.edu)
 * Copyright (C) 1996 Eddie C. Dost   (ecd@skynet.be)
 */

#ifndef _SPARC_MSI_H
#define _SPARC_MSI_H

/*
 * Locations of MSI Registers.
 */
#define MSI_MBUS_ARBEN	0xe0001008	/* MBus Arbiter Enable register */

/*
 * Useful bits in the MSI Registers.
 */
#define MSI_ASYNC_MODE  0x80000000	/* Operate the MSI asynchronously */


extern __inline__ void msi_set_sync(void)
{
	__asm__ __volatile__ ("lda [%0] %1, %%g3\n\t"
			      "andn %%g3, %2, %%g3\n\t"
			      "sta %%g3, [%0] %1\n\t" : :
			      "r" (MSI_MBUS_ARBEN),
			      "i" (ASI_M_CTL), "r" (MSI_ASYNC_MODE) : "g3");
}

#endif /* !(_SPARC_MSI_H) */
