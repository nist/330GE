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
/* $Id: sgiwd93.h,v 1.1.1.1 2007/01/25 12:51:58 jiahao_jhou Exp $
 * sgiwd93.h: SGI WD93 scsi definitions.
 *
 * Copyright (C) 1996 David S. Miller (dm@engr.sgi.com)
 */
#ifndef _SGIWD93_H
#define _SGIWD93_H

#ifndef NULL
#define NULL        0
#endif

#ifndef CMD_PER_LUN
#define CMD_PER_LUN 8
#endif

#ifndef CAN_QUEUE
#define CAN_QUEUE   16
#endif

int sgiwd93_detect(Scsi_Host_Template *);
int sgiwd93_release(struct Scsi_Host *instance);
const char *wd33c93_info(void);
int wd33c93_queuecommand(Scsi_Cmnd *, void (*done)(Scsi_Cmnd *));
int wd33c93_abort(Scsi_Cmnd *);
int wd33c93_reset(Scsi_Cmnd *, unsigned int);

#define SGIWD93_SCSI {proc_name:	   "SGIWD93", \
		      name:                "SGI WD93", \
		      detect:              sgiwd93_detect,    \
		      release:             sgiwd93_release,   \
		      queuecommand:        wd33c93_queuecommand, \
		      abort:               wd33c93_abort,   \
		      reset:               wd33c93_reset,   \
		      can_queue:           CAN_QUEUE,       \
		      this_id:             7,               \
		      sg_tablesize:        SG_ALL,          \
		      cmd_per_lun:	   CMD_PER_LUN,     \
		      use_clustering:      DISABLE_CLUSTERING }

#endif /* !(_SGIWD93_H) */
