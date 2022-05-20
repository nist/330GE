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
/* $Id: dma.h,v 1.1.1.1 2007/01/25 12:52:04 jiahao_jhou Exp $
 * linux/include/asm/dma.h: Defines for using and allocating dma channels.
 */

#ifndef _ASM_DMA_H
#define _ASM_DMA_H

/* it's useless on the Etrax, but unfortunately needed by the new
   bootmem allocator (but this should do it for this) */

#define MAX_DMA_ADDRESS PAGE_OFFSET

/* TODO: check nbr of channels on Etrax-100LX */

#define MAX_DMA_CHANNELS	10

/* dma0 and dma1 used for network (ethernet) */
#define NETWORK_TX_DMA_NBR 0
#define NETWORK_RX_DMA_NBR 1

/* dma2 and dma3 shared by par0, scsi0, ser2 and ata */
#define PAR0_TX_DMA_NBR 2
#define PAR0_RX_DMA_NBR 3
#define SCSI0_TX_DMA_NBR 2
#define SCSI0_RX_DMA_NBR 3
#define SER2_TX_DMA_NBR 2
#define SER2_RX_DMA_NBR 3
#define ATA_TX_DMA_NBR 2
#define ATA_RX_DMA_NBR 3

/* dma4 and dma5 shared by par1, scsi1, ser3 and extdma0 */
#define PAR1_TX_DMA_NBR 4
#define PAR1_RX_DMA_NBR 5
#define SCSI1_TX_DMA_NBR 4
#define SCSI1_RX_DMA_NBR 5
#define SER3_TX_DMA_NBR 4
#define SER3_RX_DMA_NBR 5
#define EXTDMA0_TX_DMA_NBR 4
#define EXTDMA0_RX_DMA_NBR 5

/* dma6 and dma7 shared by ser0, extdma1 and mem2mem */
#define SER0_TX_DMA_NBR 6
#define SER0_RX_DMA_NBR 7
#define EXTDMA1_TX_DMA_NBR 6
#define EXTDMA1_RX_DMA_NBR 7
#define MEM2MEM_TX_DMA_NBR 6
#define MEM2MEM_RX_DMA_NBR 7

/* dma8 and dma9 shared by ser1 and usb */
#define SER1_TX_DMA_NBR 8
#define SER1_RX_DMA_NBR 9
#define USB_TX_DMA_NBR 8
#define USB_RX_DMA_NBR 9

/* These are in kernel/dma.c: */
extern int request_dma(unsigned int dmanr, const char * device_id); /* reserve a DMA channel */
extern void free_dma(unsigned int dmanr);	                    /* release it */

#endif /* _ASM_DMA_H */
