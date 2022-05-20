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
 * Linux Broadcom BCM47xx GPIO char driver
 *
 * Copyright 2006, ASUSTek Inc.
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: linux_gpio.h,v 1.1.1.1 2007/01/25 12:52:12 jiahao_jhou Exp $
 */

#ifndef _linux_gpio_h_
#define _linux_gpio_h_

struct gpio_ioctl {
	uint32 mask;
	uint32 val;
};

#define GPIO_IOC_MAGIC  'G'

/* reserve/release a gpio to the caller */
#define  GPIO_IOC_RESERVE	_IOWR(GPIO_IOC_MAGIC, 1, sizeof (struct gpio_ioctl))
#define  GPIO_IOC_RELEASE	_IOWR(GPIO_IOC_MAGIC, 2, sizeof (struct gpio_ioctl))
/* ioctls to read/write the gpio registers */
#define  GPIO_IOC_OUT		_IOWR(GPIO_IOC_MAGIC, 3, sizeof (struct gpio_ioctl))
#define  GPIO_IOC_IN		_IOWR(GPIO_IOC_MAGIC, 4, sizeof (struct gpio_ioctl))
#define  GPIO_IOC_OUTEN		_IOWR(GPIO_IOC_MAGIC, 5, sizeof (struct gpio_ioctl))

#endif	/* _linux_gpio_h_ */
