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
/* $Id: dmacopy.c,v 1.1.1.1 2007/01/25 12:51:48 jiahao_jhou Exp $ 
 *
 * memcpy for large blocks, using memory-memory DMA channels 6 and 7 in Etrax
 */

#include <asm/svinto.h>
#include <asm/io.h>

#define D(x)

void *dma_memcpy(void *pdst,
		 const void *psrc,
		 unsigned int pn)
{
	static etrax_dma_descr indma, outdma;
	
	D(printk("dma_memcpy %d bytes... ", pn));

	indma.sw_len = outdma.sw_len = pn;
	indma.ctrl = d_eol | d_eop;
	outdma.ctrl = d_eol;
	indma.buf = psrc;
	outdma.buf = pdst;

	*R_DMA_CH6_FIRST = &indma;
	*R_DMA_CH7_FIRST = &outdma;
	*R_DMA_CH6_CMD = IO_STATE(R_DMA_CH6_CMD, cmd, start);
	*R_DMA_CH7_CMD = IO_STATE(R_DMA_CH7_CMD, cmd, start);
	
	while(*R_DMA_CH7_CMD == 1) /* wait for completion */ ;

	D(printk("done\n"));

}



