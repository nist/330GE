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
/* $Id: intr.h,v 1.1.1.1 2007/01/25 12:52:04 jiahao_jhou Exp $
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1992 - 1997, 2000-2002 Silicon Graphics, Inc. All rights reserved.
 */
#ifndef _ASM_IA64_SN_SN2_INTR_H
#define _ASM_IA64_SN_SN2_INTR_H

#define SGI_UART_VECTOR (0xe9)
#define SGI_SHUB_ERROR_VECTOR   (0xea)

// These two IRQ's are used by partitioning.
#define SGI_XPC_NOTIFY			(0xe7)
#define SGI_XPART_ACTIVATE		(0x30)

#define IA64_SN2_FIRST_DEVICE_VECTOR	(0x31)
#define IA64_SN2_LAST_DEVICE_VECTOR	(0xe6)

#define SN2_IRQ_RESERVED        (0x1)
#define SN2_IRQ_CONNECTED       (0x2)

#endif /* _ASM_IA64_SN_SN2_INTR_H */
