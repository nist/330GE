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
 *  linux/include/asm-arm/arch-sa1100/keyboard.h
 *  Created 16 Dec 1999 by Nicolas Pitre <nico@cam.org>
 *  This file contains the SA1100 architecture specific keyboard definitions
 */
#ifndef _SA1100_KEYBOARD_H
#define _SA1100_KEYBOARD_H

#include <linux/config.h>
#include <asm/mach-types.h>
#include <asm/arch/assabet.h>

#define kbd_disable_irq()	do { } while(0);
#define kbd_enable_irq()	do { } while(0);

extern int sa1111_kbd_init_hw(void);
extern void gc_kbd_init_hw(void);
extern void smartio_kbd_init_hw(void);
extern void cerf_kbd_init_hw(void);

static inline void kbd_init_hw(void)
{
	if ((machine_is_assabet() && machine_has_neponset()) ||
	    machine_is_graphicsmaster())
		sa1111_kbd_init_hw();
	if (machine_is_graphicsclient())
		gc_kbd_init_hw();
	if (machine_is_adsbitsy())
		smartio_kbd_init_hw();
#ifdef CONFIG_SA1100_CERF_CPLD
	if (machine_is_cerf())
		cerf_kbd_init_hw();
#endif
#ifdef CONFIG_SA1100_PT_SYSTEM3
	/* TODO: add system 3 board specific functions here */
	if (machine_is_pt_system3())
		sa1111_kbd_init_hw();
#endif
}

#endif  /* _SA1100_KEYBOARD_H */
