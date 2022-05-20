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
 *	seagate.h Copyright (C) 1992 Drew Eckhardt 
 *	low level scsi driver header for ST01/ST02 by
 *		Drew Eckhardt 
 *
 *	<drew@colorado.edu>
 */

#ifndef _SEAGATE_H
	#define SEAGATE_H
/*
	$Header
*/
#ifndef ASM
int seagate_st0x_detect(Scsi_Host_Template *);
int seagate_st0x_command(Scsi_Cmnd *);
int seagate_st0x_queue_command(Scsi_Cmnd *, void (*done)(Scsi_Cmnd *));

static int seagate_st0x_abort(Scsi_Cmnd *);
const char *seagate_st0x_info(struct Scsi_Host *);
static int seagate_st0x_reset(Scsi_Cmnd *, unsigned int); 

#define SEAGATE_ST0X  {  detect:         seagate_st0x_detect,		\
			 info:           seagate_st0x_info,		\
			 command:        seagate_st0x_command,		\
			 queuecommand:   seagate_st0x_queue_command,	\
			 abort:          seagate_st0x_abort,		\
			 reset:          seagate_st0x_reset,		\
			 can_queue:      1,				\
			 this_id:        7,				\
			 sg_tablesize:   SG_ALL,			\
			 cmd_per_lun:    1,				\
			 use_clustering: DISABLE_CLUSTERING}
#endif /* ASM */

#endif /* _SEAGATE_H */
