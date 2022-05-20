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
 *
 *
 *
 *
 */
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/module.h>
#include <asm/io.h>
#include "utils.h"


#define  miu_chan_cfg(x)      ((volatile unsigned long  *)(0xbff40000+x*4))       /* for andros */

int mbsize[8] = {1,2,4,8,16,32,64,128};

unsigned long get_mem_avail(void) {

    unsigned long  cfg[10],i,total_mem=0;

	for(i=0;i<10;i++)
		cfg[i] = *miu_chan_cfg(i);

	for(i=0;i<10;i++){
		if(cfg[i]==0x1fc160c2) continue;                           // skip empties
		if(   ( (cfg[i]>>12) & 0xf )   <=  0xb ) continue;         // skip roms
        total_mem += mbsize[(cfg[i]>>16)&0x7] *1024*1024;
	}
    return total_mem;
}




static ulong* buffer_ptr = NULL;
static ulong buffer_size = 0;

ulong* get_reserved_buffer(void) {return KSEG0ADDR(buffer_ptr);}
ulong* get_reserved_buffer_virtual(void) {return (ulong*)ReservedMemVirtualAddr;}
ulong get_reserved_buffer_size(void) {return buffer_size;}

#define MIN_GEN_MEM (4 << 20)


void  reserve_buffer(const char* cl, ulong base_mem)
{
	char* pos = strstr(cl, "reserved_buffer=");
 	if (pos) {
		buffer_size = simple_strtol(pos+strlen("reserved_buffer="),
					    0, 10);
		buffer_size <<= 20;
		if (buffer_size + MIN_GEN_MEM > base_mem)
			buffer_size = base_mem - MIN_GEN_MEM;
		if (buffer_size > 0)
			buffer_ptr = (ulong*)(base_mem - buffer_size);
		else
			buffer_size = 0;
	}
}



EXPORT_SYMBOL(get_reserved_buffer);
EXPORT_SYMBOL(get_reserved_buffer_virtual);
EXPORT_SYMBOL(get_reserved_buffer_size);


