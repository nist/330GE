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
#ifndef _ASM_IA64_SN_SIMULATOR_H
#define _ASM_IA64_SN_SIMULATOR_H

/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 * Copyright (C) 2000-2001 Silicon Graphics, Inc. All rights reserved.
 */

#include <linux/config.h>

#ifdef CONFIG_IA64_SGI_SN_SIM

#define SNMAGIC 0xaeeeeeee8badbeefL
#define IS_RUNNING_ON_SIMULATOR() ({long sn; asm("mov %0=cpuid[%1]" : "=r"(sn) : "r"(2)); sn == SNMAGIC;})

#define SIMULATOR_SLEEP()	asm("nop.i 0x8beef")

#else

#define IS_RUNNING_ON_SIMULATOR()	(0)
#define SIMULATOR_SLEEP()

#endif

#endif /* _ASM_IA64_SN_SIMULATOR_H */
