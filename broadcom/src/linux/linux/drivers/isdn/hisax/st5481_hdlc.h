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
 * Driver for ST5481 USB ISDN modem
 *
 * Author       Frode Isaksen
 * Copyright    2001 by Frode Isaksen      <fisaksen@bewan.com>
 *              2001 by Kai Germaschewski  <kai.germaschewski@gmx.de>
 * 
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

#ifndef __ST5481_HDLC_H__
#define __ST5481_HDLC_H__

struct hdlc_vars {
  	int bit_shift; 
	int hdlc_bits1;
	int data_bits;
	int ffbit_shift; // encoding only
	int state;
	int dstpos;

	int data_received:1; // set if transferring data
	int dchannel:1; // set if D channel (send idle instead of flags)
	int do_adapt56:1; // set if 56K adaptation
        int do_closing:1; // set if in closing phase (need to send CRC + flag

	unsigned short crc;

	unsigned char cbin; 
	unsigned char shift_reg;
	unsigned char ffvalue;
	
};


/*
  The return value from hdlc_decode is
  the frame length, 0 if no complete frame was decoded,
  or a negative error number
*/

#define HDLC_FRAMING_ERROR     1
#define HDLC_CRC_ERROR         2
#define HDLC_LENGTH_ERROR      3

void 
hdlc_rcv_init(struct hdlc_vars *hdlc, int do_adapt56);

int
hdlc_decode(struct hdlc_vars *hdlc, const unsigned char *src, int slen,int *count, 
	    unsigned char *dst, int dsize);

void 
hdlc_out_init(struct hdlc_vars *hdlc,int is_d_channel,int do_adapt56);

int 
hdlc_encode(struct hdlc_vars *hdlc,const unsigned char *src,unsigned short slen,int *count,
	    unsigned char *dst,int dsize);

#endif
