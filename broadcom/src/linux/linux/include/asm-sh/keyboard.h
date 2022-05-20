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
#ifndef	__ASM_SH_KEYBOARD_H
#define	__ASM_SH_KEYBOARD_H
/*
 *	$Id: keyboard.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $
 */

#include <linux/kd.h>
#include <linux/config.h>
#include <asm/machvec.h>

#ifdef CONFIG_SH_EC3104
#include <asm/keyboard-ec3104.h>
#else
static __inline__ int kbd_setkeycode(unsigned int scancode,
				     unsigned int keycode)
{
    return -EOPNOTSUPP;
}

static __inline__ int kbd_getkeycode(unsigned int scancode)
{
    return scancode > 127 ? -EINVAL : scancode;
}

#ifdef CONFIG_SH_DREAMCAST
extern int kbd_translate(unsigned char scancode, unsigned char *keycode,
			 char raw_mode);
#else
static __inline__ int kbd_translate(unsigned char scancode,
				    unsigned char *keycode, char raw_mode)
{
    *keycode = scancode;
    return 1;
}
#endif

static __inline__ char kbd_unexpected_up(unsigned char keycode)
{
    return 0200;
}

static __inline__ void kbd_leds(unsigned char leds)
{
}

extern void hp600_kbd_init_hw(void);
extern void dreamcast_kbd_init_hw(void);

static __inline__ void kbd_init_hw(void)
{
	if (MACH_HP600) {
		hp600_kbd_init_hw();
	}
}

#endif
#endif
