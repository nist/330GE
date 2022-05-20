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
extern unsigned char ec3104_kbd_sysrq_xlate[];
extern int ec3104_kbd_setkeycode(unsigned int scancode, unsigned int keycode);
extern int ec3104_kbd_getkeycode(unsigned int scancode);
extern int ec3104_kbd_translate(unsigned char, unsigned char *, char);
extern char ec3104_kbd_unexpected_up(unsigned char);
extern void ec3104_kbd_leds(unsigned char);
extern void ec3104_kbd_init_hw(void);

#define SYSRQ_KEY 0x54

#define kbd_sysrq_xlate ec3104_kbd_sysrq_xlate
#define kbd_setkeycode ec3104_kbd_setkeycode
#define kbd_getkeycode ec3104_kbd_getkeycode
#define kbd_translate ec3104_kbd_translate
#define kbd_unexpected_up ec3104_kbd_unexpected_up
#define kbd_leds ec3104_kbd_leds
#define kbd_init_hw ec3104_kbd_init_hw
