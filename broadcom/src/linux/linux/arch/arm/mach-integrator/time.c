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
 *  linux/arch/arm/mach-integrator/time.c
 *
 *  Copyright (C) 2000-2001 Deep Blue Solutions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>

#include <asm/hardware.h>
#include <asm/io.h>

#define RTC_DR		(IO_ADDRESS(INTEGRATOR_RTC_BASE) + 0)
#define RTC_MR		(IO_ADDRESS(INTEGRATOR_RTC_BASE) + 4)
#define RTC_STAT	(IO_ADDRESS(INTEGRATOR_RTC_BASE) + 8)
#define RTC_EOI		(IO_ADDRESS(INTEGRATOR_RTC_BASE) + 8)
#define RTC_LR		(IO_ADDRESS(INTEGRATOR_RTC_BASE) + 12)
#define RTC_CR		(IO_ADDRESS(INTEGRATOR_RTC_BASE) + 16)

#define RTC_CR_MIE	0x00000001

extern int (*set_rtc)(void);

static int integrator_set_rtc(void)
{
	__raw_writel(xtime.tv_sec, RTC_LR);
	return 1;
}

static int integrator_rtc_init(void)
{
	__raw_writel(0, RTC_CR);
	__raw_writel(0, RTC_EOI);

	xtime.tv_sec = __raw_readl(RTC_DR);

	set_rtc = integrator_set_rtc;

	return 0;
}

__initcall(integrator_rtc_init);
