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
 *   u14-34f.h - used by the low-level driver for UltraStor 14F/34F
 */
#ifndef _U14_34F_H
#define _U14_34F_H

#include <scsi/scsicam.h>

int u14_34f_detect(Scsi_Host_Template *);
int u14_34f_release(struct Scsi_Host *);
int u14_34f_queuecommand(Scsi_Cmnd *, void (*done)(Scsi_Cmnd *));
int u14_34f_abort(Scsi_Cmnd *);
int u14_34f_reset(Scsi_Cmnd *);
int u14_34f_biosparam(Disk *, kdev_t, int *);

#define U14_34F_VERSION "6.70.00"

#define ULTRASTOR_14_34F {                                                   \
                name:         "UltraStor 14F/34F rev. " U14_34F_VERSION " ", \
                detect:                  u14_34f_detect,                     \
                release:                 u14_34f_release,                    \
                queuecommand:            u14_34f_queuecommand,               \
                abort:                   NULL,                               \
                reset:                   NULL,                               \
                eh_abort_handler:        u14_34f_abort,                      \
                eh_device_reset_handler: NULL,                               \
                eh_bus_reset_handler:    NULL,                               \
                eh_host_reset_handler:   u14_34f_reset,                      \
                bios_param:              u14_34f_biosparam,                  \
                this_id:                 7,                                  \
                unchecked_isa_dma:       1,                                  \
                use_clustering:          ENABLE_CLUSTERING,                  \
                use_new_eh_code:         1    /* Enable new error code */    \
                }

#endif
