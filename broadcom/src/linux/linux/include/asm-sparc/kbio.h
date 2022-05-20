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
#ifndef __LINUX_KBIO_H
#define __LINUX_KBIO_H

/* Return keyboard type */
#define KIOCTYPE    _IOR('k', 9, int)
/* Return Keyboard layout */
#define KIOCLAYOUT  _IOR('k', 20, int)

enum {
    TR_NONE,
    TR_ASCII,			/* keyboard is in regular state */
    TR_EVENT,			/* keystrokes sent as firm events */
    TR_UNTRANS_EVENT		/* EVENT+up and down+no translation */
};

/* Return the current keyboard translation */
#define KIOCGTRANS  _IOR('k', 5, int)
/* Set the keyboard translation */
#define KIOCTRANS   _IOW('k', 0, int)

/* Send a keyboard command */
#define KIOCCMD     _IOW('k', 8, int)

/* Return if keystrokes are being sent to /dev/kbd */

/* Set routing of keystrokes to /dev/kbd */
#define KIOCSDIRECT _IOW('k', 10, int)

/* Set keyboard leds */
#define KIOCSLED    _IOW('k', 14, unsigned char)

/* Get keyboard leds */
#define KIOCGLED    _IOR('k', 15, unsigned char)

/* Used by KIOC[GS]RATE */
struct kbd_rate {
	unsigned char delay;	/* Delay in Hz before first repeat.	*/
	unsigned char rate;	/* In characters per second (0..50).	*/
};

/* Set keyboard rate */
#define KIOCSRATE   _IOW('k', 40, struct kbd_rate)

/* Get keyboard rate */
#define KIOCGRATE   _IOW('k', 41, struct kbd_rate)

/* Top bit records if the key is up or down */
#define KBD_UP      0x80

/* Usable information */
#define KBD_KEYMASK 0x7f

/* All keys up */
#define KBD_IDLE    0x75

#endif /* __LINUX_KBIO_H */
