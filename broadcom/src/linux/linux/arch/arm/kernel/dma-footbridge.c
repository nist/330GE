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
 *  linux/arch/arm/kernel/dma-ebsa285.c
 *
 *  Copyright (C) 1998 Phil Blundell
 *
 * DMA functions specific to EBSA-285/CATS architectures
 *
 *  Changelog:
 *   09-Nov-1998 RMK	Split out ISA DMA functions to dma-isa.c
 *   17-Mar-1999 RMK	Allow any EBSA285-like architecture to have
 *			ISA DMA controllers.
 */
#include <linux/config.h>
#include <linux/sched.h>
#include <linux/init.h>

#include <asm/dma.h>
#include <asm/io.h>

#include <asm/mach/dma.h>
#include <asm/hardware/dec21285.h>


void __init arch_dma_init(dma_t *dma)
{
#ifdef CONFIG_ISA_DMA
	if (footbridge_cfn_mode())
		isa_init_dma(dma + _ISA_DMA(0));
#endif
}
