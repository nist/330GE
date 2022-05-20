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
#ifndef _PPC64_SCATTERLIST_H
#define _PPC64_SCATTERLIST_H

/*
 * Copyright (C) 2001 PPC64 Team, IBM Corp
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/types.h>
#include <asm/dma.h>

struct scatterlist {
	char *address;		/* Virtual addr data is to be transferred to */
	struct page *page;	/* Location for highmem page, if any */
	unsigned int offset;	/* for highmem, page offset */
	unsigned int length;

	/* For TCE support */
	u32 dma_address;
	u32 dma_length;
};

#define ISA_DMA_THRESHOLD	(~0UL)

#endif /* !(_PPC64_SCATTERLIST_H) */
