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
 * Exported symbols for audio driver.
 * __NO_VERSION__ because this is still part of sound.o.
 */

#define __NO_VERSION__
#include <linux/module.h>

char audio_syms_symbol;

#include "sound_config.h"
#include "sound_calls.h"

EXPORT_SYMBOL(DMAbuf_start_dma);
EXPORT_SYMBOL(DMAbuf_open_dma);
EXPORT_SYMBOL(DMAbuf_close_dma);
EXPORT_SYMBOL(DMAbuf_inputintr);
EXPORT_SYMBOL(DMAbuf_outputintr);
EXPORT_SYMBOL(dma_ioctl);
EXPORT_SYMBOL(audio_open);
EXPORT_SYMBOL(audio_release);
