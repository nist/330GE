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
 *  linux/arch/arm/mach-mx1ads/cpu.c
 *
 *  Copyright (C) 2001 Deep Blue Solutions Ltd.
 *
 *  $Id: cpu.c,v 1.1.1.1 2007/01/25 12:51:48 jiahao_jhou Exp $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * CPU support functions
 */

#include <linux/config.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/cpufreq.h>
#include <linux/init.h>

#include <asm/hardware.h>
#include <asm/io.h>


static int __init cpu_init(void)
{
	return 0;
}

__initcall(cpu_init);
