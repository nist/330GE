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
 * structures and definitions for the int 15, ax=e820 memory map
 * scheme.
 *
 * In a nutshell, setup.S populates a scratch table in the
 * empty_zero_block that contains a list of usable address/size
 * duples.  setup.c, this information is transferred into the e820map,
 * and in init.c/numa.c, that new information is used to mark pages
 * reserved or not.
 */
#ifndef __E820_HEADER
#define __E820_HEADER

#include <linux/mmzone.h>

#define E820MAP	0x2d0		/* our map */
#define E820MAX	32		/* number of entries in E820MAP */
#define E820NR	0x1e8		/* # entries in E820MAP */

#define E820_RAM	1
#define E820_RESERVED	2
#define E820_ACPI	3 /* usable as RAM once ACPI tables have been read */
#define E820_NVS	4

#define HIGH_MEMORY	(1024*1024)

#define LOWMEMSIZE()	(0x9f000)

#define MAXMEM		(120UL * 1024 * 1024 * 1024 * 1024)  /* 120TB */ 


#ifndef __ASSEMBLY__
struct e820entry {
	u64 addr;	/* start of memory segment */
	u64 size;	/* size of memory segment */
	u32 type;	/* type of memory segment */
} __attribute__((packed));

struct e820map {
	int nr_map;
	struct e820entry map[E820MAX];
};

extern unsigned long find_e820_area(unsigned long start, unsigned long end, 
				    unsigned size);
extern void add_memory_region(unsigned long start, unsigned long size, 
			      int type);
extern void setup_memory_region(void);
extern void contig_e820_setup(void); 
extern void e820_end_of_ram(void);
extern void e820_reserve_resources(void);
extern void e820_print_map(char *who);
extern int e820_mapped(unsigned long start, unsigned long end, int type);

extern void e820_bootmem_free(pg_data_t *pgdat, unsigned long start,unsigned long end);

extern void parse_mem_cmdline (char ** cmdline_p);

extern struct e820map e820;
#endif/*!__ASSEMBLY__*/

#endif/*__E820_HEADER*/
