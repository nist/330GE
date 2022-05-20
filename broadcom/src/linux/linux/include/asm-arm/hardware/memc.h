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
 *  linux/include/asm-arm/hardware/memc.h
 *
 *  Copyright (C) Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#define VDMA_ALIGNMENT	PAGE_SIZE
#define VDMA_XFERSIZE	16
#define VDMA_INIT	0
#define VDMA_START	1
#define VDMA_END	2

#ifndef __ASSEMBLY__
extern void memc_write(unsigned int reg, unsigned long val);

#define video_set_dma(start,end,offset)				\
do {								\
	memc_write (VDMA_START, (start >> 2));			\
	memc_write (VDMA_END, (end - VDMA_XFERSIZE) >> 2);	\
	memc_write (VDMA_INIT, (offset >> 2));			\
} while (0)

#endif
