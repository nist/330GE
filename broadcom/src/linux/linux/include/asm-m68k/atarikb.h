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
** atarikb.h -- This header contains the prototypes of functions of
**              the intelligent keyboard of the Atari needed by the
**              mouse and joystick drivers.
**
** Copyright 1994 by Robert de Vries
**
** This file is subject to the terms and conditions of the GNU General Public
** License.  See the file COPYING in the main directory of this archive
** for more details.
**
** Created: 20 Feb 1994 by Robert de Vries
*/

#ifndef _LINUX_ATARIKB_H
#define _LINUX_ATARIKB_H

void ikbd_write(const char *, int);
void ikbd_mouse_button_action(int mode);
void ikbd_mouse_rel_pos(void);
void ikbd_mouse_abs_pos(int xmax, int ymax);
void ikbd_mouse_kbd_mode(int dx, int dy);
void ikbd_mouse_thresh(int x, int y);
void ikbd_mouse_scale(int x, int y);
void ikbd_mouse_pos_get(int *x, int *y);
void ikbd_mouse_pos_set(int x, int y);
void ikbd_mouse_y0_bot(void);
void ikbd_mouse_y0_top(void);
void ikbd_mouse_disable(void);
void ikbd_joystick_event_on(void);
void ikbd_joystick_event_off(void);
void ikbd_joystick_get_state(void);
void ikbd_joystick_disable(void);

/* Hook for MIDI serial driver */
extern void (*atari_MIDI_interrupt_hook) (void);
/* Hook for mouse driver */
extern void (*atari_mouse_interrupt_hook) (char *);

#endif /* _LINUX_ATARIKB_H */
