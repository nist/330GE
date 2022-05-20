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
 *  linux/include/asm-arm/map.h
 *
 *  Copyright (C) 1999-2000 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Page table mapping constructs and function prototypes
 */
struct map_desc {
	unsigned long virtual;
	unsigned long physical;
	unsigned long length;
	int domain:4,
	    prot_read:1,
	    prot_write:1,
	    cacheable:1,
	    bufferable:1,
	    last:1;
};

#define LAST_DESC \
  { last: 1 }

struct meminfo;

extern void create_memmap_holes(struct meminfo *);
extern void memtable_init(struct meminfo *);
extern void iotable_init(struct map_desc *);
extern void setup_io_desc(void);
