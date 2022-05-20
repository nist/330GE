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
/* $Id: netjet.h,v 1.1.1.1 2007/01/25 12:51:54 jiahao_jhou Exp $
 *
 * NETjet common header file
 *
 * Author       Karsten Keil
 * Copyright    by Karsten Keil      <keil@isdn4linux.de>
 *              by Matt Henderson,
 *                 Traverse Technologies P/L www.traverse.com.au
 * 
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

extern const char *CardType[];

#define byteout(addr,val) outb(val,addr)
#define bytein(addr) inb(addr)

#define NETJET_CTRL	0x00
#define NETJET_DMACTRL	0x01
#define NETJET_AUXCTRL	0x02
#define NETJET_AUXDATA	0x03
#define NETJET_IRQMASK0 0x04
#define NETJET_IRQMASK1 0x05
#define NETJET_IRQSTAT0 0x06
#define NETJET_IRQSTAT1 0x07
#define NETJET_DMA_READ_START	0x08
#define NETJET_DMA_READ_IRQ	0x0c
#define NETJET_DMA_READ_END	0x10
#define NETJET_DMA_READ_ADR	0x14
#define NETJET_DMA_WRITE_START	0x18
#define NETJET_DMA_WRITE_IRQ	0x1c
#define NETJET_DMA_WRITE_END	0x20
#define NETJET_DMA_WRITE_ADR	0x24
#define NETJET_PULSE_CNT	0x28

#define NETJET_ISAC_OFF	0xc0
#define NETJET_ISACIRQ	0x10
#define NETJET_IRQM0_READ	0x0c
#define NETJET_IRQM0_READ_1	0x04
#define NETJET_IRQM0_READ_2	0x08
#define NETJET_IRQM0_WRITE	0x03
#define NETJET_IRQM0_WRITE_1	0x01
#define NETJET_IRQM0_WRITE_2	0x02

#define NETJET_DMA_TXSIZE 512
#define NETJET_DMA_RXSIZE 128

#define HDLC_ZERO_SEARCH 0
#define HDLC_FLAG_SEARCH 1
#define HDLC_FLAG_FOUND  2
#define HDLC_FRAME_FOUND 3
#define HDLC_NULL 4
#define HDLC_PART 5
#define HDLC_FULL 6

#define HDLC_FLAG_VALUE	0x7e

u_char NETjet_ReadIC(struct IsdnCardState *cs, u_char offset);
void NETjet_WriteIC(struct IsdnCardState *cs, u_char offset, u_char value);
void NETjet_ReadICfifo(struct IsdnCardState *cs, u_char *data, int size);
void NETjet_WriteICfifo(struct IsdnCardState *cs, u_char *data, int size);

void read_tiger(struct IsdnCardState *cs);
void write_tiger(struct IsdnCardState *cs);

void netjet_fill_dma(struct BCState *bcs);
void netjet_interrupt(int intno, void *dev_id, struct pt_regs *regs);
void inittiger(struct IsdnCardState *cs);
void release_io_netjet(struct IsdnCardState *cs);

