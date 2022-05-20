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
#ifndef __I386_LITHIUM_H
#define __I386_LITHIUM_H

#include <linux/config.h>

/*
 * Lithium is the I/O ASIC on the SGI 320 and 540 Visual Workstations
 */

#define	LI_PCI_A_PHYS		0xfc000000	/* Enet is dev 3 */
#define	LI_PCI_B_PHYS		0xfd000000	/* PIIX4 is here */

/* see set_fixmap() and asm/fixmap.h */
#define LI_PCIA_VADDR   (fix_to_virt(FIX_LI_PCIA))
#define LI_PCIB_VADDR   (fix_to_virt(FIX_LI_PCIB))

/* Not a standard PCI? (not in linux/pci.h) */
#define	LI_PCI_BUSNUM	0x44			/* lo8: primary, hi8: sub */
#define LI_PCI_INTEN    0x46

#ifdef CONFIG_X86_VISWS_APIC
/* More special purpose macros... */
static __inline void li_pcia_write16(unsigned long reg, unsigned short v)
{
	*((volatile unsigned short *)(LI_PCIA_VADDR+reg))=v;
}

static __inline unsigned short li_pcia_read16(unsigned long reg)
{
	 return *((volatile unsigned short *)(LI_PCIA_VADDR+reg));
}

static __inline void li_pcib_write16(unsigned long reg, unsigned short v)
{
	*((volatile unsigned short *)(LI_PCIB_VADDR+reg))=v;
}

static __inline unsigned short li_pcib_read16(unsigned long reg)
{
	return *((volatile unsigned short *)(LI_PCIB_VADDR+reg));
}
#endif

#endif

