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
 *  linux/include/asm-arm/arch-arc/keyboard.h
 *
 *  Copyright (C) 1998-2001 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Keyboard driver definitions for Acorn Archimedes/A5000
 *  architecture
 */
#include <asm/irq.h>

#define NR_SCANCODES 128

extern void a5kkbd_init_hw(void);

#define kbd_disable_irq()	disable_irq(IRQ_KEYBOARDRX)
#define kbd_enable_irq()	enable_irq(IRQ_KEYBOARDRX)
#define kbd_init_hw()		a5kkbd_init_hw()