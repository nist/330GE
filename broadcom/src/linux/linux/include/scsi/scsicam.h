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
 * scsicam.h - SCSI CAM support functions, use for HDIO_GETGEO, etc.
 *
 * Copyright 1993, 1994 Drew Eckhardt
 *      Visionary Computing 
 *      (Unix and Linux consulting and custom programming)
 *      drew@Colorado.EDU
 *	+1 (303) 786-7975
 *
 * For more information, please consult the SCSI-CAM draft.
 */

#ifndef SCSICAM_H
#define SCSICAM_H
#include <linux/kdev_t.h>
extern int scsicam_bios_param (Disk *disk, kdev_t dev, int *ip);
extern int scsi_partsize(struct buffer_head *bh, unsigned long capacity,
           unsigned int  *cyls, unsigned int *hds, unsigned int *secs);
#endif /* def SCSICAM_H */
