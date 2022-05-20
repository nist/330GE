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
 * linux/include/asm-arm/arch-l7200/dma.h
 *
 * Copyright (C) 2000 Steve Hill (sjhill@cotw.com)
 *
 * Changelog:
 *  08-29-2000	SJH	Created
 */
#ifndef __ASM_ARCH_DMA_H
#define __ASM_ARCH_DMA_H

/* DMA is not yet implemented! It should be the same as acorn, copy over.. */

/*
 * This is the maximum DMA address that can be DMAd to.
 * There should not be more than (0xd0000000 - 0xc0000000)
 * bytes of RAM.
 */
#define MAX_DMA_ADDRESS         0xd0000000
#define MAX_DMA_CHANNELS        0

#define DMA_S0                  0

#endif /* _ASM_ARCH_DMA_H */
