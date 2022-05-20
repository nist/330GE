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
 * I/O addresses
 */

#define u_Base			(gus_base + 0x000)
#define u_Mixer			u_Base
#define u_Status		(gus_base + 0x006)
#define u_TimerControl		(gus_base + 0x008)
#define u_TimerData		(gus_base + 0x009)
#define u_IRQDMAControl		(gus_base + 0x00b)
#define u_MidiControl		(gus_base + 0x100)
#define 	MIDI_RESET		0x03
#define		MIDI_ENABLE_XMIT	0x20
#define		MIDI_ENABLE_RCV		0x80
#define u_MidiStatus		u_MidiControl
#define		MIDI_RCV_FULL		0x01
#define 	MIDI_XMIT_EMPTY		0x02
#define 	MIDI_FRAME_ERR		0x10
#define 	MIDI_OVERRUN		0x20
#define 	MIDI_IRQ_PEND		0x80
#define u_MidiData		(gus_base + 0x101)
#define u_Voice			(gus_base + 0x102)
#define u_Command		(gus_base + 0x103)
#define u_DataLo		(gus_base + 0x104)
#define u_DataHi		(gus_base + 0x105)
#define u_MixData               (gus_base + 0x106)   /* Rev. 3.7+ mixing */
#define u_MixSelect             (gus_base + 0x506)   /* registers.       */
#define u_IrqStatus		u_Status
#	define MIDI_TX_IRQ		0x01	/* pending MIDI xmit IRQ */
#	define MIDI_RX_IRQ		0x02	/* pending MIDI recv IRQ */
#	define GF1_TIMER1_IRQ		0x04	/* general purpose timer */
#	define GF1_TIMER2_IRQ		0x08	/* general purpose timer */
#	define WAVETABLE_IRQ		0x20	/* pending wavetable IRQ */
#	define ENVELOPE_IRQ		0x40	/* pending volume envelope IRQ */
#	define DMA_TC_IRQ		0x80	/* pending dma tc IRQ */

#define ICS2101		1
#	define ICS_MIXDEVS	6
#	define DEV_MIC		0
#	define DEV_LINE		1
#	define DEV_CD		2
#	define DEV_GF1		3
#	define DEV_UNUSED	4
#	define DEV_VOL		5

#	define CHN_LEFT		0
#	define CHN_RIGHT	1
#define CS4231		2
#define u_DRAMIO		(gus_base + 0x107)
