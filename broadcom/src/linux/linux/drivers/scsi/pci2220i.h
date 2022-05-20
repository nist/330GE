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
/****************************************************************************
 * Perceptive Solutions, Inc. PCI-2220I device driver for Linux.
 *
 * pci2220i.h - Linux Host Driver for PCI-2220i EIDE Adapters
 *
 * Copyright (c) 1997-1999 Perceptive Solutions, Inc.
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that redistributions of source
 * code retain the above copyright notice and this comment without
 * modification.
 *
 * Technical updates and product information at:
 *  http://www.psidisk.com
 *
 * Please send questions, comments, bug reports to:
 *  tech@psidisk.com Technical Support
 *
 ****************************************************************************/
#ifndef _PCI2220I_H
#define _PCI2220I_H

#ifndef LINUX_VERSION_CODE
#include <linux/version.h>
#endif 
#define	LINUXVERSION(v,p,s)    (((v)<<16) + ((p)<<8) + (s))

// function prototypes
int Pci2220i_Detect			(Scsi_Host_Template *tpnt);
int Pci2220i_Command		(Scsi_Cmnd *SCpnt);
int Pci2220i_QueueCommand	(Scsi_Cmnd *SCpnt, void (*done)(Scsi_Cmnd *));
int Pci2220i_Abort			(Scsi_Cmnd *SCpnt);
int Pci2220i_Reset			(Scsi_Cmnd *SCpnt, unsigned int flags);
int Pci2220i_Release		(struct Scsi_Host *pshost);
int Pci2220i_BiosParam		(Disk *disk, kdev_t dev, int geom[]);

#ifndef NULL
	#define NULL 0
#endif

#define PCI2220I {					\
	proc_name:		"pci2220i",		\
	name:			"PCI-2220I/PCI-2240I",	\
	detect:			Pci2220i_Detect,	\
	release:		Pci2220i_Release,	\
	command:		Pci2220i_Command,	\
	queuecommand:		Pci2220i_QueueCommand,	\
	abort:			Pci2220i_Abort,		\
	reset:			Pci2220i_Reset,		\
	bios_param:		Pci2220i_BiosParam,	\
	can_queue:		1,			\
	this_id:		-1,			\
	sg_tablesize:		SG_ALL,			\
	cmd_per_lun:		1,			\
	present:		0,			\
	unchecked_isa_dma:	0,			\
	use_clustering:		DISABLE_CLUSTERING,	\
	use_new_eh_code:	0			\
}
#endif
