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
/* $Id: eicon_pci.h,v 1.1.1.1 2007/01/25 12:51:54 jiahao_jhou Exp $
 *
 * ISDN low-level module for Eicon active ISDN-Cards (PCI part).
 *
 * Copyright 1998-2000 by Armin Schindler (mac@melware.de)
 * Copyright 1999,2000 Cytronics & Melware (info@melware.de)
 *
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

#ifndef eicon_pci_h
#define eicon_pci_h

#ifdef __KERNEL__

/*
 * card's description
 */
typedef struct {
	int   		  irq;	    /* IRQ		          */
	int		  channels; /* No. of supported channels  */
        void*             card;
        unsigned char     type;     /* card type                  */
        unsigned char     master;   /* Flag: Card is Quadro 1/4   */
} eicon_pci_card;

extern int eicon_pci_find_card(char *ID);

#endif  /* __KERNEL__ */

#endif	/* eicon_pci_h */

