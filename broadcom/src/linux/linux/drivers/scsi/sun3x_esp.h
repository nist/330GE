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
/* sun3x_esp.h: Defines and structures for the Sun3x ESP
 *
 * (C) 1995 Thomas Bogendoerfer (tsbogend@alpha.franken.de)
 */

#ifndef _SUN3X_ESP_H
#define _SUN3X_ESP_H

/* For dvma controller register definitions. */
#include <asm/dvma.h>

extern int sun3x_esp_detect(struct SHT *);
extern const char *esp_info(struct Scsi_Host *);
extern int esp_queue(Scsi_Cmnd *, void (*done)(Scsi_Cmnd *));
extern int esp_command(Scsi_Cmnd *);
extern int esp_abort(Scsi_Cmnd *);
extern int esp_reset(Scsi_Cmnd *, unsigned int);
extern int esp_proc_info(char *buffer, char **start, off_t offset, int length,
			 int hostno, int inout);

#define DMA_PORTS_P        (dregs->cond_reg & DMA_INT_ENAB)

#define SCSI_SUN3X_ESP {                                        \
		proc_name:      "esp",  			\
		proc_info:      &esp_proc_info,			\
		name:           "Sun ESP 100/100a/200",		\
		detect:         sun3x_esp_detect,		\
		info:           esp_info,			\
		command:        esp_command,			\
		queuecommand:   esp_queue,			\
		abort:          esp_abort,			\
		reset:          esp_reset,			\
		can_queue:      7,				\
		this_id:        7,				\
		sg_tablesize:   SG_ALL,				\
		cmd_per_lun:    1,				\
		use_clustering: DISABLE_CLUSTERING, }

#endif /* !(_SUN3X_ESP_H) */
