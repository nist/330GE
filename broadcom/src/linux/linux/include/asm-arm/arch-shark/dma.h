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
 * linux/include/asm-arm/arch-shark/dma.h
 *
 * by Alexander Schulz
 */
#ifndef __ASM_ARCH_DMA_H
#define __ASM_ARCH_DMA_H

/* Use only the lowest 4MB, nothing else works.
 * The rest is not DMAable. See dev /  .properties
 * in OpenFirmware.
 */
#define MAX_DMA_ADDRESS		0xC0400000
#define MAX_DMA_CHANNELS	8
#define DMA_ISA_CASCADE         4

static inline void __arch_adjust_zones(int node, unsigned long *zone_size, unsigned long *zhole_size) 
{
  if (node != 0) return;
  /* Only the first 4 MB (=1024 Pages) are usable for DMA */
  zone_size[1] = zone_size[0] - 1024;
  zone_size[0] = 1024;
  zhole_size[1] = zhole_size[0];
  zhole_size[0] = 0;
}

#define arch_adjust_zones(node,size,holes) __arch_adjust_zones(node,size,holes)

#endif /* _ASM_ARCH_DMA_H */

