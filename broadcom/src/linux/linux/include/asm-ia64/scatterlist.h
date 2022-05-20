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
#ifndef _ASM_IA64_SCATTERLIST_H
#define _ASM_IA64_SCATTERLIST_H

/*
 * Copyright (C) 1998-1999, 2001-2002 Hewlett-Packard Co
 *	David Mosberger-Tang <davidm@hpl.hp.com>
 */

struct scatterlist {
	/* This will disappear in 2.5.x: */
	char *address;		/* location data is to be transferred to, NULL for highmem page */
	char *orig_address;	/* for use by swiotlb */

	/* These two are only valid if ADDRESS member of this struct is NULL.  */
	struct page *page;
	unsigned int offset;
	unsigned int length;	/* buffer length */
};

#define ISA_DMA_THRESHOLD	(~0UL)

#endif /* _ASM_IA64_SCATTERLIST_H */
