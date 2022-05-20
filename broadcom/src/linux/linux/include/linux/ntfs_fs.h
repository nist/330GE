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
#ifndef _LINUX_NTFS_FS_H
#define _LINUX_NTFS_FS_H

#include <asm/byteorder.h>

#define NTFS_SECTOR_BITS 9
#define NTFS_SECTOR_SIZE 512

/*
 * Attribute flags (16-bit).
 */
typedef enum {
	ATTR_IS_COMPRESSED      = __constant_cpu_to_le16(0x0001),
	ATTR_COMPRESSION_MASK   = __constant_cpu_to_le16(0x00ff),
					/* Compression method mask. Also,
					 * first illegal value. */
	ATTR_IS_ENCRYPTED       = __constant_cpu_to_le16(0x4000),
	ATTR_IS_SPARSE          = __constant_cpu_to_le16(0x8000),
} __attribute__ ((__packed__)) ATTR_FLAGS;

/*
 * The two zones from which to allocate clusters.
 */
typedef enum {
	MFT_ZONE,
	DATA_ZONE
} NTFS_CLUSTER_ALLOCATION_ZONES;

#endif
