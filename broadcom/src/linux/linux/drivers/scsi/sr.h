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
 *      sr.h by David Giller
 *      CD-ROM disk driver header file
 *      
 *      adapted from:
 *      sd.h Copyright (C) 1992 Drew Eckhardt 
 *      SCSI disk driver header file by
 *              Drew Eckhardt 
 *
 *      <drew@colorado.edu>
 *
 *       Modified by Eric Youngdale eric@andante.org to
 *       add scatter-gather, multiple outstanding request, and other
 *       enhancements.
 */

#ifndef _SR_H
#define _SR_H

#include "scsi.h"

typedef struct {
	unsigned capacity;	/* size in blocks                       */
	Scsi_Device *device;
	unsigned int vendor;	/* vendor code, see sr_vendor.c         */
	unsigned long ms_offset;	/* for reading multisession-CD's        */
	unsigned char sector_bit_size;	/* sector size = 2^sector_bit_size      */
	unsigned char sector_bit_shift;		/* sectors/FS block = 2^sector_bit_shift */
	unsigned needs_sector_size:1;	/* needs to get sector size */
	unsigned use:1;		/* is this device still supportable     */
	unsigned xa_flag:1;	/* CD has XA sectors ? */
	unsigned readcd_known:1;	/* drive supports READ_CD (0xbe) */
	unsigned readcd_cdda:1;	/* reading audio data using READ_CD */
	struct cdrom_device_info cdi;
} Scsi_CD;

extern Scsi_CD *scsi_CDs;

int sr_do_ioctl(int, unsigned char *, void *, unsigned, int, int, struct request_sense *);

int sr_lock_door(struct cdrom_device_info *, int);
int sr_tray_move(struct cdrom_device_info *, int);
int sr_drive_status(struct cdrom_device_info *, int);
int sr_disk_status(struct cdrom_device_info *);
int sr_get_last_session(struct cdrom_device_info *, struct cdrom_multisession *);
int sr_get_mcn(struct cdrom_device_info *, struct cdrom_mcn *);
int sr_reset(struct cdrom_device_info *);
int sr_select_speed(struct cdrom_device_info *cdi, int speed);
int sr_audio_ioctl(struct cdrom_device_info *, unsigned int, void *);
int sr_dev_ioctl(struct cdrom_device_info *, unsigned int, unsigned long);

int sr_read_sector(int minor, int lba, int blksize, unsigned char *dest);
int sr_is_xa(int minor);

/* sr_vendor.c */
void sr_vendor_init(int minor);
int sr_cd_check(struct cdrom_device_info *);
int sr_set_blocklength(int minor, int blocklength);

#endif
