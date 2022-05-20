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
 * HND SiliconBackplane chipcommon support.
 *
 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: hndchipc.h,v 1.1.1.1 2007/01/25 12:52:12 jiahao_jhou Exp $
 */

#ifndef _hndchipc_h_
#define _hndchipc_h_


#if defined(mips)
#include <hndmips.h>
#endif

#if defined(__ARM_ARCH_4T__)
#include <hndarm.h>
#endif

extern void sb_serial_init(sb_t *sbh, void (*add)(void *regs, uint irq, uint baud_base,
                                                           uint reg_shift));

extern void *sb_jtagm_init(sb_t *sbh, uint clkd, bool exttap);
extern void sb_jtagm_disable(osl_t *osh, void *h);
extern uint32 jtag_rwreg(osl_t *osh, void *h, uint32 ir, uint32 dr);

#endif /* _hndchipc_h_ */
