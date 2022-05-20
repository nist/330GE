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
 * linux/include/asm-arm/arch-tbox/dma.h
 *
 * Architecture DMA routines.  We have to contend with the bizarre DMA
 * machine built into the Tbox hardware.
 *
 * Copyright (C) 1998 Philip Blundell
 */

/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

/*
 * DMA channel definitions.  Some of these are physically strange but
 * we sort it out inside dma.c so the user never has to care.  The
 * exception is the double-buffering which we can't really abstract
 * away sensibly.
 */
#define DMA_VIDEO			0
#define DMA_MPEG_B			1
#define DMA_AUDIO_B			2
#define DMA_ASHRX_B			3
#define DMA_ASHTX			4
#define DMA_MPEG			5
#define DMA_AUDIO			6
#define DMA_ASHRX			7

#define MAX_DMA_CHANNELS		0	

/*
 * This is the maximum DMA address that can be DMAd to.
 */
#define MAX_DMA_ADDRESS		0xffffffff
