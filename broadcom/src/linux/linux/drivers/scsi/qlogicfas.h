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
#ifndef _QLOGICFAS_H
#define _QLOGICFAS_H

int qlogicfas_detect(Scsi_Host_Template * );
const char * qlogicfas_info(struct Scsi_Host *);
int qlogicfas_command(Scsi_Cmnd *);
int qlogicfas_queuecommand(Scsi_Cmnd *, void (* done)(Scsi_Cmnd *));
int qlogicfas_abort(Scsi_Cmnd *);
int qlogicfas_reset(Scsi_Cmnd *, unsigned int);
int qlogicfas_biosparam(Disk *, kdev_t, int[]);

#ifndef NULL
#define NULL (0)
#endif

#ifdef PCMCIA
#define __QLINIT __devinit
#else
#define __QLINIT __init
#endif

#define QLOGICFAS {		\
	detect:         qlogicfas_detect,	\
	info:           qlogicfas_info,		\
	command:        qlogicfas_command, 	\
	queuecommand:   qlogicfas_queuecommand,	\
	abort:          qlogicfas_abort,	\
	reset:          qlogicfas_reset,	\
	bios_param:     qlogicfas_biosparam,	\
	can_queue:      0,			\
	this_id:        -1,			\
	sg_tablesize:   SG_ALL,			\
	cmd_per_lun:    1,			\
	use_clustering: DISABLE_CLUSTERING	\
}

#endif /* _QLOGICFAS_H */



