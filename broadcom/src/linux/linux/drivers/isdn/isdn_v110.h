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
/* $Id: isdn_v110.h,v 1.1.1.1 2007/01/25 12:51:54 jiahao_jhou Exp $
 *
 * Linux ISDN subsystem, V.110 related functions (linklevel).
 *
 * Copyright by Thomas Pfeiffer (pfeiffer@pds.de)
 *
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

#ifndef _isdn_v110_h_
#define _isdn_v110_h_

/* 
 * isdn_v110_encode will take raw data and encode it using V.110 
 */
extern struct sk_buff *isdn_v110_encode(isdn_v110_stream *, struct sk_buff *);

/* 
 * isdn_v110_decode receives V.110 coded data from the stream and rebuilds
 * frames from them. The source stream doesn't need to be framed.
 */
extern struct sk_buff *isdn_v110_decode(isdn_v110_stream *, struct sk_buff *);

extern int isdn_v110_stat_callback(int, isdn_ctrl *);
extern void isdn_v110_close(isdn_v110_stream * v);

#endif
