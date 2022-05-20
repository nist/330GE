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
/* Rubin encoder/decoder header       */
/* work started at   : aug   3, 1994  */
/* last modification : aug  15, 1994  */
/* $Id: compr_rubin.h,v 1.1.1.1 2007/01/25 12:52:02 jiahao_jhou Exp $ */

#include "pushpull.h"

#define RUBIN_REG_SIZE   16
#define UPPER_BIT_RUBIN    (((long) 1)<<(RUBIN_REG_SIZE-1))
#define LOWER_BITS_RUBIN   ((((long) 1)<<(RUBIN_REG_SIZE-1))-1)


struct rubin_state {
	unsigned long p;		
	unsigned long q;	
	unsigned long rec_q;
	long bit_number;
	struct pushpull pp;
	int bit_divider;
	int bits[8];
};


void init_rubin (struct rubin_state *rs, int div, int *bits);
int encode (struct rubin_state *, long, long, int);
void end_rubin (struct rubin_state *);
void init_decode (struct rubin_state *, int div, int *bits);
int decode (struct rubin_state *, long, long);
