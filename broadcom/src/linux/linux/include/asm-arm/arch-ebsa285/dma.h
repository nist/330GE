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
 *  linux/include/asm-arm/arch-ebsa285/dma.h
 *
 *  Architecture DMA routines
 *
 *  Copyright (C) 1998,1999 Russell King
 *  Copyright (C) 1998,1999 Philip Blundell
 */
#ifndef __ASM_ARCH_DMA_H
#define __ASM_ARCH_DMA_H

/*
 * This is the maximum DMA address that can be DMAd to.
 */
#define MAX_DMA_ADDRESS		0xffffffff

/*
 * The 21285 has two internal DMA channels; we call these 8 and 9.
 * On CATS hardware we have an additional eight ISA dma channels
 * numbered 0..7.
 */
#define _ISA_DMA(x)		(0+(x))
#define _DC21285_DMA(x)		(8+(x))

#define MAX_DMA_CHANNELS	10

#define DMA_FLOPPY		_ISA_DMA(2)
#define DMA_ISA_CASCADE		_ISA_DMA(4)

#endif /* _ASM_ARCH_DMA_H */
