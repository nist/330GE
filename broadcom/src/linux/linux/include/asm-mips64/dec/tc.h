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
 * Interface to the TURBOchannel related routines
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (c) 1998 Harald Koerfgen
 */
#ifndef ASM_TC_H
#define ASM_TC_H

extern unsigned long system_base;

/*
 * Search for a TURBOchannel Option Module
 * with a certain name. Returns slot number
 * of the first card not in use or -ENODEV
 * if none found.
 */
extern int search_tc_card(const char *);
/*
 * Marks the card in slot as used
 */
extern void claim_tc_card(int);
/*
 * Marks the card in slot as free
 */
extern void release_tc_card(int);
/*
 * Return base address of card in slot
 */
extern unsigned long get_tc_base_addr(int);
/*
 * Return interrupt number of slot
 */
extern unsigned long get_tc_irq_nr(int);
/*
 * Return TURBOchannel clock frequency in hz
 */
extern unsigned long get_tc_speed(void);

#endif
