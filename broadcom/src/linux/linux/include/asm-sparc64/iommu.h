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
/* $Id: iommu.h,v 1.1.1.1 2007/01/25 12:52:06 jiahao_jhou Exp $
 * iommu.h: Definitions for the sun5 IOMMU.
 *
 * Copyright (C) 1996, 1999 David S. Miller (davem@caip.rutgers.edu)
 */
#ifndef _SPARC64_IOMMU_H
#define _SPARC64_IOMMU_H

/* The format of an iopte in the page tables. */
#define IOPTE_VALID         0x8000000000000000 /* IOPTE is valid                   */
#define IOPTE_64K           0x2000000000000000 /* IOPTE is for 64k page            */
#define IOPTE_STBUF         0x1000000000000000 /* DVMA can use streaming buffer    */
#define IOPTE_INTRA         0x0800000000000000 /* SBUS slot-->slot direct transfer */
#define IOPTE_CONTEXT	    0x07ff800000000000 /* Context number		   */
#define IOPTE_PAGE          0x00007fffffffe000 /* Physical page number (PA[42:13]) */
#define IOPTE_CACHE         0x0000000000000010 /* Cached (in UPA E-cache)          */
#define IOPTE_WRITE         0x0000000000000002 /* Writeable                        */

#endif /* !(_SPARC_IOMMU_H) */
