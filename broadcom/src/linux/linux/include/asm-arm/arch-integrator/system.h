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
 *  linux/include/asm-arm/arch-integrator/system.h
 *
 *  Copyright (C) 1999 ARM Limited
 *  Copyright (C) 2000 Deep Blue Solutions Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef __ASM_ARCH_SYSTEM_H
#define __ASM_ARCH_SYSTEM_H

#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/arch/platform.h>

static void arch_idle(void)
{
	/*
	 * This should do all the clock switching
	 * and wait for interrupt tricks
	 */
	cpu_do_idle(0);
}

static inline void arch_reset(char mode)
{
	unsigned int hdr_ctrl =	(IO_ADDRESS(INTEGRATOR_HDR_BASE) + INTEGRATOR_HDR_CTRL_OFFSET);
	unsigned int val;

	/*
	 * To reset, we hit the on-board reset register
	 * in the system FPGA
	 */
	val = __raw_readl(hdr_ctrl);
	val |= INTEGRATOR_HDR_CTRL_RESET;
	__raw_writel(val, hdr_ctrl);
}

#endif
