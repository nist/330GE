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
#ifndef MVME16x_SCSI_H
#define MVME16x_SCSI_H

#include <linux/types.h>

int mvme16x_scsi_detect(Scsi_Host_Template *);
const char *NCR53c7x0_info(void);
int NCR53c7xx_queue_command(Scsi_Cmnd *, void (*done)(Scsi_Cmnd *));
int NCR53c7xx_abort(Scsi_Cmnd *);
int NCR53c7x0_release (struct Scsi_Host *);
int NCR53c7xx_reset(Scsi_Cmnd *, unsigned int);
void NCR53c7x0_intr(int irq, void *dev_id, struct pt_regs * regs);

#ifndef NULL
#define NULL 0
#endif

#ifndef CMD_PER_LUN
#define CMD_PER_LUN 3
#endif

#ifndef CAN_QUEUE
#define CAN_QUEUE 24
#endif

#include <scsi/scsicam.h>

#define MVME16x_SCSI  {name:                "MVME16x NCR53c710 SCSI", \
		       detect:              mvme16x_scsi_detect,    \
		       queuecommand:        NCR53c7xx_queue_command, \
		       abort:               NCR53c7xx_abort,   \
		       reset:               NCR53c7xx_reset,   \
		       bios_param:          scsicam_bios_param,   \
		       can_queue:           24,       \
		       this_id:             7,               \
		       sg_tablesize:        63,          \
		       cmd_per_lun:	    3,     \
		       use_clustering:      DISABLE_CLUSTERING }

#endif /* MVME16x_SCSI_H */
