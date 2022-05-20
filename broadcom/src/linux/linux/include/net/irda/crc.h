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
/*********************************************************************
 *                
 * Filename:      crc.h
 * Version:       
 * Description:   CRC routines
 * Status:        Experimental.
 * Author:        Dag Brattli <dagb@cs.uit.no>
 * Created at:    Mon Aug  4 20:40:53 1997
 * Modified at:   Sun May  2 20:25:23 1999
 * Modified by:   Dag Brattli <dagb@cs.uit.no>
 * 
 ********************************************************************/

#ifndef IRDA_CRC_H
#define IRDA_CRC_H

#include <linux/types.h>

#define INIT_FCS  0xffff   /* Initial FCS value */
#define GOOD_FCS  0xf0b8   /* Good final FCS value */

extern __u16 const irda_crc16_table[];

/* Recompute the FCS with one more character appended. */
static inline __u16 irda_fcs(__u16 fcs, __u8 c)
{
	return (((fcs) >> 8) ^ irda_crc16_table[((fcs) ^ (c)) & 0xff]);
}

/* Recompute the FCS with len bytes appended. */
unsigned short crc_calc( __u16 fcs, __u8 const *buf, size_t len);

#endif
