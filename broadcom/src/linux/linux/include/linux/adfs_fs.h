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
#ifndef _ADFS_FS_H
#define _ADFS_FS_H

#include <linux/types.h>

/*
 * Disc Record at disc address 0xc00
 */
struct adfs_discrecord {
    __u8  log2secsize;
    __u8  secspertrack;
    __u8  heads;
    __u8  density;
    __u8  idlen;
    __u8  log2bpmb;
    __u8  skew;
    __u8  bootoption;
    __u8  lowsector;
    __u8  nzones;
    __u16 zone_spare;
    __u32 root;
    __u32 disc_size;
    __u16 disc_id;
    __u8  disc_name[10];
    __u32 disc_type;
    __u32 disc_size_high;
    __u8  log2sharesize:4;
    __u8  unused40:4;
    __u8  big_flag:1;
    __u8  unused41:1;
    __u8  nzones_high;
    __u32 format_version;
    __u32 root_size;
    __u8  unused52[60 - 52];
};

#define ADFS_DISCRECORD		(0xc00)
#define ADFS_DR_OFFSET		(0x1c0)
#define ADFS_DR_SIZE		 60
#define ADFS_DR_SIZE_BITS	(ADFS_DR_SIZE << 3)
#define ADFS_SUPER_MAGIC	 0xadf5

#ifdef __KERNEL__
/*
 * Calculate the boot block checksum on an ADFS drive.  Note that this will
 * appear to be correct if the sector contains all zeros, so also check that
 * the disk size is non-zero!!!
 */
static inline int adfs_checkbblk(unsigned char *ptr)
{
	unsigned int result = 0;
	unsigned char *p = ptr + 511;

	do {
	        result = (result & 0xff) + (result >> 8);
        	result = result + *--p;
	} while (p != ptr);

	return (result & 0xff) != ptr[511];
}

#endif

#endif
