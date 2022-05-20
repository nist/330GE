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
/* Hey EMACS -*- linux-c -*-
 *
 * tiglusb - low level driver for SilverLink cable
 *
 * Copyright (C) 2000-2002, Romain Lievin <roms@lpg.ticalc.org>
 * under the terms of the GNU General Public License.
 *
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL)
 */

#ifndef _TIGLUSB_H
#define _TIGLUSB_H

/*
 * Max. number of devices supported
 */
#define MAXTIGL		16

/*
 * Max. packetsize for IN and OUT pipes
 */
#define BULK_RCV_MAX	32
#define BULK_SND_MAX	32

/*
 * The driver context...
 */

typedef enum { _stopped=0, _started } driver_state_t;

typedef struct
{
	struct usb_device	*dev;		/* USB device handle */
	struct semaphore	mutex;		/* locks this struct */

	wait_queue_head_t	wait;		/* for timed waits */
	wait_queue_head_t	remove_ok;

	int		minor;			/* which minor dev #? */
	devfs_handle_t	devfs;			/* devfs device */

	driver_state_t	state;			/* started/stopped */
	int		opened;			/* tru if open */
	int	remove_pending;
} tiglusb_t, *ptiglusb_t;

#endif
