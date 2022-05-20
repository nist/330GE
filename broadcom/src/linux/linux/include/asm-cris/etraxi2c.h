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
/* $Id: etraxi2c.h,v 1.1.1.1 2007/01/25 12:52:04 jiahao_jhou Exp $ */

#ifndef _LINUX_ETRAXI2C_H
#define _LINUX_ETRAXI2C_H

/* etraxi2c _IOC_TYPE, bits 8 to 15 in ioctl cmd */

#define ETRAXI2C_IOCTYPE 44

/* supported ioctl _IOC_NR's */

/* in write operations, the argument contains both i2c
 * slave, register and value.
 */

#define I2C_WRITEARG(slave, reg, value) (((slave) << 16) | ((reg) << 8) | (value))
#define I2C_READARG(slave, reg) (((slave) << 16) | ((reg) << 8))

#define I2C_ARGSLAVE(arg) ((arg) >> 16)
#define I2C_ARGREG(arg) (((arg) >> 8) & 0xff)
#define I2C_ARGVALUE(arg) ((arg) & 0xff)

#define I2C_WRITEREG 0x1   /* write to an i2c register */
#define I2C_READREG  0x2   /* read from an i2c register */

/*
EXAMPLE usage:

    i2c_arg = I2C_WRITEARG(STA013_WRITE_ADDR, reg, val);
    ioctl(fd, _IO(ETRAXI2C_IOCTYPE, I2C_WRITEREG), i2c_arg);

    i2c_arg = I2C_READARG(STA013_READ_ADDR, reg);
    val = ioctl(fd, _IO(ETRAXI2C_IOCTYPE, I2C_READREG), i2c_arg);

*/
#endif
