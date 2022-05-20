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
 * linux/include/asm-arm/arch-ebsa285/keyboard.h
 *
 * Keyboard driver definitions for EBSA285 architecture
 *
 * Copyright (C) 1998-2001 Russell King
 * (C) 1998 Phil Blundell
 */
#include <linux/config.h>
#include <linux/ioport.h>
#include <asm/irq.h>
#include <asm/system.h>

#define KEYBOARD_IRQ		IRQ_ISA_KEYBOARD
#define NR_SCANCODES		128

#define kbd_disable_irq()	do { } while (0)
#define kbd_enable_irq()	do { } while (0)

extern int pckbd_setkeycode(unsigned int scancode, unsigned int keycode);
extern int pckbd_getkeycode(unsigned int scancode);
extern int pckbd_translate(unsigned char scancode, unsigned char *keycode,
			   char raw_mode);
extern char pckbd_unexpected_up(unsigned char keycode);
extern void pckbd_leds(unsigned char leds);
extern void pckbd_init_hw(void);
extern unsigned char pckbd_sysrq_xlate[128];

static inline void kbd_init_hw(void)
{
	if (have_isa_bridge) {
		k_setkeycode    = pckbd_setkeycode;
		k_getkeycode    = pckbd_getkeycode;
		k_translate     = pckbd_translate;
		k_unexpected_up = pckbd_unexpected_up;
		k_leds          = pckbd_leds;
#ifdef CONFIG_MAGIC_SYSRQ
		k_sysrq_key     = 0x54;
		k_sysrq_xlate   = pckbd_sysrq_xlate;
#endif
		pckbd_init_hw();
	}
}


/*
 * The rest of this file is to do with supporting pc_keyb.c
 */

/* resource allocation */
#define kbd_request_region()	request_region(0x60, 16, "keyboard")
#define kbd_request_irq(handler) request_irq(KEYBOARD_IRQ, handler, 0, \
					     "keyboard", NULL)

/* How to access the keyboard macros on this platform.  */
#define kbd_read_input() inb(KBD_DATA_REG)
#define kbd_read_status() inb(KBD_STATUS_REG)
#define kbd_write_output(val) outb(val, KBD_DATA_REG)
#define kbd_write_command(val) outb(val, KBD_CNTL_REG)

/* Some stoneage hardware needs delays after some operations.  */
#define kbd_pause() do { } while(0)

#define aux_request_irq(hand, dev_id)					\
	request_irq(AUX_IRQ, hand, SA_SHIRQ, "PS/2 Mouse", dev_id)

#define aux_free_irq(dev_id) free_irq(AUX_IRQ, dev_id)

