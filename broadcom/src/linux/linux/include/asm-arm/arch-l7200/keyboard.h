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
 *  linux/include/asm-arm/arch-l7200/keyboard.h
 *
 *  Keyboard driver definitions for LinkUp Systems L7200 architecture
 *
 *  Copyright (C) 2000 Scott A McConnell (samcconn@cotw.com)
 *                     Steve Hill (sjhill@cotw.com)
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License. See the file COPYING in the main directory of this archive for
 *  more details.
 *
 * Changelog:
 *   07-18-2000	SAM	Created file
 *   07-28-2000	SJH	Complete rewrite
 */

#include <asm/irq.h>

/*
 * Layout of L7200 keyboard registers
 */
struct KBD_Port {       
	unsigned int KBDR;
	unsigned int KBDMR;
	unsigned int KBSBSR;
	unsigned int Reserved;
	unsigned int KBKSR;
};

#define KBD_BASE        IO_BASE_2 + 0x4000
#define l7200kbd_hwregs ((volatile struct KBD_Port *) (KBD_BASE))

extern void l7200kbd_init_hw(void);
extern int l7200kbd_translate(unsigned char scancode, unsigned char *keycode,
			      char raw_mode);

#define kbd_setkeycode(sc,kc)		(-EINVAL)
#define kbd_getkeycode(sc)		(-EINVAL)

#define kbd_translate(sc, kcp, rm)      ({ *(kcp) = (sc); 1; })
#define kbd_unexpected_up(kc)           (0200)
#define kbd_leds(leds)                  do {} while (0)
#define kbd_init_hw()                   l7200kbd_init_hw()
#define kbd_sysrq_xlate                 ((unsigned char *)NULL)
#define kbd_disable_irq()               disable_irq(IRQ_GCTC2)
#define kbd_enable_irq()                enable_irq(IRQ_GCTC2)

#define SYSRQ_KEY	13
