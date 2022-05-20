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
 *  linux/include/asm-m68k/keyboard.h
 *
 *  Created 3 Nov 1996 by Geert Uytterhoeven
 */

/*
 *  This file contains the m68k architecture specific keyboard definitions
 */

#ifndef __M68K_KEYBOARD_H
#define __M68K_KEYBOARD_H

#ifdef __KERNEL__

#include <linux/config.h>
#include <linux/kd.h>
#include <asm/machdep.h>

#ifdef CONFIG_Q40
#include <asm/q40_keyboard.h>
#endif

static __inline__ int kbd_setkeycode(unsigned int scancode,
				     unsigned int keycode)
{
#ifdef CONFIG_Q40
    if (MACH_IS_Q40)
        return q40kbd_setkeycode(scancode,keycode);
#endif
    return -EOPNOTSUPP;
}

static __inline__ int kbd_getkeycode(unsigned int scancode)
{
#ifdef CONFIG_Q40
    if (MACH_IS_Q40)
        return q40kbd_getkeycode(scancode);
#endif
    return scancode > 127 ? -EINVAL : scancode;
}

static __inline__ char kbd_unexpected_up(unsigned char keycode)
{
#ifdef CONFIG_Q40
    if (MACH_IS_Q40)
        return q40kbd_unexpected_up(keycode);
#endif
    return 0200;
}

static __inline__ void kbd_leds(unsigned char leds)
{
    if (mach_kbd_leds)
	mach_kbd_leds(leds);
}

#define kbd_init_hw		mach_keyb_init
#define kbd_translate		mach_kbd_translate
#define kbd_rate		mach_kbdrate

#define kbd_sysrq_xlate		mach_sysrq_xlate

/* resource allocation */
#define kbd_request_region()
#define kbd_request_irq(handler)

extern unsigned int SYSRQ_KEY;

#endif /* __KERNEL__ */

#endif /* __M68K_KEYBOARD_H */
