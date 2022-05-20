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
/* $Id: rawhdlc.h,v 1.1.1.1 2007/01/25 12:51:54 jiahao_jhou Exp $
 *
 * Author     Brent Baccala
 * Copyright  by Brent Baccala <baccala@FreeSoft.org>
 *
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

#ifndef RAWHDLC_H
struct hdlc_state {
	char insane_mode;
	u_char state;
	u_char r_one;
	u_char r_val;
	u_int o_bitcnt;
	u_int i_bitcnt;
	u_int fcs;
};


int make_raw_hdlc_data(u_char *src, u_int slen, u_char *dst, u_int dsize);
void init_hdlc_state(struct hdlc_state *stateptr, int mode);
int read_raw_hdlc_data(struct hdlc_state *saved_state,
                       u_char *src, u_int slen, u_char *dst, u_int dsize);
#define RAWHDLC_H
#endif
