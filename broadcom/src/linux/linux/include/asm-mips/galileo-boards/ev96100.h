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
 *
 */
#ifndef _MIPS_EV96100_H
#define _MIPS_EV96100_H

#include <asm/addrspace.h>

/*
 *   GT64120 config space base address
 */
#define GT64120_BASE    (KSEG1ADDR(0x14000000))
#define MIPS_GT_BASE    GT64120_BASE

/*
 *   PCI Bus allocation
 */
#define GT_PCI_MEM_BASE    0x12000000
#define GT_PCI_MEM_SIZE    0x02000000
#define GT_PCI_IO_BASE     0x10000000
#define GT_PCI_IO_SIZE     0x02000000
#define GT_ISA_IO_BASE     PCI_IO_BASE

/*
 *   Duart I/O ports.
 */
#define EV96100_COM1_BASE_ADDR  (0xBD000000 + 0x20)
#define EV96100_COM2_BASE_ADDR  (0xBD000000 + 0x00)


/*
 *   EV96100 interrupt controller register base.
 */
#define EV96100_ICTRL_REGS_BASE   (KSEG1ADDR(0x1f000000))

/*
 *   EV96100 UART register base.
 */
#define EV96100_UART0_REGS_BASE    EV96100_COM1_BASE_ADDR
#define EV96100_UART1_REGS_BASE    EV96100_COM2_BASE_ADDR
#define EV96100_BASE_BAUD ( 3686400 / 16 )


/*
 * Because of an error/peculiarity in the Galileo chip, we need to swap the
 * bytes when running bigendian.
 */

#define GT_WRITE(ofs, data)  \
             *(volatile u32 *)(MIPS_GT_BASE+ofs) = cpu_to_le32(data)
#define GT_READ(ofs, data)   \
             data = le32_to_cpu(*(volatile u32 *)(MIPS_GT_BASE+ofs))


#endif /* !(_MIPS_EV96100_H) */
