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
 * linux/include/asm-arm/arch-l7200/gp_timers.h
 *
 * Copyright (C) 2000 Steve Hill (sjhill@cotw.com)
 *
 * Changelog:
 *   07-28-2000	SJH	Created file
 *   08-02-2000	SJH	Used structure for registers
 */
#ifndef _ASM_ARCH_GPTIMERS_H
#define _ASM_ARCH_GPTIMERS_H

#include <asm/arch/hardware.h>

/*
 * Layout of L7200 general purpose timer registers
 */
struct GPT_Regs {
	unsigned int TIMERLOAD;
	unsigned int TIMERVALUE;
	unsigned int TIMERCONTROL;
	unsigned int TIMERCLEAR;
};

#define GPT_BASE		(IO_BASE_2 + 0x3000)
#define l7200_timer1_regs	((volatile struct GPT_Regs *) (GPT_BASE))
#define l7200_timer2_regs	((volatile struct GPT_Regs *) (GPT_BASE + 0x20))

/*
 * General register values
 */
#define	GPT_PRESCALE_1		0x00000000
#define	GPT_PRESCALE_16		0x00000004
#define	GPT_PRESCALE_256	0x00000008
#define GPT_MODE_FREERUN	0x00000000
#define GPT_MODE_PERIODIC	0x00000040
#define GPT_ENABLE		0x00000080
#define GPT_BZTOG		0x00000100
#define GPT_BZMOD		0x00000200
#define GPT_LOAD_MASK 		0x0000ffff

#endif
