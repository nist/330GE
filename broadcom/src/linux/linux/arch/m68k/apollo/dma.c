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
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/kd.h>
#include <linux/tty.h>
#include <linux/console.h>

#include <asm/setup.h>
#include <asm/bootinfo.h>
#include <asm/system.h>
#include <asm/pgtable.h>
#include <asm/apollodma.h>

/* note only works for 16 Bit 1 page DMA's */

static unsigned short next_free_xlat_entry=0;

unsigned short dma_map_page(unsigned long phys_addr,int count,int type) {

	unsigned long page_aligned_addr=phys_addr & (~((1<<12)-1));
	unsigned short start_map_addr=page_aligned_addr >> 10;
	unsigned short free_xlat_entry, *xlat_map_entry;
	int i;

	free_xlat_entry=next_free_xlat_entry;
	for(i=0,xlat_map_entry=addr_xlat_map+(free_xlat_entry<<2);i<8;i++,xlat_map_entry++) {
		outw(start_map_addr+i, xlat_map_entry);
	}

	next_free_xlat_entry+=2;
	if(next_free_xlat_entry>125)
		next_free_xlat_entry=0;
	

	return free_xlat_entry<<10;
}

void dma_unmap_page(unsigned short dma_addr) {

	return ;

}

