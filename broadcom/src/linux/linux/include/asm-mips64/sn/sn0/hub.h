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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1992 - 1997, 1999 Silicon Graphics, Inc.
 * Copyright (C) 1999 by Ralf Baechle
 */
#ifndef _ASM_SN_SN0_HUB_H
#define _ASM_SN_SN0_HUB_H

/* The secret password; used to release protection */
#define HUB_PASSWORD		0x53474972756c6573ull

#define CHIPID_HUB		0
#define CHIPID_ROUTER		1

#define HUB_REV_1_0		1
#define HUB_REV_2_0		2
#define HUB_REV_2_1		3
#define HUB_REV_2_2		4
#define HUB_REV_2_3             5
#define HUB_REV_2_4             6

#define MAX_HUB_PATH		80

#include <asm/sn/sn0/addrs.h>
#include <asm/sn/sn0/hubpi.h>
#include <asm/sn/sn0/hubmd.h>
#include <asm/sn/sn0/hubio.h>
#include <asm/sn/sn0/hubni.h>
//#include <asm/sn/sn0/hubcore.h>

#ifdef SABLE
#define IP27_NO_HUBUART_INT	1
#endif

/* Translation of uncached attributes */
#define	UATTR_HSPEC	0
#define	UATTR_IO	1
#define	UATTR_MSPEC	2
#define	UATTR_UNCAC	3

#endif /* _ASM_SN_SN0_HUB_H */
