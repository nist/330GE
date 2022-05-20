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
 * lowlevel.c
 *
 * PURPOSE
 *  Low Level Device Routines for the UDF filesystem
 *
 * CONTACTS
 *	E-mail regarding any portion of the Linux UDF file system should be
 *	directed to the development team mailing list (run by majordomo):
 *		linux_udf@hpesjro.fc.hp.com
 *
 * COPYRIGHT
 *	This file is distributed under the terms of the GNU General Public
 *	License (GPL). Copies of the GPL can be obtained from:
 *		ftp://prep.ai.mit.edu/pub/gnu/GPL
 *	Each contributing author retains all rights to their own work.
 *
 *  (C) 1999-2001 Ben Fennema
 *
 * HISTORY
 *
 *  03/26/99 blf  Created.
 */

#include "udfdecl.h"

#include <linux/blkdev.h>
#include <linux/cdrom.h>
#include <asm/uaccess.h>
#include <scsi/scsi.h>

typedef struct scsi_device Scsi_Device;
typedef struct scsi_cmnd   Scsi_Cmnd;

#include <scsi/scsi_ioctl.h>

#include <linux/udf_fs.h>
#include "udf_sb.h"

unsigned int 
udf_get_last_session(struct super_block *sb)
{
	struct cdrom_multisession ms_info;
	unsigned int vol_desc_start;
	struct block_device *bdev = sb->s_bdev;
	int i;

	vol_desc_start=0;
	ms_info.addr_format=CDROM_LBA;
	i = ioctl_by_bdev(bdev, CDROMMULTISESSION, (unsigned long) &ms_info);

#define WE_OBEY_THE_WRITTEN_STANDARDS 1

	if (i == 0)
	{
		udf_debug("XA disk: %s, vol_desc_start=%d\n",
			(ms_info.xa_flag ? "yes" : "no"), ms_info.addr.lba);
#if WE_OBEY_THE_WRITTEN_STANDARDS
		if (ms_info.xa_flag) /* necessary for a valid ms_info.addr */
#endif
			vol_desc_start = ms_info.addr.lba;
	}
	else
	{
		udf_debug("CDROMMULTISESSION not supported: rc=%d\n", i);
	}
	return vol_desc_start;
}

unsigned long
udf_get_last_block(struct super_block *sb)
{
	struct block_device *bdev = sb->s_bdev;
	int ret;
	unsigned long lblock = 0;

	ret = ioctl_by_bdev(bdev, CDROM_LAST_WRITTEN, (unsigned long) &lblock);

	if (ret) /* Hard Disk */
	{
		ret = ioctl_by_bdev(bdev, BLKGETSIZE, (unsigned long) &lblock);

		if (!ret && lblock != 0x7FFFFFFF)
			lblock = ((512 * lblock) / sb->s_blocksize);
	}

	if (!ret && lblock)
		return lblock - 1;
	else
		return 0;
}