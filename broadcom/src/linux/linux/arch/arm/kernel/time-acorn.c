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
 *  linux/arch/arm/kernel/time-acorn.c
 *
 *  Copyright (c) 1996-2000 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Changelog:
 *   24-Sep-1996	RMK	Created
 *   10-Oct-1996	RMK	Brought up to date with arch-sa110eval
 *   04-Dec-1997	RMK	Updated for new arch/arm/time.c
 */
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/init.h>

#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/hardware/ioc.h>

extern unsigned long (*gettimeoffset)(void);

static unsigned long ioctime_gettimeoffset(void)
{
	unsigned int count1, count2, status1, status2;
	unsigned long offset = 0;

	status1 = ioc_readb(IOC_IRQREQA);
	barrier ();
	ioc_writeb (0, IOC_T0LATCH);
	barrier ();
	count1 = ioc_readb(IOC_T0CNTL) | (ioc_readb(IOC_T0CNTH) << 8);
	barrier ();
	status2 = ioc_readb(IOC_IRQREQA);
	barrier ();
	ioc_writeb (0, IOC_T0LATCH);
	barrier ();
	count2 = ioc_readb(IOC_T0CNTL) | (ioc_readb(IOC_T0CNTH) << 8);

	if (count2 < count1) {
		/*
		 * This means that we haven't just had an interrupt
		 * while reading into status2.
		 */
		if (status2 & (1 << 5))
			offset = tick;
		count1 = count2;
	} else if (count2 > count1) {
		/*
		 * We have just had another interrupt while reading
		 * status2.
		 */
		offset += tick;
		count1 = count2;
	}

	count1 = LATCH - count1;
	/*
	 * count1 = number of clock ticks since last interrupt
	 */
	offset += count1 * tick / LATCH;
	return offset;
}

void __init ioctime_init(void)
{
	ioc_writeb(LATCH & 255, IOC_T0LTCHL);
	ioc_writeb(LATCH >> 8, IOC_T0LTCHH);
	ioc_writeb(0, IOC_T0GO);

	gettimeoffset = ioctime_gettimeoffset;
}
