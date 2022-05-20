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
#include <linux/config.h>
#include <linux/module.h>
#include <linux/types.h>
#include <asm/ptrace.h>
#include <asm/amigahw.h>
#include <asm/amigaints.h>
#include <asm/amipcmcia.h>

extern volatile u_short amiga_audio_min_period;
extern u_short amiga_audio_period;

/*
 * Add things here when you find the need for it.
 */
EXPORT_SYMBOL(amiga_model);
EXPORT_SYMBOL(amiga_chipset);
EXPORT_SYMBOL(amiga_hw_present);
EXPORT_SYMBOL(amiga_eclock);
EXPORT_SYMBOL(amiga_colorclock);
EXPORT_SYMBOL(amiga_chip_alloc);
EXPORT_SYMBOL(amiga_chip_free);
EXPORT_SYMBOL(amiga_chip_avail);
EXPORT_SYMBOL(amiga_chip_size);
EXPORT_SYMBOL(amiga_audio_period);
EXPORT_SYMBOL(amiga_audio_min_period);
EXPORT_SYMBOL(amiga_do_irq);
EXPORT_SYMBOL(amiga_do_irq_list);

#ifdef CONFIG_AMIGA_PCMCIA
  EXPORT_SYMBOL(pcmcia_reset);
  EXPORT_SYMBOL(pcmcia_copy_tuple);
  EXPORT_SYMBOL(pcmcia_program_voltage);
  EXPORT_SYMBOL(pcmcia_access_speed);
  EXPORT_SYMBOL(pcmcia_write_enable);
  EXPORT_SYMBOL(pcmcia_write_disable);
#endif
