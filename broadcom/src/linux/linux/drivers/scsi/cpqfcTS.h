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
#ifndef CPQFCTS_H
#define CPQFCTS_H
#include "cpqfcTSstructs.h"

// These functions are required by the Linux SCSI layers
extern int cpqfcTS_detect(Scsi_Host_Template *);
extern int cpqfcTS_release(struct Scsi_Host *);
extern const char * cpqfcTS_info(struct Scsi_Host *);
extern int cpqfcTS_proc_info(char *, char **, off_t, int, int, int);
extern int cpqfcTS_queuecommand(Scsi_Cmnd *, void (* done)(Scsi_Cmnd *));
extern int cpqfcTS_abort(Scsi_Cmnd *);
extern int cpqfcTS_reset(Scsi_Cmnd *, unsigned int);
extern int cpqfcTS_eh_abort(Scsi_Cmnd *Cmnd);
extern int cpqfcTS_eh_device_reset(Scsi_Cmnd *);
extern int cpqfcTS_biosparam(Disk *, kdev_t, int[]);
extern int cpqfcTS_ioctl( Scsi_Device *ScsiDev, int Cmnd, void *arg);

// note: since Tachyon TS supports an extended scatter/gather
// linked list of infinite length (with linked Ext S/G pages,
// limited only by available physical memory) we use SG_ALL.

#define CPQFCTS {                                \
 detect:                 cpqfcTS_detect,         \
 release:                cpqfcTS_release,        \
 info:                   cpqfcTS_info,           \
 proc_info:              cpqfcTS_proc_info,      \
 ioctl:                  cpqfcTS_ioctl,          \
 queuecommand:           cpqfcTS_queuecommand,   \
 eh_device_reset_handler:   cpqfcTS_eh_device_reset,   \
 eh_abort_handler:       cpqfcTS_eh_abort,       \
 reset:                  cpqfcTS_reset,          \
 abort:                  cpqfcTS_abort,		 \
 bios_param:             cpqfcTS_biosparam,      \
 can_queue:              CPQFCTS_REQ_QUEUE_LEN,  \
 this_id:                -1,                     \
 sg_tablesize:           SG_ALL,                 \
 cmd_per_lun:            CPQFCTS_CMD_PER_LUN,    \
 present:                0,                      \
 unchecked_isa_dma:      0,                      \
 use_clustering:         ENABLE_CLUSTERING,      \
 use_new_eh_code:        1			 \
}

#endif /* CPQFCTS_H */ 
