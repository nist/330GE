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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Machine dependent access functions for RTC registers.
 *
 * Copyright (C) 1996, 1997, 1998, 2000 Ralf Baechle
 * Copyright (C) 2002  Maciej W. Rozycki
 */
#ifndef _ASM_MC146818RTC_H
#define _ASM_MC146818RTC_H

#include <linux/config.h>

#include <asm/io.h>


/*
 * This structure defines how to access various features of
 * different machine types and how to access them.
 */
struct rtc_ops {
	/* How to access the RTC register in a DS1287.  */
	unsigned char (*rtc_read_data)(unsigned long addr);
	void (*rtc_write_data)(unsigned char data, unsigned long addr);
	int (*rtc_bcd_mode)(void);
};

extern struct rtc_ops *rtc_ops;

/*
 * Most supported machines access the RTC index register via an ISA
 * port access but the way to access the date register differs ...
 * The DECstation directly maps the RTC memory in the CPU's address
 * space with the chipset generating necessary index write/data access
 * cycles automagically.
 */
#define CMOS_READ(addr) ({ \
rtc_ops->rtc_read_data(addr); \
})
#define CMOS_WRITE(val, addr) ({ \
rtc_ops->rtc_write_data(val, addr); \
})
#define RTC_ALWAYS_BCD \
rtc_ops->rtc_bcd_mode()


#ifdef CONFIG_DECSTATION

#include <asm/dec/rtc-dec.h>

#elif defined(CONFIG_MIPS_ITE8172) || defined(CONFIG_MIPS_IVR)

#include <asm/it8172/it8172_int.h>

#define RTC_PORT(x)	(0x14014800 + (x))
#define RTC_IOMAPPED	0
#define RTC_IRQ		IT8172_RTC_IRQ

#elif defined(CONFIG_MIPS_PB1500) || defined(CONFIG_MIPS_PB1100)

#define RTC_PORT(x)	(0x0c000000 + (x))
#define RTC_IOMAPPED	0
#define RTC_IRQ		0

#else

#define RTC_PORT(x)	(0x70 + (x))
#define RTC_IRQ		8

#endif

#endif /* _ASM_MC146818RTC_H */