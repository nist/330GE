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
 * Socket-level I/O control calls.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1995 by Ralf Baechle
 */
#ifndef __ASM_MIPS_SOCKIOS_H
#define __ASM_MIPS_SOCKIOS_H

#include <asm/ioctl.h>

/* Socket-level I/O control calls. */
#define FIOGETOWN	_IOR('f', 123, int)
#define FIOSETOWN 	_IOW('f', 124, int)

#define SIOCATMARK	_IOR('s', 7, int)
#define SIOCSPGRP	_IOW('s', 8, pid_t)
#define SIOCGPGRP	_IOR('s', 9, pid_t)

#define SIOCGSTAMP	0x8906			/* Get stamp - linux-specific */

#endif /* __ASM_MIPS_SOCKIOS_H */
