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
 * Definitions for various on board processors on the sound cards. For
 * example DSP processors.
 */

/*
 * Coprocessor access types 
 */
#define COPR_CUSTOM		0x0001	/* Custom applications */
#define COPR_MIDI		0x0002	/* MIDI (MPU-401) emulation */
#define COPR_PCM		0x0004	/* Digitized voice applications */
#define COPR_SYNTH		0x0008	/* Music synthesis */
