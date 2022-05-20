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
/* $Id: keyboard.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $
 * linux/include/asm-sparc64/keyboard.h
 *
 * Created Aug 29 1997 by Eddie C. Dost (ecd@skynet.be)
 */

/*
 *  This file contains the Ultra/PCI architecture specific keyboard definitions
 */

#ifndef _SPARC64_KEYBOARD_H
#define _SPARC64_KEYBOARD_H 1

#ifdef __KERNEL__

#include <linux/kernel.h>
#include <linux/kd.h>

#define KEYBOARD_IRQ			1
#define DISABLE_KBD_DURING_INTERRUPTS	0

extern int pcikbd_setkeycode(unsigned int scancode, unsigned int keycode);
extern int pcikbd_getkeycode(unsigned int scancode);
extern int pcikbd_translate(unsigned char scancode, unsigned char *keycode,
			    char raw_mode);
extern char pcikbd_unexpected_up(unsigned char keycode);
extern void pcikbd_leds(unsigned char leds);
extern void pcikbd_init_hw(void);
extern unsigned char pcikbd_sysrq_xlate[128];

#define kbd_setkeycode			pcikbd_setkeycode
#define kbd_getkeycode			pcikbd_getkeycode
#define kbd_translate			pcikbd_translate
#define kbd_unexpected_up		pcikbd_unexpected_up
#define kbd_leds			pcikbd_leds
#define kbd_init_hw			pcikbd_init_hw
#define kbd_sysrq_xlate			pcikbd_sysrq_xlate
#define kbd_init			pcikbd_init

#define compute_shiftstate		pci_compute_shiftstate
#define getkeycode			pci_getkeycode
#define setkeycode			pci_setkeycode
#define getledstate			pci_getledstate
#define setledstate			pci_setledstate
#define register_leds			pci_register_leds

#define SYSRQ_KEY 0x54

#endif /* __KERNEL__ */

#endif /* !(_SPARC64_KEYBOARD_H) */
