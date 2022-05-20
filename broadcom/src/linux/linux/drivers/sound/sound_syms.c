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
 *	The sound core exports the following symbols to the rest of
 *	modulespace.
 *
 *      (C) Copyright 1997      Alan Cox, Licensed under the GNU GPL
 *
 *	Thu May 27 1999 Andrew J. Kroll <ag784@freenet..buffalo..edu>
 *	left out exported symbol... fixed
 */

#include <linux/module.h>
#include "sound_config.h"
#include "sound_calls.h"

char sound_syms_symbol;

EXPORT_SYMBOL(mixer_devs);
EXPORT_SYMBOL(audio_devs);
EXPORT_SYMBOL(num_mixers);
EXPORT_SYMBOL(num_audiodevs);

EXPORT_SYMBOL(midi_devs);
EXPORT_SYMBOL(num_midis);
EXPORT_SYMBOL(synth_devs);
EXPORT_SYMBOL(num_synths);

EXPORT_SYMBOL(sound_timer_devs);
EXPORT_SYMBOL(num_sound_timers);

EXPORT_SYMBOL(sound_install_audiodrv);
EXPORT_SYMBOL(sound_install_mixer);
EXPORT_SYMBOL(sound_alloc_dma);
EXPORT_SYMBOL(sound_free_dma);
EXPORT_SYMBOL(sound_open_dma);
EXPORT_SYMBOL(sound_close_dma);
EXPORT_SYMBOL(sound_alloc_audiodev);
EXPORT_SYMBOL(sound_alloc_mididev);
EXPORT_SYMBOL(sound_alloc_mixerdev);
EXPORT_SYMBOL(sound_alloc_timerdev);
EXPORT_SYMBOL(sound_alloc_synthdev);
EXPORT_SYMBOL(sound_unload_audiodev);
EXPORT_SYMBOL(sound_unload_mididev);
EXPORT_SYMBOL(sound_unload_mixerdev);
EXPORT_SYMBOL(sound_unload_timerdev);
EXPORT_SYMBOL(sound_unload_synthdev);

EXPORT_SYMBOL(load_mixer_volumes);

EXPORT_SYMBOL(conf_printf);
EXPORT_SYMBOL(conf_printf2);

MODULE_DESCRIPTION("OSS Sound subsystem");
MODULE_AUTHOR("Hannu Savolainen, et al.");
