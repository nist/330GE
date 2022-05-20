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
 *        eata.h - used by the low-level driver for EATA/DMA SCSI host adapters.
 */
#ifndef _EATA_H
#define _EATA_H

#include <scsi/scsicam.h>

int eata2x_detect(Scsi_Host_Template *);
int eata2x_release(struct Scsi_Host *);
int eata2x_queuecommand(Scsi_Cmnd *, void (*done)(Scsi_Cmnd *));
int eata2x_abort(Scsi_Cmnd *);
int eata2x_reset(Scsi_Cmnd *);
int eata2x_biosparam(Disk *, kdev_t, int *);

#define EATA_VERSION "6.70.00"

#define EATA {                                                               \
                name:              "EATA/DMA 2.0x rev. " EATA_VERSION " ",   \
                detect:                  eata2x_detect,                      \
                release:                 eata2x_release,                     \
                queuecommand:            eata2x_queuecommand,                \
                abort:                   NULL,                               \
                reset:                   NULL,                               \
                eh_abort_handler:        eata2x_abort,                       \
                eh_device_reset_handler: NULL,                               \
                eh_bus_reset_handler:    NULL,                               \
                eh_host_reset_handler:   eata2x_reset,                       \
                bios_param:              eata2x_biosparam,                   \
                this_id:                 7,                                  \
                unchecked_isa_dma:       1,                                  \
                use_clustering:          ENABLE_CLUSTERING,                  \
                use_new_eh_code:         1    /* Enable new error code */    \
             }

#endif
