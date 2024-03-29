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
 *  linux/include/asm-arm/mach/dma.h
 *
 *  Copyright (C) 1998-2000 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  This header file describes the interface between the generic DMA handler
 *  (dma.c) and the architecture-specific DMA backends (dma-*.c)
 */

struct dma_struct;
typedef struct dma_struct dma_t;

struct dma_ops {
	int	(*request)(dmach_t, dma_t *);		/* optional */
	void	(*free)(dmach_t, dma_t *);		/* optional */
	void	(*enable)(dmach_t, dma_t *);		/* mandatory */
	void 	(*disable)(dmach_t, dma_t *);		/* mandatory */
	int	(*residue)(dmach_t, dma_t *);		/* optional */
	int	(*setspeed)(dmach_t, dma_t *, int);	/* optional */
	char	*type;
};

struct dma_struct {
	struct scatterlist buf;		/* single DMA			*/
	int		sgcount;	/* number of DMA SG		*/
	struct scatterlist *sg;		/* DMA Scatter-Gather List	*/

	unsigned int	active:1;	/* Transfer active		*/
	unsigned int	invalid:1;	/* Address/Count changed	*/
	unsigned int	using_sg:1;	/* using scatter list?		*/
	dmamode_t	dma_mode;	/* DMA mode			*/
	int		speed;		/* DMA speed			*/

	unsigned int	lock;		/* Device is allocated		*/
	const char	*device_id;	/* Device name			*/

	unsigned int	dma_base;	/* Controller base address	*/
	int		dma_irq;	/* Controller IRQ		*/
	int		state;		/* Controller state		*/
	struct scatterlist cur_sg;	/* Current controller buffer	*/

	struct dma_ops	*d_ops;
};

/* Prototype: void arch_dma_init(dma)
 * Purpose  : Initialise architecture specific DMA
 * Params   : dma - pointer to array of DMA structures
 */
extern void arch_dma_init(dma_t *dma);

extern void isa_init_dma(dma_t *dma);
