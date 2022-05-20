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
 *    sd.h Copyright (C) 1992 Drew Eckhardt 
 *      SCSI disk driver header file by
 *              Drew Eckhardt 
 *
 *      <drew@colorado.edu>
 *
 *       Modified by Eric Youngdale eric@andante.org to
 *       add scatter-gather, multiple outstanding request, and other
 *       enhancements.
 */
#ifndef _SD_H
#define _SD_H
/*
   $Header: /home/user/WL520gu/src/linux/linux/drivers/scsi/sd.h,v 1.1.1.1 2007/01/25 12:51:58 jiahao_jhou Exp $
 */

#ifndef _SCSI_H
#include "scsi.h"
#endif

#ifndef _GENDISK_H
#include <linux/genhd.h>
#endif

typedef struct scsi_disk {
	unsigned capacity;	/* size in blocks */
	Scsi_Device *device;
	unsigned char ready;	/* flag ready for FLOPTICAL */
	unsigned char write_prot;	/* flag write_protect for rmvable dev */
	unsigned char sector_bit_size;	/* sector_size = 2 to the  bit size power */
	unsigned char sector_bit_shift;		/* power of 2 sectors per FS block */
	unsigned has_part_table:1;	/* has partition table */
} Scsi_Disk;

extern int revalidate_scsidisk(kdev_t dev, int maxusage);

/*
 * Used by pmac to find the device associated with a target.
 */
extern kdev_t sd_find_target(void *host, int tgt);

#define N_SD_MAJORS	8

#define SD_MAJOR_MASK	(N_SD_MAJORS - 1)

#endif

/*
 * Overrides for Emacs so that we follow Linus's tabbing style.
 * Emacs will notice this stuff at the end of the file and automatically
 * adjust the settings for this buffer only.  This must remain at the end
 * of the file.
 * ---------------------------------------------------------------------------
 * Local variables:
 * c-indent-level: 4
 * c-brace-imaginary-offset: 0
 * c-brace-offset: -4
 * c-argdecl-indent: 4
 * c-label-offset: -4
 * c-continued-statement-offset: 4
 * c-continued-brace-offset: 0
 * indent-tabs-mode: nil
 * tab-width: 8
 * End:
 */
