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
 *  linux/arch/arm/mach-shark/arch.c
 *
 *  Architecture specific stuff.
 */
#include <linux/tty.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/init.h>

#include <asm/hardware/dec21285.h>
#include <asm/elf.h>
#include <asm/setup.h>
#include <asm/mach-types.h>

#include <asm/mach/arch.h>

extern void shark_map_io(void);
extern void genarch_init_irq(void);

MACHINE_START(SHARK, "Shark")
	MAINTAINER("Alexander Schulz")
	BOOT_MEM(0x08000000, 0x40000000, 0xe0000000)
	BOOT_PARAMS(0x08003000)
	MAPIO(shark_map_io)
	INITIRQ(genarch_init_irq)
MACHINE_END
