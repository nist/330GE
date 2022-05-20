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
 * eisa_bus.h interface between the eisa BA driver and the bus enumerator
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 *
 * Copyright (c) 2002 Daniel Engstrom <5116@telia.com>
 *
 */

#ifndef ASM_EISA_H
#define ASM_EISA_H

extern void eisa_make_irq_level(int num);
extern void eisa_make_irq_edge(int num);
extern int eisa_enumerator(unsigned long eeprom_addr,
			   struct resource *io_parent, 
			   struct resource *mem_parent);
extern int eisa_eeprom_init(unsigned long addr);

#endif
