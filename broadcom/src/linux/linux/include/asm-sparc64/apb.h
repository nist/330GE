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
/* $Id: apb.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $
 * apb.h: Advanced PCI Bridge Configuration Registers and Bits
 *
 * Copyright (C) 1998  Eddie C. Dost  (ecd@skynet.be)
 */

#ifndef _SPARC64_APB_H
#define _SPARC64_APB_H

#define APB_TICK_REGISTER			0xb0
#define APB_INT_ACK				0xb8
#define APB_PRIMARY_MASTER_RETRY_LIMIT		0xc0
#define APB_DMA_ASFR				0xc8
#define APB_DMA_AFAR				0xd0
#define APB_PIO_TARGET_RETRY_LIMIT		0xd8
#define APB_PIO_TARGET_LATENCY_TIMER		0xd9
#define APB_DMA_TARGET_RETRY_LIMIT		0xda
#define APB_DMA_TARGET_LATENCY_TIMER		0xdb
#define APB_SECONDARY_MASTER_RETRY_LIMIT	0xdc
#define APB_SECONDARY_CONTROL			0xdd
#define APB_IO_ADDRESS_MAP			0xde
#define APB_MEM_ADDRESS_MAP			0xdf

#define APB_PCI_CONTROL_LOW			0xe0
#  define APB_PCI_CTL_LOW_ARB_PARK			(1 << 21)
#  define APB_PCI_CTL_LOW_ERRINT_EN			(1 << 8)

#define APB_PCI_CONTROL_HIGH			0xe4
#  define APB_PCI_CTL_HIGH_SERR				(1 << 2)
#  define APB_PCI_CTL_HIGH_ARBITER_EN			(1 << 0)

#define APB_PIO_ASFR				0xe8
#define APB_PIO_AFAR				0xf0
#define APB_DIAG_REGISTER			0xf8

#endif /* !(_SPARC64_APB_H) */
