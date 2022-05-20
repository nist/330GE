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
/* $Id: dmamap.h,v 1.1.1.1 2007/01/25 12:52:04 jiahao_jhou Exp $
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1992 - 1997, 2000-2001 Silicon Graphics, Inc. All rights reserved.
 */
#ifndef _ASM_IA64_SN_DMAMAP_H
#define _ASM_IA64_SN_DMAMAP_H

#include <asm/sn/sv.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Definitions for allocating, freeing, and using DMA maps
 */

/*
 * DMA map types
 */
#define	DMA_SCSI	0
#define	DMA_A24VME	1		/* Challenge/Onyx only 	*/
#define	DMA_A32VME	2		/* Challenge/Onyx only 	*/
#define	DMA_A64VME	3		/* SN0/Racer */

#define	DMA_EISA	4

#define	DMA_PCI32	5		/* SN0/Racer 	*/
#define	DMA_PCI64	6		/* SN0/Racer 	*/

/*
 * DMA map structure as returned by dma_mapalloc()
 */
typedef struct dmamap {
	int		dma_type;	/* Map type (see above) */
	int		dma_adap;	/* I/O adapter */
	int		dma_index;	/* Beginning map register to use */
	int		dma_size;	/* Number of map registers to use */
	paddr_t		dma_addr;	/* Corresponding bus addr for A24/A32 */
	caddr_t		dma_virtaddr;	/* Beginning virtual address that is mapped */
} dmamap_t;

struct alenlist_s;

/*
 * Prototypes of exported functions
 */
extern dmamap_t	*dma_mapalloc(int, int, int, int);
extern void	dma_mapfree(dmamap_t *);
extern int	dma_map(dmamap_t *, caddr_t, int);
extern int	dma_map2(dmamap_t *, caddr_t, caddr_t, int);
extern paddr_t	dma_mapaddr(dmamap_t *, caddr_t);
#ifdef LATER
extern int	dma_mapbp(dmamap_t *, buf_t *, int);
#endif
extern int	dma_map_alenlist(dmamap_t *, struct alenlist_s *, size_t);
extern uint	ev_kvtoiopnum(caddr_t);

/*
 * These variables are defined in master.d/kernel
 */
extern struct map *a24map[];
extern struct map *a32map[];

extern int a24_mapsize;
extern int a32_mapsize;

extern sv_t dmamapout;

#ifdef __cplusplus
}
#endif

/* standard flags values for pio_map routines,
 * including {xtalk,pciio}_dmamap calls.
 * NOTE: try to keep these in step with PIOMAP flags.
 */
#define DMAMAP_FIXED	0x1
#define DMAMAP_NOSLEEP	0x2
#define	DMAMAP_INPLACE	0x4

#define	DMAMAP_FLAGS	0x7

#endif /* _ASM_IA64_SN_DMAMAP_H */
