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
#include <linux/module.h>

#include <asm/ptrace.h>
#include <asm/traps.h>
#include <asm/atarihw.h>
#include <asm/atariints.h>
#include <asm/atarikb.h>
#include <asm/atari_joystick.h>
#include <asm/atari_stdma.h>
#include <asm/atari_stram.h>

extern void atari_microwire_cmd( int cmd );
extern int atari_MFP_init_done;
extern int atari_SCC_init_done;
extern int atari_SCC_reset_done;

EXPORT_SYMBOL(atari_mch_cookie);
EXPORT_SYMBOL(atari_mch_type);
EXPORT_SYMBOL(atari_hw_present);
EXPORT_SYMBOL(atari_switches);
EXPORT_SYMBOL(atari_dont_touch_floppy_select);
EXPORT_SYMBOL(atari_register_vme_int);
EXPORT_SYMBOL(atari_unregister_vme_int);
EXPORT_SYMBOL(stdma_lock);
EXPORT_SYMBOL(stdma_release);
EXPORT_SYMBOL(stdma_others_waiting);
EXPORT_SYMBOL(stdma_islocked);
EXPORT_SYMBOL(atari_stram_alloc);
EXPORT_SYMBOL(atari_stram_free);

EXPORT_SYMBOL(atari_mouse_buttons);
EXPORT_SYMBOL(atari_mouse_interrupt_hook);
EXPORT_SYMBOL(atari_MIDI_interrupt_hook);
EXPORT_SYMBOL(atari_MFP_init_done);
EXPORT_SYMBOL(atari_SCC_init_done);
EXPORT_SYMBOL(atari_SCC_reset_done);
EXPORT_SYMBOL(ikbd_write);
EXPORT_SYMBOL(ikbd_mouse_y0_top);
EXPORT_SYMBOL(ikbd_mouse_thresh);
EXPORT_SYMBOL(ikbd_mouse_rel_pos);
EXPORT_SYMBOL(ikbd_mouse_disable);

EXPORT_SYMBOL(atari_microwire_cmd);