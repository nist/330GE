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
 * linux/include/asm-arm/arch-l7200/system.h
 *
 * Copyright (c) 2000 Steve Hill (sjhill@cotw.com)
 *
 * Changelog
 *  03-21-2000  SJH	Created
 *  04-26-2000  SJH	Fixed functions
 *  05-03-2000  SJH	Removed usage of obsolete 'iomd.h'
 *  05-31-2000  SJH	Properly implemented 'arch_idle'
 */
#ifndef __ASM_ARCH_SYSTEM_H
#define __ASM_ARCH_SYSTEM_H

static void arch_idle(void)
{
	*(unsigned long *)(IO_BASE + 0x50004) = 1;	/* idle mode */
}

static inline void arch_reset(char mode)
{
	if (mode == 's') {
		cpu_reset(0);
	}
}

#endif
