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
/* $Id: etraxvirtex.h,v 1.1.1.1 2007/01/25 12:52:04 jiahao_jhou Exp $ */

#ifndef _LINUX_ETRAXVIRTEX_FPGA_H
#define _LINUX_ETRAXVIRTEX_FPGA_H

/* etraxvirtex_fpga _IOC_TYPE, bits 8 to 15 in ioctl cmd */

#define ETRAXVIRTEX_FPGA_IOCTYPE 45

/* supported ioctl _IOC_NR's */

/* in write operations, the argument contains both virtex
 * register and value.
 */

#define VIRTEX_FPGA_WRITEARG(reg, value) (((reg) << 16) | (value))
#define VIRTEX_FPGA_READARG(reg) ((reg) << 16)

#define VIRTEX_FPGA_ARGREG(arg) (((arg) >> 16) & 0x0fff)
#define VIRTEX_FPGA_ARGVALUE(arg) ((arg) & 0xffff)

#define VIRTEX_FPGA_WRITEREG 0x1   /* write to an (FPGA implemented) register */
#define VIRTEX_FPGA_READREG  0x2   /* read from an (FPGA implemented register */

/*
EXAMPLE usage:

    virtex_arg = VIRTEX_FPGA_WRITEARG( reg, val);
    ioctl(fd, _IO(ETRAXVIRTEX_FPGA_IOCTYPE, VIRTEX_FPGA_WRITEREG), virtex_arg);

    virtex_arg = VIRTEX_FPGA_READARG( reg);
    val = ioctl(fd, _IO(ETRAXVIRTEX_FPGA_IOCTYPE, VIRTEX_FPGA_READREG), virtex_arg);

*/
#endif
