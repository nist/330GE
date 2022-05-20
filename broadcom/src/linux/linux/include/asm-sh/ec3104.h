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
#ifndef __ASM_EC3104_H
#define __ASM_EC3104_H


/*
 * Most of the register set is at 0xb0ec0000 - 0xb0ecffff.
 *
 * as far as I've figured it out the register map is:
 * 0xb0ec0000 - id string
 * 0xb0ec0XXX - power management
 * 0xb0ec1XXX - interrupt control
 * 0xb0ec3XXX - ps2 port (touch pad on aero 8000)
 * 0xb0ec6XXX - i2c
 * 0xb0ec7000 - first serial port (proprietary connector on aero 8000)
 * 0xb0ec8000 - second serial port
 * 0xb0ec9000 - third serial port
 * 0xb0eca000 - fourth serial port (keyboard controller on aero 8000)
 * 0xb0eccXXX - GPIO
 * 0xb0ecdXXX - GPIO
 */

#define EC3104_BASE	0xb0ec0000

#define EC3104_SER4_DATA	(EC3104_BASE+0xa000)
#define EC3104_SER4_IIR		(EC3104_BASE+0xa008)
#define EC3104_SER4_MCR		(EC3104_BASE+0xa010)
#define EC3104_SER4_LSR		(EC3104_BASE+0xa014)
#define EC3104_SER4_MSR		(EC3104_BASE+0xa018)

/*
 * our ISA bus.  this seems to be real ISA.
 */
#define EC3104_ISA_BASE	0xa5000000

#define EC3104_IRQ	11
#define EC3104_IRQBASE	64

#define EC3104_IRQ_SER1	EC3104_IRQBASE + 7
#define EC3104_IRQ_SER2	EC3104_IRQBASE + 8
#define EC3104_IRQ_SER3	EC3104_IRQBASE + 9
#define EC3104_IRQ_SER4	EC3104_IRQBASE + 10

#endif /* __ASM_EC3104_H */
