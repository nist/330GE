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
 * tipar/tiser/tiusb - low level driver for handling link cables
 * designed for Texas Instruments graphing calculators.
 *
 * Copyright (C) 2000-2002, Romain Lievin <roms@lpg.ticalc.org>
 *
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL)
 */

#ifndef _TICABLE_H 
#define _TICABLE_H 1

/* Internal default constants for the kernel module */
#define TIMAXTIME 15      /* 1.5 seconds       */
#define IO_DELAY  10      /* 10 micro-seconds  */

/* Major & minor number for character devices */
#define TIPAR_MAJOR  115 /* 0 to 7 */
#define TIPAR_MINOR    0

#define TISER_MAJOR  115 /* 8 to 15 */
#define TISER_MINOR    8

#define TIUSB_MAJOR  115  /* 16 to 31 */
#define TIUSB_MINOR   16

/*
 * Request values for the 'ioctl' function.
 */
#define IOCTL_TIPAR_DELAY     _IOW('p', 0xa8, int) /* set delay   */
#define IOCTL_TIPAR_TIMEOUT   _IOW('p', 0xa9, int) /* set timeout */

#define IOCTL_TISER_DELAY     _IOW('p', 0xa0, int) /* set delay   */
#define IOCTL_TISER_TIMEOUT   _IOW('p', 0xa1, int) /* set timeout */

#define IOCTL_TIUSB_TIMEOUT        _IOW('N', 0x20, int) /* set timeout */
#define IOCTL_TIUSB_RESET_DEVICE   _IOW('N', 0x21, int) /* reset device */
#define IOCTL_TIUSB_RESET_PIPES    _IOW('N', 0x22, int) /* reset both pipes*/

#endif /* TICABLE_H */
