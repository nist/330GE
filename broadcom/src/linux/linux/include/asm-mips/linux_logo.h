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
 * include/asm-mips/linux_logo.h: This is a linux logo
 *                                to be displayed on boot.
 *
 * Copyright (C) 1996 Larry Ewing (lewing@isc.tamu.edu)
 * Copyright (C) 1998 Jakub Jelinek (jj@sunsite.mff.cuni.cz)
 *
 * You can put anything here, but:
 * LINUX_LOGO_COLORS has to be less than 224
 * image size has to be 80x80
 * values have to start from 0x20
 * (i.e. RGB(linux_logo_red[0],
 *	     linux_logo_green[0],
 *	     linux_logo_blue[0]) is color 0x20)
 * BW image has to be 80x80 as well, with MS bit
 * on the left
 * Serial_console ascii image can be any size,
 * but should contain %s to display the version
 */

#include <linux/init.h>
#include <linux/version.h>
#include <linux/config.h>

#ifndef CONFIG_DECSTATION
#	include <asm/linux_logo_sgi.h>
#else
#	include <asm/linux_logo_dec.h>
#endif

#ifndef INCLUDE_LINUX_LOGO_DATA
/* prototypes only */
extern unsigned char linux_logo_red[];
extern unsigned char linux_logo_green[];
extern unsigned char linux_logo_blue[];
extern unsigned char linux_logo[];
extern unsigned char linux_logo_bw[];
extern unsigned char linux_logo16_red[];
extern unsigned char linux_logo16_green[];
extern unsigned char linux_logo16_blue[];
extern unsigned char linux_logo16[];

#endif
