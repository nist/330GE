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
 *  This is a direct copy of the ev96100.h file, with a global
 * search and replace.  The numbers are the same.
 *
 *  The reason I'm duplicating this is so that the 64120/96100
 * defines won't be confusing in the source code.
 */
#ifndef _MIPS_EV64120_H
#define _MIPS_EV64120_H

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
#define EV64120_COM1_BASE_ADDR  (0x1d000000 + 0x20)
#define EV64120_COM2_BASE_ADDR  (0x1d000000 + 0x00)


/*
 *   EV64120 interrupt controller register base.
 */
#define EV64120_ICTRL_REGS_BASE   (KSEG1ADDR(0x1f000000))

/*
 *   EV64120 UART register base.
 */
#define EV64120_UART0_REGS_BASE    (KSEG1ADDR(EV64120_COM1_BASE_ADDR))
#define EV64120_UART1_REGS_BASE    (KSEG1ADDR(EV64120_COM2_BASE_ADDR))
#define EV64120_BASE_BAUD ( 3686400 / 16 )


/*
 * Because of an error/peculiarity in the Galileo chip, we need to swap the
 * bytes when running bigendian.
 */

#define GT_WRITE(ofs, data)  \
             *(volatile u32 *)(MIPS_GT_BASE+ofs) = cpu_to_le32(data)
#define GT_READ(ofs, data)   \
             *data = le32_to_cpu(*(volatile u32 *)(MIPS_GT_BASE+ofs))


#endif /* !(_MIPS_EV64120_H) */
