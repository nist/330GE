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
 * linux/include/asm-arm/arch-shark/system.h
 *
 * by Alexander Schulz
 */
#ifndef __ASM_ARCH_SYSTEM_H
#define __ASM_ARCH_SYSTEM_H

#include <asm/io.h>

static void arch_reset(char mode)
{
	short temp;
	cli();
	/* Reset the Machine via pc[3] of the sequoia chipset */
	outw(0x09,0x24);
	temp=inw(0x26);
	temp = temp | (1<<3) | (1<<10);
	outw(0x09,0x24);
	outw(temp,0x26);

}

static void arch_idle(void)
{
}

#endif
