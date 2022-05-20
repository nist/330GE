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
 * Exported symbols for midi driver.
 * __NO_VERSION__ because this is still part of sound.o.
 */

#define __NO_VERSION__
#include <linux/module.h>

char midi_syms_symbol;

#include "sound_config.h"
#define _MIDI_SYNTH_C_
#include "midi_synth.h"

EXPORT_SYMBOL(do_midi_msg);
EXPORT_SYMBOL(midi_synth_open);
EXPORT_SYMBOL(midi_synth_close);
EXPORT_SYMBOL(midi_synth_ioctl);
EXPORT_SYMBOL(midi_synth_kill_note);
EXPORT_SYMBOL(midi_synth_start_note);
EXPORT_SYMBOL(midi_synth_set_instr);
EXPORT_SYMBOL(midi_synth_reset);
EXPORT_SYMBOL(midi_synth_hw_control);
EXPORT_SYMBOL(midi_synth_aftertouch);
EXPORT_SYMBOL(midi_synth_controller);
EXPORT_SYMBOL(midi_synth_panning);
EXPORT_SYMBOL(midi_synth_setup_voice);
EXPORT_SYMBOL(midi_synth_send_sysex);
EXPORT_SYMBOL(midi_synth_bender);
EXPORT_SYMBOL(midi_synth_load_patch);
EXPORT_SYMBOL(MIDIbuf_avail);
