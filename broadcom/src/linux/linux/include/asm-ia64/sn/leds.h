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
#ifndef _ASM_IA64_SN_LEDS_H
#define _ASM_IA64_SN_LEDS_H

/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 * Copyright (C) 2000-2002 Silicon Graphics, Inc. All rights reserved.
 */

#include <linux/config.h>
#include <asm/smp.h>
#include <asm/sn/addrs.h>
#include <asm/sn/sn_cpuid.h>
#include <asm/sn/pda.h>

#ifdef CONFIG_IA64_SGI_SN1
#define LED0		0xc0000b00100000c0LL	
#define LED_CPU_SHIFT	3
#else
#include <asm/sn/sn2/shub.h>
#define LED0		(LOCAL_MMR_ADDR(SH_REAL_JUNK_BUS_LED0))
#define LED_CPU_SHIFT	16
#endif

#define LED_CPU_HEARTBEAT	0x01
#define LED_CPU_ACTIVITY	0x02
#define LED_MASK_AUTOTEST	0xfe

/*
 * Basic macros for flashing the LEDS on an SGI, SN1.
 */

static __inline__ void
set_led_bits(u8 value, u8 mask)
{
	pda.led_state = (pda.led_state & ~mask) | (value & mask);
#ifdef CONFIG_IA64_SGI_SN1
	*pda.led_address = (long) pda.led_state;
#else
	*pda.led_address = (short) pda.led_state;
#endif
}

#endif /* _ASM_IA64_SN_LEDS_H */

