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
#ifndef ASM_PCI_DIRECT_H
#define ASM_PCI_DIRECT_H 1

#include <linux/types.h>
#include <asm/io.h>

/* Direct PCI access. This is used for PCI accesses in early boot before
   the PCI subsystem works. */ 

#define PDprintk(x...)

static inline u32 read_pci_config(u8 bus, u8 slot, u8 func, u8 offset)
{
	u32 v; 
	outl(0x80000000 | (bus<<16) | (slot<<11) | (func<<8) | offset, 0xcf8);
	v = inl(0xcfc); 
	PDprintk("%x reading from %x: %x\n", slot, offset, v);
	return v;
}

static inline void write_pci_config(u8 bus, u8 slot, u8 func, u8 offset,
				    u32 val)
{
	PDprintk("%x writing to %x: %x\n", slot, offset, val); 
	outl(0x80000000 | (bus<<16) | (slot<<11) | (func<<8) | offset, 0xcf8);
	outl(val, 0xcfc); 
}

#endif
