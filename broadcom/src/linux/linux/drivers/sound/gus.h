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

#include "ad1848.h"

/*	From gus_card.c */
int gus_set_midi_irq(int num);
void gusintr(int irq, void *dev_id, struct pt_regs * dummy);

/*	From gus_wave.c */
int gus_wave_detect(int baseaddr);
void gus_wave_init(struct address_info *hw_config);
void gus_wave_unload (struct address_info *hw_config);
void gus_voice_irq(void);
void gus_write8(int reg, unsigned int data);
void guswave_dma_irq(void);
void gus_delay(void);
int gus_default_mixer_ioctl (int dev, unsigned int cmd, caddr_t arg);
void gus_timer_command (unsigned int addr, unsigned int val);

/*	From gus_midi.c */
void gus_midi_init(struct address_info *hw_config);
void gus_midi_interrupt(int dummy);

/*	From ics2101.c */
int ics2101_mixer_init(void);
