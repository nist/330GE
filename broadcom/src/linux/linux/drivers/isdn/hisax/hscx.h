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
/* $Id: hscx.h,v 1.1.1.1 2007/01/25 12:51:54 jiahao_jhou Exp $
 *
 * HSCX specific defines
 *
 * Author       Karsten Keil
 * Copyright    by Karsten Keil      <keil@isdn4linux.de>
 * 
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 *
 */

/* All Registers original Siemens Spec  */

#define HSCX_ISTA 0x20
#define HSCX_CCR1 0x2f
#define HSCX_CCR2 0x2c
#define HSCX_TSAR 0x31
#define HSCX_TSAX 0x30
#define HSCX_XCCR 0x32
#define HSCX_RCCR 0x33
#define HSCX_MODE 0x22
#define HSCX_CMDR 0x21
#define HSCX_EXIR 0x24
#define HSCX_XAD1 0x24
#define HSCX_XAD2 0x25
#define HSCX_RAH2 0x27
#define HSCX_RSTA 0x27
#define HSCX_TIMR 0x23
#define HSCX_STAR 0x21
#define HSCX_RBCL 0x25
#define HSCX_XBCH 0x2d
#define HSCX_VSTR 0x2e
#define HSCX_RLCR 0x2e
#define HSCX_MASK 0x20

extern int HscxVersion(struct IsdnCardState *cs, char *s);
extern void hscx_sched_event(struct BCState *bcs, int event);
extern void modehscx(struct BCState *bcs, int mode, int bc);
extern void clear_pending_hscx_ints(struct IsdnCardState *cs);
extern void inithscx(struct IsdnCardState *cs);
extern void inithscxisac(struct IsdnCardState *cs, int part);
