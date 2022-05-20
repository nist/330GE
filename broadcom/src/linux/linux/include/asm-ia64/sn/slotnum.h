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
/* $Id: slotnum.h,v 1.1.1.1 2007/01/25 12:52:04 jiahao_jhou Exp $
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1992 - 1997, 2000-2001 Silicon Graphics, Inc. All rights reserved.
 */
#ifndef _ASM_IA64_SN_SLOTNUM_H
#define _ASM_IA64_SN_SLOTNUM_H

#include <linux/config.h>

typedef	unsigned char slotid_t;

#if defined(CONFIG_IA64_SGI_SN1)
#include <asm/sn/sn1/slotnum.h>
#elif defined(CONFIG_IA64_SGI_SN2)
#include <asm/sn/sn2/slotnum.h>
#else

#error <<BOMB! slotnum defined only for SN0 and SN1 >>

#endif /* !CONFIG_IA64_SGI_SN1 */

#endif /* _ASM_IA64_SN_SLOTNUM_H */
