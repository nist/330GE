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
 * linux/include/asm-arm/arch-clps711x/keyboard.h
 *
 * Copyright (C) 1998-2001 Russell King
 */
#include <asm/mach-types.h>

#define NR_SCANCODES 128

#define kbd_disable_irq()	do { } while (0)
#define kbd_enable_irq()	do { } while (0)

/*
 * EDB7211 keyboard driver
 */
extern void edb7211_kbd_init_hw(void);
extern void clps711x_kbd_init_hw(void);

static inline void kbd_init_hw(void)
{
	if (machine_is_edb7211())
		edb7211_kbd_init_hw();

	if (machine_is_autcpu12())
		clps711x_kbd_init_hw();
}
