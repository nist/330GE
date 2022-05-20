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

/* JEDEC Flash Interface.
 * This is an older type of interface for self programming flash. It is 
 * commonly use in older AMD chips and is obsolete compared with CFI.
 * It is called JEDEC because the JEDEC association distributes the ID codes
 * for the chips.
 *
 * See the AMD flash databook for information on how to operate the interface.
 *
 * $Id: jedec.h,v 1.1.1.1 2007/01/25 12:52:07 jiahao_jhou Exp $
 */

#ifndef __LINUX_MTD_JEDEC_H__
#define __LINUX_MTD_JEDEC_H__

#include <linux/types.h>
#include <linux/mtd/map.h>

#define MAX_JEDEC_CHIPS 16

// Listing of all supported chips and their information
struct JEDECTable
{
   __u16 jedec;
   char *name;
   unsigned long size;
   unsigned long sectorsize;
   __u32 capabilities;
};

// JEDEC being 0 is the end of the chip array
struct jedec_flash_chip
{
   __u16 jedec;
   unsigned long size;
   unsigned long sectorsize;
   
   // *(__u8*)(base + (adder << addrshift)) = data << datashift
   // Address size = size << addrshift
   unsigned long base;           // Byte 0 of the flash, will be unaligned
   unsigned int datashift;       // Useful for 32bit/16bit accesses
   unsigned int addrshift;
   unsigned long offset;         // linerized start. base==offset for unbanked, uninterleaved flash
   
   __u32 capabilities;
   
   // These markers are filled in by the flash_chip_scan function
   unsigned long start;
   unsigned long length;
};

struct jedec_private
{
   unsigned long size;         // Total size of all the devices
   
   /* Bank handling. If sum(bank_fill) == size then this is linear flash.
      Otherwise the mapping has holes in it. bank_fill may be used to
      find the holes, but in the common symetric case 
      bank_fill[0] == bank_fill[*], thus addresses may be computed 
      mathmatically. bank_fill must be powers of two */
   unsigned is_banked;
   unsigned long bank_fill[MAX_JEDEC_CHIPS];
   
   struct jedec_flash_chip chips[MAX_JEDEC_CHIPS];  
};

#endif
