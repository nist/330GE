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
 * Machine dependent access functions for RTC registers.
 */
#ifndef __ASM_SPARC64_MC146818RTC_H
#define __ASM_SPARC64_MC146818RTC_H

#include <linux/config.h>
#include <asm/io.h>

#ifndef RTC_PORT
#ifdef CONFIG_PCI
extern unsigned long ds1287_regs;
#else
#define ds1287_regs (0UL)
#endif
#define RTC_PORT(x)	(ds1287_regs + (x))
#define RTC_ALWAYS_BCD	0
#endif

/*
 * The yet supported machines all access the RTC index register via
 * an ISA port access but the way to access the date register differs ...
 */
#define CMOS_READ(addr) ({ \
outb_p((addr),RTC_PORT(0)); \
inb_p(RTC_PORT(1)); \
})
#define CMOS_WRITE(val, addr) ({ \
outb_p((addr),RTC_PORT(0)); \
outb_p((val),RTC_PORT(1)); \
})

#define RTC_IRQ 8

#endif /* __ASM_SPARC64_MC146818RTC_H */
