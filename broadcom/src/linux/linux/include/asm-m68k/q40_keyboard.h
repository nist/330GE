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
 *  linux/include/asm-m68k/q40_keyboard.h
 *
 *  Q40 specific keyboard definitions
 */


#ifdef __KERNEL__


#include <asm/machdep.h>



extern int q40kbd_setkeycode(unsigned int scancode, unsigned int keycode);
extern int q40kbd_getkeycode(unsigned int scancode);
extern int q40kbd_pretranslate(unsigned char scancode, char raw_mode);
extern int q40kbd_translate(unsigned char scancode, unsigned char *keycode,
			   char raw_mode);
extern char q40kbd_unexpected_up(unsigned char keycode);
extern void q40kbd_leds(unsigned char leds);
extern int q40kbd_is_sysrq(unsigned char keycode);
extern int q40kbd_init_hw(void);
extern unsigned char q40kbd_sysrq_xlate[128];



#endif /* __KERNEL__ */





