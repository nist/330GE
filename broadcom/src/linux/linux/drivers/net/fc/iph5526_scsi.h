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
#ifndef IPH5526_SCSI_H
#define IPH5526_SCSI_H

#define IPH5526_CAN_QUEUE	32
#define IPH5526_SCSI_FC { 						 				\
        name:                   "Interphase 5526 Fibre Channel SCSI Adapter",   \
        detect:                 iph5526_detect,                  \
        release:                iph5526_release,                 \
        info:                   iph5526_info,                    \
        queuecommand:           iph5526_queuecommand,            \
		bios_param:				iph5526_biosparam,               \
        can_queue:              IPH5526_CAN_QUEUE,               \
        this_id:                -1,                              \
        sg_tablesize:           255,                             \
        cmd_per_lun:            8,                               \
        use_clustering:         DISABLE_CLUSTERING,              \
        eh_abort_handler:       iph5526_abort,                   \
        eh_device_reset_handler:NULL,                            \
        eh_bus_reset_handler:   NULL,                            \
        eh_host_reset_handler:  NULL,                            \
}

int iph5526_detect(Scsi_Host_Template *tmpt);
int iph5526_queuecommand(Scsi_Cmnd *Cmnd, void (*done) (Scsi_Cmnd *));
int iph5526_release(struct Scsi_Host *host);
int iph5526_abort(Scsi_Cmnd *Cmnd);
const char *iph5526_info(struct Scsi_Host *host);
int iph5526_biosparam(Disk * disk, kdev_t n, int ip[]);

#endif
