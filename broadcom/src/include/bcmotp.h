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
 * Write-once support for otp.
 *
 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.                
 *                                     
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ASUSTek Inc.;   
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior      
 * written permission of ASUSTek Inc..                            
 *
 * $Id: bcmotp.h,v 1.1.1.1 2007/01/25 12:52:12 jiahao_jhou Exp $
 */


extern void *otp_init(sb_t *sbh);
extern uint16 otpr(void *oh, chipcregs_t *cc, uint wn);
extern int otp_status(void *oh);
extern int otp_size(void *oh);

#ifdef BCMNVRAMW
extern int otp_write_region(void *oh, int region, uint16 *data, uint wlen);

extern int otp_nvwrite(void *oh, uint16 *data, uint wlen);
#endif

#if	defined(WLTEST)
extern int otp_dump(void *oh, int arg, char *buf, uint size);
#endif
