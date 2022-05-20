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
#ifndef MVME147_H

/* $Id: mvme147.h,v 1.1.1.1 2007/01/25 12:51:58 jiahao_jhou Exp $
 *
 * Header file for the MVME147 built-in SCSI controller for Linux
 *
 * Written and (C) 1993, Hamish Macdonald, see mvme147.c for more info
 *
 */

#include <linux/types.h>

int mvme147_detect(Scsi_Host_Template *);
int mvme147_release(struct Scsi_Host *);
const char *wd33c93_info(void);
int wd33c93_queuecommand(Scsi_Cmnd *, void (*done)(Scsi_Cmnd *));
int wd33c93_abort(Scsi_Cmnd *);
int wd33c93_reset(Scsi_Cmnd *, unsigned int);

#ifndef NULL
#define NULL 0
#endif

#ifndef CMD_PER_LUN
#define CMD_PER_LUN 2
#endif

#ifndef CAN_QUEUE
#define CAN_QUEUE 16
#endif

#ifdef HOSTS_C

#define MVME147_SCSI {proc_name:	   "MVME147",			\
		      proc_info:           NULL,			\
		      name:                "MVME147 built-in SCSI",	\
		      detect:              mvme147_detect,		\
		      release:             mvme147_release,		\
		      queuecommand:        wd33c93_queuecommand,	\
		      abort:               wd33c93_abort,		\
		      reset:               wd33c93_reset,		\
		      can_queue:           CAN_QUEUE,			\
		      this_id:             7,				\
		      sg_tablesize:        SG_ALL,			\
		      cmd_per_lun:	   CMD_PER_LUN,			\
		      use_clustering:      ENABLE_CLUSTERING }

#endif /* else def HOSTS_C */

#endif /* MVME147_H */
