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
 * linux/include/asm-arm/arch-sa1100/jornada720.h
 *
 * Created 2000/11/29 by John Ankcorn <jca@lcs.mit.edu>
 *
 * This file contains the hardware specific definitions for HP Jornada 720
 *
 */

#ifndef __ASM_ARCH_HARDWARE_H
#error "include <asm/hardware.h> instead"
#endif

#define JORNADA720_SA1111_BASE		(0x40000000)

#define GPIO_JORNADA720_KEYBOARD	GPIO_GPIO(0)
#define GPIO_JORNADA720_MOUSE		GPIO_GPIO(9)

#define GPIO_JORNADA720_KEYBOARD_IRQ	IRQ_GPIO0
#define GPIO_JORNADA720_MOUSE_IRQ		IRQ_GPIO9

#ifndef __ASSEMBLY__

void jornada720_mcu_init(void);
void jornada_contrast(int arg_contrast);
void jornada720_battery(void);
int jornada720_getkey(unsigned char *data, int size);
#endif
